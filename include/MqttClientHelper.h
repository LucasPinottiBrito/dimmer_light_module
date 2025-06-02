#pragma once
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

class MqttClientHelper {
public:
    MqttClientHelper(const char* host, int port, const char* user = nullptr, const char* pass = nullptr);
    void setup(const char* clientId);
    void loop();
    bool publish(const char* topic, const char* payload);
    bool isConnected();
    void subscribe(const char* topic);
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void disconnect();
    void reconnect();

private:
    const char* mqtt_host;
    int mqtt_port;
    const char* mqtt_user;
    const char* mqtt_pass;
    WiFiClientSecure wifiClient;
    PubSubClient mqttClient;
    const char* mqtt_clientId;
};