#include "VitoDatapoint.h"

// Konstruktor mit Referenz auf Converter
VitoDatapoint::VitoDatapoint(const char* name, uint16_t address, uint8_t length, const VitoWiFi::Converter& conv, bool isWritable, float minValue, float maxValue, bool sendUdp, String udpTemplate, bool sendMqtt, String mqttTemplate)
: datapoint(name, address, length, conv) { 
    setIsWritable(isWritable);
    setMinValue(minValue);
    setMaxValue(maxValue);
    setSendUdp(sendUdp);
    setUdpTemplate(udpTemplate);
    setSendMqtt(sendMqtt);
    setMqttTemplate(mqttTemplate);
}

VitoDatapoint::VitoDatapoint(const char* name, uint16_t address, uint8_t length, const VitoWiFi::Converter& conv)
: datapoint(name, address, length, conv) { 
    setIsWritable(false);
    setMinValue(0.0);
    setMaxValue(0.0);
    setSendUdp(false);
    setSendMqtt(false);
}

VitoDatapoint::VitoDatapoint(const char* name, uint16_t address, uint8_t length, const VitoWiFi::Converter& conv, bool sendUdp, String udpTemplate, bool sendMqtt, String mqttTemplate)
: datapoint(name, address, length, conv) { 
    setIsWritable(false);
    setMinValue(0.0);
    setMaxValue(0.0);
    setSendUdp(sendUdp);
    setUdpTemplate(udpTemplate);
    setSendMqtt(sendMqtt);
    setMqttTemplate(mqttTemplate);
}

VitoWiFi::Datapoint& VitoDatapoint::getDatapoint() {
    return datapoint;
}

void VitoDatapoint::setLastValue(float value) {
    lastValue = value;
}

float VitoDatapoint::getLastValue() const {
    return lastValue;
}

void VitoDatapoint::setIsWritable(bool value) {
    isWritable = value;
}

bool VitoDatapoint::getIsWritable() const {
    return isWritable;
}

void VitoDatapoint::setMinValue(float value) {
    minValue = value;
}

float VitoDatapoint::getMinValue() const {
    return minValue;
}

void VitoDatapoint::setMaxValue(float value) {
    maxValue = value;
}

float VitoDatapoint::getMaxValue() const {
    return maxValue;
}

void VitoDatapoint::setSendUdp(bool value) {
    sendUdp = value;
}

bool VitoDatapoint::getSendUdp() const {
    return sendUdp;
}

void VitoDatapoint::setUdpTemplate(String value) {
    udpTemplate = value;
}

String VitoDatapoint::getUdpTemplate() const {
    return udpTemplate;
}

void VitoDatapoint::setSendMqtt(bool value) {
    sendMqtt = value;
}

bool VitoDatapoint::getSendMqtt() const {
    return sendMqtt;
}

void VitoDatapoint::setMqttTemplate(String value) {
    mqttTemplate = value;
}

String VitoDatapoint::getMqttTemplate() const {
    return mqttTemplate;
}
