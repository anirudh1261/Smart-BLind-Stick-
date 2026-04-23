# How Sensors Detect Obstacles - Complete Guide

## Sensor Detection System Overview

The Smart Blind Stick uses **4 different sensors** working together to provide 360° awareness:

```
        [FRONT ULTRASONIC]
               ↓
    Detects: Head-level obstacles
    Range: 2-200 cm forward
    Angle: 15° downward
    
    
[LEFT ULTRASONIC] ← USER → [RIGHT ULTRASONIC]
    ↓                           ↓
Detects: Side obstacles    Detects: Side obstacles
Range: 2-200 cm           Range: 2-200 cm
Angle: 45° outward        Angle: 45° outward


        [IR SENSOR]
            ↓
    Detects: Ground drops
    Range: 0-30 cm
    Angle: 90° downward
```

---

## 1. Front Ultrasonic Sensor (HC-SR04)

### How It Works

**Technology**: Sound waves (ultrasonic)

**Process**:
```
1. ESP32 sends trigger pulse → Sensor
2. Sensor emits ultrasonic sound (40kHz)
3. Sound travels forward → Hits obstacle
4. Sound bounces back (echo) → Sensor
5. Sensor measures time taken
6. ESP32 calculates distance
```

**Formula**:
```
Distance (cm) = (Time × Speed of Sound) / 2

Speed of Sound = 343 m/s = 0.0343 cm/μs
Time = Echo pulse duration (microseconds)
```

**Example**:
```
Obstacle at 100cm:
- Sound travels 100cm to obstacle
- Sound travels 100cm back
- Total distance: 200cm
- Time = 200cm / 0.0343 cm/μs = 5,830 μs
- Measured time / 2 = 100cm ✓
```

### Code Implementation

```cpp
float measureDistance(uint8_t trigPin, uint8_t echoPin) {
    // 1. Send 10μs trigger pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // 2. Measure echo pulse duration
    long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
    
    // 3. Calculate distance
    float distance = duration * 0.034 / 2.0;
    
    return distance; // in cm
}
```

### Detection Zones

```
SAFE (>100cm):     ░░░░░░░░░░░░░░░░░░░░
                   No alert

WARNING (50-100cm): ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
                    Audio: "Obstacle ahead"
                    Vibration: Slow pulse (500ms)
                    Buzzer: Single beep

CRITICAL (<50cm):   ████████████████████
                    Audio: "Obstacle ahead. Please stop."
                    Vibration: Fast pulse (200ms)
                    Buzzer: Double beep
```

### What It Detects

✅ **Can Detect**:
- Walls
- Poles
- Trees
- People
- Vehicles
- Doors
- Furniture
- Hanging branches

❌ **Cannot Detect**:
- Very thin objects (< 2cm diameter)
- Soft materials (fabric, foam)
- Angled surfaces (sound reflects away)
- Objects beyond 200cm

---

## 2. Left & Right Ultrasonic Sensors

### How They Work

**Same technology** as front sensor, but mounted at **45° angles**

**Purpose**: Detect obstacles to the sides

### Detection Pattern

**Top View**:
```
              Front
                ↑
                |
                |
    45°    ←────┼────→    45°
   Left         |        Right
  Sensor        |       Sensor
                |
              User
```

**Coverage Area**:
```
        ╱─────────────╲
       ╱   DETECTION   ╲
      ╱      ZONE       ╲
     ╱                   ╲
    ╱         ●          ╲  ← User
   ╱        Stick         ╲
  ╱                       ╲
 ╱─────────────────────────╲
```

### Use Cases

**Left Sensor Detects**:
- Narrow passages (doorways)
- Walls on left side
- People approaching from left
- Parked vehicles

**Right Sensor Detects**:
- Obstacles on right side
- Curbs
- Railings
- Street furniture

### Code Logic

```cpp
SensorReading scanAll() {
    // Scan all three directions
    SensorReading front = scanFront();
    SensorReading left = scanLeft();
    SensorReading right = scanRight();
    
    // Return the MOST CRITICAL reading
    if (front.level == CRITICAL) return front;
    if (left.level == CRITICAL) return left;
    if (right.level == CRITICAL) return right;
    
    if (front.level == WARNING) return front;
    if (left.level == WARNING) return left;
    if (right.level == WARNING) return right;
    
    return front; // All safe
}
```

**Priority**: Closest obstacle gets highest priority

---

## 3. IR Sensor (Ground Detection)

### How It Works

**Technology**: Infrared light

**Process**:
```
1. IR LED emits infrared light downward
2. Light hits ground → Reflects back
3. IR receiver detects reflected light
4. Measures intensity of reflection
5. ESP32 reads analog value (0-4095)
```

### Detection Logic

