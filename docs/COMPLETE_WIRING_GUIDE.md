# Complete Wiring Guide - 4-Sensor Smart Blind Stick

## 📋 Component List

### Main Components
- [x] ESP32 DevKit (30-pin)
- [x] 4× HC-SR04 Ultrasonic Sensors
- [x] 1× IR Obstacle Avoidance Sensor
- [x] 1× NEO-6M GPS Module
- [x] 1× APR Voice Module (8-channel)
- [x] 1× Speaker (8Ω, 0.5W)
- [x] 1× Buzzer (Active or Passive)
- [x] 1× Vibration Motor (3V-5V)
- [x] 1× Push Button (SOS)
- [x] 1× LED (Status indicator)
- [x] 2× 18650 Li-ion Batteries (3.7V)

### Additional Parts
- [ ] Battery holder (2× 18650)
- [ ] Step-down converter (7.4V → 5V)
- [ ] Breadboard or PCB
- [ ] Jumper wires (M-M, M-F)
- [ ] Resistors: 220Ω (LED), 1kΩ (buzzer), 10kΩ (button)
- [ ] Transistor 2N2222 (for vibration motor)
- [ ] Heat shrink tubing
- [ ] Solder and soldering iron

---

## 🔌 Complete Pin Mapping Table

| Component | Pin/Wire | ESP32 GPIO | Type | Notes |
|-----------|----------|------------|------|-------|
| **Front Ultrasonic** | TRIG | GPIO 23 | Output | Trigger pulse |
| | ECHO | GPIO 22 | Input | Echo reading |
| | VCC | 5V | Power | |
| | GND | GND | Ground | |
| **Left Ultrasonic** | TRIG | GPIO 21 | Output | Trigger pulse |
| | ECHO | GPIO 19 | Input | Echo reading |
| | VCC | 5V | Power | |
| | GND | GND | Ground | |
| **Right Ultrasonic** | TRIG | GPIO 18 | Output | Trigger pulse |
| | ECHO | GPIO 5 | Input | Echo reading |
| | VCC | 5V | Power | |
| | GND | GND | Ground | |
| **Back Ultrasonic** | TRIG | GPIO 4 | Output | Trigger pulse |
| | ECHO | GPIO 15 | Input | Echo reading |
| | VCC | 5V | Power | |
| | GND | GND | Ground | |
| **IR Sensor** | OUT | GPIO 34 | Input (ADC) | Analog reading |
| | VCC | 3.3V | Power | NOT 5V! |
| | GND | GND | Ground | |
| **GPS Module** | RX | GPIO 17 | Output (TX2) | ESP32 TX → GPS RX |
| | TX | GPIO 16 | Input (RX2) | GPS TX → ESP32 RX |
| | VCC | 3.3V | Power | NOT 5V! |
| | GND | GND | Ground | |
| **APR Module** | TRIG1 | GPIO 25 | Output | "Obstacle ahead" |
| | TRIG2 | GPIO 26 | Output | "Pothole detected" |
| | TRIG3 | GPIO 27 | Output | "Turn/Behind" |
| | VCC | 5V | Power | |
| | GND | GND | Ground | |
| | AUDIO+ | Speaker+ | Output | |
| | AUDIO- | Speaker- | Output | |
| **Buzzer** | + | GPIO 33 | Output | Via 1kΩ resistor |
| | - | GND | Ground | |
| **Vibration Motor** | + | GPIO 32 | Output | Via transistor |
| | - | GND | Ground | |
| **SOS Button** | Pin1 | GPIO 35 | Input (Pull-up) | |
| | Pin2 | GND | Ground | |
| **Status LED** | Anode (+) | GPIO 2 | Output | Via 220Ω resistor |
| | Cathode (-) | GND | Ground | |
| **Power** | VIN | 5V | Power | From step-down |
| | GND | GND | Ground | Common ground |
| | 3.3V | OUT | Output | For GPS, IR |

