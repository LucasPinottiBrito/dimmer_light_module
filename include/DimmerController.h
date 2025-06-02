#pragma once
#include <Arduino.h>

void dimmerSetup(uint8_t zeroCrossPin, uint8_t triacPin);
void dimmerSetIntensity(uint8_t percent);
