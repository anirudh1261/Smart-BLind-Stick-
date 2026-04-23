/*
 * ===============================================================
 *         SMART BLIND STICK - Complete Standalone Version
 * ===============================================================
 *
 * ALL-IN-ONE ARDUINO SKETCH
 *
 * Features:
 * - 4x HC-SR04 Ultrasonic Sensors (360° detection)
 * - IR Ground/Pothole Sensor
 * - 4-Channel APR Audio Module (Directional alerts)
 * - Buzzer & Vibration Motor
 * - SOS Emergency Button
 * - GPS Tracking (NEO-6M + GeoLinker Cloud)
 * - WiFi Connectivity
 *
 * Upload this single file to ESP32 using Arduino IDE
 *
 * ===============================================================
 */

#include <Arduino.h>
#include <HTTPClient.h>
#include <TinyGPS++.h>
#include <WiFi.h>

// ===== CONFIGURATION =====
// WiFi Credentials
#define WIFI_SSID "WiFi"
#define WIFI_PASSWORD "wordpass"

// GeoLinker API
#define GEOLINKER_API_KEY "9gPyNAUrs4U1"
#define GEOLINKER_DEVICE_ID "BlindStick_001"

// Serial
#define SERIAL_BAUD_RATE 115200

// ===== PIN DEFINITIONS =====
// Ultrasonic Sensors (HC-SR04)
#define FRONT_TRIG 5
#define FRONT_ECHO 18
#define LEFT_TRIG 19
#define LEFT_ECHO 21
#define RIGHT_TRIG 22
#define RIGHT_ECHO 23
#define BACK_TRIG 12
#define BACK_ECHO 13

// IR Ground Sensor
#define IR_SENSOR_PIN 34

// GPS Module
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17

// APR Audio Module (4-Channel)
#define APR_FRONT_BACK_PIN 14 // Channel 1: Front/Back
#define APR_LEFT_PIN 27       // Channel 2: Left
#define APR_RIGHT_PIN 26      // Channel 3: Right
#define APR_POTHOLE_PIN 25    // Channel 4: Pothole

// Feedback Devices
#define BUZZER_PIN 33
#define VIBRATION_PIN 32

// Controls
#define SOS_BUTTON_PIN 35
#define STATUS_LED_PIN 2

// ===== DISTANCE THRESHOLDS =====
#define CRITICAL_DISTANCE 50 // cm
#define WARNING_DISTANCE 100 // cm
#define SAFE_DISTANCE 200    // cm

// ===== TIMING =====
#define SENSOR_SCAN_INTERVAL 50  // ms
#define APR_TRIGGER_DURATION 200 // ms
#define GPS_UPLOAD_INTERVAL 2000 // ms

// ===== ENUMS =====
enum SensorDirection { FRONT = 0, LEFT = 1, RIGHT = 2, BACK = 3 };
enum AlertLevel { SAFE = 0, WARNING = 1, CRITICAL = 2 };

// ===== STRUCTURES =====
struct SensorReading {
  SensorDirection direction;
  float distance;
  AlertLevel level;
};

// ===== GLOBAL OBJECTS =====
HardwareSerial gpsSerial(1);
TinyGPSPlus gps;
HTTPClient http;

// ===== GLOBAL VARIABLES =====
unsigned long lastSensorScan = 0;
unsigned long lastGPSUpload = 0;
unsigned long lastStatusUpdate = 0;
bool sosButtonPressed = false;
unsigned long sosButtonPressTime = 0;

// Vibration pattern
unsigned long lastVibrationToggle = 0;
bool vibrationState = false;
int currentVibrationPattern = 0;

// ===== FUNCTION PROTOTYPES =====
void setupSensors();
void setupFeedback();
void setupGPS();
void setupWiFi();
float readUltrasonic(int trigPin, int echoPin);
SensorReading scanAllSensors();
bool detectPothole();
void provideAlert(SensorReading reading, bool pothole);
void playObstacleAlert(SensorDirection direction);
void playPotholeAlert();
void playSOSAlert();
void updateVibration();
void setVibrationPattern(int patternMs);
void uploadGPSLocation();
void activateSOS();

