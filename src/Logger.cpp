#include "Logger.h"
#include <stdarg.h>

Logger::Logger(Stream& serial) : _serial(serial) {}

void Logger::setClient(WiFiClient* client) {
    _client = client;
}

void Logger::enableSerial(bool enable) { _useSerial = enable; }
void Logger::enableNetwork(bool enable) { _useNetwork = enable; }
void Logger::enableTimestamp(bool enable) { _useTimestamp = enable; }
void Logger::setLogLevel(LogLevel level) { _minLevel = level; }

void Logger::println(const String& msg, LogLevel level) {
    send(msg + "\r\n", level);
}

void Logger::print(const String& msg, LogLevel level) {
    send(msg, level);
}

void Logger::printf(LogLevel level, const char* format, ...) {
    if (level < _minLevel) return;

    va_list args;
    va_start(args, format);
    int size = vsnprintf(nullptr, 0, format, args);
    va_end(args);

    char* buf = new char[size + 1];
    va_start(args, format);
    vsnprintf(buf, size + 1, format, args);
    va_end(args);

    send(String(buf), level);
    delete[] buf;
}

void Logger::send(const String& msg, LogLevel level) {
    if (level < _minLevel) return;

    String output;
    if (_useTimestamp) output += getTimestamp() + " ";
    output += "[" + levelToString(level) + "] " + msg;

    if (_useSerial) _serial.print(output);

    if (_useNetwork && _client && _client->connected()) {
        size_t toSend = output.length();
        _client->write((const uint8_t*)output.c_str(), toSend);
    }
}

String Logger::getTimestamp() {
    char buf[20];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
            year(), month(), day(), hour(), minute(), second());
    return String(buf);
}

String Logger::levelToString(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        default: return "UNK";
    }
}
