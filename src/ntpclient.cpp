#include "NtpClient.h"
#include <Arduino.h>

NtpClient::NtpClient(Logger& logger)
    : _logger(logger), _localPort(8888) {}

void NtpClient::begin(IPAddress server, unsigned int localPort) {
    _timeServer = server;
    _localPort = localPort;
    _udp.begin(_localPort);
    _logger.printf(LOG_INFO, "NTP Client started. Server: %s, Port: %d\n",
                   _timeServer.toString().c_str(), _localPort);
}

void NtpClient::updateAsync() {
    _waiting = true;
    _requestTime = millis();
    sendNTPpacket();
}

bool NtpClient::updateTick() {
    if (!_waiting) return false;

    if (_udp.parsePacket()) {
        if (parsePacket()) {
            _waiting = false;
            setTime(_lastTime);
            _logger.println("Time updated successfully.", LOG_INFO);
            return true;
        }
    }

    if (millis() - _requestTime > 1500) { // Timeout
        _waiting = false;
        _logger.println("No NTP response received.", LOG_WARN);
    }

    return false;
}

bool NtpClient::parsePacket() {
    _udp.read(_packetBuffer, NTP_PACKET_SIZE);

    unsigned long secsSince1900 =  (unsigned long)_packetBuffer[40] << 24;
    secsSince1900 |= (unsigned long)_packetBuffer[41] << 16;
    secsSince1900 |= (unsigned long)_packetBuffer[42] << 8;
    secsSince1900 |= (unsigned long)_packetBuffer[43];

    time_t t = secsSince1900 - 2208988800UL + _tzHours * 3600;

    // Sommerzeit korrekt anwenden
    if (isSummertime(year(t), month(t), day(t), hour(t))) t += 3600;

    _lastTime = t;
    return true;
}

bool NtpClient::isSummertime(int year, int month, int day, int hour) {
    // EU: last Sunday March -> last Sunday October
    if (month < 3 || month > 10) return false;
    if (month > 3 && month < 10) return true;

    auto lastSunday = [](int y, int m) -> int {
        int d;
        if (m == 3) d = 31 - ((5 * y / 4 + 4) % 7);
        else if (m == 10) d = 31 - ((5 * y / 4 + 1) % 7);
        else d = 0;
        return d;
    };

    int lastSundayMarch = lastSunday(year, 3);
    int lastSundayOct   = lastSunday(year,10);

    if (month == 3) return day > lastSundayMarch || (day == lastSundayMarch && hour >= 2);
    if (month == 10) return day < lastSundayOct || (day == lastSundayOct && hour < 3);

    return false;
}

void NtpClient::sendNTPpacket() {
    memset(_packetBuffer, 0, NTP_PACKET_SIZE);
    _packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    _packetBuffer[1] = 0;
    _packetBuffer[2] = 6;
    _packetBuffer[3] = 0xEC;
    _packetBuffer[12] = 49;
    _packetBuffer[13] = 0x4E;
    _packetBuffer[14] = 49;
    _packetBuffer[15] = 52;

    _udp.beginPacket(_timeServer, 123);
    _udp.write(_packetBuffer, NTP_PACKET_SIZE);
    _udp.endPacket();
}