---

## 🔋 Power Distribution System

### Power Architecture

```
┌──────────────────────────────────────────────────────┐
│                  POWER SYSTEM                        │
│                                                      │
│  Battery Pack (2× 18650 in series)                  │
│  ┌────────────┐                                     │
│  │  Battery 1 │  3.7V                               │
│  │  (3.7V)    ├─────┐                               │
│  └────────────┘     │                               │
│                     ├──→ 7.4V Total                 │
│  ┌────────────┐     │                               │
│  │  Battery 2 │     │                               │
│  │  (3.7V)    ├─────┘                               │
│  └────────────┘                                     │
│                                                      │
│         │                                            │
│         ▼                                            │
│  ┌─────────────────┐                                │
│  │ Step-Down       │                                │
│  │ Converter       │                                │
│  │ (LM2596/XL4015) │                                │
│  │                 │                                │
│  │ IN: 7.4V        │                                │
│  │ OUT: 5.0V       │                                │
│  │ Current: 2-3A   │                                │
│  └────────┬────────┘                                │
│           │                                          │
│           ├──→ 5V Rail                              │
│           │                                          │
│    ┌──────┴──────┬──────┬──────┬──────┐            │
│    │             │      │      │      │            │
│  ESP32         HC-SR04  APR   Vib  Buzzer          │
│  (VIN)        (×4 VCC)  VCC   (via (via           │
│                               trans) res)          │
│                                                      │
│  ESP32 3.3V Output → GPS VCC, IR VCC                │
└──────────────────────────────────────────────────────┘
```

### Wiring Steps

#### 1. Battery Connection (Series)
```
Battery 1 (+) ──────┐
                    │
Battery 2 (-)  ─────┤
                    │
                    ├───→ 7.4V to Step-Down IN+
                    
Battery 1 (-)  ─────┐
                    │
Battery 2 (+)  ─────┤
                    │
                    ├───→ GND to Step-Down IN-
```

#### 2. Step-Down Converter Setup
```
IN+ (7.4V)  ←  Battery Pack (+)
IN- (GND)   ←  Battery Pack (-)

OUT+ (5V)   →  ESP32 VIN + All 5V components
OUT- (GND)  →  Common Ground
```

**Important**: Adjust output voltage to exactly 5.0V using potentiometer!

---

## 📡 Sensor Wiring Details

### 1. Front Ultrasonic Sensor (HC-SR04)

```
┌─────────────────────────────────────────┐
│         HC-SR04 (FRONT)                 │
│  ┌─────────────────────────────┐        │
│  │  ●    ●    Transmitter/      │        │
│  │            Receiver          │        │
│  │                              │        │
│  │  VCC TRIG ECHO GND          │        │
│  └───┬───┬────┬────┬───────────┘        │
│      │   │    │    │                    │
│      │   │    │    │                    │
│     Red Yellow Green Black             │
└──────┼───┼────┼────┼────────────────────┘
       │   │    │    │
       │   │    │    └──→ ESP32 GND
       │   │    └───────→ ESP32 GPIO 22 (ECHO)
       │   └────────────→ ESP32 GPIO 23 (TRIG)
       └────────────────→ 5V Power Rail
```

**Pin Functions**:
- **VCC**: 5V power supply
- **TRIG**: Trigger pin (receives 10μs pulse from ESP32)
- **ECHO**: Echo pin (sends pulse duration to ESP32)
- **GND**: Ground

**Wire Colors** (Standard):
- Red: VCC (5V)
- Yellow: TRIG
- Green: ECHO
- Black: GND

### 2. Left Ultrasonic Sensor

```
HC-SR04 (LEFT)         ESP32
──────────────         ─────
VCC (Red)       →      5V
TRIG (Yellow)   →      GPIO 21
ECHO (Green)    →      GPIO 19
GND (Black)     →      GND
```

### 3. Right Ultrasonic Sensor

