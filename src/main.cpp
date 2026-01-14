#include <WiFi.h>
#include <WebServer.h>
#include <VitoWiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include "esp_task_wdt.h"
#include <HardwareSerial.h>
#include <TimeLib.h>
#include <Timer.h>
#include <ESP32Ping.h>

#include "Configuration.h"
#include "Logger.h"
#include "NtpClient.h"
#include "Controller.h"
#include "UdpDatapointSender.h"
#include "UdpServer.h"
#include "MqttDatapointSender.h"
#include "Vito.h"
#include "SystemStatus.h"

/////////////////////////////////////////////////////////////////////////////
// Hardware / Globals

SystemStatus sys;
Timer timer;
WebServer server(WEBSERVER_PORT);
WiFiServer telnetServer(TELNETSERVER_PORT);
WiFiClient telnetClient;

Logger logger(SERIAL2);
NtpClient ntp(logger);

HardwareSerial serial2UART(OPTOLINKSERIAL);  // UART2
Vito vito(&serial2UART, RX_PIN, TX_PIN, &logger);
Controller controller(server, logger, &vito, &sys);
UdpDatapointSender udpSender(&vito, &logger);
UdpServer udpServer(&logger, &vito);
MqttDatapointSender mqttSender(&vito, &logger);

uint8_t networkFailCount = 0;
volatile bool otaRunning = false;

/////////////////////////////////////////////////////////////////////////////
// Utils

bool checkNetwork() {
    if (WiFi.status() != WL_CONNECTED) {
        logger.println("Network check failed: WiFi disconnected => " +  NETWORK_GATEWAY, LOG_WARN);
        return false;
    }

    if (!Ping.ping(NETWORK_GATEWAY.c_str(), 3 )) {
        logger.println("Network check failed: gateway unreachable => " +  NETWORK_GATEWAY, LOG_WARN);
        return false;
    }

    logger.println("Network check OK => " +  NETWORK_GATEWAY, LOG_DEBUG);
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// Setup

void setup() {
    delay(500);
    SERIAL2.begin(SERIALBAUDRATE);
    sys.begin();

    // Logger Setup
    logger.enableSerial(true);
    logger.enableNetwork(true);
    logger.enableTimestamp(true);
    logger.setClient(&telnetClient);
    logger.setLogLevel(LOG_DEBUG);

    // Watchdog
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = WDT_TIMEOUT * 1000,
        .idle_core_mask = (1 << portNUM_PROCESSORS) - 1, // alle kerne ueberwachen
        .trigger_panic = true
    };
    esp_task_wdt_init(&wdt_config);
    esp_task_wdt_add(NULL);

    // Statische IP setzen
    if (NETWORK_STATIC_IP){

        IPAddress network_local_ip;
        IPAddress network_gateway;
        IPAddress network_subnet;
        IPAddress network_dns;

        network_local_ip.fromString(NETWORK_LOCAL_IP);
        network_gateway.fromString(NETWORK_GATEWAY);
        network_subnet.fromString(NETWORK_SUBNET);
        network_dns.fromString(NETWORK_DNS);            

        if (!WiFi.config(network_local_ip, network_gateway, network_subnet, network_dns)) {
            SERIAL2.println("Statische IP konnte nicht gesetzt werden");
        }
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        if (millis() - start > WIFI_TIMEOUT_MS) {
            break;
        }
    }

    if (WiFi.status() != WL_CONNECTED) {
        // WiFiManager
        WiFiManager wm;

        if (!wm.autoConnect(HOSTNAME)) {
            SERIAL2.println("WiFi Verbindung fehlgeschlagen!");
            ESP.restart();
        }
        WiFi.hostname(HOSTNAME);
        logger.printf(LOG_INFO, "Verbunden! IP: %s\n", WiFi.localIP().toString().c_str());        
    }

    // Gateway erreichbar?
    timer.every(NETWORK_CHECK_INTERVAL, []() {
        if (otaRunning) return;
        if (!checkNetwork()) {
            networkFailCount++;
            logger.printf(LOG_WARN, "Network fail count: %d\r\n", networkFailCount);

            if (networkFailCount >= NETWORK_CHECK_FAILS) {
                logger.println("Network check wiederholt fehlgeschlagen => reboot", LOG_ERROR);
                delay(1000);
                ESP.restart();
            }
        } else {
            logger.println("Network check erfolgreich.", LOG_ERROR);
            networkFailCount = 0;
        }
    });

    // NTP starten
    ntp.begin(IPAddress(NTP_SERVER_IP));

    ntp.updateAsync();
    timer.every(NTP_UPDATE_INTERVALL, []() { ntp.updateAsync(); }); 

    // mDNS
    if (MDNS.begin(HOSTNAME))
        logger.println("mDNS gestartet", LOG_INFO);

    // Telnet
    telnetServer.begin();
    logger.println("Telnet-Server gestartet!", LOG_INFO);

    // nach Neustart erst nach x Sekunden (startupDelay) mit der Abfrage starten
    vito.setStartupDelay(STARTUP_DELAY_MS);
    // Daten alle x Sekunden lesen
    vito.setPollInterval(VITO_POLL_INTERVALL);

    // UDP Datenpunkte senden
    if (UDP_SENDER_ENABLED){
        logger.println("UDP Sender gestartet!", LOG_INFO);
        timer.after( (UDP_STARTUP_DELAY_MS), []() {            
            timer.every(UDP_UPDATE_INTERVALL, []() {
                udpSender.sendAllDatapoints();
            });
        });
    }

    logger.println("Setup finished", LOG_INFO);

    // REST Controller starten
    controller.begin();

    // UDP Sender starten
    udpSender.begin(UDP_RECEIVER_IP, UDP_RECEIVER_PORT);

    // UDP Server starten
    if (UDP_SERVER_ENABLED){
        udpServer.begin(UDP_SERVER_PORT);
    }

    // MQTT Sender starten
    mqttSender.begin(MQTT_SERVER, MQTT_SERVER_PORT, HOSTNAME, MQTT_USER, MQTT_PASSWORD);

    // MQTT Datenpunkte senden
    if (MQTT_SENDER_ENABLED){
        logger.println("MQTT Sender gestartet!", LOG_INFO);
        timer.after( (MQTT_STARTUP_DELAY_MS), []() {            
            timer.every(MQTT_UPDATE_INTERVALL, []() {
                mqttSender.sendAllDatapoints();
            });
        });
    }

    // OTA Setup
    ArduinoOTA.setHostname(HOSTNAME);
    ArduinoOTA.onStart([]() { 
            otaRunning = true;
            esp_task_wdt_delete(NULL);
            logger.println("OTA Start", LOG_INFO); 
        });
    ArduinoOTA.onEnd([]() { 
            logger.println("OTA Ende", LOG_INFO); 
            ESP.restart();
        });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        logger.printf(LOG_INFO, "OTA Fortschritt: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        logger.printf(LOG_ERROR, "OTA Fehler[%u]: ", error);
        if (error == OTA_AUTH_ERROR) logger.println("Auth Fehler");
        else if (error == OTA_BEGIN_ERROR) logger.println("Begin Fehler");
        else if (error == OTA_CONNECT_ERROR) logger.println("Connect Fehler");
        else if (error == OTA_RECEIVE_ERROR) logger.println("Receive Fehler");
        else if (error == OTA_END_ERROR) logger.println("End Fehler");
    });
    ArduinoOTA.begin();
   
}

