#include <Arduino.h>
#include "pins.h"

static bool isPressed() { return digitalRead(PIN_BOOT_AUTO) == (BOOT_ACTIVE_LOW ? LOW : HIGH); }

void setup() {
  Serial.begin(115200); delay(300);
  pinMode(PIN_BOOT_AUTO, INPUT_PULLUP);
  Serial.println("ZLX-ESP32-2 (C5) BOOT button example");
  Serial.println("GPIO28, active LOW. Hold 5 seconds to trigger the example event.");
}

void loop() {
  static bool oldState = false; static uint32_t start = 0; static bool fired = false;
  bool now = isPressed();
  if (now && !oldState) { start = millis(); fired = false; Serial.println("BOOT pressed"); }
  if (now && !fired && millis() - start >= 5000) { fired = true; Serial.println("BOOT held 5s: long-hold event detected"); }
  if (!now && oldState) { Serial.printf("BOOT released after %lu ms\n", (unsigned long)(millis()-start)); }
  oldState = now; delay(10);
}
