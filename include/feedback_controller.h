#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#include "sensor_manager.h"
#include <Arduino.h>

class FeedbackController {
private:
  uint8_t vibrationPin;
  uint8_t buzzerPin;
  uint8_t aprTrigger1, aprTrigger2, aprTrigger3;

  unsigned long lastVibrationToggle;
  bool vibrationState;
  int currentPattern;

  void triggerAudio(uint8_t channel);
  void setVibrationPattern(int patternMs);
  void playBuzzerTone(int frequency, int duration);
  void playBuzzerPattern(AlertLevel level);

public:
  FeedbackController();
  void begin();

  // Audio feedback
  void playObstacleAlert(SensorDirection direction);
  void playPotholeAlert();
  void playSOSAlert();
  void playBatteryLowAlert();

  // Vibration feedback
  void startVibration(AlertLevel level);
  void stopVibration();
  void updateVibration(); // Call in loop() for pulsing

  // Buzzer feedback (backup/additional alerts)
  void playBuzzerAlert(AlertLevel level);
  void playBuzzerSOS();

  // Combined feedback
  void provideAlert(SensorReading reading, bool potholeDetected);
};

#endif
