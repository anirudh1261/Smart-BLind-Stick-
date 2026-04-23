#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "gps_handler.h"
#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

class WebServerHandler {
private:
  AsyncWebServer *server;
  AsyncEventSource *events;
  GPSHandler *gpsHandler;

  String ssid;
  String password;
  bool apMode;

  void setupRoutes();
  String getStatusJSON();
  String getLocationJSON();

public:
  WebServerHandler();
  void begin(GPSHandler *gps, const char *wifiSSID = nullptr,
             const char *wifiPassword = nullptr);
  void update();

  // WiFi management
  bool isConnected();
  String getIPAddress();
  void startAccessPoint(const char *apName = "BlindStick-AP");

  // Location sharing
  void broadcastLocation();
  void sendSOSAlert();
};

#endif
