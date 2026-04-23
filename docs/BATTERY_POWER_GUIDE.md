# Battery Power System - 4× 18650 Configuration

## 🔋 Your Batteries

**What you have**: 4× 18650 Li-ion batteries (3.7V each)

**Capacity**: 2600-3500mAh per cell (depending on model)

---

## ⚡ Best Configuration: 2S2P (Recommended)

### What is 2S2P?

**2S** = 2 in Series (voltage doubles)  
**2P** = 2 in Parallel (capacity doubles)

```
┌─────────────────────────────────────────────┐
│         2S2P CONFIGURATION                  │
│                                             │
│   Series Pair 1          Series Pair 2     │
│  ┌──────────┐          ┌──────────┐        │
│  │Battery 1 │          │Battery 3 │        │
│  │  3.7V    ├──(+)─────┤  3.7V    │        │
│  └────┬─────┘          └────┬─────┘        │
│       │(-)                  │(-)            │
│       │                     │               │
│  ┌────▼─────┐          ┌───▼──────┐        │
│  │Battery 2 │          │Battery 4 │        │
│  │  3.7V    │          │  3.7V    │        │
│  └────┬─────┘          └────┬─────┘        │
│       │(+)                  │(+)            │
│       │                     │               │
│       └──────┬──────────────┘               │
│              │                              │
│         7.4V @ Double Capacity              │
│              │                              │
│              ▼                              │
│       To Step-Down Converter                │
└─────────────────────────────────────────────┘
```

### Why 2S2P is Best?

✅ **Voltage**: 7.4V (perfect for step-down to 5V)  
✅ **Capacity**: 2× original (e.g., 2600mAh × 2 = 5200mAh)  
✅ **Efficiency**: >90% with step-down converter  
✅ **Runtime**: **~15-20 hours** with your system!  
✅ **Safe**: Voltage within safe range for components

---

## 🔌 2S2P Wiring Diagram

### Step-by-Step Connection

**Step 1: Create First Series Pair (Battery 1 + Battery 2)**

```
Battery 1:  (+) ────────┐
                        │
Battery 2:  (-) ────────┘

Battery 1:  (-) ────────┐
                        ├──→ 7.4V Output (Group A)
Battery 2:  (+) ────────┘
```

**Step 2: Create Second Series Pair (Battery 3 + Battery 4)**

```
Battery 3:  (+) ────────┐
                        │
Battery 4:  (-) ────────┘

Battery 3:  (-) ────────┐
                        ├──→ 7.4V Output (Group B)
Battery 4:  (+) ────────┘
```

**Step 3: Connect Groups in Parallel**

```
Group A (+) ───┬───→ Step-Down IN+
               │
Group B (+) ───┘

Group A (-) ───┬───→ Step-Down IN-
               │
Group B (-) ───┘
```

### Complete Wiring

```
         Battery 1 (+)
              │
         ┌────┴────┐
         │         │
    Battery 2 (-)  Battery 3 (+)
         │         │
         │    ┌────┴────┐
         │    │         │
    Battery 2 (+)  Battery 4 (-)
         │         │
         │         │
         └────┬────┴────┬────→ To Step-Down (+)
              │         │
         Battery 1 (-) Battery 3 (-)
              │         │
              └─────┬───┘
                    │
                    └──────────→ To Step-Down (-)
```

---

## 🔧 Required Components

### 1. Battery Holder

**Option A**: 4× 18650 Battery Holder
- Search: "4S 18650 battery holder"
- Cost: ₹50-100
- Easy to replace batteries

**Option B**: DIY Battery Pack
- Use battery spacers
- Solder connections
- Add protection circuit

### 2. Step-Down Converter

**Recommended**: LM2596 or XL4015

**Specifications**:
- Input: 7-35V DC
- Output: Adjustable 1.25-35V
- Current: 3A continuous
- Efficiency: ~92%

**Where to Buy**:
- Amazon: "LM2596 DC-DC Buck Converter"
- Cost: ₹80-150

**Setup**:
1. Connect input to battery pack (7.4V)
2. Adjust potentiometer for exactly 5.0V output
3. Test with multimeter before connecting ESP32

### 3. Protection Circuit (Highly Recommended)

**4S BMS (Battery Management System)**
- Prevents over-discharge (<3.0V per cell)
- Prevents over-charge (>4.2V per cell)
- Balances cells during charging
- Short circuit protection

**Search**: "4S 18650 BMS" or "4S Li-ion Protection Board"  
**Cost**: ₹100-200

---

## 📊 Power Calculations

### With 2600mAh Batteries

```
Total Capacity: 2600mAh × 2 (parallel) = 5200mAh @ 7.4V

Energy: 5200mAh × 7.4V = 38.48 Wh

At 5V (after conversion):
Usable Capacity = 38.48Wh / 5V = 7696mAh @ 5V
(accounting for 90% efficiency: ~6926mAh)

Your System Draw: ~350mA average

Runtime = 6926mAh / 350mA = 19.8 hours ≈ 20 hours!
```

### With 3500mAh Batteries (Premium)