```
HC-SR04 (RIGHT)        ESP32
──────────────         ─────
VCC (Red)       →      5V
TRIG (Yellow)   →      GPIO 18
ECHO (Green)    →      GPIO 5
GND (Black)     →      GND
```

### 4. Back Ultrasonic Sensor (NEW!)

```
HC-SR04 (BACK)         ESP32
──────────────         ─────
VCC (Red)       →      5V
TRIG (Yellow)   →      GPIO 4
ECHO (Green)    →      GPIO 15
GND (Black)     →      GND
```

---

### 5. IR Sensor (Ground Detection)

```
┌─────────────────────────────────┐
│      IR Sensor Module           │
│  ┌─────────────────┐            │
│  │  IR LED  ║  RX  │            │
│  │    ↓     ║  ↑   │            │
│  │  VCC OUT GND    │            │
│  └───┬──┬────┬─────┘            │
│      │  │    │                  │
└──────┼──┼────┼──────────────────┘
       │  │    │
       │  │    └──→ ESP32 GND
       │  └───────→ ESP32 GPIO 34 (ADC1_CH6)
       └──────────→ ESP32 3.3V (NOT 5V!)
```

**Critical**: Use 3.3V, NOT 5V!

**Reading Type**: Analog (0-4095)
- Ground present: 200-400
- Pothole/drop: 500-4095

---

### 6. GPS Module (NEO-6M)

```
┌──────────────────────────────────┐
│      NEO-6M GPS Module           │
│  ┌────────────────────┐          │
│  │   [Ceramic Patch   │          │
│  │    Antenna]        │          │
│  │                    │          │
│  │  VCC TX RX GND     │          │
│  └───┬──┬──┬───┬──────┘          │
│      │  │  │   │                 │
└──────┼──┼──┼───┼─────────────────┘
       │  │  │   │
       │  │  │   └──→ ESP32 GND
       │  │  └──────→ ESP32 GPIO 17 (TX2)
       │  └─────────→ ESP32 GPIO 16 (RX2)
       └────────────→ ESP32 3.3V (NOT 5V!)
```

**Critical Connections**:
- GPS TX → ESP32 RX (GPIO 16)
- GPS RX → ESP32 TX (GPIO 17)
- **Power**: 3.3V only! 5V will damage GPS!

**Serial Communication**:
- Baud rate: 9600
- Protocol: NMEA sentences
- Update rate: 1 Hz

---

## 🔊 Audio Output Wiring

### 7. APR Voice Module

```
┌─────────────────────────────────────────┐
│        APR-9301 Module                  │
│  ┌──────────────────────────────┐       │
│  │  Volume  ┌──┐                │       │
│  │   Pot    └──┘  [Audio IC]    │       │
│  │                               │       │
│  │  VCC TRIG1 TRIG2 TRIG3       │       │
│  │  GND  SP+  SP-               │       │
│  └───┬───┬────┬─────┬───┬───┬───┘       │
│      │   │    │     │   │   │           │
└──────┼───┼────┼─────┼───┼───┼───────────┘
       │   │    │     │   │   │
       │   │    │     │   │   └──→ Speaker (-)
       │   │    │     │   └──────→ Speaker (+)
       │   │    │     └──────────→ ESP32 GPIO 27
       │   │    └────────────────→ ESP32 GPIO 26
       │   └─────────────────────→ ESP32 GPIO 25
       └─────────────────────────→ ESP32 GND
      
5V  →  APR VCC
```

**Trigger Logic**:
```cpp
// To play audio on channel 1:
digitalWrite(APR_TRIGGER_1, HIGH);
delay(100);  // 100ms pulse
digitalWrite(APR_TRIGGER_1, LOW);
```

**Audio Channels**:
- TRIG1 (GPIO 25): "Obstacle ahead"
- TRIG2 (GPIO 26): "Pothole detected"
- TRIG3 (GPIO 27): "Turn/Behind/Other"

