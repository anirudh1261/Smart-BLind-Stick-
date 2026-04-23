/*
 * Simple GeoLinker wrapper - direct API implementation
 * Based on user's working GeoLinker code
 */

#ifndef SIMPLE_GEOLINKER_H
#define SIMPLE_GEOLINKER_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <WiFi.h>

// Status codes
#define STATUS_SENT 1
#define STATUS_GPS_ERROR 2
#define STATUS_NETWORK_ERROR 3
#define STATUS_PARSE_ERROR 4

class SimpleGeoLinker {
private:
  HardwareSerial *gpsSerial;
  TinyGPSPlus gps;
  String apiKey;
  String deviceID;
  unsigned long lastUpload;
  unsigned long uploadInterval;
  HTTPClient http;

  bool hasValidFix() {
    return gps.location.isValid() && gps.location.age() < 2000;
  }

  bool uploadLocation() {
    if (!WiFi.isConnected() || !hasValidFix())
      return false;

    // Prepare JSON
    String url = "https://www.circuitdigest.cloud/geolinker";
    String payload = "{\"device_id\":\"" + deviceID + "\",\"timestamp\":[\"" +
                     getTimestamp() + "\"],\"lat\":[" +
                     String(gps.location.lat(), 6) + "],\"long\":[" +
                     String(gps.location.lng(), 6) + "]}";

    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", apiKey);

    int httpCode = http.POST(payload);
    http.end();

    return (httpCode == 200);
  }

  String getTimestamp() {
    char buf[32];
    sprintf(buf, "2026-01-05 %02d:%02d:%02d", gps.time.hour(),
            gps.time.minute(), gps.time.second());
    return String(buf);
  }

public:
  SimpleGeoLinker() : lastUpload(0), uploadInterval(2000) {}

  void begin(HardwareSerial &serial, const char *key, const char *id) {
    gpsSerial = &serial;
    apiKey = String(key);
    deviceID = String(id);
  }

  uint8_t loop() {
    // Read GPS data
    while (gpsSerial->available()) {
      gps.encode(gpsSerial->read());
    }

    // Check if time to upload
    if (millis() - lastUpload < uploadInterval)
      return 0;
    lastUpload = millis();

    // Upload if have fix
    if (!hasValidFix())
      return STATUS_GPS_ERROR;
    if (!WiFi.isConnected())
      return STATUS_NETWORK_ERROR;

    return uploadLocation() ? STATUS_SENT : STATUS_NETWORK_ERROR;
  }
};

#endif