```
Normal Ground:
    IR LED → ████ Ground
    Strong reflection
    ADC Value: 200-400
    Status: SAFE ✓

Pothole/Drop:
    IR LED → ░░░░ (no ground)
    Weak/no reflection
    ADC Value: 500-4095
    Status: DANGER ⚠️
```

### Code Implementation

```cpp
bool detectPothole() {
    int irValue = analogRead(IR_SENSOR_PIN);
    
    if (irValue > POTHOLE_THRESHOLD) {
        // No ground detected = Pothole!
        return true;
    }
    return false;
}
```

**Threshold**: 500 (adjustable based on sensor)

### What It Detects

✅ **Can Detect**:
- Potholes
- Stairs (downward)
- Curbs
- Ditches
- Sudden drops
- Missing manhole covers

❌ **Cannot Detect**:
- Stairs going up
- Small bumps (<3cm)
- Transparent surfaces
- Very dark surfaces (absorb IR)

### Mounting Position

**Critical**: Must be 5cm from ground
```
    Stick
      |
      |
      |
    [IR] ← 5cm from ground
      ↓
  ─────── Ground
```

**Too high**: Won't detect potholes in time  
**Too low**: Will hit ground while walking

---

## 4. GPS Module (NEO-6M)

### How It Works

**Technology**: Satellite signals

**Process**:
```
1. GPS antenna receives signals from satellites
2. Needs signals from 4+ satellites
3. Calculates position using trilateration
4. Outputs NMEA sentences via UART
5. ESP32 parses coordinates
```

### Satellite Communication

```
        🛰️ Satellite 1
       ╱
      ╱
     ╱    🛰️ Satellite 2
    ╱    ╱
   ╱    ╱
  ╱    ╱   🛰️ Satellite 3
 ╱    ╱   ╱
╱    ╱   ╱
    ╱   ╱  🛰️ Satellite 4
   ╱   ╱  ╱
  ╱   ╱  ╱
 ╱   ╱  ╱
[GPS Module]
```

**Minimum**: 4 satellites for 3D position  
**Optimal**: 8+ satellites for accuracy

### NMEA Data Format

**Raw Data** (from GPS):
```
$GPGGA,123519,2861.39,N,07720.90,E,1,08,0.9,545.4,M,46.9,M,,*47
```

**Parsed Data**:
```
Time: 12:35:19
Latitude: 28.6139° N
Longitude: 77.2090° E
Satellites: 8
Altitude: 545.4 m
```

### Code Implementation

```cpp
void updateGPS() {
    while (gpsSerial->available()) {
        char c = gpsSerial->read();
        gps.encode(c); // TinyGPS++ library
    }
    
    if (gps.location.isValid()) {
        float lat = gps.location.lat();
        float lng = gps.location.lng();
        int sats = gps.satellites.value();
        
        // Send to GeoLinker cloud
        uploadLocation(lat, lng);
    }
}
```

### Accuracy

**Typical**: 3-5 meters in open areas  
**Urban**: 10-15 meters (buildings block signals)  
**Indoor**: No fix (needs sky view)

---

## How All Sensors Work Together

### Main Loop (Every 100ms)

```cpp
void loop() {
    // 1. SCAN ALL ULTRASONIC SENSORS
    SensorReading front = scanFront();   // 50ms
    SensorReading left = scanLeft();     // 50ms
    SensorReading right = scanRight();   // 50ms
    
    // 2. CHECK IR SENSOR
    bool pothole = detectPothole();      // 1ms
    
    // 3. DETERMINE PRIORITY
    if (pothole) {
        // HIGHEST PRIORITY
        playPotholeAlert();
        startVibration(DANGER);
        playBuzzerAlert(DANGER);
    } else if (front.level == CRITICAL) {
        playObstacleAlert(FRONT);
        startVibration(CRITICAL);
        playBuzzerAlert(CRITICAL);
    } else if (left.level == CRITICAL) {
        playObstacleAlert(LEFT);
        startVibration(CRITICAL);
    } else if (right.level == CRITICAL) {
        playObstacleAlert(RIGHT);
        startVibration(CRITICAL);
    } else if (front.level == WARNING) {
        playObstacleAlert(FRONT);
        startVibration(WARNING);
        playBuzzerAlert(WARNING);
    }
    
    // 4. UPDATE GPS (every 1 second)
    if (millis() - lastGPSUpdate > 1000) {
        updateGPS();
        lastGPSUpdate = millis();
    }
}
```

### Priority System

```
1. POTHOLE (IR Sensor)          ← HIGHEST PRIORITY
   ↓
2. CRITICAL OBSTACLE (<50cm)    ← IMMEDIATE DANGER
   ↓
3. WARNING OBSTACLE (50-100cm)  ← CAUTION
   ↓
4. SAFE (>100cm)                ← NO ALERT
```

