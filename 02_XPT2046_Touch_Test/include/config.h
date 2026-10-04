#pragma once
#include <Arduino.h>

static constexpr uint8_t LCD_ROTATION = 1;  // 320x240 landscape
static constexpr int LCD_WIDTH = 320;
static constexpr int LCD_HEIGHT = 240;
static constexpr bool LCD_INVERT = false;
static constexpr uint32_t LCD_SPI_HZ = 40000000;
static constexpr uint32_t SD_SPI_HZ = 4000000;

// XPT2046 fixed mapping used by the factory-test firmware.
static constexpr uint32_t TOUCH_SPI_HZ = 2000000;
static constexpr uint16_t TOUCH_MIN_PRESSURE = 70;
static constexpr bool TOUCH_SWAP_XY = true;
static constexpr bool TOUCH_INVERT_X = false;
static constexpr bool TOUCH_INVERT_Y = false;
static constexpr int32_t TOUCH_RAW_X_MIN = 220;
static constexpr int32_t TOUCH_RAW_X_MAX = 3880;
static constexpr int32_t TOUCH_RAW_Y_MIN = 220;
static constexpr int32_t TOUCH_RAW_Y_MAX = 3880;

static constexpr uint32_t AUDIO_PWM_CARRIER_HZ = 62500;
static constexpr uint8_t AUDIO_PWM_BITS = 8;
static constexpr uint32_t GPS_DEFAULT_BAUD = 9600;

// BAT+ -> 100k -> GPIO1 -> 100k -> GND
static constexpr float BAT_ADC_DIVIDER_RATIO = 2.0f;
static constexpr float BAT_ADC_CALIBRATION = 1.000f;
static constexpr uint16_t BAT_ADC_SAMPLE_COUNT = 32;
static constexpr uint16_t BAT_ADC_SAMPLE_DELAY_US = 250;
