# Quick Setup Guide - Mobile Hotspot Tracking

## Your Configuration

**Mobile Hotspot Credentials** (Already configured in code):
- **SSID**: `WiFi`
- **Password**: `wordpass`
- **Mode**: Station Mode (connects to hotspot)

---

## Step-by-Step Setup

### 1. Enable Mobile Hotspot on Your Phone

**Android**:
1. Open **Settings**
2. Go to **Network & Internet** → **Hotspot & Tethering**
3. Tap **WiFi Hotspot**
4. Verify settings:
   - Network name: `WiFi`
   - Password: `wordpass`
5. **Turn ON** the hotspot

**iPhone**:
1. Open **Settings**
2. Tap **Personal Hotspot**
3. Verify settings:
   - WiFi name: `WiFi`
   - Password: `wordpass`
4. Toggle **Allow Others to Join** to ON

---

### 2. Upload Firmware to ESP32

```bash
cd ~/Desktop/Antigravity/BlindStick
pio run --target upload
pio device monitor
```

**What you'll see in Serial Monitor**:
```
=== Smart Blind Stick ===
Initializing system...

[WiFi] Connecting to WiFi
.........
[WiFi] Connected!
[WiFi] IP: 192.168.43.100

[GPS] Initializing...
[GPS] Waiting for satellite fix (this may take 1-2 minutes)

[MQTT] Configured
[MQTT] Device ID: A1B2C3D4E5F6
[MQTT] Location Topic: blindstick/A1B2C3D4E5F6/location

=== SHARE THIS WITH FAMILY ===
Device ID: A1B2C3D4E5F6
Location Topic: blindstick/A1B2C3D4E5F6/location
==============================

[MQTT] Connecting to broker... Connected!
[SensorManager] Initialized
[FeedbackController] Initialized

=== System Ready ===
```

**Important**: Copy the **Device ID** and **Location Topic** - you'll need these for family tracking!

---

### 3. Wait for GPS Fix

- Take the stick **outdoors** (clear view of sky)
- Wait **1-2 minutes** for GPS to acquire satellites
- You'll see in serial monitor:
  ```
  [GPS] Location: 28.613900, 77.209000 | Sats: 8 | Alt: 250.5m
  [MQTT] Published location: 28.613900, 77.209000
  ```

---

### 4. Family Tracking Setup

#### Option A: Web Dashboard (Easiest)

1. **Create tracking page** (`family_tracker.html`):

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Blind Stick Tracker</title>
    <script src="https://unpkg.com/mqtt/dist/mqtt.min.js"></script>
    <style>
        body {
            font-family: Arial, sans-serif;
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            background: #f5f5f5;
        }
        .status {
            background: white;
            padding: 20px;
            border-radius: 10px;
            margin-bottom: 20px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }
        .location {
            font-size: 24px;
            font-weight: bold;
            color: #2196F3;
        }
        #map {
            width: 100%;
            height: 400px;
            border-radius: 10px;
            background: #e0e0e0;
        }
        .btn {
            display: inline-block;
            padding: 10px 20px;
            background: #2196F3;
            color: white;
            text-decoration: none;
            border-radius: 5px;
            margin: 10px 5px;
        }
    </style>
</head>
<body>
    <h1>🦯 Smart Blind Stick Tracker</h1>
    
    <div class="status">
        <h2>Connection Status</h2>
        <p id="status">Connecting to MQTT broker...</p>
    </div>
    
    <div class="status">
        <h2>Current Location</h2>
        <div class="location" id="location">Waiting for GPS...</div>
        <p id="details"></p>
        <a href="#" class="btn" id="maps-link" target="_blank">📍 View on Google Maps</a>
    </div>
    
    <div id="map"></div>

    <script>
        // REPLACE THIS WITH YOUR DEVICE ID
        const DEVICE_ID = 'A1B2C3D4E5F6';  // ← Change this!
        const TOPIC = `blindstick/${DEVICE_ID}/location`;
        
        // Connect to MQTT broker
        const client = mqtt.connect('wss://broker.hivemq.com:8884/mqtt');
        
        client.on('connect', function() {
            document.getElementById('status').innerHTML = 
                '✅ Connected to MQTT broker';
            client.subscribe(TOPIC);
            console.log('Subscribed to:', TOPIC);
        });
        
        client.on('message', function(topic, message) {
            const data = JSON.parse(message.toString());
            
            // Update location display
            document.getElementById('location').innerHTML = 
                `${data.lat.toFixed(6)}, ${data.lng.toFixed(6)}`;
            
            document.getElementById('details').innerHTML = 
                `Altitude: ${data.alt.toFixed(1)}m | ` +
                `Speed: ${data.speed.toFixed(1)} km/h | ` +
                `Satellites: ${data.sats}`;
            
            // Update Google Maps link
            const mapsUrl = `https://maps.google.com/?q=${data.lat},${data.lng}`;
            document.getElementById('maps-link').href = mapsUrl;
            
            // Embed map
            document.getElementById('map').innerHTML = 
                `<iframe width="100%" height="100%" frameborder="0" 
                 style="border:0; border-radius:10px;" 
                 src="https://maps.google.com/maps?q=${data.lat},${data.lng}&output=embed">
                 </iframe>`;
            
            console.log('Location updated:', data);
        });
        
        client.on('error', function(err) {
            document.getElementById('status').innerHTML = 
                '❌ Connection error: ' + err.message;
        });
    </script>
