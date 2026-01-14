#pragma once

#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Vito.h"
#include "Logger.h"

class MqttDatapointSender {
public:
    MqttDatapointSender(Vito* vito, Logger* logger);

    void begin(
        const char* broker,
        uint16_t port,
        const char* clientId,
        const char* user = nullptr,
        const char* password = nullptr
    );

    void loop();
    bool isConnected();

    void sendAllDatapoints();

private:
    String applyTemplate(const String& tpl, const char* name, float value);

    Vito* vito;
    Logger* logger;

    WiFiClient wifiClient;
    PubSubClient mqtt;

    const char* broker;
    uint16_t port;
    const char* clientId;
    const char* user;
    const char* password;

    unsigned long lastReconnectAttempt = 0;
    bool reconnect();
};
