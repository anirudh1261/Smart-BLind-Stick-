# Wiring Checklist - Ready to Connect! ✅

## 🎉 Great Progress!

You've mounted:
- ✅ Top sensor (Front/Forward detection)
- ✅ Down sensor (Back/Rear detection) 
- ✅ Left sensor (Left side detection)
- ✅ Right sensor (Right side detection)
- ✅ IR sensor (Ground/Pothole detection)

**Now let's wire everything to the ESP32!**

---

## 🔌 Step-by-Step Wiring Instructions

### **IMPORTANT: Power OFF while wiring!**
- ❌ Do NOT connect battery/USB yet
- ✅ Wire everything first, test with multimeter, THEN power on

---

## 1️⃣ **Top Sensor (FRONT)** - Forward Detection

```
HC-SR04 Top Sensor    →    ESP32
─────────────────────────────────
VCC (Red)             →    5V
TRIG (Yellow)         →    GPIO 23
ECHO (Green)          →    GPIO 22
GND (Black)           →    GND
```

**Notes**: 
- This is your primary forward-looking sensor
- Should be angled 15° downward
- Detects obstacles ahead (head level)

---

## 2️⃣ **Down Sensor (BACK)** - Rear Detection

```
HC-SR04 Down Sensor   →    ESP32
─────────────────────────────────
VCC (Red)             →    5V
TRIG (Yellow)         →    GPIO 4
ECHO (Green)          →    GPIO 15
GND (Black)           →    GND
```

**Notes**:
- Detects obstacles behind you
- Useful when backing up
- Should face straight back (180° from front)

---

## 3️⃣ **Left Sensor** - Left Side Detection

```
HC-SR04 Left Sensor   →    ESP32
─────────────────────────────────
VCC (Red)             →    5V
TRIG (Yellow)         →    GPIO 21
ECHO (Green)          →    GPIO 19
GND (Black)           →    GND
```

**Notes**:
- Angled 45° to the left
- Detects side obstacles
- Helps with narrow passages

---

## 4️⃣ **Right Sensor** - Right Side Detection

```
HC-SR04 Right Sensor  →    ESP32
─────────────────────────────────
VCC (Red)             →    5V
TRIG (Yellow)         →    GPIO 18
ECHO (Green)          →    GPIO 5
GND (Black)           →    GND
```

**Notes**:
- Angled 45° to the right
- Mirrors left sensor
- Complete side coverage

---

## 5️⃣ **IR Sensor** - Ground Detection

```
IR Sensor Module      →    ESP32
─────────────────────────────────
VCC                   →    3.3V (IMPORTANT: NOT 5V!)
OUT (Signal)          →    GPIO 34
GND                   →    GND
```

**⚠️ CRITICAL**:
- IR sensor MUST use 3.3V (NOT 5V!)
- 5V will damage the sensor
- Should be 5cm from ground, pointing down

---

## 6️⃣ **Power Connections**

### If using 4× 18650 batteries (2S2P):

```
Battery Pack (7.4V)   →    Step-Down Converter
─────────────────────────────────────────────
Pack (+)              →    LM2596 IN+
Pack (-)              →    LM2596 IN-

Step-Down Converter   →    ESP32
─────────────────────────────────
OUT+ (5V)             →    VIN
OUT- (GND)            →    GND
```

**Setup Step-Down**:
1. Before connecting ESP32, adjust pot to exactly 5.0V
2. Measure with multimeter
3. Then connect to ESP32

### If using USB (for testing):

```
USB Cable             →    ESP32
─────────────────────────────────
USB-A end             →    Computer/Power adapter
Micro-USB end         →    ESP32 Micro-USB port
```

---

## ✅ Pre-Power Checklist

**Before turning on power, verify**:

- [ ] All sensor VCC → 5V (except IR sensor)
- [ ] IR sensor VCC → 3.3V (NOT 5V!)
- [ ] All GND connected to ESP32 GND
- [ ] No loose wires
- [ ] No short circuits (VCC to GND)
- [ ] Correct GPIO pins (double-check against list)
- [ ] Step-down set to 5.0V (if using batteries)

**Use multimeter to verify**:
- [ ] Continuity between sensor VCC and ESP32 5V
- [ ] Continuity between sensor GND and ESP32 GND
- [ ] No continuity between VCC and GND (no short!)

---

## 🔋 Power On Sequence

**Step 1: USB Power (Safest for First Test)**

1. Connect USB cable to computer
2. ESP32 should power on (blue LED lights up)
3. Watch for smoke/heat (if any, disconnect immediately!)
4. If okay, proceed to Step 2

**Step 2: Check Serial Monitor**

```bash
cd ~/Desktop/Antigravity/BlindStick
pio device monitor
```

**You should see**:
```
Smart Blind Stick Starting...
[SensorManager] Initialized (4 ultrasonic + 1 IR)
```

If you see errors, power off and check wiring.

---

