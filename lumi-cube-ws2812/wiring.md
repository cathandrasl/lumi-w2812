# Lumi Cube MVP - Wiring Guide

## 🔌 CORRECTED Component Connections

### **Buttons (Tactile Switches)**
```
Button 1 (Bill)    → GPIO 25 → GND  (CHANGED from 18)
Button 2 (Menu)    → GPIO 26 → GND  (CHANGED from 19)
Button 3 (Service) → GPIO 27 → GND  (CHANGED from 21)
```
- Uses internal pull-ups (no external resistors needed)
- Button press = LOW signal

### **1.69" TFT Display (ST7789V) - CONFIRMED WORKING**
```
VCC → 3.3V
GND → GND
SCL → GPIO 18 (SCLK)
SDA → GPIO 23 (MOSI)
RES → GPIO 4 (Reset)
DC → GPIO 2 (Data/Command)
CS → GPIO 5 (Chip Select)
BLK → GPIO 15 (Backlight)
```

### **WS2812 LED Strip (7 LEDs)**
```
LED Strip VCC → 5V
LED Strip GND → GND
LED Strip DIN → GPIO 32  (CHANGED from 2 - was conflicting with display)
```

### **Vibration Motor**
```
Vibration Motor VCC → 5V
Vibration Motor GND → GND
Vibration Motor IN  → GPIO 33  (CHANGED from 4 - was conflicting with display)
```

## 🚨 **PIN CONFLICTS FIXED:**
- **GPIO 18**: Was used for both buttons and display - now display only
- **GPIO 2**: Was used for both LEDs and display - now display only  
- **GPIO 4**: Was used for both vibration and display - now display only

## 📋 Component Status

| Component | Status | GPIO | Notes |
|-----------|--------|------|-------|
| Bill Button | ⚠️ **REWIRE** | 25 | Was GPIO 18 |
| Menu Button | ⚠️ **REWIRE** | 26 | Was GPIO 19 |
| Service Button | ⚠️ **REWIRE** | 27 | Was GPIO 21 |
| Display | ✅ Working | 18,23,4,2,5,15 | Confirmed wiring |
| LED Strip | ⚠️ **REWIRE** | 32 | Was GPIO 2 |
| Vibration Motor | ⚠️ **REWIRE** | 33 | Was GPIO 4 |

## 🔧 **IMMEDIATE ACTION NEEDED:**
1. **Rewire buttons** to GPIO 25, 26, 27
2. **Rewire LED strip** to GPIO 32
3. **Rewire vibration motor** to GPIO 33
4. **Upload test code** to verify all components work