---

## Real-World Scenarios

### Scenario 1: Walking on Sidewalk

```
Situation:
    Tree ahead (80cm)
    Pole on left (60cm)
    Clear on right

Detection:
    Front: WARNING (80cm)
    Left: CRITICAL (60cm)
    Right: SAFE (200cm+)

Response:
    Alert: "Obstacle on left. Turn right."
    Vibration: Fast pulse
    Buzzer: Double beep
    
Action:
    User turns right to avoid pole
```

### Scenario 2: Approaching Stairs

```
Situation:
    Stairs going down
    IR sensor detects drop

Detection:
    IR: POTHOLE detected
    Front: SAFE
    Left: SAFE
    Right: SAFE

Response:
    Alert: "Pothole detected. Step carefully."
    Vibration: Continuous
    Buzzer: Long beep
    
Action:
    User stops and feels for stairs
```

### Scenario 3: Narrow Doorway

```
Situation:
    Door frame 80cm wide
    User approaching center

Detection:
    Front: SAFE (door open)
    Left: WARNING (40cm to frame)
    Right: WARNING (40cm to frame)

Response:
    Alert: "Obstacle on left/right"
    Vibration: Slow pulse
    Buzzer: Single beep
    
Action:
    User walks straight through center
```

---

## Sensor Limitations

### Ultrasonic Sensors

**Weather Effects**:
- Rain: Droplets can cause false readings
- Wind: Can deflect sound waves
- Temperature: Affects speed of sound slightly

**Solutions**:
- Multiple sensors for redundancy
- Software filtering of erratic readings
- Ignore readings > 200cm

### IR Sensor

**Surface Issues**:
- Very dark surfaces: Low reflection
- Shiny surfaces: Specular reflection
- Wet surfaces: Different reflection

**Solutions**:
- Adjustable threshold in code
- Test in different conditions
- Combine with ultrasonic data

### GPS Module

**Signal Issues**:
- Buildings: Block satellite signals
- Trees: Reduce accuracy
- Indoor: No fix possible

**Solutions**:
- Offline data buffering
- Last known location
- WiFi positioning (future)

---

## Calibration & Testing

### Ultrasonic Calibration

```cpp
void calibrate() {
    Serial.println("Calibrating sensors...");
    
    // Take 10 readings in open space
    float readings[10];
    for (int i = 0; i < 10; i++) {
        readings[i] = measureDistance(FRONT_TRIG, FRONT_ECHO);
        delay(100);
    }
    
    // Calculate average (should be >200cm in open space)
    float avg = calculateAverage(readings, 10);
    Serial.print("Calibration: ");
    Serial.print(avg);
    Serial.println(" cm");
}
```

### IR Threshold Adjustment

```cpp
void findIRThreshold() {
    // Hold over normal ground
    int groundValue = analogRead(IR_SENSOR_PIN);
    Serial.print("Ground: ");
    Serial.println(groundValue);
    
    // Hold over edge/pothole
    delay(2000);
    int potholeValue = analogRead(IR_SENSOR_PIN);
    Serial.print("Pothole: ");
    Serial.println(potholeValue);
    
    // Set threshold between them
    int threshold = (groundValue + potholeValue) / 2;
    Serial.print("Suggested threshold: ");
    Serial.println(threshold);
}
```

---

## Summary

### Detection Coverage

```
Vertical Coverage:
    ┌─────────────┐
    │   Front     │ ← 85cm (head level)
    │  Ultrasonic │
    ├─────────────┤
    │  Left/Right │ ← 60cm (waist level)
    │  Ultrasonic │
    ├─────────────┤
    │             │
    │   Clear     │
    │             │
    ├─────────────┤
    │ IR Sensor   │ ← 5cm (ground level)
    └─────────────┘
```

### Sensor Specifications

| Sensor | Range | Angle | Update Rate | Purpose |
|--------|-------|-------|-------------|---------|
| Front Ultrasonic | 2-200cm | 15° down | 100ms | Head obstacles |
| Left Ultrasonic | 2-200cm | 45° left | 100ms | Side obstacles |
| Right Ultrasonic | 2-200cm | 45° right | 100ms | Side obstacles |
| IR Sensor | 0-30cm | 90° down | 100ms | Ground drops |
| GPS Module | Global | 360° | 1s | Location tracking |

### Response Times

```
Sensor Scan:     ~150ms (all 3 ultrasonic)
IR Check:        ~1ms
Alert Decision:  ~5ms
Audio Playback:  ~100ms
Vibration Start: ~1ms
Buzzer Beep:     ~100ms

Total Response:  ~250ms from detection to alert
```

**Fast enough** for walking speed (1-2 m/s)!

---

**The sensors work together to create a complete awareness system, detecting obstacles from all directions and at all heights!** 🎯
