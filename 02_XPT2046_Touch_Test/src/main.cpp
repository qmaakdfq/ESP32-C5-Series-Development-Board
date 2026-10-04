#include <Arduino.h>
#include <TFT_eSPI.h>
#include "pins.h"
#include "config.h"

TFT_eSPI tft;

struct TouchRaw { int16_t x = 0, y = 0, z = 0; };
struct TouchPoint { int16_t x = 0, y = 0; bool valid = false; };

static uint8_t transferByte(uint8_t value) {
  uint8_t result = 0;
  uint32_t delayUs = 500000UL / TOUCH_SPI_HZ;
  if (delayUs < 1) delayUs = 1;
  for (int bit = 7; bit >= 0; --bit) {
    digitalWrite(PIN_RTP_DIN, (value >> bit) & 1);
    delayMicroseconds(delayUs);
    digitalWrite(PIN_RTP_SCK, HIGH);
    result = (result << 1) | digitalRead(PIN_RTP_DOUT);
    delayMicroseconds(delayUs);
    digitalWrite(PIN_RTP_SCK, LOW);
  }
  return result;
}

static uint16_t read12(uint8_t command) {
  transferByte(command);
  uint16_t value = (uint16_t(transferByte(0)) << 8) | transferByte(0);
  return (value >> 3) & 0x0FFF;
}

static bool pressed() { return digitalRead(PIN_RTP_IRQ) == LOW; }

static TouchRaw readTouchRaw() {
  TouchRaw r;
  if (!pressed()) return r;
  long sx = 0, sy = 0, sz = 0;
  for (int i = 0; i < 5; ++i) {
    digitalWrite(PIN_RTP_CS, LOW);
    uint16_t x = read12(0xD0), y = read12(0x90);
    uint16_t z1 = read12(0xB0), z2 = read12(0xC0);
    digitalWrite(PIN_RTP_CS, HIGH);
    sx += x; sy += y; sz += z1 + 4095 - z2;
  }
  r.x = sx / 5; r.y = sy / 5; r.z = (sz > 0) ? (sz / 5) : 0;
  return r;
}

static TouchPoint mapTouch(const TouchRaw& r) {
  TouchPoint p;
  if (r.z < TOUCH_MIN_PRESSURE) return p;
  int32_t ax = TOUCH_SWAP_XY ? r.y : r.x;
  int32_t ay = TOUCH_SWAP_XY ? r.x : r.y;
  long x = map(ax, TOUCH_RAW_X_MIN, TOUCH_RAW_X_MAX, 0, LCD_WIDTH - 1);
  long y = map(ay, TOUCH_RAW_Y_MIN, TOUCH_RAW_Y_MAX, 0, LCD_HEIGHT - 1);
  x = constrain(x, 0, LCD_WIDTH - 1); y = constrain(y, 0, LCD_HEIGHT - 1);
  if (TOUCH_INVERT_X) x = LCD_WIDTH - 1 - x;
  if (TOUCH_INVERT_Y) y = LCD_HEIGHT - 1 - y;
  p.x = x; p.y = y; p.valid = true;
  return p;
}

static void clearCanvas() {
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(5, 35, 310, 200, TFT_CYAN);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("XPT2046 Touch Test - draw inside box", 10, 12);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LCD_BL, OUTPUT); digitalWrite(PIN_LCD_BL, HIGH);
  pinMode(PIN_RTP_SCK, OUTPUT); pinMode(PIN_RTP_DIN, OUTPUT);
  pinMode(PIN_RTP_DOUT, INPUT); pinMode(PIN_RTP_CS, OUTPUT);
  pinMode(PIN_RTP_IRQ, INPUT_PULLUP); digitalWrite(PIN_RTP_CS, HIGH);
  digitalWrite(PIN_RTP_SCK, LOW);
  tft.init(); tft.setRotation(LCD_ROTATION); tft.invertDisplay(LCD_INVERT);
  clearCanvas();
  Serial.println("Touch: DOUT=3 SCK=4 DIN=5 CS=23 IRQ=25");
}

void loop() {
  static uint32_t last = 0;
  if (pressed() && millis() - last >= 12) {
    last = millis();
    TouchRaw raw = readTouchRaw();
    TouchPoint p = mapTouch(raw);
    if (p.valid) {
      tft.fillCircle(p.x, p.y, 2, TFT_GREEN);
      Serial.printf("raw=(%d,%d) z=%d -> xy=(%d,%d)\n", raw.x, raw.y, raw.z, p.x, p.y);
    }
  }
}
