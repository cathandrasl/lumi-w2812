# Lumi Cube MVP - Wiring Guide

## 🔌 Current Component Connections

### **Buttons (Tactile Switches)**
```
Button 1 (Bill)    → GPIO 18 → GND
Button 2 (Menu)    → GPIO 19 → GND  
Button 3 (Service) → GPIO 21 → GND
```
- Uses internal pull-ups (no external resistors needed)
- Button press = LOW signal

### **Vibration Motor**
```
Vibration Motor VCC → 3.3V (or 5V for stronger vibration)
Vibration Motor GND → GND
Vibration Motor IN  → GPIO 4
```
- **Recommended**: Use 5V for stronger vibration
- Current draw: ~60mA operating, ~90mA startup

## 📋 Component Status

| Component | Status | GPIO | Notes |
|-----------|--------|------|-------|
| Bill Button | ✅ Working | 18 | Triggers single vibration |
| Menu Button | ✅ Working | 19 | Triggers double vibration |
| Service Button | ✅ Working | 21 | Triggers triple vibration |
| Vibration Motor | ✅ Working | 4 | Connected to 5V for stronger pulse |

## 🚀 Next Components to Wire

### **WS2812 LED Strip (7 LEDs)**
```
LED Strip VCC → 5V
LED Strip GND → GND
LED Strip DIN → GPIO 2
```

### **1.69" TFT Display (ST7789V)**
```
Display VCC → 3.3V
Display GND → GND
Display SCK → GPIO 14
Display MOSI → GPIO 13
Display RES → GPIO 12
Display DC → GPIO 27
Display CS → GPIO 15
Display BLK → GPIO 32
```

## ⚡ Power Requirements

| Component | Voltage | Current |
|-----------|---------|---------|
| ESP32 | 3.3V | ~240mA |
| Vibration Motor | 5V | ~60-90mA |
| LED Strip (7 LEDs) | 5V | ~420mA (max) |
| TFT Display | 3.3V | ~20-50mA |
| **Total** | **Mixed** | **~750mA max** |

## 🔧 Tips

- **Vibration too weak?** → Connect VCC to 5V instead of 3.3V
- **Button bouncing?** → Software debouncing handles most cases
- **Power issues?** → Use external 5V power supply for motors and LEDs