# Smart Blind Stick - Hardware Assembly Guide

## PVC Pipe Specifications
- **Length**: 90-100 cm (adjustable to user height)
- **Diameter**: 25-32 mm (1-1.25 inch)
- **Material**: Lightweight PVC or aluminum
- **Sections**: 3 foldable sections (optional for portability)

## Sensor Placement Diagram

```
                    [FRONT ULTRASONIC]
                           ↓
    ═══════════════════════════════════════
    ║                                     ║
    ║  [LEFT]  ←  USER GRIP AREA  → [RIGHT]
    ║           ULTRASONIC           ULTRASONIC
    ║                                     ║
    ║         [VIBRATION MOTOR]          ║
    ║         (Inside handle)            ║
    ║                                     ║
    ║         [ESP32 + BATTERY]          ║
    ║         (Waterproof box)           ║
    ║                                     ║
    ║         [APR MODULE]               ║
    ║         (With speaker)             ║
    ║                                     ║
    ║         [GPS MODULE]               ║
    ║         (Top section)              ║
    ║                                     ║
    ║         [SOS BUTTON]               ║
    ║         (Thumb accessible)         ║
    ║                                     ║
    ═══════════════════════════════════════
                    ↓
              [IR SENSOR]
           (Bottom tip - 5cm from ground)
```

## Detailed Sensor Positions

### 1. Front Ultrasonic Sensor (HC-SR04)
- **Location**: Top of stick, angled 15° downward
- **Height from ground**: 80-85 cm
- **Detection range**: Forward path (0-200 cm)
- **Purpose**: Detect head-level obstacles (branches, poles, walls)
- **Mounting**: Use 3D-printed bracket or hot glue + zip ties

### 2. Left Ultrasonic Sensor (HC-SR04)
- **Location**: Left side, 60 cm from ground
- **Angle**: 45° outward from stick axis
- **Detection range**: Left side obstacles
- **Purpose**: Detect side obstacles, narrow passages
- **Mounting**: Angled bracket facing left-forward

### 3. Right Ultrasonic Sensor (HC-SR04)
- **Location**: Right side, 60 cm from ground (mirror of left)
- **Angle**: 45° outward from stick axis
- **Detection range**: Right side obstacles
- **Purpose**: Detect side obstacles, doorways
- **Mounting**: Angled bracket facing right-forward

### 4. IR Sensor (Obstacle Avoidance Module)
- **Location**: Bottom tip of stick
- **Height from ground**: 3-5 cm
- **Angle**: Pointing straight down
- **Detection range**: Ground level (0-30 cm)
- **Purpose**: Detect potholes, stairs, curbs, ground drops
- **Mounting**: Waterproof housing at tip

### 5. GPS Module (NEO-6M)
- **Location**: Top section (best sky visibility)
- **Height from ground**: 85-90 cm
- **Orientation**: Antenna facing upward
- **Purpose**: Location tracking, navigation
- **Mounting**: Inside transparent plastic cover

### 6. Vibration Motor
- **Location**: Handle grip area
- **Position**: Inside PVC pipe, touching inner wall
- **Height from ground**: 70-80 cm (where hand grips)
- **Purpose**: Tactile feedback
- **Mounting**: Hot glue or foam padding

### 7. APR Audio Module + Speaker
- **Location**: Middle section
- **Height from ground**: 50-60 cm
- **Speaker position**: Side-facing (toward user)
- **Volume**: Adjustable, loud enough for outdoor use
- **Mounting**: Waterproof enclosure with speaker grille

### 8. ESP32 + Battery Pack
- **Location**: Middle-lower section
- **Height from ground**: 30-50 cm
- **Enclosure**: Waterproof IP65 box
- **Battery**: 2x 18650 Li-ion (7.4V, 2600mAh)
- **Mounting**: Velcro straps for easy access

### 9. SOS Emergency Button
- **Location**: Handle area, thumb-accessible
- **Height from ground**: 75-80 cm
- **Type**: Waterproof momentary push button
- **Color**: Red (for easy identification)
- **Mounting**: Drilled hole with rubber seal

### 10. Status LED
- **Location**: Top section, visible to user
- **Height from ground**: 85 cm
- **Color**: Blue (system on), Red (battery low)
- **Mounting**: Small drilled hole with LED holder

## Wiring Layout

### Cable Management
1. **Main Power Bus**: Red (+7.4V), Black (GND)
2. **Sensor Cables**: Run inside PVC pipe
3. **Waterproofing**: Use heat shrink tubing at all joints
4. **Connectors**: Use JST connectors for modularity
5. **Cable Routing**: Avoid sharp bends, secure with zip ties every 10cm

### Power Distribution
```
Battery Pack (7.4V)
    ↓
Step-Down Converter (5V for ESP32)
    ↓
ESP32 (3.3V regulated output)
    ↓
├── Ultrasonic Sensors (3x)
├── IR Sensor
├── GPS Module
├── APR Module
└── Vibration Motor (via transistor)
```

## Assembly Steps

1. **Prepare PVC Pipe**
   - Cut to desired length
   - Sand edges smooth
   - Drill holes for sensors and button

2. **Mount Sensors**
   - Install ultrasonic sensors with angled brackets
   - Secure IR sensor at bottom tip
   - Place GPS module at top with clear view

3. **Install Electronics**
   - Mount ESP32 in waterproof box
   - Connect battery pack with switch
   - Install APR module with speaker

4. **Wire Components**
   - Run all cables inside pipe
   - Use color-coded wires
   - Test each connection before sealing

5. **Add Feedback Systems**
   - Install vibration motor in handle
   - Mount speaker facing user
   - Add SOS button in grip area

6. **Weatherproofing**
   - Seal all openings with silicone
   - Use waterproof connectors
   - Test with water spray

7. **Final Testing**
   - Upload firmware
   - Test each sensor individually
   - Verify audio and vibration
   - Check battery life

## Ergonomic Considerations

- **Handle Grip**: Add foam padding for comfort
- **Weight Balance**: Keep heavy components (battery) in middle
- **Total Weight**: Target < 400g for easy handling
- **Foldability**: Use telescopic sections for portability
- **Wrist Strap**: Add safety strap to prevent drops

## Maintenance Access

- **Battery Compartment**: Easy-open lid with latch
- **Charging Port**: Micro-USB accessible without opening
- **Sensor Cleaning**: Removable protective covers
- **Firmware Update**: USB port in handle area
