#pragma once

#include <WiFiUdp.h>
#include <IPAddress.h>
#include "Vito.h"
#include "Logger.h"

class UdpDatapointSender {
public:
    UdpDatapointSender(Vito* vito, Logger* logger);

    void begin(IPAddress targetIp, uint16_t targetPort);
    void sendAllDatapoints();

private:
    Vito* vito;
    Logger* logger;
    WiFiUDP udp;

    IPAddress remoteIp;
    uint16_t remotePort;
};
