#include "Configuration.h"
#include "MqttDatapointSender.h"


MqttDatapointSender::MqttDatapointSender(Vito* vito, Logger* logger)
: vito(vito), logger(logger), mqtt(wifiClient) {}

void MqttDatapointSender::begin(
    const char* broker,
    uint16_t port,
    const char* clientId,
    const char* user,
    const char* password
) {
    this->broker = broker;
    this->port = port;
    this->clientId = clientId;
    this->user = user;
    this->password = password;

    mqtt.setServer(broker, port);
}

void MqttDatapointSender::loop() {
    if (!mqtt.connected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = now;
            reconnect();
        }
    } else {
        mqtt.loop();
    }
}

bool MqttDatapointSender::reconnect() {
    logger->println("MQTT reconnect...", LOG_INFO);

    bool ok;
    if (user && strlen(user) > 0) {
        ok = mqtt.connect(clientId, user, password);
    } else {
        ok = mqtt.connect(clientId);
    }

    if (ok) {
        logger->println("MQTT connected", LOG_INFO);
    } else {
        logger->printf(LOG_WARN, "MQTT failed, rc=%d\r\n", mqtt.state());
    }

    return ok;
}

bool MqttDatapointSender::isConnected() {
    return mqtt.connected();
}

void MqttDatapointSender::sendAllDatapoints() {
    if (!vito || !mqtt.connected()) return;

    VitoDatapoint* dps = vito->getDatapoints();
    uint8_t count = vito->getDatapointCount();

    for (uint8_t i = 0; i < count; i++) {
        if (!dps[i].getSendMqtt()) continue;

        const char* name = dps[i].getDatapoint().name();
        float value = dps[i].getLastValue();

        String topic = String(MQTT_TOPIC) + "/" + String(name);

        String mqtt_template = dps[i].getMqttTemplate();
        mqtt_template.replace("<v>", String(value,2));
        char payload[mqtt_template.length()+1];
        mqtt_template.toCharArray(payload, sizeof(payload));

        bool ok = mqtt.publish(topic.c_str(), payload, true);

        logger->printf(
            ok ? LOG_DEBUG : LOG_WARN,
            "MQTT publish %s => %s\r\n",
            topic.c_str(),
            payload
        );

        delay(2);
    }
}

String MqttDatapointSender::applyTemplate(
    const String& tpl,
    const char* name,
    float value
) {
    String out = tpl;
    out.replace("{name}", name);
    out.replace("{value}", String(value, 2));
    return out;
}
