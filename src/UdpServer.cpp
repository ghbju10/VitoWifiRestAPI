#include "UdpServer.h"


UdpServer::UdpServer(Logger* logger, Vito* vito)
: logger(logger), vito(vito) {}

void UdpServer::begin(uint16_t port) {
    listenPort = port;

    if (udp.begin(listenPort)) {
        logger->printf(LOG_INFO, "UDP server listening on port %d\r\n", listenPort);
    } else {
        logger->printf(LOG_ERROR, "UDP server failed to start on port %d\r\n", listenPort);
    }
}

void UdpServer::loop() {
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
        handlePacket(packetSize);
    }
}

void UdpServer::handlePacket(int packetSize) {
    int len = udp.read(rxBuffer, sizeof(rxBuffer) - 1);
    if (len <= 0) return;

    rxBuffer[len] = '\0';

    IPAddress remote = udp.remoteIP();
    uint16_t remotePort = udp.remotePort();

    logger->printf(
        LOG_INFO,
        "UDP RX from %s:%d -> %s\r\n",
        remote.toString().c_str(),
        remotePort,
        rxBuffer
    );

    handleCommand(rxBuffer, remote, remotePort);

}

void UdpServer::handleCommand(const char* cmd, IPAddress remote, uint16_t port) {
    char buffer[256];
    strncpy(buffer, cmd, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char* action = strtok(buffer, ":");
    char* key    = strtok(nullptr, ":");
    char* value  = strtok(nullptr, ":");

    if (!action || !key || !value) {
        logger->printf(
            LOG_WARN,
            "Malformed UDP command: '%s'\r\n",
            cmd
        );
        return;
    }

    if (strcmp(action, "set") == 0) {
        float fvalue = atof(value);       
        bool ok = vito->writeByName(key, fvalue);         
        logger->printf(
            ok ? LOG_DEBUG : LOG_WARN,
            "UDP SET ->%s<- => ->%.2f<-\r\n",
            key,
            fvalue
        );
        return;
    }

}
