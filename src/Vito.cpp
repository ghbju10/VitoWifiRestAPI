#include "Vito.h"

// Liste aller Datapoints
VitoDatapoint Vito::datapoints[] = {
    VitoDatapoint(  "aussenTemp", 0x0800, 2, VitoWiFi::div10, 
                    // UDP
                    true, "VITO_TEMP_AUSSEN:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_AUSSEN,node_location=TEMP_KG_TR_Vito temperatur=<v>"
                ),
    VitoDatapoint("aussenTempTiefpass", 0x5525, 2, VitoWiFi::div10),
    VitoDatapoint("aussenTempGedaempft", 0x5527, 2, VitoWiFi::div10),
    VitoDatapoint("abgasTemp", 0x0808, 2, VitoWiFi::div10,
                    // UDP
                    true, "VITO_TEMP_Abgas_Ist:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_Abgas_Ist,node_location=TEMP_KG_TR_Vito temperatur=<v>"
                ),
    VitoDatapoint("kesselIstTemp", 0x0802, 2, VitoWiFi::div10,
                    // UDP
                    true, "VITO_TEMP_Kessel_Ist:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_Kessel_Ist,node_location=TEMP_KG_TR_Vito temperatur=<v>"              
                ),
    VitoDatapoint("kesselIstTempTiefpass", 0x0810, 2, VitoWiFi::div10),
    VitoDatapoint(  "kesselSollTemp", 0x555A, 2, VitoWiFi::div10,
                    // UDP
                    true, "VITO_TEMP_Kessel_Soll:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_Kessel_Soll,node_location=TEMP_KG_TR_Vito temperatur=<v>"                     
                ),
    VitoDatapoint(  "warmwasserSollTemp", 0x6300, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, 5, 60, 
                    // UDP
                    true, "VITO_TEMP_WW_Soll:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_WW_Soll,node_location=TEMP_KG_TR_Vito temperatur=<v>"  
                ),
    VitoDatapoint(  "warmwasserIstTemp", 0x0804, 2, VitoWiFi::div10, 
                    // UDP
                    true, "VITO_TEMP_WW_Ist:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_WW_Ist,node_location=TEMP_KG_TR_Vito temperatur=<v>"
                ),
    VitoDatapoint(  "warmwasserSpeicherIstTemp", 0x0812, 2, VitoWiFi::div10),
    VitoDatapoint(  "vorlaufSollTempM2", 0x3544, 2, VitoWiFi::div10,
                    // UDP
                    true, "VITO_TEMP_VorlaufSollM2:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_VorlaufSollM2,node_location=TEMP_KG_TR_Vito temperatur=<v>"
                ),
    VitoDatapoint(  "vorlaufSollTempM3", 0x4544, 2, VitoWiFi::div10,
                    // UDP
                    true, "VITO_TEMP_VorlaufSollM3:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_VorlaufSollM3,node_location=TEMP_KG_TR_Vito temperatur=<v>"
                ),
    VitoDatapoint(  "vorlaufIstTempM2", 0x3900, 2, VitoWiFi::div10,
                    // UDP
                    true, "VITO_TEMP_VorlaufIstM2:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_VorlaufIstM2,node_location=TEMP_KG_TR_Vito temperatur=<v>"
                ),
    VitoDatapoint(  "vorlaufIstTempM3", 0x4900, 2, VitoWiFi::div10,
                    // UDP
                    true, "VITO_TEMP_VorlaufIstM3:<v>",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_TEMP_VorlaufIstM3,node_location=TEMP_KG_TR_Vito temperatur=<v>"
                ),
    VitoDatapoint(  "ruecklaufIstTempM1", 0x080A, 2, VitoWiFi::div10),
    VitoDatapoint(  "ruecklaufIstTempM2", 0x3902, 2, VitoWiFi::div10),
    VitoDatapoint(  "ruecklaufIstTempM3", 0x4902, 2, VitoWiFi::div10),
    VitoDatapoint(  "raumSollTempM1", 0x2306, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, 5, 25, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "raumSollTempM2", 0x3306, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, 5, 25, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "raumSollTempM3", 0x4306, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, 5, 25, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "neigungM1", 0x27D3, 1, VitoWiFi::noconv),
    VitoDatapoint(  "neigungM2", 0x37D3, 1, VitoWiFi::noconv),
    VitoDatapoint(  "neigungM3", 0x47D3, 1, VitoWiFi::noconv),
    VitoDatapoint(  "niveauM1", 0x27D4, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, -13, 40, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "niveauM2", 0x37D4, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, -13, 40, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "niveauM3", 0x47D4, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, -13, 40, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "nachladeunterdrueckung", 0x6551, 1, VitoWiFi::noconv),
    VitoDatapoint("speicherladepumpe", 0x6513, 1, VitoWiFi::noconv),
    // Betriebsart 0 = Abschaltbetrieb / 1 = nur Warmwasser / 2 = Heizung + Warmwasser
    VitoDatapoint(  "betriebsartM1", 0x2323, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, 0, 2, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "betriebsartM2", 0x3323, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, 0, 2, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "betriebsartM3", 0x4323, 1, VitoWiFi::noconv, 
                    // WRITABLE
                    true, 0, 2, 
                    // UDP
                    false, "",
                    // MQTT
                    false, ""
                ),
    VitoDatapoint(  "sparbetrieb", 0x3302, 1, VitoWiFi::noconv),
    VitoDatapoint(  "partybetrieb", 0x3303, 1, VitoWiFi::noconv),
    VitoDatapoint(  "brennerstoerung", 0x0883, 1, VitoWiFi::noconv),
    VitoDatapoint(  "flamme", 0x55D3, 1, VitoWiFi::noconv),
    VitoDatapoint(  "umschaltventil", 0x0A10, 1, VitoWiFi::noconv),
    VitoDatapoint(  "umwaelzpumpe", 0x7660, 1, VitoWiFi::noconv),
    VitoDatapoint(  "umwaelzpumpeDrehzahl", 0x0A3C, 1, VitoWiFi::noconv),
    VitoDatapoint(  "heizkreispumpe", 0x3906, 1, VitoWiFi::noconv),
    VitoDatapoint(  "brennerstarts", 0x088A, 4, VitoWiFi::noconv, 
                    // UDP
                    false, "",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_Brennerstarts,node_location=TEMP_KG_TR_Vito brennerstarts=<v>"
                ),
    VitoDatapoint(  "brennerstunden", 0x08A7, 4, VitoWiFi::noconv, 
                    // UDP
                    false, "",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_Brennerstunden,node_location=TEMP_KG_TR_Vito brennerstunden=<v>"
                ),
    VitoDatapoint(  "kesselLeistung Ist", 0xA38F, 1, VitoWiFi::noconv, 
                    // UDP
                    false, "",
                    // MQTT
                    true, "ha_data,node=VITO,sensor_typ=VITO_Leistung_Ist,node_location=TEMP_KG_TR_Vito leistung_prozent=<v>"
                )
};

