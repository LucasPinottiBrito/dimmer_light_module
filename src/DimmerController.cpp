#include "DimmerController.h"

static uint8_t zcPin;
static uint8_t triacPin;

static const unsigned long DEBOUNCE_US = 8000UL;
static const unsigned long SEMICYCLE_US = 10000UL;
static const unsigned int MIN_DELAY = 2000;
static const unsigned int MAX_DELAY = 8000;

static volatile unsigned int dim_delay_us = 5000;
static volatile bool skipHalf = false;
static volatile unsigned long lastEdgeTime = 0;

#define US_TO_TICKS(us) ((us) * 80UL)

void ICACHE_RAM_ATTR fireTriac() {
  digitalWrite(triacPin, HIGH);
  delayMicroseconds(80);
  digitalWrite(triacPin, LOW);
}

void ICACHE_RAM_ATTR zeroCrossISR() {
  unsigned long now = micros();
  unsigned long dt = now - lastEdgeTime;
  if (dt < DEBOUNCE_US) return;
  lastEdgeTime = now;

  skipHalf = !skipHalf;
  if (skipHalf) return;

  timer1_disable();
  timer1_write(US_TO_TICKS(dim_delay_us));
  timer1_enable(TIM_DIV1, TIM_EDGE, TIM_SINGLE);
}

void dimmerSetup(uint8_t zeroCrossPin, uint8_t triacPin_) {
  zcPin = zeroCrossPin;
  triacPin = triacPin_;

  pinMode(zcPin, INPUT);
  pinMode(triacPin, OUTPUT);
  digitalWrite(triacPin, LOW);

  attachInterrupt(digitalPinToInterrupt(zcPin), zeroCrossISR, RISING);

  timer1_isr_init();
  timer1_attachInterrupt(fireTriac);
  timer1_disable();
}

void dimmerSetIntensity(uint8_t percent) {
  if (percent > 100) percent = 100;
  dim_delay_us = map(percent, 0, 100, MAX_DELAY, MIN_DELAY);
}