## 🧪 Quick Sensor Test

**Upload test firmware**:

```bash
cd ~/Desktop/Antigravity/BlindStick
pio run --target upload
pio device monitor
```

### Test Each Sensor:

**1. Top/Front Sensor Test**:
- Wave hand 100cm in front of top sensor
- Serial should show: `Front: XX cm`
- Distance should change as you move hand

**2. Down/Back Sensor Test**:
- Wave hand behind stick
- Serial should show: `Back: XX cm`

**3. Left Sensor Test**:
- Wave hand to the left
- Serial should show: `Left: XX cm`

**4. Right Sensor Test**:
- Wave hand to the right
- Serial should show: `Right: XX cm`

**5. IR Sensor Test**:
- Hold stick over table edge
- Serial should show: `Pothole: YES` or high ADC value (>500)
- Hold over solid ground
- Serial should show: `Pothole: NO` or low ADC value (<400)

---

## 🐛 Troubleshooting

### Sensor shows 0cm or 999cm

**Problem**: No reading or max reading

**Check**:
- [ ] VCC connected to 5V (measure with multimeter)
- [ ] GND connected
- [ ] TRIG and ECHO pins correct (not swapped)
- [ ] Sensor facing correct direction (not blocked)
- [ ] Wire connections solid (no loose wires)

**Fix**: Re-check wiring, swap sensor with known good one

### IR Sensor always shows pothole

**Problem**: ADC value always >500

**Check**:
- [ ] Voltage is 3.3V (NOT 5V!)
- [ ] Sensor 5-10cm from ground
- [ ] Not pointing at dark/black surface
- [ ] Sensor clean (no dust on IR LED/receiver)

**Fix**: Adjust threshold in code or reposition sensor

### ESP32 won't power on

**Problem**: No blue LED, no serial output

**Check**:
- [ ] USB cable is data cable (not charge-only)
- [ ] USB port has power
- [ ] No short circuit (VCC to GND)
- [ ] ESP32 not damaged

**Fix**: Try different USB cable/port, check for shorts

### One sensor works, others don't

**Problem**: Only some sensors respond

**Check**:
- [ ] Each sensor has VCC and GND
- [ ] Correct GPIO pins for each sensor
- [ ] Sensors not sharing same GPIO

**Fix**: Verify pin assignments match config.h

---

## 📐 Sensor Orientation Guide

### Correct Setup:

```
        [TOP/FRONT]
            ↓
        (15° down)
            
[LEFT]  ←  STICK  →  [RIGHT]
(45° left)    ↑    (45° right)
            
          [BACK]
            ↑
      (straight back)
            
         [IR] ↓
      (straight down)
```

### Check Angles:

- **Top/Front**: Should point forward and slightly down (15°)
- **Left**: Should point 45° to the left
- **Right**: Should point 45° to the right  
- **Back**: Should point straight backward (180° from front)
- **IR**: Should point straight down at ground (90°)

---

## 🎯 Next Steps After Wiring

Once all sensors test successfully:

### 1. Add Remaining Components

- [ ] GPS module (GPIO 16/17) - Use 3.3V!
- [ ] APR voice module (GPIO 25/26/27)
- [ ] Speaker (connect to APR)
- [ ] Buzzer (GPIO 33)
- [ ] Vibration motor (GPIO 32)
- [ ] SOS button (GPIO 35)
- [ ] LED (GPIO 2)

### 2. Upload Final Firmware

```bash
cd ~/Desktop/Antigravity/BlindStick
pio run --target upload
```

### 3. Test Complete System

- [ ] All sensors working
- [ ] Audio alerts playing
- [ ] Vibration motor working
- [ ] GPS getting fix (outdoors)
- [ ] SOS button triggers alert

### 4. Field Test

- Walk around indoors
- Test obstacle detection
- Verify audio/vibration feedback
- Check battery life

---

## 📞 Quick Reference

### Pin Summary

| Component | GPIO | Voltage |
|-----------|------|---------|
| Top/Front TRIG | 23 | 5V |
| Top/Front ECHO | 22 | 5V |
| Left TRIG | 21 | 5V |
| Left ECHO | 19 | 5V |
| Right TRIG | 18 | 5V |
| Right ECHO | 5 | 5V |
| Down/Back TRIG | 4 | 5V |
| Down/Back ECHO | 15 | 5V |
| IR Sensor | 34 | **3.3V** |

### Test Commands

**Upload**:
```bash
pio run --target upload
```

**Monitor**:
```bash
pio device monitor
```

**Exit monitor**: `Ctrl+C`

---

## ✨ You're Almost There!

**Current Status**: ✅ Sensors mounted  
**Next Step**: 🔌 Wire to ESP32  
**Then**: 🧪 Test  
**Finally**: 🚀 Complete system!

Wire carefully, test each connection, and you'll have a working system very soon! 💪

**Questions?** Just ask! I'm here to help troubleshoot any issues.