</body>
</html>
```

2. **Update Device ID** in the HTML (line 50)
3. **Open the file** in any web browser
4. **See real-time location** updated every 5 seconds!

---

#### Option B: Mobile App (Android/iOS)

**Android - MQTT Dashboard**:
1. Download "MQTT Dashboard" from Play Store
2. Open app → Add new connection
3. Settings:
   - Name: `Blind Stick Tracker`
   - Server: `broker.hivemq.com`
   - Port: `1883`
   - Client ID: (leave auto)
4. Add new tile → **Text**
5. Settings:
   - Topic: `blindstick/YOUR_DEVICE_ID/location`
   - Name: `Location`
6. Save and view real-time updates!

**iOS - MQTTool**:
1. Download "MQTTool" from App Store
2. Add new connection
3. Host: `broker.hivemq.com`, Port: `1883`
4. Subscribe to: `blindstick/YOUR_DEVICE_ID/location`
5. View incoming messages with GPS coordinates

---

## Testing the System

### 1. Verify WiFi Connection

**Serial Monitor should show**:
```
[WiFi] Connecting to WiFi
..........
[WiFi] Connected!
[WiFi] IP: 192.168.43.100
```

If it shows "Failed to connect":
- Check hotspot is ON
- Verify SSID is exactly "WiFi" (case-sensitive)
- Verify password is "wordpass"
- Restart ESP32

---

### 2. Verify GPS Fix

**Take stick outdoors** (balcony/terrace/open area)

**Serial Monitor should show**:
```
[GPS] Location: 28.613900, 77.209000 | Sats: 8 | Alt: 250.5m
```

If it shows "Waiting for satellite fix":
- Ensure clear view of sky (no roof/trees)
- Wait 2-3 minutes for cold start
- Check GPS antenna connection

---

### 3. Verify MQTT Publishing

**Serial Monitor should show**:
```
[MQTT] Published location: 28.613900, 77.209000
```

If it shows "Publish failed":
- Check internet connection on hotspot
- Verify MQTT broker is reachable
- Try different broker (test.mosquitto.org)

---

## Data Usage Monitoring

**On Android**:
1. Settings → Network & Internet → Data Usage
2. Select your SIM card
3. View app data usage
4. Look for "Hotspot & Tethering"

**Expected Usage**:
- Per hour: ~70 KB
- Per day (8 hours): ~560 KB
- Per month: ~17 MB

**Cost**: Less than ₹1 per month!

---

## Troubleshooting

### Stick Not Connecting to Hotspot

**Check**:
- [ ] Hotspot is enabled and broadcasting
- [ ] SSID is "WiFi" (exact match, case-sensitive)
- [ ] Password is "wordpass" (no typos)
- [ ] Stick is within 10 meters of phone
- [ ] Hotspot allows new connections (check device limit)

**Fix**:
```cpp
// In config.h, verify:
#define WIFI_SSID "WiFi"
#define WIFI_PASSWORD "wordpass"
#define AP_MODE_ENABLED false
```

---

### Family Can't See Location

**Check**:
- [ ] Stick is connected to hotspot (check serial monitor)
- [ ] GPS has valid fix (4+ satellites)
- [ ] MQTT is connected (check serial monitor)
- [ ] Family used correct Device ID in tracker
- [ ] Family's device has internet connection

**Debug**:
1. Open serial monitor
2. Look for: `[MQTT] Published location`
3. If missing, check GPS and WiFi status
4. If present, verify Device ID matches in tracker

---

### High Battery Drain

**Reduce power consumption**:

```cpp
// In config.h, increase intervals:
#define MQTT_PUBLISH_INTERVAL 10000  // 10 seconds instead of 5
#define SENSOR_SCAN_INTERVAL 200     // 200ms instead of 100ms
```

**Or use power-saving mode**:
```cpp
// Add to main.cpp
WiFi.setSleep(WIFI_PS_MIN_MODEM);  // Enable WiFi sleep
```

---

## Next Steps

1. ✅ **Test indoors** first (with hotspot nearby)
2. ✅ **Test outdoors** for GPS fix
3. ✅ **Share Device ID** with family
4. ✅ **Family opens tracker** and verifies location
5. ✅ **Go for a walk** and test real-time tracking!

---

## Summary

**Your Setup**:
```
Mobile Hotspot: "WiFi" (password: wordpass)
    ↓
Blind Stick connects automatically
    ↓
Gets GPS from satellites
    ↓
Publishes to MQTT cloud (broker.hivemq.com)
    ↓
Family views on web/app from anywhere
```

**Cost**: ~₹0.50/month (data usage)  
**Range**: Unlimited (works anywhere with mobile coverage)  
**Update Frequency**: Every 5 seconds  
**Family Viewers**: Unlimited

---

**Ready to test!** 🚀

Upload the firmware, enable your hotspot, and start tracking!
