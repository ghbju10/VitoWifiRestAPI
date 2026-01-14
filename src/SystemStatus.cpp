#include "SystemStatus.h"
#include <esp_system.h>
#include <esp_heap_caps.h>

SystemStatus::SystemStatus()
: bootMillis(0) {}

void SystemStatus::begin() {
    bootMillis = millis();
}

void SystemStatus::update() {
    // aktuell leer – Platz für spätere Erweiterungen
}

// -------- System --------

uint32_t SystemStatus::uptimeSeconds() const {
    return (millis() - bootMillis) / 1000;
}

String SystemStatus::uptimeString() const {
    uint32_t s = uptimeSeconds();
    uint32_t d = s / 86400; s %= 86400;
    uint32_t h = s / 3600;  s %= 3600;
    uint32_t m = s / 60;    s %= 60;

    char buf[32];
    snprintf(buf, sizeof(buf), "%ud %02u:%02u:%02u", d, h, m, s);
    return String(buf);
}

uint32_t SystemStatus::freeHeap() const {
    return ESP.getFreeHeap();
}

uint32_t SystemStatus::minFreeHeap() const {
    return ESP.getMinFreeHeap();
}

uint8_t SystemStatus::heapFragmentation() const {
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t maxBlock = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);

    if (freeHeap == 0) return 0;
    return 100 - ((maxBlock * 100) / freeHeap);
}

// -------- Reset --------

esp_reset_reason_t SystemStatus::resetReason() const {
    return esp_reset_reason();
}

String SystemStatus::resetReasonString() const {
    switch (resetReason()) {
        case ESP_RST_POWERON: return "poweron";
        case ESP_RST_SW:      return "software";
        case ESP_RST_PANIC:   return "panic";
        case ESP_RST_INT_WDT: return "wdt_int";
        case ESP_RST_TASK_WDT:return "wdt_task";
        case ESP_RST_WDT:     return "wdt";
        case ESP_RST_DEEPSLEEP:return "deepsleep";
        case ESP_RST_BROWNOUT:return "brownout";
        default:              return "unknown";
    }
}

// -------- Network --------

bool SystemStatus::wifiConnected() const {
    return WiFi.status() == WL_CONNECTED;
}

int8_t SystemStatus::wifiRSSI() const {
    return wifiConnected() ? WiFi.RSSI() : 0;
}

String SystemStatus::ipAddress() const {
    return wifiConnected() ? WiFi.localIP().toString() : String("0.0.0.0");
}
