#pragma once

#include <WiFiUdp.h>
#include "Vito.h"
#include "Logger.h"

class UdpServer {
public:
    explicit UdpServer(Logger* logger, Vito* vito);

    void begin(uint16_t port = 5555);
    void loop();

private:
    void handlePacket(int packetSize);
    void handleCommand(const char* cmd, IPAddress remote, uint16_t port);

    Logger* logger;
    Vito* vito;
    WiFiUDP udp;
    uint16_t listenPort;

    char rxBuffer[256];
};
