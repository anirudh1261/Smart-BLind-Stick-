# Smart Blind Stick - Quick Reference Card

## Sensor Placement Summary

| Sensor | Position | Height | Angle | Purpose |
|--------|----------|--------|-------|---------|
| **Front Ultrasonic** | Top | 80-85cm | 15° down | Head-level obstacles |
| **Left Ultrasonic** | Left side | 60cm | 45° left | Side obstacles |
| **Right Ultrasonic** | Right side | 60cm | 45° right | Side obstacles |
| **IR Sensor** | Bottom tip | 5cm | Down | Potholes, stairs |
| **GPS Module** | Top section | 85-90cm | Up | Location tracking |
| **Vibration Motor** | Handle | 75cm | - | Tactile feedback |
| **APR + Speaker** | Middle | 55cm | Side | Audio alerts |
| **ESP32 + Battery** | Middle-low | 40cm | - | Main controller |
| **SOS Button** | Handle | 75cm | - | Emergency |

## APR Audio Channels

| Ch | Pin | Message | Duration | Use Case |
|----|-----|---------|----------|----------|
| 1 | GPIO25 | "Obstacle ahead. Please stop." | 2s | Front obstacle |
| 2 | GPIO26 | "Pothole detected. Step carefully." | 2s | Ground drop |
| 3 | GPIO27 | "Obstacle on left/right. Turn." | 2s | Side obstacle |
| 4 | GPIO14 | "Path is clear. You may proceed." | 1.5s | Safe path |
| 5 | GPIO12 | "Battery low. Please charge soon." | 2s | Low battery |
| 6 | GPIO13 | "Smart stick activated. Stay safe." | 2s | Startup |
| 7 | GPIO15 | "Emergency alert sent. Help is on the way." | 3s | SOS pressed |
| 8 | GPIO4 | "GPS signal lost. Location unavailable." | 2s | No GPS |

## Alert Levels

| Distance | Alert Level | Vibration | Audio | LED |
|----------|-------------|-----------|-------|-----|
| > 100cm | SAFE | None | None | Green |
| 50-100cm | WARNING | Slow pulse (500ms) | "Obstacle ahead" | Yellow |
| < 50cm | CRITICAL | Fast pulse (200ms) | Urgent voice | Red |
| Pothole | DANGER | Continuous | "Pothole detected" | Red flash |

## Power Consumption

| Component | Current | Voltage | Notes |
|-----------|---------|---------|-------|
| ESP32 | 80-240mA | 5V | Active/WiFi |
| Ultrasonic (3x) | 45mA | 5V | During scan |
| GPS Module | 45mA | 3.3V | Acquiring |
| APR Module | 30mA | 5V | Playback |
| Vibration Motor | 100mA | 5V | Active |
| **Total Peak** | ~500mA | - | All active |
| **Typical** | ~150mA | - | Normal use |

**Battery Life**: 2x 18650 (2600mAh) = ~17 hours typical use

## Maintenance Checklist

### Daily
- [ ] Check battery level
- [ ] Test SOS button
- [ ] Verify audio output

### Weekly
- [ ] Clean sensor lenses
- [ ] Check for loose connections
- [ ] Test all alert modes

### Monthly
- [ ] Full system calibration
- [ ] Battery health check
- [ ] Firmware update (if available)

## Emergency Contacts

**Guardian Phone**: +91-XXXXXXXXXX (Update in config.h)  
**Emergency Services**: 112  
**Technical Support**: [Your contact]

---

**Version**: 1.0  
**Last Updated**: January 2026  
**Project**: Smart Blind Stick for Visually Impaired