const uint8_t Vito::datapointCount = sizeof(datapoints) / sizeof(datapoints[0]);

// Konstruktor
Vito::Vito(HardwareSerial* serial, int rx_pin, int tx_pin, Logger* log)
: serial(serial), logger(log), vitoWiFi(serial)
{
    // UART initialisieren
    serial->begin(19200, SERIAL_8N1, rx_pin, tx_pin);

    // Vitowifi starten
    vitoWiFi.begin();

    // Callbacks setzen
    vitoWiFi.onResponse([&](const VitoWiFi::PacketVS2& resp, const VitoWiFi::Datapoint& req){
        this->handleResponse(resp, req);
    });

    vitoWiFi.onError([&](VitoWiFi::OptolinkResult error, const VitoWiFi::Datapoint& req){
        this->handleError(error, req);
    });
}

// Zugriff auf Datapoints
VitoDatapoint* Vito::getDatapoints() {
    return datapoints;
}

uint8_t Vito::getDatapointCount() {
    return datapointCount;
}

// Loop
void Vito::loop() {
    if (!this->writeActive){
        vitoWiFi.loop();
        readAll();
    }
}

// Callbacks
void Vito::handleResponse(const VitoWiFi::PacketVS2& response, const VitoWiFi::Datapoint& request) {

    if (response.dataLength() == 0 || response.data() == nullptr) {
        logger->println("Empty response received (likely write ACK)", LOG_DEBUG);
        return;
    }

    logger->println("Raw data received:", LOG_DEBUG);
    const uint8_t* data = response.data();
    for (uint8_t i = 0; i < response.dataLength(); ++i)
        logger->printf(LOG_DEBUG, " %02x\r\n", data[i]);

    if (response.packetType() == VitoWiFi::PacketType::ERROR) {
        logger->printf(LOG_WARN, "Request %s returned error\r\n", request.name());
        return;
    }

    if (request.converter() == VitoWiFi::div10) {
        float value = request.decode(response);
        logger->printf(LOG_INFO, "%s %.1f\r\n", request.name(), value);
        VitoDatapoint* dp = getDatapointByName(request.name());
        if (dp) {
            dp->setLastValue(value);
        }
    } else if (request.converter() == VitoWiFi::noconv) {
        if (request.length() == 1){
            uint8_t value = request.decode(response);
            VitoDatapoint* dp = getDatapointByName(request.name());
            if (dp) {
                dp->setLastValue(value);
            }
            logger->printf(LOG_INFO, "%s %u\r\n", request.name(), value);
        } else if (request.length() == 4){
            uint32_t value = request.decode(response);
            VitoDatapoint* dp = getDatapointByName(request.name());
            if (dp) {
                dp->setLastValue(value);
            }
            logger->printf(LOG_INFO, "%s %u\r\n", request.name(), value);
        }
    }     
}

