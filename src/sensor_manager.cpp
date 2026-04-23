#include "sensor_manager.h"
#include "config.h"

SensorManager::SensorManager() {
  // Initialize sensor configurations - 4 sensors for 360° coverage
  sensors[FRONT] = {FRONT_TRIG_PIN, FRONT_ECHO_PIN, FRONT};
  sensors[LEFT] = {LEFT_TRIG_PIN, LEFT_ECHO_PIN, LEFT};
  sensors[RIGHT] = {RIGHT_TRIG_PIN, RIGHT_ECHO_PIN, RIGHT};
  sensors[BACK] = {BACK_TRIG_PIN, BACK_ECHO_PIN, BACK}; // NEW
  irSensorPin = IR_SENSOR_PIN;
}

void SensorManager::begin() {
  // Setup ultrasonic sensors - 4 sensors now
  for (int i = 0; i < 4; i++) {
    pinMode(sensors[i].trigPin, OUTPUT);
    pinMode(sensors[i].echoPin, INPUT);
    digitalWrite(sensors[i].trigPin, LOW);
  }

  // Setup IR sensor
  pinMode(irSensorPin, INPUT);

  Serial.println("[SensorManager] Initialized (4 ultrasonic + 1 IR)");
  delay(100);
}

float SensorManager::measureDistance(uint8_t trigPin, uint8_t echoPin) {
  // Clear trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send 10μs pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout

  // Calculate distance in cm
  if (duration == 0) {
    return 999.0; // No echo = very far or error
  }

  float distance = duration * SOUND_SPEED / 2.0;

  // Validate range (2cm - 400cm for HC-SR04)
  if (distance < 2 || distance > 400) {
    return 999.0;
  }

  return distance;
}

AlertLevel SensorManager::calculateAlertLevel(float distance) {
  if (distance < CRITICAL_DISTANCE) {
    return CRITICAL;
  } else if (distance < WARNING_DISTANCE) {
    return WARNING;
  } else {
    return SAFE;
  }
}

SensorReading SensorManager::scanFront() {
  SensorReading reading;
  reading.distance =
      measureDistance(sensors[FRONT].trigPin, sensors[FRONT].echoPin);
  reading.level = calculateAlertLevel(reading.distance);
  reading.direction = FRONT;
  reading.timestamp = millis();

  return reading;
}

SensorReading SensorManager::scanLeft() {
  SensorReading reading;
  reading.distance =
      measureDistance(sensors[LEFT].trigPin, sensors[LEFT].echoPin);
  reading.level = calculateAlertLevel(reading.distance);
  reading.direction = LEFT;
  reading.timestamp = millis();

  return reading;
}

SensorReading SensorManager::scanRight() {
  SensorReading reading;
  reading.distance =
      measureDistance(sensors[RIGHT].trigPin, sensors[RIGHT].echoPin);
  reading.level = calculateAlertLevel(reading.distance);
  reading.direction = RIGHT;
  reading.timestamp = millis();

  return reading;
}

SensorReading SensorManager::scanBack() {
  SensorReading reading;
  reading.distance =
      measureDistance(sensors[BACK].trigPin, sensors[BACK].echoPin);
  reading.level = calculateAlertLevel(reading.distance);
  reading.direction = BACK;
  reading.timestamp = millis();

  return reading;
}

SensorReading SensorManager::scanAll() {
  SensorReading front = scanFront();
  delay(10); // Small delay between readings
  SensorReading left = scanLeft();
  delay(10);
  SensorReading right = scanRight();
  delay(10);
  SensorReading back = scanBack(); // NEW: Scan rear sensor

  // Return the most critical reading from all 4 sensors
  SensorReading mostCritical = front;

  if (left.level > mostCritical.level ||
      (left.level == mostCritical.level &&
       left.distance < mostCritical.distance)) {
    mostCritical = left;
  }

  if (right.level > mostCritical.level ||
      (right.level == mostCritical.level &&
       right.distance < mostCritical.distance)) {
    mostCritical = right;
  }

  if (back.level > mostCritical.level ||
      (back.level == mostCritical.level &&
       back.distance < mostCritical.distance)) {
    mostCritical = back;
  }

  return mostCritical;
}

bool SensorManager::detectPothole() {
  // Read IR sensor  (inverted logic: LOW = ground present, HIGH = gap/pothole)
  int irValue = digitalRead(irSensorPin);

  // Return true if pothole/gap detected
  return (irValue == HIGH); // HIGH = no ground = pothole!
}

int SensorManager::getGroundDistance() { return analogRead(irSensorPin); }

void SensorManager::calibrate() {
  Serial.println("[SensorManager] Calibrating sensors...");

  // Take multiple readings and average
  for (int i = 0; i < 3; i++) {
    float frontDist =
        measureDistance(sensors[FRONT].trigPin, sensors[FRONT].echoPin);
    Serial.printf("Front: %.2f cm\n", frontDist);
    delay(100);
  }

  Serial.println("[SensorManager] Calibration complete");
}

bool SensorManager::isObstacleDetected() {
  SensorReading reading = scanAll();
  return (reading.level >= WARNING);
}
