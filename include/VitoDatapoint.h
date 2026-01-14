#ifndef VITO_DATAPOINT_H
#define VITO_DATAPOINT_H

#include <VitoWiFi.h>
#include <string>

class VitoDatapoint {
public:
    // Referenz auf Converter verwenden
    VitoDatapoint(const char* name, uint16_t address, uint8_t length, const VitoWiFi::Converter& conv, bool isWritable, float minValue, float maxValue, bool sendUdp, String udpTemplate, bool sendMqtt, String mqttTemplate);

    VitoDatapoint(const char* name, uint16_t address, uint8_t length, const VitoWiFi::Converter& conv);

    VitoDatapoint(const char* name, uint16_t address, uint8_t length, const VitoWiFi::Converter& conv, bool sendUdp, String udpTemplate, bool sendMqtt, String mqttTemplate);

    VitoWiFi::Datapoint& getDatapoint();

    void setLastValue(float value);
    float getLastValue() const;

    void setIsWritable(bool value);
    bool getIsWritable() const;

    void setMinValue(float value);
    float getMinValue() const;

    void setMaxValue(float value);
    float getMaxValue() const;

    void setSendUdp(bool value);
    bool getSendUdp() const;  
    
    void setUdpTemplate(String value);
    String getUdpTemplate() const;      

    void setSendMqtt(bool value);
    bool getSendMqtt() const;  
    
    void setMqttTemplate(String value);
    String getMqttTemplate() const;   
private:
    VitoWiFi::Datapoint datapoint;
    std::string description;
    float lastValue = 0.0;
    bool isWritable = false; 
    float minValue = 0.0; 
    float maxValue = 0.0;
    bool sendUdp = false;
    String udpTemplate = "";
    bool sendMqtt = false;
    String mqttTemplate = "";
};

#endif
