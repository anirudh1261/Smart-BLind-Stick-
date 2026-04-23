# 🚀 Smart Blind Stick with 360° Obstacle Detection & GPS Tracking

## 🦯 Project Overview

An affordable, intelligent mobility aid for visually impaired individuals using ESP32, ultrasonic sensors, IR sensor, GPS, and audio feedback.

**Cost**: ~₹1,840 (~$22 USD)  
**Battery Life**: 17+ hours  
**Detection Range**: 0-200 cm  
**Response Time**: <200ms

---

## ✨ Features

- ✅ **Multi-directional Obstacle Detection** (Front, Left, Right)
- ✅ **Pothole & Stair Detection** (IR ground sensor)
- ✅ **Audio Alerts** (8-channel voice guidance)
- ✅ **Vibration Feedback** (Intensity-based haptic alerts)
- ✅ **GPS Location Tracking** (Emergency positioning)
- ✅ **SOS Emergency Button** (Long-press activation)
- ✅ **Low Power Consumption** (17+ hour battery life)
- ✅ **Weatherproof Design** (IP65 enclosure)

---

## 🛠️ Hardware Requirements

| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 DevKit | 1 | Main controller |
| HC-SR04 Ultrasonic | 3 | Obstacle detection |
| IR Sensor Module | 1 | Ground/pothole detection |
| NEO-6M GPS | 1 | Location tracking |
| APR Voice Module (8ch) | 1 | Audio alerts |
| Vibration Motor | 1-2 | Tactile feedback |
| 18650 Battery | 2 | Power supply |
| PVC Pipe (25mm) | 1m | Stick body |
| Push Button | 1 | SOS trigger |
| Speaker (8Ω, 0.5W) | 1 | Audio output |

---

## 📁 Project Structure

```
BlindStick/
├── src/
│   ├── main.cpp                    # Main application
│   ├── sensor_manager.cpp          # Sensor control
│   └── feedback_controller.cpp     # Audio/vibration
├── include/
│   ├── config.h                    # Pin definitions
│   ├── sensor_manager.h
│   └── feedback_controller.h
├── docs/
│   ├── assembly_guide.md           # Hardware assembly
│   ├── apr_audio_script.md         # Voice recordings
│   └── quick_reference.md          # Cheat sheet
└── platformio.ini                  # Build configuration
```

---

## 🚀 Quick Start

### 1. Install PlatformIO

```bash
pip install platformio
```

### 2. Clone/Download Project

```bash
cd ~/Desktop/Antigravity/BlindStick
```

### 3. Update Configuration

Edit `include/config.h`:
```cpp
#define GUARDIAN_PHONE "+91-XXXXXXXXXX"  // Your emergency contact
```

### 4. Upload Firmware

```bash
pio run --target upload
pio device monitor
```

### 5. Assemble Hardware

Follow detailed guide in `docs/assembly_guide.md`

---

## 📊 Pin Configuration

### Ultrasonic Sensors
| Sensor | TRIG Pin | ECHO Pin |
|--------|----------|----------|
| Front  | GPIO23   | GPIO22   |
| Left   | GPIO21   | GPIO19   |
| Right  | GPIO18   | GPIO5    |

### Other Peripherals
| Component | Pin(s) |
|-----------|--------|
| IR Sensor | GPIO34 (ADC) |
| GPS Module | RX=GPIO16, TX=GPIO17 |
| APR Triggers | GPIO25, 26, 27 |
| Vibration Motor | GPIO32 (PWM) |
| SOS Button | GPIO35 (Pull-up) |
| Status LED | GPIO2 |

---

## 🎤 APR Audio Channels

| Channel | Message | Use Case |
|---------|---------|----------|
| 1 | "Obstacle ahead. Please stop." | Front obstacle |
| 2 | "Pothole detected. Step carefully." | Ground drop |
| 3 | "Obstacle on left/right. Turn." | Side obstacle |
| 4 | "Path is clear. You may proceed." | Safe path |
| 5 | "Battery low. Please charge soon." | Low battery |
| 6 | "Smart stick activated. Stay safe." | Startup |
| 7 | "Emergency alert sent." | SOS pressed |
| 8 | "GPS signal lost." | No GPS |

See `docs/apr_audio_script.md` for recording instructions.

---

## 🧪 Testing

### Unit Tests
```bash
# Test individual sensors
pio test -e test_ultrasonic
pio test -e test_ir_sensor
pio test -e test_audio
```

### Field Testing
1. Indoor navigation (hallways, rooms)
2. Outdoor obstacles (poles, trees, walls)
3. Ground detection (stairs, curbs, potholes)
4. Emergency SOS activation
5. Battery endurance (8+ hours)

---

## 📖 Documentation

- **[Implementation Plan](docs/implementation_plan.md)** - System architecture & development phases
- **[Assembly Guide](docs/assembly_guide.md)** - Hardware setup & sensor placement
- **[APR Audio Script](docs/apr_audio_script.md)** - Voice recording guide
- **[Quick Reference](docs/quick_reference.md)** - Pin mappings & alert levels
- **[Walkthrough](docs/walkthrough.md)** - Complete project guide

---

## 🔧 Troubleshooting

### No Audio Output
- Check speaker connections (8Ω impedance)
- Verify APR module power (5V)
- Test trigger pins with multimeter

### Inaccurate Distance Readings
- Clean ultrasonic sensor lenses
- Recalibrate (power cycle in open area)
- Check for loose wiring

### Weak Vibration
- Verify motor connections
- Test with different vibration motor
- Check transistor/MOSFET operation

### GPS Not Acquiring
- Move to open area (clear sky view)
- Wait 2-3 minutes for cold start
- Check antenna orientation (upward)

---

## 🛡️ Safety Notice

⚠️ **Important:**
- This device is an **aid**, not a replacement for traditional mobility training
- Always use with proper cane techniques
- Test in safe environment before outdoor use
- Regular maintenance is essential

---

## 🤝 Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create feature branch
3. Test thoroughly
4. Submit pull request

---

## 📄 License

MIT License - Open source for educational and humanitarian use

---

## 👥 Credits

**Developed for**: Visually impaired community  
**Purpose**: Affordable assistive technology  
**Target Users**: 39+ million blind individuals worldwide

---

## 📞 Support

- **Issues**: GitHub Issues
- **Email**: anirudh.ganji15@gmail.com
- **Documentation**: See `docs/` folder

---

**Version**: 1.0  
**Status**: ✅ Ready for Assembly & Testing  
**Last Updated**: January 3, 2026
