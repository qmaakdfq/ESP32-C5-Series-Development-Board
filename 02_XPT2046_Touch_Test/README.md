# 02_XPT2046_Touch_Test

XPT2046 resistive-touch fixed mapping with live drawing on the TFT.

## Hardware

- Product: ZLX-ESP32-2 (C5)
- Module: ESP32-C5-WROOM-1U-N8R8
- Serial monitor: 115200 baud

## Build

Open the **current example folder** in VS Code + PlatformIO, then build/upload.

```bash
pio run -e esp32c5_n8r8
pio run -e esp32c5_n8r8 -t upload
pio device monitor -b 115200
```

> See `include/pins.h` for GPIO mapping. These examples are split from the ZLX-ESP32-2 (C5) factory-test firmware. Verify GPIO mapping before adapting the code to different hardware.

Documentation: https://www.zlxchina.com/  
Technical support: mq19880204@gmail.com
