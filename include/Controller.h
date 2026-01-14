#pragma once
#include <ArduinoJson.h>
#include <WebServer.h>
#include "Logger.h"
#include "Vito.h"
#include "SystemStatus.h"

class Controller {
public:
    Controller(WebServer& server, Logger& logger, Vito* vito, SystemStatus* sys);

    void begin();

private:
    WebServer& _server;
    Logger& _logger;
    Vito* _vito;
    SystemStatus* _sys;

    // REST-Handler
    void handleStatus();
    void handleSet();
    void handleReboot();
    void handleGetDatapoint();
    void handleSetDatapoint();

    // Hilfsfunktionen können privat hinzugefügt werden
    void addVitoStatus(JsonDocument& doc);
    void addSystemStatus(JsonDocument& doc);

};
