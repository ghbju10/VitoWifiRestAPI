#pragma once
#include <Arduino.h>
#include <WiFiClient.h>
#include <TimeLib.h>

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};

class Logger {
public:
    Logger(Stream& serial);

    void setClient(WiFiClient* client);
    void println(const String& msg, LogLevel level = LOG_INFO);
    void print(const String& msg, LogLevel level = LOG_INFO);
    void printf(LogLevel level, const char* format, ...);

    void enableSerial(bool enable);
    void enableNetwork(bool enable);
    void enableTimestamp(bool enable);
    void setLogLevel(LogLevel level);

private:
    Stream& _serial;
    WiFiClient* _client = nullptr;

    bool _useSerial = true;
    bool _useNetwork = false;
    bool _useTimestamp = false;
    LogLevel _minLevel = LOG_DEBUG;

    void send(const String& msg, LogLevel level);
    String getTimestamp();
    String levelToString(LogLevel level);
};
