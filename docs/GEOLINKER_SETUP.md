# GeoLinker GPS Tracking Setup Guide

## What is GeoLinker?

**GeoLinker** is a cloud-based GPS tracking service by Circuit Digest that provides:
- ✅ Real-time GPS location visualization on maps
- ✅ Route tracking and history
- ✅ Offline data storage (when WiFi unavailable)
- ✅ Automatic reconnection
- ✅ Free tier with 10,000 data points
- ✅ Simple HTTP API integration

**Advantages over MQTT**:
- Built-in map visualization (no need to create HTML dashboard)
- Automatic route tracking
- Offline data buffering
- Professional web interface
- Free and easy to use

---

## Step 1: Create Circuit Digest Cloud Account

### 1.1 Register

1. Visit: **https://circuitdigest.cloud**
2. Click **"Login"** (top right corner)
3. Click **"Register Now"**
4. Fill in details:
   - Email address
   - Password
   - Name
5. Click **"Register Now"**
6. Verify your email (check inbox/spam)

### 1.2 Login

1. Go to: **https://circuitdigest.cloud**
2. Click **"Login"**
3. Enter your email and password
4. Click **"Sign In"**

---

## Step 2: Generate API Key

### 2.1 Access API Key Page

1. After login, click **"My Account"** (top right)
2. You'll see the API Key generation page

### 2.2 Generate Key

1. Enter the captcha text shown
2. Click **"Generate API Key"**
3. Your API key will appear in a table
4. Click the **copy icon** to copy the key

**Example API Key**: `a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6`

