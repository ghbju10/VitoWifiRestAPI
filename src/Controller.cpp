#include "Controller.h"
#include <ArduinoJson.h>
#include "esp_task_wdt.h"
#include "Vito.h"
#include "SystemStatus.h"

Controller::Controller(WebServer& server, Logger& logger, Vito* vito, SystemStatus* sys)
    : _server(server), _logger(logger), _vito(vito), _sys(sys) {}

void Controller::begin() {
    _server.on("/status", HTTP_GET, [this]() { handleStatus(); });
    _server.on("/reboot", HTTP_POST, [this]() { handleReboot(); });
    _server.on("/get",  HTTP_GET,  [this]() { handleGetDatapoint(); });
    _server.on("/set",  HTTP_POST, [this]() { handleSetDatapoint(); });
    _server.begin();
    _logger.println("REST Controller started", LOG_INFO);
}

void Controller::handleStatus() {
    esp_task_wdt_reset();
    _logger.println("handleStatus() called", LOG_DEBUG);

    StaticJsonDocument<2048> doc;
    doc["status"] = "ok";

    addSystemStatus(doc);
    addVitoStatus(doc);

    String output;
    serializeJson(doc, output);
    _server.send(200, "application/json", output);
}

void Controller::handleGetDatapoint() {
    esp_task_wdt_reset();
    _logger.println("handleGetDatapoint() called", LOG_DEBUG);

    if (!_server.hasArg("dp")) {
        _server.send(400, "text/plain", "missing argument: dp");
        return;
    }

    String name = _server.arg("dp");
    VitoDatapoint* dp = _vito->getDatapointByName(name.c_str());
    if (!dp) {
        _server.send(404, "text/plain", "datapoint not found");
        return;
    }

    StaticJsonDocument<512> doc;
    JsonObject o = doc.createNestedObject("datapoint");

    const auto& raw = dp->getDatapoint();

    o["name"]     = raw.name();
    o["address"]  = raw.address();
    o["length"]   = raw.length();
    o["writable"] = dp->getIsWritable();
    o["min"]      = dp->getMinValue();
    o["max"]      = dp->getMaxValue();
    o["value"]    = dp->getLastValue();

    String output;
    serializeJson(doc, output);
    _server.send(200, "application/json", output);
}

void Controller::handleSetDatapoint() {
    esp_task_wdt_reset();
    _logger.println("handleSetDatapoint() called", LOG_DEBUG);

    if (!_server.hasArg("dp")) {
        _server.send(400, "text/plain", "missing argument: dp");
        _logger.println("400 missing argument: dp", LOG_DEBUG);
        return;
    }
    if (!_server.hasArg("plain")) {
        _server.send(400, "text/plain", "missing JSON payload");
        _logger.println("400 missing JSON payload", LOG_DEBUG);
        return;
    }

    String name = _server.arg("dp");
    VitoDatapoint* dp = _vito->getDatapointByName(name.c_str());
    if (!dp) {
        _server.send(404, "text/plain", "datapoint not found");
        _logger.println("404 datapoint not found", LOG_DEBUG);
        return;
    }
    if (!dp->getIsWritable()) {
        _server.send(403, "text/plain", "datapoint is read-only");
        _logger.println("403 datapoint is read-only", LOG_DEBUG);
        return;
    }

    // JSON payload
    StaticJsonDocument<200> body;
    if (deserializeJson(body, _server.arg("plain"))) {
        _server.send(400, "text/plain", "invalid JSON");
        _logger.println("400 invalid JSON", LOG_DEBUG);
        return;
    }
    if (!body.containsKey("value")) {
        _server.send(400, "text/plain", "missing field: value");
        _logger.println("400 missing field: value", LOG_DEBUG);
        return;
    }

    float newValue = body["value"].as<float>();

    // Min/Max check
    if (newValue < dp->getMinValue() || newValue > dp->getMaxValue()) {
        _server.send(406, "text/plain", "value out of allowed range");
        _logger.println("406 value out of allowed range", LOG_DEBUG);
        return;
    }

    // Write to Vito    
    bool ok = _vito->write(dp, newValue);

    if (!ok) {
        _server.send(500, "text/plain", "write failed");
        _logger.println("500 write failed", LOG_DEBUG);
        return;
    }

    // Erfolg
    StaticJsonDocument<200> doc;
    doc["status"] = "ok";
    doc["written"] = newValue;

    String output;
    serializeJson(doc, output);
    _server.send(200, "application/json", output);
    _logger.println("200 ok", LOG_DEBUG);
}

void Controller::addVitoStatus(JsonDocument& doc) {
    if (!_vito) return;

    VitoDatapoint* dps = _vito->getDatapoints();
    uint8_t count = _vito->getDatapointCount();

    JsonObject vitoObj = doc.createNestedObject("vito");

    for (uint8_t i = 0; i < count; i++) {
        const char* name = dps[i].getDatapoint().name();
        float value = dps[i].getLastValue();
        vitoObj[name] = value;
    }
}

void Controller::handleReboot() {
    esp_task_wdt_reset();
    _logger.println("handleReboot() called", LOG_WARN);

    _server.send(200, "application/json", "{\"status\":\"rebooting\"}");
    ESP.restart();
}

void Controller::addSystemStatus(JsonDocument& doc) {

    JsonObject sys = doc.createNestedObject("system");

    sys["uptime"]          = _sys->uptimeString();
    sys["uptime_seconds"]  = _sys->uptimeSeconds();

    sys["free_heap"]       = _sys->freeHeap();
    sys["min_free_heap"]   = _sys->minFreeHeap();
    sys["heap_fragmentation"] = _sys->heapFragmentation();

    sys["wifi_connected"] = _sys->wifiConnected();
    sys["wifi_rssi"]      = _sys->wifiRSSI();
    sys["ip"]             = _sys->ipAddress();

    sys["reset_reason"]   = _sys->resetReasonString();
}

