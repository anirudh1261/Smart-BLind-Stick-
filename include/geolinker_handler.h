#ifndef GEOLINKER_HANDLER_H
#define GEOLINKER_HANDLER_H

#include "gps_handler.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

class GeoLinkerHandler {
private:
  const char *apiKey;
  const char *deviceID;
  const char *apiURL = "https://www.circuitdigest.cloud/geolinker";
  GPSHandler *gpsHandler;
  HTTPClient http;

  unsigned long lastUploadTime = 0;
  const unsigned long uploadInterval = 5000; // Upload every 5 seconds

public:
  GeoLinkerHandler();
  void begin(GPSHandler *gps, const char *key, const char *id);
  bool uploadLocation();
  void update();
};

#endif
