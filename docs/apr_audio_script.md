# APR Audio Module - Recording Script

## Module Overview
- **Model**: APR9600 / APR33A3 (8-channel voice module)
- **Recording Time**: 340 seconds total (divided across 8 channels)
- **Playback**: Triggered by GPIO pins
- **Quality**: 8kHz sampling, clear voice output
- **Language**: English (can be customized to Hindi/regional languages)

## Channel Assignments

### Channel 1 (Trigger Pin: GPIO25) - "Obstacle Ahead"
**Duration**: 2 seconds  
**Script**: 
```
"Obstacle ahead. Please stop."
```
**Tone**: Urgent but calm  
**Use Case**: Front ultrasonic sensor detects obstacle < 100cm  
**Trigger Frequency**: High (most common alert)

---

### Channel 2 (Trigger Pin: GPIO26) - "Pothole Detected"
**Duration**: 2 seconds  
**Script**: 
```
"Pothole detected. Step carefully."
```
**Tone**: Warning, clear  
**Use Case**: IR sensor detects ground drop  
**Trigger Frequency**: Medium (outdoor use)

---

### Channel 3 (Trigger Pin: GPIO27) - "Turn Left/Right"
**Duration**: 2 seconds  
**Script**: 
```
"Obstacle on your left. Turn right."
```
**Alternative**: 
```
"Obstacle on your right. Turn left."
```
**Tone**: Directional guidance  
**Use Case**: Side sensors detect obstacles  
**Note**: Can record two versions or use text-to-speech for dynamic direction

---

### Channel 4 - "Clear Path"
**Duration**: 1.5 seconds  
**Script**: 
```
"Path is clear. You may proceed."
```
**Tone**: Reassuring  
**Use Case**: All sensors show safe distance  
**Trigger Frequency**: Low (optional feedback)

---

### Channel 5 - "Battery Low"
**Duration**: 2 seconds  
**Script**: 
```
"Battery low. Please charge soon."
```
**Tone**: Informative  
**Use Case**: Battery voltage < 20%  
**Trigger Frequency**: Once per session

---

### Channel 6 - "System Ready"
**Duration**: 2 seconds  
**Script**: 
```
"Smart stick activated. Stay safe."
```
**Tone**: Welcoming  
**Use Case**: Startup confirmation  
**Trigger Frequency**: Once at boot

---

### Channel 7 - "Emergency Alert"
**Duration**: 3 seconds  
**Script**: 
```
"Emergency alert sent. Help is on the way."
```
**Tone**: Reassuring  
**Use Case**: SOS button pressed  
**Trigger Frequency**: Rare (emergency only)

---

### Channel 8 - "GPS Signal Lost"
**Duration**: 2 seconds  
**Script**: 
```
"GPS signal lost. Location unavailable."
```
**Tone**: Informative  
**Use Case**: GPS module can't acquire satellites  
**Trigger Frequency**: Low (indoor use)

---

## Recording Instructions

### Equipment Needed
1. **Microphone**: Clear, noise-cancelling mic
2. **Recording Software**: Audacity (free) or phone voice recorder
3. **Environment**: Quiet room, no echo
4. **Speaker**: Clear voice, moderate pace

### Recording Steps

1. **Prepare Audio Files**
   - Record each message separately
   - Save as WAV format (8kHz, mono, 16-bit)
   - Trim silence at start/end
   - Normalize volume to -3dB

2. **Edit for Clarity**
   - Remove background noise
   - Add 0.2s silence at beginning
   - Ensure consistent volume across all channels
   - Export each as: `ch1.wav`, `ch2.wav`, etc.

3. **Upload to APR Module**
   - Connect APR module to computer via USB adapter
   - Use APR programming software (manufacturer-specific)
   - Upload each WAV file to corresponding channel
   - Test playback before installation

### Voice Guidelines
- **Speed**: Moderate (not too fast, not too slow)
- **Clarity**: Pronounce each word clearly
- **Volume**: Loud enough for outdoor use
- **Tone**: Calm and reassuring (avoid panic-inducing tone)
- **Language**: Can record in multiple languages

### Alternative: Text-to-Speech
If professional recording is not available:
- Use Google Text-to-Speech or Amazon Polly
- Select clear, natural-sounding voice
- Export as WAV files
- Process as described above

## Wiring APR Module

### Pin Connections
```
APR Module          ESP32
---------          -------
VCC        →       5V
GND        →       GND
TRIG1      →       GPIO25
TRIG2      →       GPIO26
TRIG3      →       GPIO27
TRIG4      →       GPIO14 (optional)
TRIG5      →       GPIO12 (optional)
TRIG6      →       GPIO13 (optional)
TRIG7      →       GPIO15 (optional)
TRIG8      →       GPIO4  (optional)
AUDIO OUT  →       Speaker (8Ω, 0.5W)
```

### Trigger Logic
- **Idle State**: All trigger pins LOW
- **Play Audio**: Pull trigger pin HIGH for 100ms
- **Playback**: Module plays corresponding channel
- **Busy Pin**: Optional feedback when audio is playing

## Testing Procedure

1. **Individual Channel Test**
   ```cpp
   digitalWrite(APR_TRIGGER_1, HIGH);
   delay(100);
   digitalWrite(APR_TRIGGER_1, LOW);
   delay(3000); // Wait for playback
   ```

2. **Volume Adjustment**
   - APR module has onboard potentiometer
   - Adjust for outdoor audibility
   - Test in noisy environment

3. **Speaker Selection**
   - Use 8Ω, 0.5-1W speaker
   - Weatherproof speaker recommended
   - Mount facing user for clear audio

## Multilingual Support (Optional)

### Hindi Version
- Channel 1: "सामने रुकावट है। कृपया रुकें।"
- Channel 2: "गड्ढा है। सावधानी से चलें।"
- Channel 3: "बाएं/दाएं मुड़ें।"

### Regional Language
- Record in user's native language
- Maintain same tone and clarity
- Test with target user group

## Troubleshooting

**Issue**: No audio output
- Check speaker connections
- Verify 5V power supply
- Test with multimeter

**Issue**: Distorted audio
- Reduce volume on APR module
- Check for loose connections
- Re-record with better quality

**Issue**: Wrong channel plays
- Verify trigger pin assignments
- Check for short circuits
- Re-upload audio files

## Future Enhancements

1. **Dynamic Voice**: Use ESP32 with I2S DAC for text-to-speech
2. **Bluetooth Audio**: Stream alerts to user's earphones
3. **Voice Commands**: Add microphone for voice control
4. **Multi-language**: Switch language via button press
