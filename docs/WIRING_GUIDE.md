# Smart Blind Stick - Complete Wiring Guide

## Components List

### Main Components
- [x] ESP32 DevKit (30 pins)
- [x] 3x HC-SR04 Ultrasonic Sensors
- [x] 1x IR Obstacle Avoidance Sensor
- [x] 1x NEO-6M GPS Module
- [x] 1x APR Voice Module (8-channel)
- [x] 1x Vibration Motor (3V-5V)
- [x] 1x Push Button (SOS)
- [x] 1x Speaker (8Ω, 0.5W)
- [x] 1x LED (Status indicator)
- [x] 2x 18650 Li-ion Batteries (3.7V)

### Additional Components
- [ ] Battery holder (2x 18650)
- [ ] Step-down converter (7.4V → 5V)
- [ ] Breadboard or PCB
- [ ] Jumper wires (Male-Male, Male-Female)
- [ ] Resistors: 220Ω (for LED), 10kΩ (for button pull-up)
- [ ] Transistor/MOSFET (for vibration motor - 2N2222 or similar)
- [ ] Heat shrink tubing
- [ ] Zip ties
- [ ] PVC pipe (25mm diameter, 90cm length)

---

## Power Distribution Diagram

```
┌─────────────────────────────────────────────────────────┐
│                   POWER SYSTEM                          │
│                                                         │
│  Battery Pack (2x 18650)                               │
│  ┌──────────────┐                                      │
│  │   7.4V       │                                      │
│  │   5200mAh    │                                      │
│  └──────┬───────┘                                      │
│         │                                               │
│         │                                               │
│  ┌──────▼────────────┐                                 │
│  │  Step-Down        │                                 │
│  │  Converter        │                                 │
│  │  (LM2596/XL4015)  │                                 │
│  │                   │                                 │
│  │  Input: 7.4V      │                                 │
│  │  Output: 5V       │                                 │
│  └──────┬────────────┘                                 │
│         │                                               │
│         ├─────────────────┐                            │
│         │                 │                            │
│    ┌────▼────┐      ┌────▼────────────┐              │
│    │  ESP32  │      │  5V Peripherals │              │
│    │   VIN   │      │                 │              │
│    │         │      │  - Ultrasonic   │              │
│    │ (Has    │      │  - GPS Module   │              │
│    │ internal│      │  - APR Module   │              │
│    │ 3.3V    │      │  - Vibration    │              │
│    │ regulator)     │                 │              │
│    └────┬────┘      └─────────────────┘              │
│         │                                              │
│    ┌────▼────┐                                        │
│    │  3.3V   │                                        │
│    │Peripherals                                       │
│    │         │                                        │
│    │ - IR    │                                        │
│    │ Sensor  │                                        │
│    └─────────┘                                        │
└─────────────────────────────────────────────────────────┘
```

---

## ESP32 Pinout Reference

```
                    ┌─────────────┐
                    │   ESP32     │
                    │   DevKit    │
                    │             │
         3V3 ──────►│ 3V3     GND │◄────── GND
         EN  ──────►│ EN      D23 │◄────── Front TRIG
         D36 ──────►│ D36     D22 │◄────── Front ECHO
         D39 ──────►│ D39     TX0 │
         D34 ──────►│ D34     RX0 │
         D35 ──────►│ D35     D21 │◄────── Left TRIG
         D32 ──────►│ D32     D19 │◄────── Left ECHO
         D33 ──────►│ D33     D18 │◄────── Right TRIG
         D25 ──────►│ D25     D5  │◄────── Right ECHO
         D26 ──────►│ D26     D17 │◄────── GPS TX
         D27 ──────►│ D27     D16 │◄────── GPS RX
         D14 ──────►│ D14     D4  │
         D12 ──────►│ D12     D2  │◄────── Status LED
         GND ──────►│ GND     D15 │
         D13 ──────►│ D13     GND │◄────── GND
         D9  ──────►│ D9      VIN │◄────── 5V Input
         D10 ──────►│ D10     3V3 │
         D11 ──────►│ D11     GND │◄────── GND
         VIN ──────►│ VIN     GND │◄────── GND
                    └─────────────┘
```

---

## Detailed Wiring Connections

### 1. Front Ultrasonic Sensor (HC-SR04)

```
HC-SR04 Front          ESP32
─────────────          ─────
VCC (Red)      ───►    5V
TRIG (Yellow)  ───►    GPIO23
ECHO (Green)   ───►    GPIO22
GND (Black)    ───►    GND
```

**Notes**:
- Use 5V power (not 3.3V)
- ECHO pin is 5V tolerant on ESP32
- Keep wires short (<20cm) to reduce noise

