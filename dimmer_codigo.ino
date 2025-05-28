// DEFINIÇÕES DE PINOS
#define pinZC  D6  // GPIO12
#define pinDIM D5  // GPIO14

#define intervaloLeitura 1000
#define periodo 8333  // Meia onda (60Hz) em microssegundos

#define iMin 20
#define iMax 90

volatile int intensidade = 0;
volatile bool zcDetected = false;
unsigned long controleTempo;

void ICACHE_RAM_ATTR sinalZC() {
  zcDetected = true;
}

void ICACHE_RAM_ATTR triggerDimmer() {
  digitalWrite(pinDIM, HIGH);
  delayMicroseconds(6);  // Pulso do gate do TRIAC
  digitalWrite(pinDIM, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(pinDIM, OUTPUT);
  pinMode(pinZC, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinZC), sinalZC, RISING);

  timer1_isr_init();
  timer1_attachInterrupt(triggerDimmer);
  timer1_disable();  // Só liga quando necessário

  intensidade = 50;
  controleTempo = millis();

  Serial.println("Setup concluído.");
}

void loop() {
  if (millis() - controleTempo > intervaloLeitura) {
    controleTempo = millis();

    // Exemplo fixo: intensidade constante
    intensidade = 50;
  }

  if (zcDetected) {
    zcDetected = false;

    if (intensidade < iMin) return;
    if (intensidade > iMax) intensidade = iMax;

    int delayInt = periodo - (intensidade * (periodo / 100));

    // Configura o Timer1 para disparar após delayInt microssegundos
    timer1_write((delayInt) * 5); // timer1_write usa ciclos de clock (1 ciclo ≈ 0.2µs)
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  }
}
