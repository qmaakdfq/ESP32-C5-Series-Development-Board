#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "pins.h"
#include "config.h"

static const char* TEST_FILE = "/zlx_example_test.txt";

static bool mountCard() {
  pinMode(PIN_LCD_CS, OUTPUT); digitalWrite(PIN_LCD_CS, HIGH);
  pinMode(PIN_SD_CS, OUTPUT); digitalWrite(PIN_SD_CS, HIGH);
  SPI.end(); delay(10);
  SPI.begin(PIN_LCD_SCK, PIN_SD_MISO, PIN_LCD_MOSI, PIN_SD_CS);
  if (!SD.begin(PIN_SD_CS, SPI, SD_SPI_HZ, "/sd", 3, false)) return false;
  return SD.cardType() != CARD_NONE;
}

static void cardInfo() {
  uint8_t type = SD.cardType();
  const char* name = type == CARD_MMC ? "MMC" : type == CARD_SD ? "SDSC" :
                     type == CARD_SDHC ? "SDHC/SDXC" : "Unknown";
  Serial.printf("Type: %s\n", name);
  Serial.printf("Card size: %llu MB\n", SD.cardSize() / 1048576ULL);
  Serial.printf("Filesystem total: %llu MB\n", SD.totalBytes() / 1048576ULL);
  Serial.printf("Filesystem used: %llu MB\n", SD.usedBytes() / 1048576ULL);
}

static bool readWriteTest() {
  String tx = "ZLX-ESP32-2 TF example\r\ntime=" + String(millis()) + "\r\n";
  SD.remove(TEST_FILE);
  File f = SD.open(TEST_FILE, FILE_WRITE);
  if (!f) return false;
  size_t n = f.print(tx); f.close();
  if (n != tx.length()) return false;

  f = SD.open(TEST_FILE, FILE_READ);
  if (!f) return false;
  String rx;
  while (f.available() && rx.length() < 512) rx += char(f.read());
  f.close();
  Serial.println("Read back:"); Serial.print(rx);
  return rx == tx;
}

void setup() {
  Serial.begin(115200); delay(500);
  Serial.println("ZLX-ESP32-2 (C5) TF card example");
  Serial.println("SPI: SCK=6 MOSI=7 MISO=2 CS=27, 4MHz");
  if (!mountCard()) {
    Serial.println("TF mount FAILED. Check card, power, MISO and CS.");
    return;
  }
  cardInfo();
  Serial.printf("R/W test: %s\n", readWriteTest() ? "PASS" : "FAIL");
  SD.end();
}

void loop() {}