/////////////////////////////////////////////////////////////////////////////
// Loop

void loop() {
    
    ArduinoOTA.handle();

    // bei OTA Update keine anderen Prozesse ausfuehren
    if (otaRunning) {
        delay(1);
        return;
    }    

    // Telnet Handling
    if (telnetServer.hasClient()) {

        if (telnetClient && telnetClient.connected()) {
          telnetClient.stop(); // alte Verbindung schließen
        }
        telnetClient = telnetServer.accept();
        logger.setClient(&telnetClient);

        // Willkommensbanner ausgeben
        String banner;
        banner += "====================================\r\n";
        banner += "Willkommen am ESP32 IoT-Device!\r\n";
        banner += "Hostname: " + String(HOSTNAME) + "\r\n";

        if (ntp.getLastTime() != 0) {
            time_t now = ntp.getLastTime();
            char buf[20];
            sprintf(buf, "%02d:%02d:%02d", hour(now), minute(now), second(now));
            banner += "Uhrzeit: " + String(buf) + "\r\n";           
        } else {
            banner += "Uhrzeit: nicht verfügbar\r\n";
        }

        banner += "====================================\r\n";
        telnetClient.print(banner);

        logger.printf(LOG_INFO, "Neuer Telnet-Client verbunden: %s\r\n", telnetClient.remoteIP().toString().c_str());
    }

    // NTP Tick
    ntp.updateTick();

    vito.loop();   
    mqttSender.loop();
    if (UDP_SERVER_ENABLED){
        udpServer.loop();
    }
    
    sys.update();
    server.handleClient();
    esp_task_wdt_reset();
    timer.update();
}

