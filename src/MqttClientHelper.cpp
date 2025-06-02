#include "MqttClientHelper.h"

MqttClientHelper::MqttClientHelper(const char* host, int port, const char* user, const char* pass)
    : mqtt_host(host), mqtt_port(port), mqtt_user(user), mqtt_pass(pass),
      mqttClient(wifiClient), mqtt_clientId(nullptr) {
    wifiClient.setInsecure();
}

void MqttClientHelper::setup(const char* clientId) {
    mqtt_clientId = clientId;
    mqttClient.setServer(mqtt_host, mqtt_port);
    reconnect();
}

void MqttClientHelper::loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();
}

bool MqttClientHelper::publish(const char* topic, const char* payload) {
    return mqttClient.publish(topic, payload);
}

bool MqttClientHelper::isConnected() {
    return mqttClient.connected();
}

void MqttClientHelper::disconnect() {
    mqttClient.disconnect();
}

void MqttClientHelper::reconnect() {
    while (!mqttClient.connected()) {
        if (mqttClient.connect(mqtt_clientId, mqtt_user, mqtt_pass)) {
            Serial.println("MQTT conectado!");
        } else {
            Serial.print("Falha na conexão MQTT, rc=");
            Serial.println(mqttClient.state());
            delay(3000);
        }
    }
}

void MqttClientHelper::subscribe(const char* topic) {
    mqttClient.subscribe(topic);
}

void MqttClientHelper::setCallback(MQTT_CALLBACK_SIGNATURE) {
    mqttClient.setCallback(callback);
}