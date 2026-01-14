#pragma once
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "Logger.h"

class NtpClient {
public:
    NtpClient(Logger& logger);

    void begin(IPAddress server, unsigned int localPort = 8888);
    void updateAsync();       // startet Anfrage, non-blocking
    bool updateTick();        // muss regelmäßig aufgerufen werden, true = Zeit aktualisiert
    time_t getLastTime() const { return _lastTime; }

private:
    Logger& _logger;
    WiFiUDP _udp;
    IPAddress _timeServer;
    int _tzHours = 1;
    unsigned int _localPort;

    static const int NTP_PACKET_SIZE = 48;
    byte _packetBuffer[NTP_PACKET_SIZE];

    bool _waiting = false;
    uint32_t _requestTime = 0;
    time_t _lastTime = 0;

    void sendNTPpacket();
    bool parsePacket();
    bool isSummertime(int year, int month, int day, int hour);
};
