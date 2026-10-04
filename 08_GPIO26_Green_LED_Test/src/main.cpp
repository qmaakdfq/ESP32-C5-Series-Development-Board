#include <Arduino.h>
#include "pins.h"

static void setLed(bool on) {
  digitalWrite(PIN_GREEN_LED, on ? HIGH : LOW);
  Serial.printf("GPIO26=%s, green LED %s\n", on ? "HIGH" : "LOW", on ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200); delay(300);
  pinMode(PIN_GREEN_LED, OUTPUT);
  Serial.println("ZLX-ESP32-2 (C5) GPIO26 green LED example");
}

void loop() {
  setLed(true); delay(500);
  setLed(false); delay(500);
}