**Important**:
- Keep this key secret (don't share publicly)
- You get 10,000 GPS data points per key
- Can generate new key when limit reached

---

## Step 3: Configure Your Blind Stick

### 3.1 Update config.h

Open `BlindStick/include/config.h` and update:

```cpp
// ===== GEOLINKER GPS TRACKING =====
#define ENABLE_GEOLINKER true
#define GEOLINKER_API_KEY "a1b2c3d4e5f6g7h8i9j0k1l2m3n4o5p6"  // ← Paste your key here
#define GEOLINKER_DEVICE_ID "BlindStick_User1"                  // ← Unique name
#define GEOLINKER_UPDATE_INTERVAL 5                             // Send every 5 seconds
#define GEOLINKER_OFFLINE_BUFFER 20                             // Store 20 records offline
```

**Device ID**: Choose a unique name (e.g., "BlindStick_Rahul", "BlindStick_001")

### 3.2 WiFi Credentials

Make sure WiFi is configured (already done):

```cpp
#define WIFI_SSID "WiFi"           // Your mobile hotspot
#define WIFI_PASSWORD "wordpass"   // Your hotspot password
```

---

## Step 4: Upload Firmware

### 4.1 Compile and Upload

```bash
cd ~/Desktop/Antigravity/BlindStick
pio run --target upload
```

### 4.2 Monitor Serial Output

```bash
pio device monitor
```

**You should see**:
```
[GeoLinker] Initializing GPS Tracker...
[GeoLinker] Library initialized
[GeoLinker] API key configured
[GeoLinker] Device ID: BlindStick_User1
[GeoLinker] Update interval: 5 seconds
[GeoLinker] Offline storage: ENABLED
[GeoLinker] WiFi SSID: WiFi
[GeoLinker] Connecting to WiFi...
[GeoLinker] ✅ WiFi connected successfully!

=== GeoLinker GPS Tracker Ready ===

📍 View your location at:
   https://circuitdigest.cloud/geolinker
   Login with your Circuit Digest account
=====================================

[GPS] Waiting for satellite fix...
[GPS] Location: 28.613900, 77.209000 | Sats: 8
[GeoLinker] ✓ Data transmitted to cloud!
```

---

## Step 5: View Location on Map

### 5.1 Access GeoLinker Dashboard

1. Open browser: **https://circuitdigest.cloud/geolinker**
2. Login with your Circuit Digest account
3. You'll see a map interface

### 5.2 Select Your Device

1. Look for device dropdown/list
2. Select your device: **"BlindStick_User1"** (or whatever you named it)
3. Map will show your current location

### 5.3 Real-Time Tracking

**What you'll see**:
- 📍 Current GPS position (marker on map)
- 🛣️ Route traveled (line connecting points)
- ⏰ Timestamp of each location
- 📊 Speed, altitude, satellite count
- 📈 Location history

**Updates**:
- New location every 5 seconds (as configured)
- Map auto-refreshes
- Route line extends as you move

---

## Step 6: Family Tracking

### 6.1 Share Access

**Option 1: Share Login**
- Give family your Circuit Digest account credentials
- They login and view your device

**Option 2: Share Link** (if available)
- Some versions allow public sharing links
- Check GeoLinker dashboard for share button

### 6.2 Mobile Access

Family can track from:
- **Desktop**: Any web browser
- **Mobile**: Browser on phone/tablet
- **No app needed**: Works on all devices

---

## Features Explained

### Offline Data Storage

**What it does**:
- When WiFi disconnects, GPS data is stored locally
- Up to 20 records buffered (configurable)
- When WiFi reconnects, buffered data is uploaded first
- Then real-time data resumes

**Example**:
```
14:00 - WiFi connected, sending data
14:05 - WiFi lost, buffering data locally
14:10 - Still offline, buffer has 10 records
14:15 - WiFi reconnected!
14:15 - Uploading 10 buffered records
14:16 - Back to real-time updates
```

### Automatic Reconnection

**What it does**:
- If WiFi drops, automatically tries to reconnect
- Retries every few seconds
- No manual intervention needed

### Route Tracking

**What it does**:
- Connects GPS points with lines
- Shows path traveled
- Displays timestamps
- Can view historical routes

---

## Troubleshooting

### "WiFi connection failed"

**Check**:
- [ ] Mobile hotspot is ON
- [ ] SSID is "WiFi" (exact match)
- [ ] Password is "wordpass" (no typos)
- [ ] ESP32 is within range (<10m)

**Fix**:
- Restart hotspot
- Restart ESP32
- Check serial monitor for error details

---

### "GPS module error"

**Check**:
- [ ] GPS module powered (3.3V, NOT 5V!)
- [ ] TX/RX pins connected correctly
- [ ] GPS module outdoors (clear sky view)
- [ ] Waited 2-3 minutes for satellite fix

**Fix**:
- Move to open area
- Check wiring (TX→RX16, RX→TX17)
- Verify GPS module LED is blinking

---

### "Server rejected - Check API key"

**Check**:
- [ ] API key copied correctly (no extra spaces)
- [ ] API key not expired
- [ ] Usage limit not exceeded (10,000 points)

**Fix**:
- Generate new API key
- Copy-paste carefully
- Update `config.h` and re-upload

---

### "No location on map"

**Check**:
- [ ] Logged into correct Circuit Digest account
- [ ] Selected correct device ID
- [ ] GPS has valid fix (4+ satellites)
- [ ] Data is being sent (check serial monitor)

**Fix**:
- Refresh browser page
- Check device ID matches
- Verify serial shows "Data transmitted"

---

## Data Usage & Limits

### Free Tier Limits

**Per API Key**:
- 10,000 GPS data points
- Unlimited devices (use same key)
- Unlimited viewers

**Data Point Calculation**:
```
Update interval: 5 seconds
Points per hour: 3600 / 5 = 720 points
Points per day (8 hours): 720 × 8 = 5,760 points
Days until limit: 10,000 / 5,760 = ~1.7 days
```

**Solution**: Generate new API key when limit reached (takes 30 seconds)

### Mobile Data Usage

**Per Update** (5 seconds):
- GPS coordinates: ~50 bytes
- HTTP overhead: ~200 bytes
- **Total**: ~250 bytes

**Per Hour**:
- 720 updates × 250 bytes = 180 KB

**Per Day** (8 hours):
- 180 KB × 8 = 1.44 MB

**Per Month**:
- 1.44 MB × 30 = ~43 MB

**Cost** (India):
- ₹10 per GB
- 43 MB / 1024 MB × ₹10 = **₹0.42/month**

---

## Comparison: GeoLinker vs MQTT

| Feature | GeoLinker | MQTT |
|---------|-----------|------|
| **Setup Difficulty** | Easy (just API key) | Medium (need HTML dashboard) |
| **Map Visualization** | Built-in | Manual (create HTML) |
| **Route Tracking** | Automatic | Manual implementation |
| **Offline Storage** | Built-in | Manual implementation |
| **Cost** | Free (10K points) | Free (unlimited) |
| **Data Usage** | ~43 MB/month | ~17 MB/month |
| **Family Access** | Login to website | Subscribe to topic |
| **Mobile App** | Web browser | MQTT client app |

**Recommendation**: **GeoLinker** for easiest setup and best user experience!

---

## Advanced Configuration

### Adjust Update Frequency

**More frequent** (every 2 seconds):
```cpp
#define GEOLINKER_UPDATE_INTERVAL 2  // Uses data faster
```

**Less frequent** (every 10 seconds):
```cpp
#define GEOLINKER_UPDATE_INTERVAL 10  // Saves data points
```

### Increase Offline Buffer

**For areas with poor connectivity**:
```cpp
#define GEOLINKER_OFFLINE_BUFFER 50  // Store 50 records
```

**Note**: Uses more RAM on ESP32

### Change Timezone

**For different regions**:
```cpp
// In geolinker_handler.cpp, line 38:
geo->setTimeOffset(0, 0);  // UTC
geo->setTimeOffset(-5, 0); // EST (UTC-5)
geo->setTimeOffset(8, 0);  // China (UTC+8)
```

---

## Summary

**Setup Steps**:
1. ✅ Register at circuitdigest.cloud
2. ✅ Generate API key
3. ✅ Update `config.h` with API key
4. ✅ Upload firmware to ESP32
5. ✅ View location at circuitdigest.cloud/geolinker

**What Family Needs**:
- Circuit Digest account credentials (or share yours)
- Web browser (any device)
- Internet connection

**Cost**:
- **Setup**: Free
- **API**: Free (10K points, renewable)
- **Data**: ~₹0.42/month

**Range**: Unlimited (works anywhere with mobile coverage)

---

**Ready to track!** 🌍

Enable your mobile hotspot, upload the firmware, and start tracking your location on GeoLinker!