### 8. Speaker Connection

```
APR Module                  Speaker (8Ω, 0.5W)
──────────                  ──────────────────
SP+ (AUDIO OUT+)  ──────→   Speaker + (Red)
SP- (GND)         ──────→   Speaker - (Black)
```

**Speaker Specifications**:
- Impedance: 8Ω (standard)
- Power: 0.5W - 1W
- Size: 28mm - 40mm diameter
- Type: Cone or mylar

### 9. Buzzer (Backup Audio)

```
                1kΩ Resistor
ESP32 GPIO 33 ────[1kΩ]────┐
                            │
                        ┌───▼───┐
                        │Buzzer │
                        │  (+)  │
                        │       │
                        │  (-)  │
                        └───┬───┘
                            │
ESP32 GND  ─────────────────┘
```

**For Passive Buzzer** (tone generation):
```cpp
tone(BUZZER_PIN, frequency, duration);
```

**For Active Buzzer** (on/off only):
```cpp
digitalWrite(BUZZER_PIN, HIGH);  // Beep
delay(100);
digitalWrite(BUZZER_PIN, LOW);   // Silent
```

---

## 🔧 Control Outputs

### 10. Vibration Motor

```
ESP32 GPIO 32 ───[1kΩ]───┐
                          │
                      ┌───▼───┐
                      │  B    │  2N2222
          ┌───────────┤  C    │  Transistor
          │           │       │
          │           │  E    │
          │           └───┬───┘
          │               │
          │              GND
          │
       ┌──▼──┐
   5V ─┤  +  │  Vibration Motor
       │     │  (3V-5V DC)
   GND─┤  -  │
       └─────┘
```

**Components**:
- Transistor: 2N2222 NPN
- Base resistor: 1kΩ
- Motor: 3V-5V vibration motor

**Alternative** (if motor draws <40mA):
```
ESP32 GPIO 32 ───→ Motor (+)
GND           ───→ Motor (-)
```

---

## 📥 Input Connections

### 11. SOS Button

```
Method 1: With Internal Pull-up (Recommended)

                    ESP32 GPIO 35
                    (Input Pull-up)
                          ↑
                          │
                     ┌────┴────┐
                     │ Button  │
                     └────┬────┘
                          │
                         GND
```

**Code**:
```cpp
pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);
// Button pressed = LOW
// Button released = HIGH
```

**Method 2: With External Pull-up**

```
3.3V ───[10kΩ]───┬───→ ESP32 GPIO 35
                 │
            ┌────▼────┐
            │ Button  │
            └────┬────┘
                 │
                GND
```

### 12. Status LED

```
                220Ω Resistor
ESP32 GPIO 2 ────[220Ω]────┬──→ LED Anode (+)
                            │    (Long leg)
                        ┌───▼───┐
                        │  LED  │
                        └───┬───┘
                            │
ESP32 GND  ─────────────────┴──→ LED Cathode (-)
                                 (Short leg)
```

**LED Polarity**:
- Anode (+): Long leg → to GPIO via resistor
- Cathode (-): Short leg → to GND

**Current Limiting**:
- 220Ω resistor for 3.3V logic
- Limits current to ~10mA

---

## 🔌 ESP32 Complete Pinout Diagram

