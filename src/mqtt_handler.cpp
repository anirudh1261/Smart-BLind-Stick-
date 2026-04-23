#include "mqtt_handler.h"
#include "config.h"
#include <ArduinoJson.h>

MQTTHandler::MQTTHandler() {
  mqttClient = new PubSubClient(wifiClient);
  lastPublish = 0;
  publishInterval = 5000; // Default: 5 seconds
  connected = false;
}

void MQTTHandler::begin(GPSHandler *gps, const char *broker, int port) {
  gpsHandler = gps;

  // Generate unique device ID
  deviceID = generateDeviceID();

  // Setup topics
  topicLocation = "blindstick/" + deviceID + "/location";
  topicSOS = "blindstick/" + deviceID + "/sos";
  topicStatus = "blindstick/" + deviceID + "/status";

  // Configure MQTT client
  mqttClient->setServer(broker, port);
  mqttClient->setKeepAlive(60);

  Serial.println("[MQTT] Configured");
  Serial.println("[MQTT] Device ID: " + deviceID);
  Serial.println("[MQTT] Location Topic: " + topicLocation);
  Serial.println("[MQTT] SOS Topic: " + topicSOS);
  Serial.println("\n=== SHARE THIS WITH FAMILY ===");
  Serial.println("Device ID: " + deviceID);
  Serial.println("Location Topic: " + topicLocation);
  Serial.println("==============================\n");

  // Initial connection attempt
  reconnect();
}

void MQTTHandler::reconnect() {
  if (!WiFi.isConnected()) {
    Serial.println("[MQTT] WiFi not connected, skipping MQTT");
    connected = false;
    return;
  }

  if (mqttClient->connected()) {
    connected = true;
    return;
  }

  Serial.print("[MQTT] Connecting to broker...");

  // Attempt connection
  String clientID = "BlindStick_" + deviceID;
  if (mqttClient->connect(clientID.c_str())) {
    Serial.println(" Connected!");
    connected = true;

    // Publish online status
    publishStatus("online");
  } else {
    Serial.print(" Failed, rc=");
    Serial.println(mqttClient->state());
    connected = false;
  }
}

String MQTTHandler::generateDeviceID() {
  // Use ESP32's unique MAC address as device ID
  uint8_t mac[6];
  WiFi.macAddress(mac);

  char id[13];
  sprintf(id, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3],
          mac[4], mac[5]);

  return String(id);
}

void MQTTHandler::update() {
  // Maintain MQTT connection
  if (!mqttClient->connected()) {
    reconnect();
  }

  if (connected) {
    mqttClient->loop();

    // Periodic location publishing
    if (millis() - lastPublish >= publishInterval) {
      publishLocation();
      lastPublish = millis();
    }
  }
}

void MQTTHandler::publishLocation() {
  if (!connected || !gpsHandler->hasValidFix()) {
    return;
  }

  GPSLocation loc = gpsHandler->getLocation();

  // Create JSON payload
  StaticJsonDocument<256> doc;
  doc["lat"] = loc.latitude;
  doc["lng"] = loc.longitude;
  doc["alt"] = loc.altitude;
  doc["speed"] = loc.speed;
  doc["sats"] = loc.satellites;
  doc["timestamp"] = millis();
  doc["maps_link"] = gpsHandler->getGoogleMapsLink();

  String payload;
  serializeJson(doc, payload);

  // Publish to MQTT
  bool success =
      mqttClient->publish(topicLocation.c_str(), payload.c_str(), true);

  if (success) {
    Serial.printf("[MQTT] Published location: %.6f, %.6f\n", loc.latitude,
                  loc.longitude);
  } else {
    Serial.println("[MQTT] Publish failed!");
  }
}

void MQTTHandler::publishSOS() {
  if (!connected) {
    Serial.println("[MQTT] Cannot send SOS - not connected");
    return;
  }

  GPSLocation loc = gpsHandler->getLocation();

  // Create SOS payload
  StaticJsonDocument<256> doc;
  doc["type"] = "SOS";
  doc["lat"] = loc.latitude;
  doc["lng"] = loc.longitude;
  doc["maps_link"] = gpsHandler->getGoogleMapsLink();
  doc["timestamp"] = millis();
  doc["message"] = "EMERGENCY ALERT - User activated SOS button";

  String payload;
  serializeJson(doc, payload);

  // Publish SOS (retained message)
  bool success = mqttClient->publish(topicSOS.c_str(), payload.c_str(), true);

  if (success) {
    Serial.println("[MQTT] SOS ALERT SENT!");
    Serial.println("[MQTT] Location: " + gpsHandler->getGoogleMapsLink());
  } else {
    Serial.println("[MQTT] SOS publish failed!");
  }
}

void MQTTHandler::publishStatus(String status) {
  if (!connected)
    return;

  StaticJsonDocument<128> doc;
  doc["status"] = status;
  doc["timestamp"] = millis();
  doc["battery"] = "85%"; // TODO: Read actual battery level

  String payload;
  serializeJson(doc, payload);

  mqttClient->publish(topicStatus.c_str(), payload.c_str());
}

bool MQTTHandler::isConnected() { return connected && mqttClient->connected(); }

void MQTTHandler::setPublishInterval(unsigned long intervalMs) {
  publishInterval = intervalMs;
  Serial.printf("[MQTT] Publish interval set to %lu ms\n", intervalMs);
}

String MQTTHandler::getLocationTopic() { return topicLocation; }

String MQTTHandler::getSOSTopic() { return topicSOS; }

String MQTTHandler::getDeviceID() { return deviceID; }