```
Total Capacity: 3500mAh × 2 = 7000mAh @ 7.4V

Energy: 7000mAh × 7.4V = 51.8 Wh

At 5V:
Usable: ~9324mAh @ 5V

Runtime = 9324mAh / 350mA = 26.6 hours ≈ 27 hours!
```

**Result**: All-day usage + multi-day standby!

---

## 🔋 Alternative Configurations

### Option 1: 4S (All in Series) - NOT RECOMMENDED

```
Battery 1 (+) → Battery 2 (-) → Battery 3 (-) → Battery 4 (-)
Battery 1 (-)                                    Battery 4 (+)
Result: 14.8V @ 2600mAh
```

❌ **Why Not?**
- Too high voltage (14.8V)
- Step-down gets hot
- Less efficient
- Harder to regulate

### Option 2: 4P (All in Parallel) - NOT RECOMMENDED

```
All (+) connected together
All (-) connected together
Result: 3.7V @ 10400mAh
```

❌ **Why Not?**
- Too low voltage (3.7V)
- Need boost converter (less efficient)
- More complex circuitry

### ✅ Best Choice: 2S2P
- Perfect voltage (7.4V)
- Double capacity
- Efficient step-down
- Safe and reliable

---

## 🛠️ Assembly Instructions

### Materials Needed
- 4× 18650 batteries (matched capacity/brand)
- 4S battery holder OR solder + wire
- Step-down converter (LM2596)
- 4S BMS protection board (optional but recommended)
- Power switch
- Multimeter
- Tape/labels

### Step 1: Charge Batteries

1. **Charge all 4 batteries fully** (4.2V each)
2. **Use same brand/capacity** for all 4
3. **Measure voltage** of each (should be 4.15-4.20V)
4. **Label batteries**: A1, A2, B1, B2

### Step 2: Create Series Pairs

**Pair A (Battery 1 + Battery 2)**:
```
Battery 1 (+) to wire
Battery 2 (-) to Battery 1 (-)
Battery 2 (+) to wire
Result: 7.4V between two wires
```

**Pair B (Battery 3 + Battery 4)**:
```
Battery 3 (+) to wire
Battery 4 (-) to Battery 3 (-)
Battery 4 (+) to wire
Result: 7.4V between two wires
```

### Step 3: Connect in Parallel

```
Pair A (+) ──┬─→ Red wire (to step-down +)
             │
Pair B (+) ──┘

Pair A (-) ──┬─→ Black wire (to step-down -)
             │
Pair B (-) ──┘
```

### Step 4: Add Protection (Optional)

**With BMS**:
```
Battery Pack (+) → BMS B+ terminal
Battery Pack (-) → BMS B- terminal

BMS P+ → Step-Down Input (+)
BMS P- → Step-Down Input (-)
```

### Step 5: Configure Step-Down

1. **Without load**, connect battery to step-down input
2. **Measure output** with multimeter
3. **Adjust pot** until exactly 5.00V
4. **Add load** (LED + resistor)
5. **Verify voltage** stays at 5V under load

### Step 6: Connect to ESP32

```
Step-Down OUT (+) → ESP32 VIN
Step-Down OUT (-) → ESP32 GND
```

### Step 7: Test System

1. **Power on** → ESP32 LED lights up
2. **Serial monitor** → Shows boot messages
3. **Load test** → Run all sensors for 5 minutes
4. **Check voltage** → Should stay at 5V
5. **Check temperature** → Step-down should be warm, not hot

---

## 📐 Physical Layout

### Compact Arrangement

```
┌─────────────────────────────────┐
│  Battery Pack Enclosure         │
│                                 │
│  [Bat1] [Bat2]  ← Series Pair A │
│  [Bat3] [Bat4]  ← Series Pair B │
│                                 │
│  [BMS Board]    ← Protection    │
│  [Step-Down]    ← Regulator     │
│  [Switch]       ← Power Control │
│                                 │
│  Velcro straps to PVC pipe      │
└─────────────────────────────────┘

Total Size: ~10cm × 6cm × 3cm
Weight: ~250g
```

---

## ⚡ Charging Your Battery Pack

### Option 1: Remove & Charge Individually (Safest)

1. Remove all 4 batteries
2. Charge each in external charger
3. Wait until all reach 4.2V
4. Reinstall in pack

### Option 2: Balance Charging with BMS

1. Connect 4S balance charger to BMS
2. Set charger to "4S Li-ion" mode
3. Charge at 1C rate (2.6A for 2600mAh cells)
4. BMS balances cells automatically

**Recommended Charger**: 
- "iMAX B6" or similar
- Supports 4S Li-ion
- Balance charging capability

---

## 🔒 Safety Features

### Essential Protections

1. **BMS Protection Board**:
   - Over-discharge: Cuts off at 3.0V per cell
   - Over-charge: Stops at 4.2V per cell
   - Short circuit: Instant cutoff
   - Over-current: Limits to 5A

2. **Fuse** (add between battery and step-down):
   - Rating: 2A or 3A
   - Protects against short circuits
   - Easy to replace

3. **Power Switch**:
   - Disconnect battery when not in use
   - Prevents parasitic drain
   - Emergency shutoff

### Connection Diagram with Safety

