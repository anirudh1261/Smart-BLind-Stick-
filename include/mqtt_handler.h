#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "gps_handler.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

class MQTTHandler {
private:
  WiFiClient wifiClient;
  PubSubClient *mqttClient;
  GPSHandler *gpsHandler;

  String deviceID;
  String topicLocation;
  String topicSOS;
  String topicStatus;

  unsigned long lastPublish;
  unsigned long publishInterval;

  bool connected;

  void reconnect();
  String generateDeviceID();

public:
  MQTTHandler();
  void begin(GPSHandler *gps, const char *broker = "broker.hivemq.com",
             int port = 1883);
  void update();

  // Publishing methods
  void publishLocation();
  void publishSOS();
  void publishStatus(String status);

  // Connection management
  bool isConnected();
  void setPublishInterval(unsigned long intervalMs);

  // Topic getters (for family to subscribe)
  String getLocationTopic();
  String getSOSTopic();
  String getDeviceID();
};

#endif