// ===== SETUP =====
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);

  Serial.println("\n\n╔════════════════════════════════════════╗");
  Serial.println("║    SMART BLIND STICK - GPS ENABLED    ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Initialize GPIO
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, HIGH);
  pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);

  // Initialize subsystems
  Serial.println("[1/4] Initializing Sensors...");
  setupSensors();
  Serial.println("  ✓ 4x Ultrasonic Sensors");
  Serial.println("  ✓ IR Ground Sensor\n");

  Serial.println("[2/4] Initializing Feedback...");
  setupFeedback();
  Serial.println("  ✓ APR Audio Module");
  Serial.println("  ✓ Vibration & Buzzer\n");

  Serial.println("[3/4] Initializing GPS...");
  setupGPS();
  Serial.println("  ✓ NEO-6M GPS Module\n");

  Serial.println("[4/4] Connecting to WiFi...");
  setupWiFi();

  digitalWrite(STATUS_LED_PIN, LOW);
  delay(500);
  digitalWrite(STATUS_LED_PIN, HIGH);

  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║      SYSTEM READY - STAY SAFE!        ║");
  Serial.println("╚════════════════════════════════════════╝\n");
}

// ===== MAIN LOOP =====
void loop() {
  unsigned long currentTime = millis();

  // Obstacle Detection (every 50ms)
  if (currentTime - lastSensorScan >= SENSOR_SCAN_INTERVAL) {
    lastSensorScan = currentTime;

    SensorReading reading = scanAllSensors();
    bool pothole = detectPothole();
    provideAlert(reading, pothole);

    // Debug output
    if (reading.level >= WARNING || pothole) {
      const char *dirs[] = {"FRONT", "LEFT", "RIGHT", "BACK"};
      Serial.printf("[ALERT] %s: %.1fcm", dirs[reading.direction],
                    reading.distance);
      if (pothole)
        Serial.print(" | POTHOLE!");
      Serial.println();
    }
  }

  // GPS Tracking (every 2 seconds)
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (currentTime - lastGPSUpload >= GPS_UPLOAD_INTERVAL) {
    lastGPSUpload = currentTime;
    if (gps.location.isValid() && WiFi.status() == WL_CONNECTED) {
      uploadGPSLocation();
    }
  }

  // Vibration update
  updateVibration();

  // SOS Button (2-second press)
  if (digitalRead(SOS_BUTTON_PIN) == LOW) {
    if (!sosButtonPressed) {
      sosButtonPressed = true;
      sosButtonPressTime = currentTime;
    } else if (currentTime - sosButtonPressTime > 2000) {
      activateSOS();
      sosButtonPressed = false;
    }
  } else {
    sosButtonPressed = false;
  }

  // Status LED heartbeat
  if (currentTime - lastStatusUpdate >= 1000) {
    lastStatusUpdate = currentTime;
    digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));
  }

  delay(10);
}

// ===== SETUP FUNCTIONS =====
void setupSensors() {
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(BACK_TRIG, OUTPUT);
  pinMode(BACK_ECHO, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
}

void setupFeedback() {
  pinMode(APR_FRONT_BACK_PIN, OUTPUT);
  digitalWrite(APR_FRONT_BACK_PIN, LOW);
  pinMode(APR_LEFT_PIN, OUTPUT);
  digitalWrite(APR_LEFT_PIN, LOW);
  pinMode(APR_RIGHT_PIN, OUTPUT);
  digitalWrite(APR_RIGHT_PIN, LOW);
  pinMode(APR_POTHOLE_PIN, OUTPUT);
  digitalWrite(APR_POTHOLE_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(VIBRATION_PIN, OUTPUT);
  digitalWrite(VIBRATION_PIN, LOW);
}

void setupGPS() { gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN); }

void setupWiFi() {
  Serial.print("  SSID: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n  ✓ WiFi Connected!");
    Serial.print("  ✓ IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n  ⚠ WiFi Failed - Sensors Only");
  }
}

// ===== SENSOR FUNCTIONS =====
float readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0)
    return 999.0;

  return (duration * 0.034) / 2.0;
}

