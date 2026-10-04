#include <Arduino.h>
#include <cmath>
#include "pins.h"
#include "config.h"

static int estimatePercent(float v) {
  struct P { float v; int p; };
  static constexpr P c[] = {{3.20f,0},{3.40f,8},{3.55f,15},{3.65f,25},{3.72f,35},{3.77f,45},
                            {3.82f,55},{3.87f,65},{3.93f,75},{4.00f,84},{4.10f,94},{4.20f,100}};
  if (v <= c[0].v) return 0; size_t n = sizeof(c)/sizeof(c[0]); if (v >= c[n-1].v) return 100;
  for (size_t i=1;i<n;++i) if (v <= c[i].v) {
    float f=(v-c[i-1].v)/(c[i].v-c[i-1].v);
    return constrain(int(lroundf(c[i-1].p + f*(c[i].p-c[i-1].p))),0,100);
  }
  return 0;
}

static void sampleBattery() {
  analogRead(PIN_BAT_ADC); delayMicroseconds(250);
  uint32_t rawSum=0; uint64_t mvSum=0;
  for (uint16_t i=0;i<BAT_ADC_SAMPLE_COUNT;++i) {
    rawSum += analogRead(PIN_BAT_ADC); mvSum += analogReadMilliVolts(PIN_BAT_ADC);
    delayMicroseconds(BAT_ADC_SAMPLE_DELAY_US);
  }
  uint16_t raw = rawSum / BAT_ADC_SAMPLE_COUNT;
  uint32_t mv = mvSum / BAT_ADC_SAMPLE_COUNT;
  float gpioV = mv / 1000.0f;
  float batV = gpioV * BAT_ADC_DIVIDER_RATIO * BAT_ADC_CALIBRATION;
  bool present = batV >= 2.50f;
  Serial.printf("raw=%u  GPIO1=%lumV  battery=%.3fV  present=%s  estimate=%d%%\n",
                raw, (unsigned long)mv, batV, present ? "yes" : "no", present ? estimatePercent(batV) : 0);
}

void setup() {
  Serial.begin(115200); delay(300);
  pinMode(PIN_BAT_ADC, INPUT); analogReadResolution(12); analogSetPinAttenuation(PIN_BAT_ADC, ADC_11db);
  Serial.println("Battery divider: BAT+ -> 100k -> GPIO1 -> 100k -> GND");
  Serial.println("Battery voltage ~= GPIO1 voltage x 2 x calibration factor");
}
void loop() { sampleBattery(); delay(1000); }