```
                        ┌─────────────────┐
                        │     ESP32       │
                        │    DevKit       │
                        │                 │
            3V3  ───────│ 3V3         GND │───────  GND
            EN   ───────│ EN          D23 │───────  Front TRIG
           D36   ───────│ D36         D22 │───────  Front ECHO
           D39   ───────│ D39         TX0 │
           D34   ───────│ D34 (IR)    RX0 │
           D35   ───────│ D35 (SOS)   D21 │───────  Left TRIG
           D32   ───────│ D32 (VIB)   D19 │───────  Left ECHO
           D33   ───────│ D33 (BUZ)   D18 │───────  Right TRIG
           D25   ───────│ D25 (APR1)  D5  │───────  Right ECHO
           D26   ───────│ D26 (APR2)  D17 │───────  GPS TX
           D27   ───────│ D27 (APR3)  D16 │───────  GPS RX
           D14   ───────│ D14         D4  │───────  Back TRIG
           D12   ───────│ D12         D2  │───────  Status LED
           GND   ───────│ GND         D15 │───────  Back ECHO
           D13   ───────│ D13         GND │───────  GND
           D9    ───────│ D9          VIN │───────  5V Input
           D10   ───────│ D10         3V3 │───────  GPS/IR Power
           D11   ───────│ D11         GND │───────  GND
           VIN   ───────│ VIN         GND │───────  GND
                        └─────────────────┘
```

---

## 📊 Power Requirements

### Current Draw Table

| Component | Voltage | Current | Count | Total |
|-----------|---------|---------|-------|-------|
| ESP32 (WiFi ON) | 5V | 160mA | 1 | 160mA |
| HC-SR04 | 5V | 15mA | 4 | 60mA |
| GPS NEO-6M | 3.3V | 45mA | 1 | 45mA |
| APR Module | 5V | 20mA | 1 | 20mA |
| Vibration Motor | 5V | 80mA | 1 | 80mA |
| Buzzer | 5V | 30mA | 1 | 30mA |
| IR Sensor | 3.3V | 20mA | 1 | 20mA |
| LED | 3.3V | 10mA | 1 | 10mA |
| **TOTAL** | | | | **~425mA** |

**Peak Current** (all active): ~500-600mA  
**Average Current**: ~300-350mA  
**Step-down rating**: 2A minimum (recommended: 3A)

### Battery Life Calculation

```
Battery: 2× 18650 (2600mAh each) = 5200mAh @ 7.4V
Capacity at 5V: 5200mAh × 0.9 (efficiency) = 4680mAh

Battery Life = 4680mAh / 350mA = ~13.4 hours

With WiFi/GPS intermittent: ~15-17 hours
```

---

## 🛠️ Assembly Instructions

### Step 1: Prepare Components

1. **Test each component** individually before integration
2. **Label all wires** with masking tape
3. **Organize by subsystem** (sensors, audio, power, etc.)
4. **Gather tools**: Soldering iron, multimeter, wire strippers

### Step 2: Power System First

1. **Wire batteries in series**:
   ```
   Battery 1 (+) to Battery 2 (-)
   Battery 1 (-) to Step-down GND
   Battery 2 (+) to Step-down VIN
   ```

2. **Adjust step-down output**:
   - Connect multimeter to output
   - Turn potentiometer until exactly 5.0V
   - Test with load (LED + resistor)

3. **Add power switch** (optional):
   ```
   Battery (+) ──→ Switch ──→ Step-down VIN
   ```

### Step 3: ESP32 Power Connection

```
Step-down OUT+ (5V)  ──→  ESP32 VIN
Step-down OUT- (GND) ──→  ESP32 GND
ESP32 3.3V           ──→  GPS VCC, IR VCC
```

**Test**: Power on, ESP32 LED should light up

### Step 4: Sensors (One at a Time)

**Front Ultrasonic**:
1. Connect VCC → 5V, GND → GND
2. Connect TRIG → GPIO 23, ECHO → GPIO 22
3. Upload test code, verify readings
4. Repeat for Left, Right, Back sensors

**IR Sensor**:
1. Connect VCC → 3.3V (IMPORTANT!)
2. Connect OUT → GPIO 34
3. Connect GND → GND
4. Test with serial monitor

**GPS Module**:
1. Connect VCC → 3.3V (IMPORTANT!)
2. Connect TX → GPIO 16, RX → GPIO 17
3. Connect GND → GND
4. Take outdoors for testing

### Step 5: Audio System