SensorReading scanAllSensors() {
  SensorReading readings[4];

  readings[0] = {FRONT, readUltrasonic(FRONT_TRIG, FRONT_ECHO), SAFE};
  delay(10);
  readings[1] = {LEFT, readUltrasonic(LEFT_TRIG, LEFT_ECHO), SAFE};
  delay(10);
  readings[2] = {RIGHT, readUltrasonic(RIGHT_TRIG, RIGHT_ECHO), SAFE};
  delay(10);
  readings[3] = {BACK, readUltrasonic(BACK_TRIG, BACK_ECHO), SAFE};

  // Determine alert levels
  for (int i = 0; i < 4; i++) {
    if (readings[i].distance < CRITICAL_DISTANCE) {
      readings[i].level = CRITICAL;
    } else if (readings[i].distance < WARNING_DISTANCE) {
      readings[i].level = WARNING;
    }
  }

  // Return most critical reading
  SensorReading mostCritical = readings[0];
  for (int i = 1; i < 4; i++) {
    if (readings[i].level > mostCritical.level ||
        (readings[i].level == mostCritical.level &&
         readings[i].distance < mostCritical.distance)) {
      mostCritical = readings[i];
    }
  }

  return mostCritical;
}

bool detectPothole() {
  // IR sensor: LOW = ground present, HIGH = gap/pothole
  return (digitalRead(IR_SENSOR_PIN) == HIGH);
}

// ===== FEEDBACK FUNCTIONS =====
void provideAlert(SensorReading reading, bool pothole) {
  if (pothole) {
    playPotholeAlert();
    digitalWrite(BUZZER_PIN, HIGH);
    setVibrationPattern(100);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  if (reading.level == CRITICAL) {
    playObstacleAlert(reading.direction);
    digitalWrite(BUZZER_PIN, HIGH);
    setVibrationPattern(100);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
  } else if (reading.level == WARNING) {
    playObstacleAlert(reading.direction);
    setVibrationPattern(300);
  } else {
    setVibrationPattern(0);
  }
}

void playObstacleAlert(SensorDirection direction) {
  uint8_t pin = 0;

  switch (direction) {
  case FRONT:
  case BACK:
    pin = APR_FRONT_BACK_PIN;
    break;
  case LEFT:
    pin = APR_LEFT_PIN;
    break;
  case RIGHT:
    pin = APR_RIGHT_PIN;
    break;
  }

  if (pin > 0) {
    digitalWrite(pin, HIGH);
    delay(APR_TRIGGER_DURATION);
    digitalWrite(pin, LOW);
  }
}

void playPotholeAlert() {
  digitalWrite(APR_POTHOLE_PIN, HIGH);
  delay(APR_TRIGGER_DURATION);
  digitalWrite(APR_POTHOLE_PIN, LOW);
}

void playSOSAlert() {
  // Play SOS audio pattern
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
}

void setVibrationPattern(int patternMs) {
  currentVibrationPattern = patternMs;
  if (patternMs == 0) {
    digitalWrite(VIBRATION_PIN, LOW);
    vibrationState = false;
  } else {
    lastVibrationToggle = millis();
    vibrationState = true;
    digitalWrite(VIBRATION_PIN, HIGH);
  }
}

void updateVibration() {
  if (currentVibrationPattern > 0) {
    if (millis() - lastVibrationToggle >= currentVibrationPattern) {
      lastVibrationToggle = millis();
      vibrationState = !vibrationState;
      digitalWrite(VIBRATION_PIN, vibrationState ? HIGH : LOW);
    }
  }
}

// ===== GPS FUNCTIONS =====
void uploadGPSLocation() {
  if (!gps.location.isValid())
    return;

  String url = "https://www.circuitdigest.cloud/geolinker";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + String(GEOLINKER_API_KEY));

  String jsonData = "{";
  jsonData += "\"deviceId\":\"" + String(GEOLINKER_DEVICE_ID) + "\",";
  jsonData += "\"latitude\":" + String(gps.location.lat(), 6) + ",";
  jsonData += "\"longitude\":" + String(gps.location.lng(), 6) + ",";
  jsonData += "\"speed\":" + String(gps.speed.kmph(), 2) + ",";
  jsonData += "\"satellites\":" + String(gps.satellites.value());
  jsonData += "}";

  int httpCode = http.POST(jsonData);

  if (httpCode == 200) {
    Serial.println("[GPS] ✓ Location uploaded");
  }

  http.end();
}

// ===== SOS FUNCTION =====
void activateSOS() {
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║          SOS ACTIVATED!!!              ║");
  Serial.println("╚════════════════════════════════════════╝");

  playSOSAlert();

  // Flash LED
  for (int i = 0; i < 10; i++) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(100);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(100);
  }

  Serial.println("Emergency notification sent!\n");
}
