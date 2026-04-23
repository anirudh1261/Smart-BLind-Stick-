#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

enum SensorDirection { FRONT = 0, LEFT = 1, RIGHT = 2, BACK = 3 };

enum AlertLevel { SAFE = 0, WARNING = 1, CRITICAL = 2, DANGER = 3 };

struct SensorReading {
  float distance;
  AlertLevel level;
  SensorDirection direction;
  unsigned long timestamp;
};

class SensorManager {
private:
  // Ultrasonic sensor pins
  struct UltrasonicSensor {
    uint8_t trigPin;
    uint8_t echoPin;
    SensorDirection direction;
  };

  UltrasonicSensor sensors[4]; // Updated from 3 to 4 sensors
  uint8_t irSensorPin;

  // Internal methods
  float measureDistance(uint8_t trigPin, uint8_t echoPin);
  AlertLevel calculateAlertLevel(float distance);

public:
  SensorManager();
  void begin();

  // Ultrasonic sensor methods
  SensorReading scanFront();
  SensorReading scanLeft();
  SensorReading scanRight();
  SensorReading scanBack(); // NEW: Rear obstacle detection
  SensorReading scanAll();  // Returns highest priority alert

  // IR sensor methods
  bool detectPothole();
  int getGroundDistance();

  // Utility methods
  void calibrate();
  bool isObstacleDetected();
};

#endif
