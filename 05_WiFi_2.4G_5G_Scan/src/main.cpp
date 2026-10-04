#include <Arduino.h>
#include <Network.h>
#include <WiFi.h>
#include <esp_wifi.h>

static void scanAllBands() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false, false);
  delay(100);
#if defined(CONFIG_SOC_WIFI_SUPPORT_5G) && CONFIG_SOC_WIFI_SUPPORT_5G
  esp_wifi_set_band_mode(WIFI_BAND_MODE_AUTO);
#endif
  Serial.println("Scanning 2.4GHz / 5GHz Wi-Fi...");
  int n = WiFi.scanNetworks(false, true, false, 300, 0);
  if (n < 0) { Serial.printf("Scan failed: %d\n", n); return; }
  int n24 = 0, n5 = 0;
  for (int i = 0; i < n; ++i) {
    int ch = WiFi.channel(i);
    if (ch <= 14) ++n24; else ++n5;
    String ssid = WiFi.SSID(i); if (!ssid.length()) ssid = "<Hidden>";
    Serial.printf("%2d. %4ddBm  CH%-3d  %-4s  %s\n", i + 1, WiFi.RSSI(i), ch,
                  ch <= 14 ? "2.4G" : "5G", ssid.c_str());
  }
  Serial.printf("Total=%d, 2.4GHz=%d, 5GHz=%d\n", n, n24, n5);
  WiFi.scanDelete();
}

void setup() {
  Serial.begin(115200); delay(500);
  Serial.println("ZLX-ESP32-2 (C5) Wi-Fi scan example");
  scanAllBands();
}
void loop() {}
