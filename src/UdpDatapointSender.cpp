#include "UdpDatapointSender.h"

UdpDatapointSender::UdpDatapointSender(Vito* vito, Logger* logger)
: vito(vito), logger(logger) {}

void UdpDatapointSender::begin(IPAddress targetIp, uint16_t targetPort) {
    remoteIp = targetIp;
    remotePort = targetPort;
    udp.begin(0); // zufälliger lokaler Port
}

void UdpDatapointSender::sendAllDatapoints() {
    if (!vito) return;

    VitoDatapoint* dps = vito->getDatapoints();
    uint8_t count = vito->getDatapointCount();   

    for (uint8_t i = 0; i < count; i++) {

        if ( dps[i].getSendUdp() ){
            const char* name = dps[i].getDatapoint().name();
            float value = dps[i].getLastValue();
            String udpTemplate = dps[i].getUdpTemplate();
            udpTemplate.replace("<v>", String(value,2));

            // Payload: name:value
            char payload[64];
            snprintf(payload, sizeof(payload), "%s", udpTemplate.c_str());

            udp.beginPacket(remoteIp, remotePort);
            udp.write((uint8_t*)payload, strlen(payload));
            udp.endPacket();

            logger->printf(LOG_DEBUG, "UDP sent: %s\r\n", payload);

            delay(2); // kleine Entlastung fürs WLAN
        }

    }
}
