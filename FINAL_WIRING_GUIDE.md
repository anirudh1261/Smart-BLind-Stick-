# 🔌 Complete Wiring Guide - Smart Blind Stick with GPS Tracking

**Firmware Version**: Latest (4-channel APR with Front/Back combined + Pothole)  
**Last Updated**: January 5, 2026

---

## 📋 Quick Pin Reference Table

| Component | Pin Type | ESP32 GPIO | Notes |
|-----------|----------|------------|-------|
| **ULTRASONIC SENSORS (HC-SR04)** |
| Front Sensor TRIG | Output | GPIO 5 | 5V sensor |
| Front Sensor ECHO | Input | GPIO 18 | Reads 5V (tolerant) |
| Left Sensor TRIG | Output | GPIO 19 | 5V sensor |
| Left Sensor ECHO | Input | GPIO 21 | Reads 5V (tolerant) |
| Right Sensor TRIG | Output | GPIO 22 | 5V sensor |
| Right Sensor ECHO | Input | GPIO 23 | Reads 5V (tolerant) |
| Back Sensor TRIG | Output | GPIO 12 | 5V sensor |
| Back Sensor ECHO | Input | GPIO 13 | Reads 5V (tolerant) |
| **IR GROUND SENSOR** |
| IR Sensor OUT | Input | GPIO 34 | 5V sensor, digital output |
| **GPS MODULE (NEO-6M)** |
| GPS TX | Input (RX) | GPIO 16 | 3.3V module |
| GPS RX | Output (TX) | GPIO 17 | 3.3V module |
| **APR AUDIO MODULE (4 Channel)** |
| Front/Back Alert | Output | GPIO 14 | Trigger K1 |
| Left Alert | Output | GPIO 27 | Trigger K2 |
| Right Alert | Output | GPIO 26 | Trigger K3 |
| Pothole Alert | Output | GPIO 25 | Trigger K4 |
| **FEEDBACK DEVICES** |
| Buzzer | Output | GPIO 33 | Active/Passive |
| Vibration Motor | Output | GPIO 32 | Via transistor |
| **CONTROLS & INDICATORS** |
| SOS Button | Input | GPIO 35 | Pull-up enabled |
| Status LED | Output | GPIO 2 | Built-in LED |

---

## 1️⃣ ULTRASONIC SENSORS (4x HC-SR04)

### Power Requirements
- **VCC**: 5V (from ESP32 VIN or external 5V supply)
- **Current**: ~15mA each, 60mA total for 4 sensors

### Wiring Details

```
┌─────────────────────────────────────────────────┐
│                FRONT SENSOR (HC-SR04)           │
├─────────────────────────────────────────────────┤
│  VCC   →  5V  (ESP32 VIN or power supply)      │
│  TRIG  →  GPIO 5                                │
│  ECHO  →  GPIO 18  (5V signal - ESP32 tolerant)│
│  GND   →  GND                                   │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│                LEFT SENSOR (HC-SR04)            │
├─────────────────────────────────────────────────┤
│  VCC   →  5V                                    │
│  TRIG  →  GPIO 19                               │
│  ECHO  →  GPIO 21                               │
│  GND   →  GND                                   │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│                RIGHT SENSOR (HC-SR04)           │
├─────────────────────────────────────────────────┤
│  VCC   →  5V                                    │
│  TRIG  →  GPIO 22                               │
│  ECHO  →  GPIO 23                               │
│  GND   →  GND                                   │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│                BACK SENSOR (HC-SR04)            │
├─────────────────────────────────────────────────┤
│  VCC   →  5V                                    │
│  TRIG  →  GPIO 12                               │
│  ECHO  →  GPIO 13                               │
│  GND   →  GND                                   │
└─────────────────────────────────────────────────┘
```

### ⚠️ Level Shifter Note
**NOT REQUIRED** - ESP32 GPIOs are 5V-tolerant for ECHO pins.  
For extra safety (optional): Add voltage dividers (1kΩ + 2kΩ) on ECHO lines.

---

## 2️⃣ IR GROUND SENSOR (Pothole Detection)

```
┌─────────────────────────────────────────────────┐
│         IR OBSTACLE SENSOR (FC-51/TCRT5000)     │
├─────────────────────────────────────────────────┤
│  VCC   →  5V                                    │
│  GND   →  GND                                   │
│  OUT   →  GPIO 34  (Digital: HIGH=object)       │
└─────────────────────────────────────────────────┘
```

**Sensitivity**: Adjust potentiometer for 5-15cm detection range  
**Logic**: HIGH = ground detected, LOW = gap/pothole detected

---

## 3️⃣ GPS MODULE (NEO-6M)

