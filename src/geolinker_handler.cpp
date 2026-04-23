#include "geolinker_handler.h"

GeoLinkerHandler::GeoLinkerHandler() {}

void GeoLinkerHandler::begin(GPSHandler *gps, const char *key, const char *id) {
  gpsHandler = gps;
  apiKey = key;
  deviceID = id;

  Serial.println("[GeoLinker] Initialized");
  Serial.print("[GeoLinker] Device ID: ");
  Serial.println(deviceID);
}

bool GeoLinkerHandler::uploadLocation() {
  if (!WiFi.isConnected()) {
    Serial.println("[GeoLinker] WiFi not connected");
    return false;
  }

  if (!gpsHandler->hasValidFix()) {
    Serial.println("[GeoLinker] No GPS fix");
    return false;
  }

  GPSLocation loc = gpsHandler->getLocation();

  // Create JSON payload
  StaticJsonDocument<512> doc;
  doc["device_id"] = deviceID;

  // Create arrays (GeoLinker expects arrays even for single points)
  JsonArray timestamps = doc.createNestedArray("timestamp");
  JsonArray lats = doc.createNestedArray("lat");
  JsonArray longs = doc.createNestedArray("long");

  // Add current data
  char timestamp[32];
  sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", 2024, 1,
          1, // Placeholder date - ideally use RTC or NTP
          (int)(millis() / 3600000) % 24, (int)(millis() / 60000) % 60,
          (int)(millis() / 1000) % 60);

  timestamps.add(timestamp);
  lats.add(loc.latitude);
  longs.add(loc.longitude);

  // Serialize to string
  String payload;
  serializeJson(doc, payload);

  // Make HTTP POST request
  http.begin(apiURL);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", apiKey);

  Serial.println("[GeoLinker] Uploading location...");
  Serial.printf("  Lat: %.6f, Lng: %.6f\n", loc.latitude, loc.longitude);

  int httpCode = http.POST(payload);

  if (httpCode > 0) {
    Serial.printf("[GeoLinker] Response: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("[GeoLinker] Upload successful!");
      Serial.println(response);
      http.end();
      return true;
    }
  } else {
    Serial.printf("[GeoLinker] Upload failed: %s\n",
                  http.errorToString(httpCode).c_str());
  }

  http.end();
  return false;
}

void GeoLinkerHandler::update() {
  unsigned long currentTime = millis();

  if (currentTime - lastUploadTime >= uploadInterval) {
    lastUploadTime = currentTime;
    uploadLocation();
  }
}
