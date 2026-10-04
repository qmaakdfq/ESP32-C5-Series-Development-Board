# ZLX-ESP32-2 (C5) GPIO Pinout / GPIO 引脚表

| Function / 功能 | GPIO | Notes / 说明 |
|---|---:|---|
| GPS TX -> MCU RX | 0 | GT-U7 TX -> ESP32 RX |
| Battery ADC / 电池ADC | 1 | 100k/100k divider |
| TF MISO | 2 | LCD/TF shared SPI input |
| XPT2046 DOUT | 3 | Touch software SPI |
| XPT2046 SCK | 4 | Touch software SPI |
| XPT2046 DIN | 5 | Touch software SPI |
| LCD/TF SCK | 6 | Shared SPI clock |
| LCD/TF MOSI | 7 | Shared SPI MOSI |
| Audio PWM / 音频PWM | 8 | SC8002B input |
| MCU TX -> GPS RX | 9 | ESP32 TX -> GT-U7 RX |
| LCD CS | 10 | ST7789 CS |
| LCD Backlight / 背光 | 13 | Active HIGH |
| LCD DC | 14 | ST7789 DC |
| XPT2046 CS | 23 | Touch CS |
| AMP EN / 功放使能 | 24 | Active HIGH |
| XPT2046 IRQ | 25 | Touch IRQ |
| Green LED / 绿色LED | 26 | HIGH=ON |
| TF CS | 27 | TF card CS |
| BOOT | 28 | Active LOW |

RESET_AUTO is connected directly to ESP32-C5 EN and is not a software GPIO.  
RESET_AUTO 直接连接 ESP32-C5 EN，不属于可软件控制 GPIO。
