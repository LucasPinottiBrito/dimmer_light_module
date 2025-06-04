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

// Modo que varia brilho aleatoriamente por 15s
void crazyMode() {
    Serial.println("🔀 Crazy Mode ON");
    unsigned long start = millis();

    while (millis() - start < 15000) {
        int pct = random(0, 101);
        dimmerSetIntensity(pct);
        Serial.printf("Crazy brightness: %d%%\n", pct);
        delay(1000);
    }

    Serial.println("🔀 Crazy Mode OFF");
}

// Modo que sobe de 0 a 100% em 10s
void raiseMode() {
    Serial.println("⏫ Raise Mode ON");

    const int duration = 5000;
    const int steps = 90;
    const int delayPerStep = duration / steps;

    for (int pct = 10; pct <= 100; pct++) {
        dimmerSetIntensity(pct);
        Serial.printf("Raise brightness: %d%%\n", pct);
        delay(delayPerStep);
    }

    Serial.println("⏫ Raise Mode OFF");
}

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

            if (pct == 43) {
                crazyMode();
                return;
            } else if (pct == 17) {
                raiseMode();
                return;
            }

            pct = constrain(pct, 0, 100);
            dimmerSetIntensity(pct);
            Serial.printf("Brilho ajustado: %d%%\n", pct);
        }
    });
}

void loop() {
    mqttClient.loop();
}