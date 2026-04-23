# 🧪 Sensor Test Instructions

## Quick Start

1. **Upload** `SENSOR_TEST.ino` to your ESP32
2. **Open Serial Monitor** (115200 baud)
3. **Watch the output** - sensors update every second

---

## 📊 What You Should See

### ✅ Good Output Example:
```
╔══════════════════════════════════════════╗
║         SENSOR READING CYCLE             ║
╚══════════════════════════════════════════╝
[FRONT] ✓ 125.3 cm  🟢 SAFE
[LEFT ] ✓ 89.7 cm   🟡 WARNING
[RIGHT] ✓ 215.4 cm  ⚪ CLEAR
[BACK ] ✓ 45.2 cm   🔴 CRITICAL!
[IR GROUND] ✓ Ground Detected (Normal)
```

### ❌ Problem Indicators:

| Message | Problem | Fix |
|---------|---------|-----|
| `❌ NO SIGNAL - Check wiring!` | Sensor not responding | Check VCC, GND, TRIG, ECHO connections |
| `⚠️ TOO CLOSE - Sensor blocked?` | Reading < 2cm | Remove obstruction or check sensor |
| `⚠️ GAP/POTHOLE DETECTED!` | IR sees no ground | Normal if sensor is in air, check if over surface |

---

## 🔍 Testing Each Sensor

### Test Ultrasonic Sensors:

1. **Wave your hand** in front of each sensor
2. **Watch distance decrease** as hand gets closer
3. **Alert levels**:
   - 🔴 **< 50cm**: CRITICAL
   - 🟡 **50-100cm**: WARNING
   - 🟢 **100-200cm**: SAFE
   - ⚪ **> 200cm**: CLEAR

### Test IR Ground Sensor:

1. **Hold over flat surface** → Should show "Ground Detected"
2. **Hold over gap/edge** → Should show "GAP/POTHOLE DETECTED!"
3. **Adjust potentiometer** if needed (5-15cm range ideal)

---

## 🐛 Troubleshooting

### Front Sensor Not Working:
- Check ESP32 GPIO 5 (TRIG) and GPIO 18 (ECHO)
- Verify 5V and GND connections
- Swap with a known working sensor to test

### Left Sensor Not Working:
- Check GPIO 19 (TRIG) and GPIO 21 (ECHO)

### Right Sensor Not Working:
- Check GPIO 22 (TRIG) and GPIO 23 (ECHO)

### Back Sensor Not Working:
- Check GPIO 12 (TRIG) and GPIO 13 (ECHO)

### IR Sensor Issues:
- Check GPIO 34 connection
- Verify 5V power
- Adjust sensitivity potentiometer

### All Sensors Showing NO SIGNAL:
- Check 5V power supply
- Verify common GND
- Check ESP32 is powered on

---

## ✅ When All Tests Pass:

Once all sensors show valid readings:

1. **Upload main BlindStick firmware**
2. **Sensors will automatically work** in the full system
3. **GPS tracking** and **APR audio** will activate

---

## 📝 Pin Reference

| Sensor | TRIG GPIO | ECHO GPIO | VCC | GND |
|--------|-----------|-----------|-----|-----|
| Front  | 5         | 18        | 5V  | GND |
| Left   | 19        | 21        | 5V  | GND |
| Right  | 22        | 23        | 5V  | GND |
| Back   | 12        | 13        | 5V  | GND |
| IR Sensor | -      | 34 (OUT)  | 5V  | GND |

**Ready to test your sensors!** 🚀