**APR Module**:
1. Upload audio messages first (via PC/programmer)
2. Connect VCC → 5V, GND → GND
3. Connect TRIG1/2/3 → GPIO 25/26/27
4. Connect speaker to SP+/SP-
5. Test each channel

**Buzzer**:
1. Add 1kΩ resistor inline
2. Connect to GPIO 33 and GND
3. Test with tone() function

### Step 6: Feedback & Controls

**Vibration Motor**:
1. Build transistor circuit
2. Test motor separately
3. Connect to GPIO 32
4. Verify vibration strength

**SOS Button**:
1. Connect one pin to GPIO 35
2. Other pin to GND
3. Enable internal pull-up in code
4. Test button press detection

**Status LED**:
1. Add 220Ω resistor
2. Connect anode (+) to GPIO 2
3. Connect cathode (-) to GND
4. Test with blink sketch

### Step 7: Cable Management

1. **Group wires by function**:
   - Power: Red/Black
   - Sensors: Yellow/Green
   - Control: Blue/White

2. **Route inside pipe**:
   - Use cable ties every 10cm
   - Leave slack at sensor points
   - Avoid crossing power/signal wires

3. **Seal entry points**:
   - Hot glue around sensor mounts
   - Heat shrink on wire splices
   - Silicone for waterproofing

---

## 🧪 Testing Procedure

### Pre-Power Test
- [ ] Verify all connections with multimeter (continuity)
- [ ] Check for short circuits (5V to GND should be open)
- [ ] Confirm polarity of batteries and all components
- [ ] Double-check GPS and IR use 3.3V (NOT 5V!)

### Power-On Test
- [ ] Connect battery → Step-down shows 5V output
- [ ] ESP32 powers on (blue LED visible)
- [ ] Serial monitor shows startup messages
- [ ] All sensor VCC lines measure correct voltage

### Individual Component Tests
- [ ] Front sensor: Detects hand at 100cm
- [ ] Left sensor: Detects object at 60cm
- [ ] Right sensor: Detects object at 60cm
- [ ] Back sensor: Detects hand at 80cm
- [ ] IR sensor: ADC value changes over edge
- [ ] GPS: Satellite count > 0 (outdoors)
- [ ] APR: Each channel plays audio
- [ ] Buzzer: Produces tone
- [ ] Vibration: Motor vibrates
- [ ] Button: Serial shows press/release
- [ ] LED: Blinks on command

### System Integration Test
- [ ] All sensors scan sequentially
- [ ] Obstacle triggers correct alert
- [ ] Pothole triggers warning
- [ ] SOS activates emergency mode
- [ ] GPS location uploads to GeoLinker
- [ ] Battery voltage monitored
- [ ] No interference between components

---

## ⚠️ Common Issues & Solutions

### Issue 1: ESP32 Won't Boot

**Symptoms**: No serial output, LED doesn't light

**Check**:
- [ ] VIN has 5V (measure with multimeter)
- [ ] GND connected
- [ ] USB cable is data-capable (not charge-only)
- [ ] No short circuit between VIN and GND

**Fix**:
- Press EN button to reset
- Disconnect all peripherals, test bare ESP32
- Try different USB port/cable

### Issue 2: Ultrasonic Returns 0 or 999

**Symptoms**: No distance reading or always max

**Check**:
- [ ] VCC is 5V (measure at sensor)
- [ ] TRIG and ECHO not swapped
- [ ] Sensor facing forward (not blocked)
- [ ] Wire length <30cm

**Fix**:
- Swap with known-good sensor
- Check for loose connections
- Verify pin numbers in code

### Issue 3: GPS No Fix

**Symptoms**: Satellite count = 0, no coordinates

**Check**:
- [ ] VCC is 3.3V (NOT 5V!)
- [ ] TX/RX not swapped (TX→RX16, RX→TX17)
- [ ] Antenna has clear sky view
- [ ] Waited 2-3 minutes for cold start

**Fix**:
- Move to open area (away from buildings)
- Check baud rate (9600 for NEO-6M)
- Verify UART2 initialization

