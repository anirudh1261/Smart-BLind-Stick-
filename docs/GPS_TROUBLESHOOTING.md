# GPS Troubleshooting Guide

## ❌ **Problem**: "Invalid NMEA format" Error

This means the ESP32 is **NOT receiving valid GPS data**. Common causes:

---

## 🔍 **Step 1: Check GPS Wiring**

### **Correct Connections**:

```
NEO-6M GPS Module    →    ESP32
─────────────────────────────────
VCC (Red)            →    3.3V  ⚠️ NOT 5V!
TX (Yellow/White)    →    GPIO 16 (RX)
RX (Green/Blue)      →    GPIO 17 (TX)
GND (Black)          →    GND
```

### ⚠️ **Common Mistakes**:

1. **TX/RX Swapped**: GPS TX must go to ESP32 RX (GPIO 16)
2. **5V Instead of 3.3V**: NEO-6M uses 3.3V, NOT 5V!
3. **Loose Connections**: Check all wire connections
4. **Wrong Pins**: Verify GPIO 16 and 17 are correct

---

## 🧪 **Step 2: Upload GPS Test Code**

Upload `GPS_TEST.ino` to check raw GPS output:

```bash
# Open Arduino IDE or upload via PlatformIO
# You should see NMEA sentences like:
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
```

### **What You Should See**:

✅ **Good**: NMEA sentences starting with `$GP...`  
❌ **Bad**: No output or garbage characters  
❌ **Bad**: Random characters: `��␀�x�x��`

---

## 📡 **Step 3: Get GPS Fix (GO OUTDOORS!)**

### **GPS WILL NOT WORK INDOORS!**

GPS needs clear view of sky to receive satellite signals.

**To get GPS fix**:

1. ✅ **Go outside** (balcony, terrace, open ground)
2. ✅ **Clear sky view** (no buildings/trees blocking)  
3. ✅ **Wait 2-5 minutes** for cold start
4. ✅ **Check LED** on GPS module (should blink when locked)

### **GPS Fix Indicators**:

- **No satellites**: Slow blinking LED, no valid coordinates
- **Acquiring fix**: LED blinking faster
- **GPS locked**: LED solid or fast blink, coordinates in NMEA data

---

## 🔧 **Step 4: Common Issues**

### Issue 1: No Data at All

**Symptoms**: Serial shows `[Waiting for GPS data...]` but nothing else

**Check**:
- [ ] GPS module powered (LED on GPS should be on)
- [ ] TX wire from GPS connected to GPIO 16
- [ ] Correct baud rate (9600 for NEO-6M)
- [ ] GPS module not damaged

**Fix**: Try powering GPS from ESP32 5V pin instead of 3.3V (some modules tolerate it)

### Issue 2: Garbage Characters

**Symptoms**: Serial shows: `��␀�x�x��`

**Cause**: Wrong baud rate

**Fix**: Try different baud rates:
```cpp
gpsSerial.begin(4800, SERIAL_8N1, GPS_RX, GPS_TX);  // Try 4800
gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);  // Standard
gpsSerial.begin(115200, SERIAL_8N1, GPS_RX, GPS_TX); // Try 115200
```

### Issue 3: Valid NMEA but No Coordinates

**Symptoms**: See `$GPGGA` sentences but coordinates are all zeros

**Cause**: GPS hasn't acquired satellite lock

**Fix**: 
- Go outside
- Wait longer (up to 10 minutes for very first start)
- Check antenna is facing upward
- Ensure ceramic antenna isn't covered

---

## ✅ **Step 5: Verify GPS is Working**

Once GPS_TEST.ino shows valid NMEA sentences:

```
$GPGGA,120312.00,1234.5678,N,07812.3456,E,1,08,0.9,545.4,M,46.9,M,,*47
```

You should see:
- **Time**: `120312.00` (12:03:12 UTC)
- **Latitude**: `1234.5678,N` (12° 34.5678' North)
- **Longitude**: `07812.3456,E` (78° 12.3456' East)
- **Fix Quality**: `1` (GPS fix)
- **Satellites**: `08` (8 satellites in view)

**Then** you can go back to BlindStick code or GeoLinker test!

---

## 🎯 **Quick Checklist**

Before testing GeoLinker code:

- [ ] GPS wired to 3.3V (NOT 5V)
- [ ] GPS TX → ESP32 GPIO 16
- [ ] GPS RX → ESP32 GPIO 17
- [ ] All GND connected
- [ ] GPS_TEST.ino shows valid NMEA sentences
- [ ] GPS has satellite lock (tested outdoors)
- [ ] WiFi credentials correct in code

---

## 📝 **Your Current Settings**

Based on your code:

- **WiFi SSID**: "WiFI" (note the capital 'I')
- **WiFi Password**: "wordpass"
- **API Key**: `9gPyNAUrs4U1`
- **Device ID**: "ESP-32_Tracker"
- **GPS Baud**: 9600
- **GPS Pins**: RX=16, TX=17

**Double-check WiFi SSID** - is it "WiFi" or "WiFI"? Case matters!

---

**Start with GPS_TEST.ino to verify GPS module is working before trying GeoLinker!**
