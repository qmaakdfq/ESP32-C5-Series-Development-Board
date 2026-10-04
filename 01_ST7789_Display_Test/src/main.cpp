#include <Arduino.h>
#include <TFT_eSPI.h>
#include "pins.h"
#include "config.h"

TFT_eSPI tft;
int pattern = 0;
uint32_t nextPattern = 0;

static void backlight(bool on) {
  digitalWrite(PIN_LCD_BL, on == LCD_BL_ACTIVE_HIGH ? HIGH : LOW);
}

static void drawPattern(int p) {
  tft.fillScreen(TFT_BLACK);
  switch (p % 5) {
    case 0: {
      const uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_CYAN,
                                 TFT_MAGENTA, TFT_YELLOW, TFT_WHITE, TFT_BLACK};
      for (int i = 0; i < 8; ++i) tft.fillRect(i * 40, 0, 40, 240, colors[i]);
      break;
    }
    case 1:
      for (int x = 0; x < 320; ++x) {
        uint8_t g = map(x, 0, 319, 0, 255);
        tft.drawFastVLine(x, 0, 240, tft.color565(g, g, g));
      }
      break;
    case 2:
      for (int x = 0; x < 320; x += 20) tft.drawFastVLine(x, 0, 240, TFT_GREEN);
      for (int y = 0; y < 240; y += 20) tft.drawFastHLine(0, y, 320, TFT_GREEN);
      tft.drawLine(0, 0, 319, 239, TFT_RED);
      tft.drawLine(319, 0, 0, 239, TFT_RED);
      break;
    case 3:
      tft.fillScreen(TFT_NAVY);
      tft.fillCircle(70, 85, 42, TFT_YELLOW);
      tft.drawCircle(245, 85, 42, TFT_WHITE);
      tft.fillTriangle(35, 210, 100, 130, 135, 215, TFT_MAGENTA);
      tft.drawRoundRect(180, 135, 100, 65, 10, TFT_CYAN);
      tft.setTextColor(TFT_WHITE, TFT_NAVY);
      tft.setTextSize(2);
      tft.drawString("ST7789 320x240", 74, 12);
      break;
    case 4:
      tft.fillScreen(TFT_WHITE);
      tft.setTextColor(TFT_BLACK, TFT_WHITE);
      tft.setTextSize(2);
      tft.drawString("White Pixel Check", 65, 110);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LCD_BL, OUTPUT);
  backlight(false);
  tft.init();
  tft.setRotation(LCD_ROTATION);
  tft.invertDisplay(LCD_INVERT);
  backlight(true);
  drawPattern(pattern);
  nextPattern = millis() + 2500;
  Serial.println("ZLX-ESP32-2 (C5) ST7789 display example started");
}

void loop() {
  if ((int32_t)(millis() - nextPattern) >= 0) {
    pattern = (pattern + 1) % 5;
    drawPattern(pattern);
    nextPattern = millis() + 2500;
  }
}
