
#pragma once

// Hardware Serial fuer Logging
#define SERIAL2 Serial
#define SERIALBAUDRATE 115200

// DNS Hostname 
#define HOSTNAME "IoT-VitoESP32"

// Watchdog Timeout in s
#define WDT_TIMEOUT 120

// Serial fuer Optolink // UART2
#define OPTOLINKSERIAL 2
#define RX_PIN 16
#define TX_PIN 17

// WLAN Einstellungen
#define WIFI_SSID     "SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"
#define WIFI_TIMEOUT_MS 15000


// IP Einstellungen
#define NETWORK_STATIC_IP true
#define NETWORK_LOCAL_IP String("192.168.0.100")
#define NETWORK_GATEWAY String("192.168.0.1")
#define NETWORK_SUBNET String("255.255.255.0")
#define NETWORK_DNS String("192.168.0.1")

#define NETWORK_CHECK_INTERVAL 60000
#define NETWORK_CHECK_FAILS 3

// Webserver
#define WEBSERVER_PORT 80

// Telnet Logging
#define TELNETSERVER_PORT 23

// NTP 
#define NTP_SERVER_IP IPAddress(192,168,0,1)

// alle 12h
#define NTP_UPDATE_INTERVALL 43200000

// Startup Delay
// Erst nach ... ms mit der Vito Abfrage starten
const unsigned long STARTUP_DELAY_MS = 20000UL;

// Vito Poll Intervall
const unsigned long VITO_POLL_INTERVALL = 20000UL;

// UDP Sender aktivieren
const bool UDP_SENDER_ENABLED = true;

// UDP Sender - IP des UDP Empfaengers
#define UDP_RECEIVER_IP IPAddress(192,168,0,200)

// UDP Sender - Port des UDP Empfaengers
#define UDP_RECEIVER_PORT 5555

// UDP Sender - Startup Deleay
const unsigned long UDP_STARTUP_DELAY_MS = 30000UL;

// UDP Sender - Update Interval
const unsigned long UDP_UPDATE_INTERVALL = 60000UL;

// UDP Server aktivieren
const bool UDP_SERVER_ENABLED = true;

// UDP Server Port
const int UDP_SERVER_PORT = 5555;

// MQTT Sender aktivieren
const bool MQTT_SENDER_ENABLED = true;

const unsigned long MQTT_STARTUP_DELAY_MS = 30000UL;

const unsigned long MQTT_UPDATE_INTERVALL = 60000UL;

#define MQTT_SERVER "192.168.0.150"

const int MQTT_SERVER_PORT = 1883;

#define MQTT_USER "mqtt_user"

#define MQTT_PASSWORD "mqtt_password"

#define MQTT_TOPIC "influxdb/in/vito"
