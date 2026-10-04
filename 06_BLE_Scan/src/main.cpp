#include <Arduino.h>
#include <NimBLEDevice.h>
#include <vector>
#include <algorithm>

void setup() {
  Serial.begin(115200); delay(500);
  Serial.println("ZLX-ESP32-2 (C5) BLE scan example");
  NimBLEDevice::init("ZLX-ESP32-2-Example");
  NimBLEScan* scanner = NimBLEDevice::getScan();
  scanner->setActiveScan(true); scanner->setInterval(60); scanner->setWindow(45); scanner->setMaxResults(50);
  Serial.println("Scanning for 5 seconds...");
  NimBLEScanResults results = scanner->getResults(5000, false);
  int count = results.getCount();
  std::vector<int> order(count); for (int i = 0; i < count; ++i) order[i] = i;
  std::sort(order.begin(), order.end(), [&](int a, int b) {
    auto* da = results.getDevice(a); auto* db = results.getDevice(b);
    return da && db ? da->getRSSI() > db->getRSSI() : da != nullptr;
  });
  Serial.printf("Found %d BLE advertising devices\n", count);
  for (int rank = 0; rank < count; ++rank) {
    auto* d = results.getDevice(order[rank]); if (!d) continue;
    String name = d->haveName() ? String(d->getName().c_str()) : String("<Unnamed>");
    Serial.printf("%2d. %4ddBm  %s  %s\n", rank + 1, d->getRSSI(), d->getAddress().toString().c_str(), name.c_str());
  }
  scanner->clearResults();
}
void loop() {}