### Issue 4: No Audio from APR

**Symptoms**: Speaker silent, no voice alerts

**Check**:
- [ ] Speaker impedance is 8Ω (measure)
- [ ] Audio files uploaded to APR
- [ ] Volume pot not at minimum
- [ ] Trigger pins connected correctly
- [ ] APR has 5V power

**Fix**:
- Re-upload audio files
- Turn volume pot clockwise
- Test speaker with multimeter (should show ~8Ω)
- Try different trigger pin

### Issue 5: Buzzer Not Working

**Symptoms**: No beep sound

**Check**:
- [ ] Polarity correct (+ to GPIO, - to GND)
- [ ] 1kΩ resistor present (for passive)
- [ ] GPIO 33 toggling (check with LED)

**Fix**:
- Test buzzer with 5V directly
- For passive: Use tone() function
- For active: Use digitalWrite()
- Try different buzzer

### Issue 6: Vibration Motor Weak

**Symptoms**: Can barely feel vibration

**Check**:
- [ ] Transistor wired correctly (B-C-E)
- [ ] Motor touching pipe wall
- [ ] Motor rated voltage (3V-5V)
- [ ] Base resistor value (1kΩ)

**Fix**:
- Test motor with 5V directly
- Use stronger motor
- Check transistor with multimeter
- Add weight to motor shaft

---

## 📝 Wire Color Convention

### Recommended Colors

| Function | Color | Example Usage |
|----------|-------|---------------|
| **5V Power** | Red | Sensor VCC, Motor + |
| **3.3V Power** | Orange | GPS VCC, IR VCC |
| **Ground** | Black | All GND connections |
| **Digital Signal** | Yellow | TRIG pins, GPIO outputs |
| **Analog Signal** | Green | ECHO pins, IR OUT |
| **Serial TX** | Blue | GPS TX, ESP32 TX |
| **Serial RX** | White | GPS RX, ESP32 RX |
| **Audio** | Purple | Speaker wires |

### Wire Gauge

- **Power (VIN, 5V)**: 22 AWG or thicker
- **Signals (GPIO)**: 24-26 AWG
- **Sensors (short runs)**: 24-28 AWG

---

## 🔒 Safety Warnings

### Electrical Safety
- ⚠️ **Never** connect 5V to 3.3V components (GPS, IR)
- ⚠️ **Always** check polarity before powering on
- ⚠️ **Never** short circuit battery terminals
- ⚠️ Use protected 18650 batteries only
- ⚠️ Add 1A fuse on battery output

### Soldering Safety
- Use ventilation or fume extractor
- Wear safety glasses
- Don't touch hot iron tip
- Unplug iron when not in use
- Keep flammable materials away

### Battery Safety
- Don't exceed 1C discharge rate
- Monitor temperature during use
- Use balanced charging
- Replace damaged batteries immediately
- Store in fireproof container

---

## ✅ Final Checkout List

Before sealing everything:

- [ ] All connections verified with continuity test
- [ ] No short circuits present
- [ ] Correct voltage on all power rails (5V, 3.3V)
- [ ] All 4 ultrasonic sensors working
- [ ] IR sensor detects ground drops
- [ ] GPS gets satellite fix (outdoors)
- [ ] All APR channels play audio
- [ ] Buzzer produces tones
- [ ] Vibration motor works
- [ ] SOS button triggers alert
- [ ] Status LED blinks
- [ ] WiFi connects to hotspot
- [ ] GeoLinker receives location
- [ ] Battery lasts >10 hours
- [ ] All wires secured and labeled
- [ ] Waterproofing applied
- [ ] System operates for 30 minutes without errors

---

**Complete wiring time**: 8-12 hours for first build  
**With experience**: 4-6 hours

**Ready to wire! Follow this guide step by step, test as you go, and you'll have a fully functional Smart Blind Stick!** 🛠️