void Vito::handleError(VitoWiFi::OptolinkResult error, const VitoWiFi::Datapoint& request) {
    logger->printf(LOG_WARN, "Datapoint \"%s\" error: ", request.name());
    switch (error) {
        case VitoWiFi::OptolinkResult::TIMEOUT: logger->println("timeout"); break;
        case VitoWiFi::OptolinkResult::LENGTH:  logger->println("length"); break;
        case VitoWiFi::OptolinkResult::NACK:    logger->println("nack"); break;
        case VitoWiFi::OptolinkResult::CRC:     logger->println("crc"); break;
        case VitoWiFi::OptolinkResult::ERROR:   logger->println("error"); break;
    }
}

// Polling & Startup Delay
void Vito::readAll() {
    unsigned long now = millis();

    // Startup-Delay prüfen
    if (!startupComplete) {
        if (startupMillis == 0) startupMillis = now;
        if (now - startupMillis < startupDelay) return;
        startupComplete = true;
        datapointIndex = 0;
        readValues = true;
    }

    // Polling-Intervall prüfen
    if (!readValues && (now - lastMillis > pollInterval)) {
        lastMillis = now;
        readValues = true;
        datapointIndex = 0;
    }

    // Datapoints lesen
    if (readValues && datapointIndex < datapointCount) {
        if (vitoWiFi.read(datapoints[datapointIndex].getDatapoint())) {
            logger->printf(LOG_DEBUG, "readDatapoints[%d] done\r\n", datapointIndex);
            datapointIndex++;
        }
        if (datapointIndex >= datapointCount) readValues = false;
    }
}

VitoDatapoint* Vito::getDatapointByName(const char* name) {
    for (auto& dp : datapoints) {
        if (strcmp(dp.getDatapoint().name(), name) == 0) {
            return &dp;
        }
    }
    return nullptr; 
}

bool Vito::write(VitoDatapoint* dp, float value) {

    this->writeActive = true;

    if (!dp) {
        logger->println("write(): datapoint is null", LOG_ERROR);
        this->writeActive = false;
        return false;
    }

    VitoWiFi::Datapoint& vitoWifiDp = dp->getDatapoint();

    // Schreibbarkeit prüfen
    if (!dp->getIsWritable()) {
        logger->printf(LOG_WARN, "WRITE blocked: %s is read-only\r\n", vitoWifiDp.name());
        this->writeActive = false;
        return false;
    }

    // Min/Max prüfen
    if (value < dp->getMinValue() || value > dp->getMaxValue()) {
        logger->printf(LOG_WARN, "WRITE blocked: %s value %.2f out of range (%.2f - %.2f)\r\n",
                       vitoWifiDp.name(), value, dp->getMinValue(), dp->getMaxValue());
        this->writeActive = false;
        return false;
    }

    // Type handling
    switch (vitoWifiDp.length()) {

        case 1: {
            uint8_t newValue = 0;
            if (value>=0){
                newValue = value;
            } else{
                newValue = 256 + value; 
            }
            uint8_t v = static_cast<uint8_t>(newValue); 
                       
            bool result = vitoWiFi.write(vitoWifiDp, v);
            logger->printf(LOG_INFO, "WRITE (1) uint8_t %s = %.2f\r\n", vitoWifiDp.name(), value);
            this->writeActive = false;
            return result;
        }

        case 2: {
            if (vitoWifiDp.converter() == VitoWiFi::div10) {
                uint16_t v = static_cast<uint16_t>(value * 10);
                bool result = vitoWiFi.write(vitoWifiDp, v);
                logger->printf(LOG_INFO, "WRITE (2) VitoWiFi::div10 %s = %.2f\r\n", vitoWifiDp.name(), value);
                this->writeActive = false;
                return result;
            } else {
                uint16_t v = static_cast<uint16_t>(value);
                bool result = vitoWiFi.write(vitoWifiDp, v);
                logger->printf(LOG_INFO, "WRITE (2) uint16_t %s = %.2f\r\n", vitoWifiDp.name(), value);
                this->writeActive = false;
                return result;
            }
        }

        case 4: {
            uint32_t v = static_cast<uint32_t>(value);
            bool result = vitoWiFi.write(vitoWifiDp, v);
            logger->printf(LOG_INFO, "WRITE (4) uint32_t %s = %.2f\r\n", vitoWifiDp.name(), value);
            this->writeActive = false;
            return result;
        }
    }

    logger->printf(LOG_ERROR, "WRITE failed: unsupported length %d\r\n", vitoWifiDp.length());
    this->writeActive = false;
    return false;
}

bool Vito::writeByName(const char* name, float value) {
    if (!name) {
        logger->printf(LOG_ERROR, "writeByName: name is NULL\r\n");
        return false;
    }

    auto dp = this->getDatapointByName(name);

    if (!dp) {
        logger->printf(LOG_ERROR,
            "writeByName: datapoint NOT FOUND: %s\r\n",
            name
        );
        return false;
    }

    logger->printf(LOG_DEBUG,
        "writeByName OK: %s = %.2f\r\n",
        name, value
    );

    return this->write(dp, value);    
}