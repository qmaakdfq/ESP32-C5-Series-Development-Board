#include <Arduino.h>
#include <cstring>
#include <cstdlib>
#include "pins.h"
#include "config.h"

HardwareSerial gps(1);
char lineBuf[180];
size_t lineLen = 0;
bool capture = false;

static bool checksumOK(char* s) {
  if (!s || s[0] != '$') return false;
  char* star = strchr(s, '*'); if (!star || strlen(star) < 3) return false;
  uint8_t sum = 0; for (char* p = s + 1; p < star; ++p) sum ^= uint8_t(*p);
  char hex[3] = {star[1], star[2], 0};
  return sum == strtoul(hex, nullptr, 16);
}

static double coordToDeg(const char* v, const char* hemi) {
  if (!v || !*v) return 0.0;
  double raw = atof(v); int deg = int(raw / 100.0); double min = raw - deg * 100.0;
  double out = deg + min / 60.0;
  if (hemi && (*hemi == 'S' || *hemi == 'W')) out = -out;
  return out;
}

static void parseLine(char* s) {
  if (!checksumOK(s)) { Serial.print("BAD CHECKSUM: "); Serial.println(s); return; }
  Serial.println(s);
  char work[180]; strncpy(work, s, sizeof(work)); work[sizeof(work)-1] = 0;
  char* star = strchr(work, '*'); if (star) *star = 0;
  char* fields[24] = {}; int n = 0;
  char* p = work;
  while (p && n < 24) { fields[n++] = p; char* comma = strchr(p, ','); if (!comma) break; *comma = 0; p = comma + 1; }
  if (n < 2) return;
  const char* type = strlen(fields[0]) >= 6 ? fields[0] + 3 : fields[0];
  if (!strcmp(type, "GGA") && n > 9) {
    int quality = atoi(fields[6]); int sats = atoi(fields[7]); float hdop = atof(fields[8]); float alt = atof(fields[9]);
    double lat = coordToDeg(fields[2], fields[3]); double lon = coordToDeg(fields[4], fields[5]);
    Serial.printf("GGA fix=%d sats=%d HDOP=%.2f alt=%.1fm lat=%.6f lon=%.6f\n", quality, sats, hdop, alt, lat, lon);
  } else if (!strcmp(type, "RMC") && n > 9) {
    Serial.printf("RMC status=%s UTC=%s date=%s speed(knots)=%s\n", fields[2], fields[1], fields[9], fields[7]);
  }
}

void setup() {
  Serial.begin(115200); delay(300);
  gps.setRxBufferSize(2048);
  gps.begin(GPS_DEFAULT_BAUD, SERIAL_8N1, PIN_GPS_TX, PIN_GPS_RX);
  Serial.println("GT-U7 GPS/GNSS example");
  Serial.println("GPIO0 = MCU RX <- GPS TX, GPIO9 = MCU TX -> GPS RX, 9600 8N1");
}

void loop() {
  while (gps.available()) {
    uint8_t b = gps.read();
    if (b == '$') { lineLen = 0; lineBuf[lineLen++] = '$'; capture = true; continue; }
    if (!capture) continue;
    if (b == '\n') {
      if (lineLen >= 6) { lineBuf[lineLen] = 0; parseLine(lineBuf); }
      lineLen = 0; capture = false; continue;
    }
    if (b == '\r') continue;
    if (b < 0x20 || b > 0x7E || lineLen >= sizeof(lineBuf)-1) { lineLen = 0; capture = false; continue; }
    lineBuf[lineLen++] = char(b);
  }
}
