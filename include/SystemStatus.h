#pragma once

#include <Arduino.h>
#include <WiFi.h>

class SystemStatus {
public:
    SystemStatus();

    void begin();
    void update();

    // -------- System --------
    uint32_t uptimeSeconds() const;
    String   uptimeString() const;

    uint32_t freeHeap() const;
    uint32_t minFreeHeap() const;
    uint8_t  heapFragmentation() const;

    // -------- Reset / Boot --------
    esp_reset_reason_t resetReason() const;
    String resetReasonString() const;

    // -------- Network --------
    bool     wifiConnected() const;
    int8_t   wifiRSSI() const;
    String   ipAddress() const;

private:
    uint32_t bootMillis;
};
