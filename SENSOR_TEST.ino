/*
 * ===============================================================
 *           SENSOR TEST - Smart Blind Stick
 * ===============================================================
 *
 * Tests all sensors to verify wiring:
 * - 4x HC-SR04 Ultrasonic Sensors (Front, Left, Right, Back)
 * - 1x IR Ground Sensor (Pothole Detection)
 *
 * Upload this code to verify all sensors are working correctly
 * before using the full BlindStick firmware.
 *
 * ===============================================================
 */

// ===== ULTRASONIC SENSOR PINS =====
// Front Sensor
#define FRONT_TRIG 5
#define FRONT_ECHO 18

// Left Sensor
#define LEFT_TRIG 19
#define LEFT_ECHO 21

// Right Sensor
#define RIGHT_TRIG 22
#define RIGHT_ECHO 23

// Back Sensor
#define BACK_TRIG 12
#define BACK_ECHO 13

// ===== IR GROUND SENSOR =====
#define IR_SENSOR_PIN 34

// ===== STATUS LED =====
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n\n╔══════════════════════════════════════════╗");
  Serial.println("║    SMART BLIND STICK - SENSOR TEST      ║");
  Serial.println("╚══════════════════════════════════════════╝\n");

  // Setup Ultrasonic Sensors
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(BACK_TRIG, OUTPUT);
  pinMode(BACK_ECHO, INPUT);

  // Setup IR Sensor
  pinMode(IR_SENSOR_PIN, INPUT);

  // Setup LED
  pinMode(LED_PIN, OUTPUT);

  Serial.println("✓ All pins initialized");
  Serial.println("✓ Serial monitor ready (115200 baud)");
  Serial.println("\n─────────────────────────────────────────\n");
  Serial.println("Starting sensor tests...\n");
  delay(1000);
}

void loop() {
  // Blink LED to show system is alive
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));

  Serial.println("╔══════════════════════════════════════════╗");
  Serial.println("║         SENSOR READING CYCLE             ║");
  Serial.println("╚══════════════════════════════════════════╝");

  // Test all ultrasonic sensors
  testUltrasonic("FRONT", FRONT_TRIG, FRONT_ECHO);
  testUltrasonic("LEFT ", LEFT_TRIG, LEFT_ECHO);
  testUltrasonic("RIGHT", RIGHT_TRIG, RIGHT_ECHO);
  testUltrasonic("BACK ", BACK_TRIG, BACK_ECHO);

  // Test IR sensor
  testIRSensor();

  Serial.println("─────────────────────────────────────────\n");
  delay(1000); // Update every second
}

// Function to test ultrasonic sensor
void testUltrasonic(const char *name, int trigPin, int echoPin) {
  // Send trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms

  // Calculate distance
  float distance = duration * 0.034 / 2.0;

  // Display result
  Serial.print("[");
  Serial.print(name);
  Serial.print("] ");

  if (duration == 0 || distance > 400) {
    Serial.println("❌ NO SIGNAL - Check wiring!");
  } else if (distance < 2) {
    Serial.println("⚠️  TOO CLOSE - Sensor blocked?");
  } else {
    Serial.print("✓ ");
    Serial.print(distance, 1);
    Serial.print(" cm");

    // Alert levels
    if (distance < 50) {
      Serial.println("  🔴 CRITICAL!");
    } else if (distance < 100) {
      Serial.println("  🟡 WARNING");
    } else if (distance < 200) {
      Serial.println("  🟢 SAFE");
    } else {
      Serial.println("  ⚪ CLEAR");
    }
  }
}

// Function to test IR sensor
void testIRSensor() {
  int irValue = digitalRead(IR_SENSOR_PIN);

  Serial.print("[IR GROUND] ");

  if (irValue == HIGH) {
    Serial.println("✓ Ground Detected (Normal)");
  } else {
    Serial.println("⚠️  GAP/POTHOLE DETECTED!");
  }
}
