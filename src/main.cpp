#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "DimmerController.h"
#include "MqttClientHelper.h"
#include "WiFiManagerHelper.h"

#define ZERO_CROSS_PIN 5  // D1
#define TRIAC_PIN      4  // D2

const String deviceId = "e00002";

MqttClientHelper mqttClient(
    "e519d15084bd448286ec5dd6851ab338.s2.eu.hivemq.cloud",
    8883,
    "Rfid_device",
    "Rfid_device1"
);

void setup() {
    Serial.begin(115200);
    WiFiManagerHelper::connect();
    dimmerSetup(ZERO_CROSS_PIN, TRIAC_PIN);

    mqttClient.setup(deviceId.c_str());
    mqttClient.subscribe("/smartroom/dimmer/control");

    mqttClient.setCallback([](char* topic, byte* payload, unsigned int length) {
        if (strcmp(topic, "/smartroom/dimmer/control") == 0) {
            char buffer[6] = {0};
            strncpy(buffer, (char*)payload, min(length, 5U));
            int pct = atoi(buffer);
            pct = constrain(pct, 0, 100);
            dimmerSetIntensity(pct);
            Serial.printf("Brilho ajustado: %d%%\n", pct);
        }
    });
}

void loop() {
    mqttClient.loop();
}