---

### 2. Left Ultrasonic Sensor (HC-SR04)

```
HC-SR04 Left           ESP32
────────────           ─────
VCC (Red)      ───►    5V
TRIG (Yellow)  ───►    GPIO21
ECHO (Green)   ───►    GPIO19
GND (Black)    ───►    GND
```

---

### 3. Right Ultrasonic Sensor (HC-SR04)

```
HC-SR04 Right          ESP32
─────────────          ─────
VCC (Red)      ───►    5V
TRIG (Yellow)  ───►    GPIO18
ECHO (Green)   ───►    GPIO5
GND (Black)    ───►    GND
```

---

### 4. IR Sensor (Ground Detection)

```
IR Sensor              ESP32
─────────             ─────
VCC (Red)      ───►    3.3V (or 5V)
OUT (Yellow)   ───►    GPIO34 (ADC1_CH6)
GND (Black)    ───►    GND
```

**Notes**:
- GPIO34 is ADC-capable (analog input)
- Some IR sensors have digital output (HIGH/LOW)
- Adjust `POTHOLE_THRESHOLD` in code based on sensor

---

### 5. GPS Module (NEO-6M)

```
NEO-6M GPS             ESP32
──────────             ─────
VCC (Red)      ───►    3.3V (NOT 5V!)
TX (Yellow)    ───►    GPIO16 (RX2)
RX (Green)     ───►    GPIO17 (TX2)
GND (Black)    ───►    GND
```

**Important**:
- NEO-6M operates at 3.3V (5V will damage it!)
- TX from GPS → RX on ESP32
- RX from GPS → TX on ESP32
- Antenna should face upward for best reception

---

### 6. APR Voice Module (8-Channel)

```
APR Module             ESP32
──────────             ─────
VCC        ───►        5V
GND        ───►        GND
TRIG1      ───►        GPIO25
TRIG2      ───►        GPIO26
TRIG3      ───►        GPIO27
TRIG4      ───►        GPIO14 (optional)
TRIG5      ───►        GPIO12 (optional)
TRIG6      ───►        GPIO13 (optional)
TRIG7      ───►        GPIO15 (optional)
TRIG8      ───►        GPIO4  (optional)
AUDIO OUT  ───►        Speaker (+)
GND        ───►        Speaker (-)
```

**Speaker Connection**:
```
APR Audio Out (+) ───► Speaker (+)
APR GND          ───► Speaker (-)
```

**Notes**:
- Use 8Ω, 0.5W speaker
- Volume potentiometer on APR module
- Trigger pins: Pull HIGH for 100ms to play

---

### 7. Vibration Motor

```
                    ┌──────────┐
ESP32 GPIO32 ───►   │ 2N2222   │
                    │Transistor│
                    │          │
                    │ B  C  E  │
                    └──┬──┬──┬─┘
                       │  │  │
                       │  │  └──► GND
                       │  │
                       │  └──────► Motor (+)
                       │
                       └─────────► 1kΩ Resistor to GPIO32

Motor (-)  ───►  GND
5V         ───►  Motor (+) via transistor
```

**Circuit Diagram**:
```
         1kΩ
GPIO32 ──────┐
             │
         ┌───▼───┐
         │  B    │
         │       │  2N2222
    ┌────┤  C    │
    │    │       │
    │    │  E    │
    │    └───┬───┘
    │        │
    │       GND
    │
    │    Vibration Motor
    │    ┌─────────┐
    └────┤ +       │
         │         │
    GND──┤ -       │
         └─────────┘
```

**Alternative (Simpler)**:
If motor draws <40mA, connect directly:
```
GPIO32 ───► Motor (+)
GND    ───► Motor (-)
```

---

### 8. SOS Button

```
                    ┌──────────┐
                    │  Button  │
                    └──┬────┬──┘
                       │    │
GPIO35 ────────────────┘    │
                            │
GND ────────────────────────┘
```

**With Internal Pull-up** (configured in code):
```cpp
pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);
```

**External Pull-up** (optional, if needed):
```
3.3V ──────┬─── 10kΩ Resistor ───┬─── GPIO35
           │                      │
           │                  ┌───▼───┐
           │                  │Button │
           │                  └───┬───┘
           │                      │
GND ───────┴──────────────────────┘
```

---

### 9. Status LED

```
                    220Ω
GPIO2 ──────────────┐
                    │
                ┌───▼───┐
                │  LED  │ (Anode +)
                │   │   │
                └───┬───┘
                    │ (Cathode -)
                   GND
```

**LED Polarity**:
- Long leg (Anode) → Resistor → GPIO2
- Short leg (Cathode) → GND

