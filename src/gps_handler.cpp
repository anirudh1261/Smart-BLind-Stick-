#include "gps_handler.h"
#include "config.h"

GPSHandler::GPSHandler() {
  initialized = false;
  lastUpdate = 0;
  lastLocation = {0, 0, 0, 0, 0, false, 0};
}

void GPSHandler::begin(HardwareSerial *serial) {
  gpsSerial = serial;
  gpsSerial->begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  initialized = true;

  Serial.println("[GPS] Initializing...");
  Serial.println("[GPS] Waiting for satellite fix (this may take 1-2 minutes)");
}

void GPSHandler::update() {
  if (!initialized)
    return;

  // Read all available GPS data
  while (gpsSerial->available() > 0) {
    char c = gpsSerial->read();
    gps.encode(c);
  }

  // Update location if we have a valid fix
  if (gps.location.isValid()) {
    lastLocation.latitude = gps.location.lat();
    lastLocation.longitude = gps.location.lng();
    lastLocation.altitude = gps.altitude.meters();
    lastLocation.speed = gps.speed.kmph();
    lastLocation.satellites = gps.satellites.value();
    lastLocation.isValid = true;
    lastLocation.timestamp = millis();
    lastUpdate = millis();

    // Debug output (only on location change)
    static double lastLat = 0, lastLng = 0;
    if (abs(lastLat - lastLocation.latitude) > 0.0001 ||
        abs(lastLng - lastLocation.longitude) > 0.0001) {
      Serial.printf("[GPS] Location: %.6f, %.6f | Sats: %d | Alt: %.1fm\n",
                    lastLocation.latitude, lastLocation.longitude,
                    lastLocation.satellites, lastLocation.altitude);
      lastLat = lastLocation.latitude;
      lastLng = lastLocation.longitude;
    }
  }
}

GPSLocation GPSHandler::getLocation() { return lastLocation; }

bool GPSHandler::hasValidFix() {
  return lastLocation.isValid && isLocationFresh();
}

String GPSHandler::getLocationString() {
  if (!hasValidFix()) {
    return "No GPS fix";
  }

  char buffer[100];
  sprintf(buffer, "%.6f,%.6f", lastLocation.latitude, lastLocation.longitude);
  return String(buffer);
}

String GPSHandler::getGoogleMapsLink() {
  if (!hasValidFix()) {
    return "No GPS fix";
  }

  char buffer[200];
  sprintf(buffer, "https://maps.google.com/?q=%.6f,%.6f", lastLocation.latitude,
          lastLocation.longitude);
  return String(buffer);
}

float GPSHandler::getDistanceTo(double lat, double lng) {
  if (!hasValidFix())
    return -1;

  return TinyGPSPlus::distanceBetween(lastLocation.latitude,
                                      lastLocation.longitude, lat, lng);
}

int GPSHandler::getSatelliteCount() { return gps.satellites.value(); }

bool GPSHandler::isLocationFresh(unsigned long maxAgeMs) {
  return (millis() - lastUpdate) < maxAgeMs;
}