```
Battery Pack (+) ──→ [Fuse 3A] ──→ [Switch] ──→ BMS IN+
                                                     │
Battery Pack (-) ──→ BMS IN-                        │
                        │                            │
                     BMS OUT  ──→ Step-Down ──→ ESP32
```

---

## 🧪 Testing Checklist

### Before Full Assembly

- [ ] Each battery measures 3.7-4.2V
- [ ] Series pairs measure 7.4-8.4V
- [ ] Parallel connection maintains 7.4V
- [ ] Step-down outputs exactly 5.0V
- [ ] No heat after 5 minutes under load
- [ ] Voltage stable during sensor operation

### During Operation

- [ ] Monitor battery voltage (should stay >6.8V)
- [ ] Check step-down temperature (<60°C)
- [ ] Verify all sensors working
- [ ] Runtime achieves >15 hours

### Charging

- [ ] All cells balance to within 0.05V
- [ ] Charging stops at 4.2V per cell
- [ ] No excessive heat during charge
- [ ] BMS LED indicates proper operation

---

## 📊 Comparison: 2 vs 4 Batteries

| Feature | 2× Batteries | 4× Batteries (2S2P) |
|---------|--------------|---------------------|
| **Voltage** | 7.4V | 7.4V |
| **Capacity** | 2600mAh | 5200mAh |
| **Runtime** | ~13 hours | ~20 hours |
| **Weight** | ~90g | ~180g |
| **Cost** | ₹300 | ₹600 |
| **Size** | Smaller | Larger |
| **Best For** | Short trips | All-day use |

**Recommendation**: Use all 4 batteries for maximum runtime!

---

## 🛒 Shopping List

### Essential Components

| Item | Specification | Est. Price (₹) |
|------|--------------|----------------|
| 18650 Batteries | 3.7V, 2600-3500mAh | 150 × 4 = 600 |
| Battery Holder | 4× 18650 holder | 50-100 |
| Step-Down | LM2596, 3A | 80-150 |
| BMS Board | 4S, 10A Li-ion | 100-200 |
| Power Switch | SPST rocker | 20-30 |
| Fuse Holder | 3A automotive | 20-30 |
| Wires | 20 AWG, red/black | 30-50 |
| **TOTAL** | | **₹900-1200** |

### Tools Needed

- Soldering iron
- Multimeter
- Wire strippers
- Screwdrivers
- Heat shrink tubing
- Electrical tape

---

## 💡 Pro Tips

### 1. Battery Matching
- Use same brand/model for all 4
- Buy batteries together (same manufacturing batch)
- Match internal resistance if possible

### 2. Wiring
- Use thick wires (20 AWG) for battery connections
- Solder all connections for reliability
- Add heat shrink over all solder joints
- Use different color wires (red=+, black=-)

### 3. Mounting
- Use waterproof enclosure for battery pack
- Mount low on stick for balance
- Use velcro straps for easy removal
- Keep weight centered

### 4. Maintenance
- Check voltage monthly
- Rebalance cells every 10 charges
- Replace all 4 batteries together (don't mix old/new)
- Clean contacts every few months

---

## ⚠️ Safety Warnings

### DO NOT:
- ❌ Mix different battery capacities
- ❌ Mix old and new batteries
- ❌ Connect positive to negative (short circuit!)
- ❌ Charge beyond 4.2V per cell
- ❌ Discharge below 3.0V per cell
- ❌ Expose to extreme temperatures
- ❌ Pierce or damage battery cases

### DO:
- ✅ Use protection circuit (BMS)
- ✅ Monitor voltage regularly
- ✅ Store at 50% charge when not in use
- ✅ Use in well-ventilated area
- ✅ Keep away from flammable materials
- ✅ Have fire extinguisher nearby while charging

---

## 🔋 Expected Performance

### Runtime Estimates

**Active Usage** (sensors + GPS + WiFi):
- Current draw: 350-450mA
- Runtime: **18-20 hours**

**Standby** (sensors only, WiFi off):
- Current draw: 150-200mA
- Runtime: **35-40 hours**

**GPS Tracking** (intermittent WiFi):
- Current draw: 250-300mA
- Runtime: **24-26 hours**

### Real-World Usage

**Typical Day**:
```
8:00 AM:  Turn on (100% charge)
8:00 AM - 12:00 PM: Active use (4 hours, -20%)
12:00 PM - 1:00 PM: Standby (1 hour, -2%)
1:00 PM - 6:00 PM: Active use (5 hours, -25%)
6:00 PM: Still have ~50% charge remaining!
```

---

## 📞 Support & Resources

### Datasheets
- **LM2596**: [Texas Instruments](https://www.ti.com/lit/ds/symlink/lm2596.pdf)
- **18650 Safety**: [Battery University](https://batteryuniversity.com)
- **BMS Guide**: YouTube: "4S BMS Wiring Tutorial"

### Online Resources
- Arduino Forum: Battery power discussions
- ESP32.com: Power optimization tips
- YouTube: "18650 battery pack tutorial"

---

**Your 4-battery setup will provide excellent runtime and reliability!** 🔋

Use the 2S2P configuration for best results. Happy building!
