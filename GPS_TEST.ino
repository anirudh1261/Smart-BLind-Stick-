/*
 * GPS Module Diagnostic Tool
 * Tests GPS wiring and raw NMEA output
 */

#define GPS_RX 16 // ESP32 GPIO16 -> GPS TX
#define GPS_TX 17 // ESP32 GPIO17 -> GPS RX

HardwareSerial gpsSerial(1);

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n\n=== GPS MODULE DIAGNOSTIC ===\n");

  // Initialize GPS serial
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Serial.println("GPS Serial initialized on:");
  Serial.println("  ESP32 GPIO16 (RX) <- GPS TX");
  Serial.println("  ESP32 GPIO17 (TX) -> GPS RX");
  Serial.println("\nListening for GPS data...\n");
  Serial.println("Expected output: NMEA sentences ($GPGGA, $GPRMC, etc.)");
  Serial.println("If you see nothing or garbage, check wiring!\n");
  Serial.println("==========================================\n");
}

void loop() {
  // Read and display raw GPS data
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    Serial.write(c); // Print exactly what GPS sends
  }

  // Show activity indicator every 5 seconds
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 5000) {
    lastBlink = millis();
    Serial.println("[Waiting for GPS data...]");
  }
}