---

## Complete Wiring Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         ESP32 DevKit                            │
│                                                                 │
│  GPIO23 ──► Front Ultrasonic TRIG                              │
│  GPIO22 ──► Front Ultrasonic ECHO                              │
│  GPIO21 ──► Left Ultrasonic TRIG                               │
│  GPIO19 ──► Left Ultrasonic ECHO                               │
│  GPIO18 ──► Right Ultrasonic TRIG                              │
│  GPIO5  ──► Right Ultrasonic ECHO                              │
│                                                                 │
│  GPIO34 ──► IR Sensor OUT                                      │
│                                                                 │
│  GPIO16 ──► GPS RX                                             │
│  GPIO17 ──► GPS TX                                             │
│                                                                 │
│  GPIO25 ──► APR TRIG1 ("Obstacle ahead")                       │
│  GPIO26 ──► APR TRIG2 ("Pothole detected")                     │
│  GPIO27 ──► APR TRIG3 ("Turn left/right")                      │
│                                                                 │
│  GPIO32 ──► Vibration Motor (via transistor)                   │
│  GPIO35 ──► SOS Button (with internal pull-up)                 │
│  GPIO2  ──► Status LED (via 220Ω resistor)                     │
│                                                                 │
│  VIN    ──► 5V from step-down converter                        │
│  GND    ──► Common ground                                      │
│  3.3V   ──► GPS VCC, IR Sensor VCC                            │
└─────────────────────────────────────────────────────────────────┘
```

---

## Power Connections Summary

### 5V Rail (from step-down converter)
```
5V ─┬─► ESP32 VIN
    ├─► Front Ultrasonic VCC
    ├─► Left Ultrasonic VCC
    ├─► Right Ultrasonic VCC
    ├─► APR Module VCC
    └─► Vibration Motor (via transistor)
```

### 3.3V Rail (from ESP32 regulator)
```
ESP32 3.3V ─┬─► GPS Module VCC
            └─► IR Sensor VCC
```

### Ground (Common)
```
GND ─┬─► ESP32 GND
     ├─► All sensor GNDs
     ├─► Battery (-)
     ├─► Step-down converter GND
     └─► Speaker (-)
