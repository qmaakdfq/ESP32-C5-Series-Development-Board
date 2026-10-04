#pragma once
#include <Arduino.h>

// ZLX-ESP32-2 (C5) / ESP32-C5-WROOM-1U-N8R8 GPIO map
static constexpr int PIN_GPS_TX       = 0;   // MCU RX <- GPS TX
static constexpr int PIN_BAT_ADC      = 1;
static constexpr int PIN_SD_MISO      = 2;
static constexpr int PIN_RTP_DOUT     = 3;
static constexpr int PIN_RTP_SCK      = 4;
static constexpr int PIN_RTP_DIN      = 5;
static constexpr int PIN_LCD_SCK      = 6;
static constexpr int PIN_LCD_MOSI     = 7;
static constexpr int PIN_AUDIO_PWM    = 8;
static constexpr int PIN_GPS_RX       = 9;   // MCU TX -> GPS RX
static constexpr int PIN_LCD_CS       = 10;
static constexpr int PIN_LCD_BL       = 13;
static constexpr int PIN_LCD_DC       = 14;
static constexpr int PIN_RTP_CS       = 23;
static constexpr int PIN_AMP_EN       = 24;
static constexpr int PIN_RTP_IRQ      = 25;
static constexpr int PIN_GREEN_LED    = 26;
static constexpr int PIN_SD_CS        = 27;
static constexpr int PIN_BOOT_AUTO    = 28;

static constexpr bool BOOT_ACTIVE_LOW      = true;
static constexpr bool AMP_EN_ACTIVE_HIGH   = true;
static constexpr bool GREEN_LED_ACTIVE_LOW = false; // HIGH = ON
static constexpr bool LCD_BL_ACTIVE_HIGH   = true;