```
┌─────────────────────────────────────────────────┐
│              NEO-6M GPS MODULE                  │
├─────────────────────────────────────────────────┤
│  VCC   →  3.3V  ⚠️ NOT 5V!                     │
│  TX    →  GPIO 16  (ESP32 RX)                   │
│  RX    →  GPIO 17  (ESP32 TX)                   │
│  GND   →  GND                                   │
└─────────────────────────────────────────────────┘
```

**CRITICAL**: Use **3.3V** for GPS, NOT 5V!  
**Baud Rate**: 9600 (configured in firmware)  
**LED Indicator**: Blinks when satellite lock acquired

---

## 4️⃣ APR AUDIO MODULE (4-Channel Configuration)

### NEW Configuration (Front/Back Combined + Pothole)

```
┌─────────────────────────────────────────────────┐
│           APR33A3 AUDIO MODULE                  │
├─────────────────────────────────────────────────┤
│  VCC   →  5V                                    │
│  GND   →  GND                                   │
│  K1    →  GPIO 14  (Front/Back: "Obstacle")     │
│  K2    →  GPIO 27  (Left: "Obstacle Left")      │
│  K3    →  GPIO 26  (Right: "Obstacle Right")    │
│  K4    →  GPIO 25  (Pothole: "Pothole Detected")│
│  SPK+  →  Speaker Positive                      │
│  SPK-  →  Speaker Negative                      │
└─────────────────────────────────────────────────┘
```

### 🎙️ Audio Messages to Record

| Channel | GPIO | Trigger | Message to Record |
|---------|------|---------|-------------------|
| K1 | 14 | Front/Back | "Obstacle detected" |
| K2 | 27 | Left | "Obstacle on left" |
| K3 | 26 | Right | "Obstacle on right" |
| K4 | 25 | Pothole | "Pothole detected" |

**Trigger Mode**: HIGH pulse (200ms)  
**Speaker**: 0.5W - 1W, 8Ω recommended

---

## 5️⃣ BUZZER (Backup Audio Alert)

```
┌─────────────────────────────────────────────────┐
│              BUZZER (Active/Passive)            │
├─────────────────────────────────────────────────┤
│  Positive (+)  →  GPIO 33                       │
│  Negative (-)  →  GND                           │
└─────────────────────────────────────────────────┘
```

**Type**: Active or Passive (3.3V/5V compatible)  
**Usage**: Backup alerts when APR is unavailable

---

## 6️⃣ VIBRATION MOTOR

```
┌─────────────────────────────────────────────────┐
│            VIBRATION MOTOR CIRCUIT              │
├─────────────────────────────────────────────────┤
│                                                 │
│  ESP32 GPIO 32                                  │
│       │                                         │
│       ├─── 1kΩ ───┬─── NPN Transistor Base     │
│                    │    (2N2222 / BC547)        │
│                    │                            │
│                 Emitter → GND                   │
│                 Collector → Motor (-)           │
│                                                 │
│  Motor (+) ←── 5V (via diode 1N4007)           │
│                                                 │
└─────────────────────────────────────────────────┘
```

**Component List**:
- NPN Transistor (2N2222 or BC547)
- 1kΩ resistor (base)
- 1N4007 diode (flyback protection)
- Vibration motor (3V-5V DC)

---

## 7️⃣ SOS BUTTON & STATUS LED

### SOS Button

```
┌─────────────────────────────────────────────────┐
│                SOS BUTTON                       │
├─────────────────────────────────────────────────┤
│                                                 │
│         GPIO 35 ───┬─── Button ─── GND         │
│                    │                            │
│                10kΩ Pull-up (internal enabled)  │
│                    │                            │
│                   3.3V                          │
└─────────────────────────────────────────────────┘
```

**Logic**: Press & hold for **2 seconds** to activate SOS  
**Internal Pull-up**: Enabled in firmware

### Status LED

```
┌─────────────────────────────────────────────────┐
│              STATUS LED (Optional)              │
├─────────────────────────────────────────────────┤
│  GPIO 2 ─── 220Ω ─── LED (+) ─── GND           │
└─────────────────────────────────────────────────┘
```

**Note**: GPIO 2 is built-in LED on most ESP32 boards

---

## ⚡ POWER SUPPLY GUIDE

### Option 1: 4× 18650 Battery (Recommended)