```

---

## Step-by-Step Assembly

### Step 1: Prepare Components

1. **Test all components individually**:
   - Upload simple test sketches
   - Verify each sensor works
   - Check GPS gets satellite fix

2. **Organize wires by color**:
   - Red: 5V/3.3V power
   - Black: GND
   - Yellow: Signal/Data
   - Green: Secondary signals

---

### Step 2: Build Power System

1. **Connect batteries**:
   ```
   Battery 1 (+) ──► Battery 2 (-)  (Series connection)
   Battery 1 (-)  ──► Step-down IN (-)
   Battery 2 (+)  ──► Step-down IN (+)
   ```
   **Result**: 7.4V output

2. **Configure step-down converter**:
   - Adjust potentiometer to output exactly 5.0V
   - Use multimeter to verify
   - Connect load (LED) to test stability

3. **Add power switch** (optional):
   ```
   Battery (+) ──► Switch ──► Step-down IN (+)
   ```

---

### Step 3: Mount ESP32

1. **Secure ESP32 to breadboard or PCB**
2. **Connect power**:
   ```
   Step-down OUT (+) ──► ESP32 VIN
   Step-down OUT (-) ──► ESP32 GND
   ```
3. **Test**: ESP32 LED should light up

---

### Step 4: Wire Sensors (One at a Time)

**Front Ultrasonic**:
1. Connect VCC, GND first
2. Connect TRIG to GPIO23
3. Connect ECHO to GPIO22
4. Upload test code
5. Verify distance readings

**Repeat for Left and Right sensors**

**IR Sensor**:
1. Connect to 3.3V (or 5V based on sensor)
2. Connect OUT to GPIO34
3. Test with hand over sensor

**GPS Module**:
1. **Important**: Use 3.3V only!
2. Connect TX → GPIO16, RX → GPIO17
3. Take outdoors for testing
4. Wait 2-3 minutes for fix

---

### Step 5: Wire Feedback Systems

**APR Module**:
1. Connect VCC (5V), GND
2. Connect TRIG1, TRIG2, TRIG3
3. Connect speaker to AUDIO OUT
4. Upload audio files first
5. Test each trigger pin

**Vibration Motor**:
1. Build transistor circuit on breadboard
2. Connect to GPIO32
3. Test with simple blink code

**LED**:
1. Connect with 220Ω resistor
2. Test with blink sketch

**SOS Button**:
1. Connect one terminal to GPIO35
2. Other terminal to GND
3. Enable internal pull-up in code

---

### Step 6: Cable Management

1. **Bundle wires by function**:
   - Power wires together (red/black)
   - Sensor wires together
   - Keep high-current wires separate

2. **Use heat shrink tubing**:
   - Cover all solder joints
   - Prevent short circuits
   - Waterproof connections

3. **Secure with zip ties**:
   - Every 10cm along PVC pipe
   - Leave some slack for movement
   - Don't over-tighten

---

### Step 7: Testing Checklist

- [ ] Power system outputs stable 5V
- [ ] ESP32 boots successfully
- [ ] All 3 ultrasonic sensors read distances
- [ ] IR sensor detects ground
- [ ] GPS gets satellite fix (outdoors)
- [ ] APR plays all audio channels
- [ ] Vibration motor responds to alerts
- [ ] SOS button triggers emergency
- [ ] LED blinks as status indicator
- [ ] WiFi connects to hotspot
- [ ] MQTT publishes location

---

## Troubleshooting

### ESP32 Won't Boot

**Check**:
- [ ] VIN has 5V (measure with multimeter)
- [ ] GND is connected
- [ ] No short circuits
- [ ] USB cable is data-capable (not charge-only)

**Fix**:
- Press EN button to reset
- Disconnect all peripherals, test bare ESP32
- Check for reversed polarity

---

### Ultrasonic Sensor Not Working

**Check**:
- [ ] VCC is 5V (not 3.3V)
- [ ] TRIG and ECHO pins not swapped
- [ ] Sensor facing forward (not blocked)
- [ ] Wires <30cm long

**Fix**:
- Swap sensor with known-good one
- Check for loose connections
- Verify pin numbers in code match wiring

---

### GPS No Fix

**Check**:
- [ ] VCC is 3.3V (NOT 5V!)
- [ ] TX/RX not swapped
- [ ] Antenna facing upward
- [ ] Clear view of sky (outdoors)
- [ ] Waited 2-3 minutes

**Fix**:
- Move to open area (away from buildings)
- Check baud rate (9600 for NEO-6M)
- Verify UART2 pins (16, 17)

---

### No Audio from APR

**Check**:
- [ ] Speaker is 8Ω (not 4Ω or 16Ω)
- [ ] Audio files uploaded to APR
- [ ] Volume potentiometer not at minimum
- [ ] Trigger pins connected correctly

**Fix**:
- Test speaker with multimeter (should show ~8Ω)
- Re-upload audio files
- Try different trigger pin

---

## Safety Warnings

⚠️ **Battery Safety**:
- Never short circuit batteries
- Use protected 18650 cells
- Don't exceed 1A discharge rate
- Monitor temperature during use

⚠️ **Soldering Safety**:
- Use ventilation
- Wear safety glasses
- Don't touch hot iron tip
- Unplug when not in use

⚠️ **Electrical Safety**:
- Double-check polarity before powering on
- Use fuse (1A) on battery output
- Insulate all exposed connections
- Keep away from water (until waterproofed)

---

## Final Assembly Tips

1. **Test before permanent installation**:
   - Breadboard everything first
   - Run for 24 hours to check stability
   - Fix any issues before soldering

2. **Use quality connectors**:
   - JST connectors for easy disconnection
   - Screw terminals for power
   - Heat shrink all solder joints

3. **Label everything**:
   - Mark each wire with tape
   - Note pin numbers
   - Create wiring diagram photo

4. **Waterproofing**:
   - Silicone sealant on connections
   - IP65 enclosure for ESP32
   - Conformal coating on PCB

5. **Strain relief**:
   - Hot glue wire entry points
   - Use cable glands
   - Don't pull on solder joints

---

## Tools Required

### Essential
- [ ] Soldering iron (25-40W)
- [ ] Solder (60/40 or lead-free)
- [ ] Wire strippers
- [ ] Multimeter
- [ ] Screwdriver set
- [ ] Hot glue gun

### Recommended
- [ ] Helping hands / PCB holder
- [ ] Desoldering pump
- [ ] Heat shrink tubing kit
- [ ] Label maker
- [ ] Wire crimping tool
- [ ] Breadboard for testing

---

## Estimated Assembly Time

- **Component testing**: 2-3 hours
- **Power system**: 1 hour
- **Sensor wiring**: 2-3 hours
- **Feedback systems**: 1-2 hours
- **Cable management**: 1 hour
- **Testing & debugging**: 2-4 hours

**Total**: 10-15 hours for first build

---

**Ready to start wiring!** 🔧

Follow this guide step-by-step, test each component as you go, and you'll have a fully functional Smart Blind Stick!
