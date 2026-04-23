/*
 * ===============================================================
 *           SENSOR TEST - All Hardware Verification
 * ===============================================================
 * Tests:
 * - 4x HC-SR04 Ultrasonic Sensors
 * - IR Ground Sensor
 * - Buzzer
 * ===============================================================
 */

#include <Arduino.h>

// Ultrasonic pins
#define FRONT_TRIG 5
#define FRONT_ECHO 18
#define LEFT_TRIG 19
#define LEFT_ECHO 21
#define RIGHT_TRIG 22
#define RIGHT_ECHO 23
#define BACK_TRIG 12
#define BACK_ECHO 13

// IR & Buzzer
#define IR_SENSOR_PIN 34
#define BUZZER_PIN 33
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n\n╔════════════════════════════════════════╗");
  Serial.println("║   SMART BLIND STICK - SENSOR TEST     ║");
  Serial.println("╚════════════════════════════════════════╝\n");

  // Setup all pins
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(BACK_TRIG, OUTPUT);
  pinMode(BACK_ECHO, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("✓ All pins initialized");

  // Test buzzer
  Serial.println("✓ Testing buzzer...");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(100);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("\nStarting sensor tests...\n");
  delay(1000);
}

void testUltrasonic(const char *name, int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  float distance = duration * 0.034 / 2.0;

  Serial.print("[");
  Serial.print(name);
  Serial.print("] ");

  if (duration == 0 || distance > 400) {
    Serial.println("❌ NO SIGNAL");
  } else if (distance < 2) {
    Serial.println("⚠️  TOO CLOSE");
  } else {
    Serial.print("✓ ");
    Serial.print(distance, 1);
    Serial.print(" cm");

    if (distance < 50)
      Serial.println("  🔴 CRITICAL!");
    else if (distance < 100)
      Serial.println("  🟡 WARNING");
    else if (distance < 200)
      Serial.println("  🟢 SAFE");
    else
      Serial.println("  ⚪ CLEAR");
  }
}

void testIRSensor() {
  int irValue = digitalRead(IR_SENSOR_PIN);
  Serial.print("[IR GROUND] ");

  if (irValue == LOW) {
    Serial.println("✓ Ground Detected");
  } else {
    Serial.println("⚠️  GAP/POTHOLE!");
  }
}

void loop() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));

  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║       SENSOR READING CYCLE             ║");
  Serial.println("╚════════════════════════════════════════╝");

  testUltrasonic("FRONT", FRONT_TRIG, FRONT_ECHO);
  testUltrasonic("LEFT ", LEFT_TRIG, LEFT_ECHO);
  testUltrasonic("RIGHT", RIGHT_TRIG, RIGHT_ECHO);
  testUltrasonic("BACK ", BACK_TRIG, BACK_ECHO);
  testIRSensor();

  Serial.println("─────────────────────────────────────────\n");
  delay(1000);
}