```
┌──────────────────────────────────────────────────┐
│         4× 18650 Li-ion Battery System           │
├──────────────────────────────────────────────────┤
│                                                  │
│  Configuration: 2S2P (2 Series, 2 Parallel)     │
│  Voltage: 7.4V nominal                           │
│  Capacity: ~5200mAh (with 2600mAh cells)         │
│                                                  │
│  [Battery 1+] ─┬─ [Battery 2+]                  │
│                │                                 │
│                ├──→ 7.4V (to BMS positive)       │
│                │                                 │
│  [Battery 3+] ─┴─ [Battery 4+]                  │
│                                                  │
│  [All negatives] → BMS negative                  │
│                                                  │
│  BMS Output → LM2596 Step-Down Converter         │
│      ↓                                           │
│   5V Output → ESP32 VIN + Sensors                │
│                                                  │
└──────────────────────────────────────────────────┘
```

**Required Components**:
- 4× 18650 Li-ion batteries (2600mAh recommended)
- 4S BMS (protection circuit)
- LM2596 DC-DC step-down (7.4V → 5V)
- Battery holder (2×2 configuration)
- 3A fuse (safety)

**Runtime**: ~8-10 hours continuous use

### Option 2: USB Power Bank

```
USB Power Bank (5V, 2A minimum)
      ↓
   ESP32 VIN
      ↓
  All Components
```

**Advantages**: Rechargeable, portable  
**Minimum**: 10,000mAh for 6-8 hours

---

## 🔌 COMPLETE CONNECTION DIAGRAM

```
                    ┌──────────────────┐
                    │    ESP32 DEV     │
                    │      BOARD       │
                    └──────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
    ┌───▼───┐         ┌─────▼─────┐      ┌─────▼─────┐
    │  GPS  │         │ Ultrasonic│      │    APR    │
    │NEO-6M │         │  Sensors  │      │  Module   │
    │       │         │  (4x)     │      │ 4-Channel │
    │TX→16  │         │           │      │           │
    │RX←17  │         │Front: 5,18│      │K1←14 (F/B)│
    │3.3V   │         │Left:19,21 │      │K2←27 (L)  │
    └───────┘         │Right:22,23│      │K3←26 (R)  │
                      │Back:12,13 │      │K4←25 (Pot)│
                      │5V Power   │      │5V Power   │
                      └───────────┘      └───────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
    ┌───▼───┐         ┌─────▼─────┐      ┌─────▼─────┐
    │ IR    │         │  Buzzer   │      │ Vibration │
    │Sensor │         │  GPIO 33  │      │  Motor    │
    │GPIO 34│         │   5V      │      │  GPIO 32  │
    │5V     │         └───────────┘      │(Transistor)│
    └───────┘                            └───────────┘
                            │
                      ┌─────▼─────┐
                      │SOS Button │
                      │  GPIO 35  │
                      │   + LED   │
                      └───────────┘

        POWER DISTRIBUTION:
        ├── 5V Rail: HC-SR04 (×4), IR, APR, Buzzer
        ├── 3.3V Rail: GPS Module
        └── ESP32: VIN (5V input)
```

---

## ✅ Pre-Flight Checklist

### Before Powering On:

- [ ] All GND connections common
- [ ] 5V components on 5V rail
- [ ] 3.3V for GPS (NOT 5V!)
- [ ] No short circuits
- [ ] Transistor for vibration motor
- [ ] Diode on motor (flyback protection)
- [ ] SOS button wired correctly
- [ ] APR speaker connected

### After Power On:

- [ ] Status LED blinking (heartbeat)
- [ ] GPS LED blinking (satellite search)
- [ ] WiFi connects (check serial monitor)
- [ ] Ultrasonic sensors respond to hand waves
- [ ] IR sensor detects ground
- [ ] APR plays audio on trigger
- [ ] Vibration motor works
- [ ] SOS button functional

---

## 🐛 Troubleshooting

| Problem | Check |
|---------|-------|
| GPS not working | 3.3V power? TX/RX swapped? Outdoors? |
| Ultrasonic reading 0cm | 5V power? TRIG/ECHO correct? |
| APR no sound | Speaker connected? 5V power? Recordings done? |
| SOS keeps triggering | Button floating? Add pull-up or connect properly |
| Vibration not working | Transistor circuit? Diode direction? |
| WiFi won't connect | SSID/password correct in config.h? |

---

## 📱 WiFi Configuration

Remember to configure in `include/config.h`:

```cpp
#define WIFI_SSID "WiFi"        // Your hotspot name
#define WIFI_PASSWORD "wordpass" // Your password
```

---

## 🎯 Final Notes

- **No level shifters needed** for HC-SR04 (ESP32 is 5V-tolerant)
- **GPS must be outdoors** for satellite lock
- **APR recordings** required for audio feedback
- **GeoLinker tracking** active at https://www.circuitdigest.cloud/geolinker
- **Device ID**: BlindStick_001

**Your Smart Blind Stick is ready for assembly!** 🚀
