#ifndef VITO_H
#define VITO_H

#include <VitoWiFi.h>
#include "Logger.h"
#include <HardwareSerial.h>
#include "VitoDatapoint.h"

class Vito {
public:
    Vito(HardwareSerial* serial, int rx_pin, int tx_pin, Logger* logger);

    void begin(HardwareSerial* serial);
    void loop();

    VitoDatapoint* getDatapoints();
    uint8_t getDatapointCount();
    VitoDatapoint* getDatapointByName(const char* name);

    void setPollInterval(unsigned long interval) { pollInterval = interval; }
    unsigned long getPollInterval() const { return pollInterval; }

    void setStartupDelay(unsigned long delayMs) { startupDelay = delayMs; }
    unsigned long getStartupDelay() const { return startupDelay; }

    bool write(VitoDatapoint* dp, float value);
    bool writeByName(const char* name, float value);

private:
    HardwareSerial* serial;
    Logger* logger;

    bool writeActive = false;

    // interne Instanz
    VitoWiFi::VitoWiFi<VitoWiFi::VS2> vitoWiFi;

    static VitoDatapoint datapoints[];
    static const uint8_t datapointCount;

    uint8_t datapointIndex = 0;
    bool readValues = false;
    unsigned long lastMillis = 0;
    unsigned long pollInterval = 20000UL;

    unsigned long startupDelay = 20000UL;
    unsigned long startupMillis = 0;
    bool startupComplete = false;

    void readAll();

    // Callbacks
    void handleResponse(const VitoWiFi::PacketVS2& response, const VitoWiFi::Datapoint& request);
    void handleError(VitoWiFi::OptolinkResult error, const VitoWiFi::Datapoint& request);
    
};

#endif
