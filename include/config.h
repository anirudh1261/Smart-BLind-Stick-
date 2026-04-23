#ifndef CONFIG_H
#define CONFIG_H

// ===== PIN DEFINITIONS =====

// Ultrasonic Sensors (HC-SR04) - 360° Coverage
#define FRONT_TRIG_PIN 23
#define FRONT_ECHO_PIN 22
#define LEFT_TRIG_PIN 21
#define LEFT_ECHO_PIN 19
#define RIGHT_TRIG_PIN 18
#define RIGHT_ECHO_PIN 5
#define BACK_TRIG_PIN 4  // NEW: Rear obstacle detection
#define BACK_ECHO_PIN 15 // NEW: Complete 360° coverage

// IR Sensor (Ground/Pothole Detection)
#define IR_SENSOR_PIN 34

// GPS Module (NEO-6M)
#define GPS_RX_PIN 16
#define GPS_TX_PIN 17

// ===== APR AUDIO MODULE - 4 Channel =====
// Channel 1: Front/Back obstacle (combined)
// Channel 2: Left obstacle
// Channel 3: Right obstacle
// Channel 4: Pothole detected
#define APR_FRONT_BACK_PIN 14    // "Obstacle ahead/behind" (Ch1)
#define APR_LEFT_PIN 27          // "Obstacle on left" (Ch2)
#define APR_RIGHT_PIN 26         // "Obstacle on right" (Ch3)
#define APR_POTHOLE_PIN 25       // "Pothole detected" (Ch4)
#define APR_TRIGGER_DURATION 200 // Pulse duration (ms)

// ===== BUZZER PIN =====
#define BUZZER_PIN 33 // Active/Passive buzzer for backup alerts

// Vibration Motor
#define VIBRATION_PIN 32

// SOS Emergency Button
#define SOS_BUTTON_PIN 35

// Status LED
#define STATUS_LED_PIN 2

// ===== DISTANCE THRESHOLDS (cm) =====
#define CRITICAL_DISTANCE 50 // Immediate danger
#define WARNING_DISTANCE 100 // Caution zone
#define SAFE_DISTANCE 200    // Detection range

// ===== IR SENSOR THRESHOLDS =====
#define POTHOLE_THRESHOLD 500 // ADC value indicating ground drop

// ===== TIMING CONSTANTS =====
#define SENSOR_SCAN_INTERVAL 100 // ms between scans
#define GPS_UPDATE_INTERVAL 1000 // ms between GPS reads
#define DEBOUNCE_DELAY 50        // ms for button debounce

// ===== VIBRATION PATTERNS =====
#define VIB_PATTERN_WARNING 500  // Slow pulse (ms)
#define VIB_PATTERN_CRITICAL 200 // Fast pulse (ms)
#define VIB_PATTERN_DANGER 0     // Continuous

// ===== BUZZER PATTERNS =====
#define BUZZER_FREQ_WARNING 1000  // Hz (low pitch for warning)
#define BUZZER_FREQ_CRITICAL 2000 // Hz (medium pitch for critical)
#define BUZZER_FREQ_DANGER 3000   // Hz (high pitch for danger)
#define BUZZER_FREQ_SOS 2500      // Hz (SOS alert)
#define BUZZER_DURATION_SHORT 100 // ms (short beep)
#define BUZZER_DURATION_LONG 500  // ms (long beep)

// ===== SYSTEM SETTINGS =====
#define SERIAL_BAUD_RATE 115200
#define GPS_BAUD_RATE 9600
#define SOUND_SPEED 0.034 // cm/μs

// ===== EMERGENCY CONTACTS =====
#define GUARDIAN_PHONE "+919876543210" // Replace with actual number

// ===== WIFI CONFIGURATION =====
// Option 1: Connect to mobile hotspot (for outdoor tracking)
#define WIFI_SSID "WiFi"         // Your mobile hotspot name
#define WIFI_PASSWORD "wordpass" // Your mobile hotspot password

// Option 2: Use Access Point mode (creates its own WiFi network)
#define AP_MODE_ENABLED false // Set to true to create own WiFi instead
#define AP_SSID "BlindStick-Tracker"
#define AP_PASSWORD "12345678" // Min 8 characters

// ===== WEB SERVER =====
#define WEB_SERVER_PORT 80
#define LOCATION_UPDATE_INTERVAL 2000 // ms (how often to send location to web)

// ===== GEOLINKER GPS TRACKING =====
// CircuitDigest Cloud GPS Tracking Service
// Dashboard: https://www.circuitdigest.cloud/geolinker
#define ENABLE_GEOLINKER true // Set to false to disable cloud tracking
#define GEOLINKER_API_KEY "9gPyNAUrs4U1" // Your Circuit Digest API key
#define GEOLINKER_DEVICE_ID                                                    \
  "BlindStick_001"                  // Unique identifier for this device
#define GEOLINKER_UPDATE_INTERVAL 5 // seconds (how often to send GPS data)
#define GEOLINKER_OFFLINE_BUFFER 20 // Max offline records to store

// Get your free API key at: https://circuitdigest.cloud
// View tracked location at: https://circuitdigest.cloud/geolinker

#endif
