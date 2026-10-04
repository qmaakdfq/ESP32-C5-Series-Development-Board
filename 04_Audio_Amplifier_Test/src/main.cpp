#include <Arduino.h>
#include "pins.h"
#include "config.h"

static bool pwmAttached = false;
static void amp(bool on) { digitalWrite(PIN_AMP_EN, on == AMP_EN_ACTIVE_HIGH ? HIGH : LOW); }

static bool pwmStart() {
  if (pwmAttached) return true;
  if (!ledcAttach(PIN_AUDIO_PWM, AUDIO_PWM_CARRIER_HZ, AUDIO_PWM_BITS)) return false;
  pwmAttached = true; ledcWrite(PIN_AUDIO_PWM, 0); return true;
}

static void stopAudio() {
  if (pwmAttached) {
    ledcWriteTone(PIN_AUDIO_PWM, 0); ledcWrite(PIN_AUDIO_PWM, 0);
    ledcDetach(PIN_AUDIO_PWM); pwmAttached = false;
  }
  pinMode(PIN_AUDIO_PWM, OUTPUT); digitalWrite(PIN_AUDIO_PWM, LOW); amp(false);
}

static void runTest() {
  if (!pwmStart()) { Serial.println("LEDC attach failed"); return; }
  amp(true);
  Serial.println("1kHz tone for 1 second");
  ledcWriteTone(PIN_AUDIO_PWM, 1000); delay(1000);
  Serial.println("Sweep 200Hz -> 4kHz");
  for (int f = 200; f <= 4000; f += 30) { ledcWriteTone(PIN_AUDIO_PWM, f); delay(12); }
  stopAudio(); Serial.println("Audio test complete");
}

void setup() {
  Serial.begin(115200); delay(300);
  pinMode(PIN_AMP_EN, OUTPUT); pinMode(PIN_AUDIO_PWM, OUTPUT);
  stopAudio();
  Serial.println("Audio: GPIO8 PWM -> SC8002B, AMP_EN=GPIO24");
  runTest();
}

void loop() {}
