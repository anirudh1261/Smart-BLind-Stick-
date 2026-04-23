#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <Arduino.h>
#include <TinyGPSPlus.h>

struct GPSLocation {
  double latitude;
  double longitude;
  float altitude;
  float speed;
  int satellites;
  bool isValid;
  unsigned long timestamp;
};

class GPSHandler {
private:
  TinyGPSPlus gps;
  HardwareSerial *gpsSerial;
  GPSLocation lastLocation;

  unsigned long lastUpdate;
  bool initialized;

public:
  GPSHandler();
  void begin(HardwareSerial *serial);

  // GPS data methods
  void update();
  GPSLocation getLocation();
  bool hasValidFix();

  // Utility methods
  String getLocationString();
  String getGoogleMapsLink();
  float getDistanceTo(double lat, double lng); // Distance in meters

  // Status
  int getSatelliteCount();
  bool isLocationFresh(unsigned long maxAgeMs = 5000);
};

#endif
