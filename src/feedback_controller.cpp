#include "feedback_controller.h"
#include "config.h"

FeedbackController::FeedbackController() {
  vibrationPin = VIBRATION_PIN;
  buzzerPin = BUZZER_PIN;

  lastVibrationToggle = 0;
  vibrationState = false;
  currentPattern = 0;
}

void FeedbackController::begin() {
  // APR Module - 4 Channel Setup
  pinMode(APR_FRONT_BACK_PIN, OUTPUT); // Front/Back combined
  pinMode(APR_LEFT_PIN, OUTPUT);
  pinMode(APR_RIGHT_PIN, OUTPUT);
  pinMode(APR_POTHOLE_PIN, OUTPUT); // Pothole detection
  digitalWrite(APR_FRONT_BACK_PIN, LOW);
  digitalWrite(APR_LEFT_PIN, LOW);
  digitalWrite(APR_RIGHT_PIN, LOW);
  digitalWrite(APR_POTHOLE_PIN, LOW);

  Serial.println("[FeedbackController] Initialized (APR + Buzzer)");
}

void FeedbackController::triggerAudio(uint8_t channel) {
  // APR module: Pull pin HIGH for 100ms to trigger audio
  digitalWrite(channel, HIGH);
  delay(100);
  digitalWrite(channel, LOW);
}

void FeedbackController::setVibrationPattern(int patternMs) {
  currentPattern = patternMs;
  lastVibrationToggle = millis();
  vibrationState = true;
  digitalWrite(vibrationPin, HIGH);
}

void FeedbackController::playObstacleAlert(SensorDirection direction) {
  uint8_t triggerPin = 0;

  // Map sensor direction to APR channel
  switch (direction) {
  case FRONT:
  case BACK: // Front and Back use SAME channel
    triggerPin = APR_FRONT_BACK_PIN;
    Serial.println("[Audio] Playing: Obstacle Detected");
    break;
  case LEFT:
    triggerPin = APR_LEFT_PIN;
    Serial.println("[Audio] Playing: Obstacle Left");
    break;
  case RIGHT:
    triggerPin = APR_RIGHT_PIN;
    Serial.println("[Audio] Playing: Obstacle Right");
    break;
  }

  // Trigger APR channel
  if (triggerPin > 0) {
    digitalWrite(triggerPin, HIGH);
    delay(APR_TRIGGER_DURATION);
    digitalWrite(triggerPin, LOW);
  }
}

void FeedbackController::playPotholeAlert() {
  // Use dedicated pothole channel
  Serial.println("[Audio] Playing: Pothole Detected");
  digitalWrite(APR_POTHOLE_PIN, HIGH);
  delay(APR_TRIGGER_DURATION);
  digitalWrite(APR_POTHOLE_PIN, LOW);
}

void FeedbackController::playSOSAlert() {
  Serial.println("[Audio] SOS activated");
  // Continuous beep pattern
  for (int i = 0; i < 3; i++) {
    triggerAudio(aprTrigger1);
    delay(200);
  }
}

void FeedbackController::playBatteryLowAlert() {
  Serial.println("[Audio] Battery low");
  triggerAudio(aprTrigger3);
}

void FeedbackController::startVibration(AlertLevel level) {
  switch (level) {
  case SAFE:
    stopVibration();
    break;
  case WARNING:
    setVibrationPattern(VIB_PATTERN_WARNING);
    Serial.println("[Vibration] Warning pattern");
    break;
  case CRITICAL:
    setVibrationPattern(VIB_PATTERN_CRITICAL);
    Serial.println("[Vibration] Critical pattern");
    break;
  case DANGER:
    currentPattern = 0; // Continuous
    digitalWrite(vibrationPin, HIGH);
    Serial.println("[Vibration] Continuous");
    break;
  }
}

void FeedbackController::stopVibration() {
  digitalWrite(vibrationPin, LOW);
  vibrationState = false;
  currentPattern = 0;
}

void FeedbackController::updateVibration() {
  // Handle pulsing vibration patterns
  if (currentPattern > 0) {
    unsigned long now = millis();
    if (now - lastVibrationToggle >= currentPattern) {
      vibrationState = !vibrationState;
      digitalWrite(vibrationPin, vibrationState ? HIGH : LOW);
      lastVibrationToggle = now;
    }
  }
}

void FeedbackController::provideAlert(SensorReading reading,
                                      bool potholeDetected) {
  // Pothole has highest priority
  if (potholeDetected) {
    playPotholeAlert();
    startVibration(DANGER);
    return;
  }

  // Handle obstacle alerts
  if (reading.level >= WARNING) {
    playObstacleAlert(reading.direction);
    startVibration(reading.level);
  } else {
    stopVibration();
  }
}
