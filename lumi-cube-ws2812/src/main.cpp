#include <Arduino.h>
#include "config.h"
#include "buttons.h"
#include "vibration.h"
#include "leds.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("=== Lumi Cube MVP - Buttons + Vibration + LEDs Test ===");
    
    // Initialize components
    LEDs::init();         // LEDs first (includes boot sequence)
    Buttons::init();
    Vibration::init();
    
    Serial.println("Ready! Press buttons for combined feedback:");
    Serial.println("  Power LED: Green (on)");
    Serial.println("  WiFi LED: Red (not connected)");
    Serial.println("  Button cluster: White (ready)");
    Serial.println("");
    Serial.println("All buttons use same colors:");
    Serial.println("  1. GREEN (1 second) - button pressed");
    Serial.println("  2. BLUE (1 second) - processing"); 
    Serial.println("  3. WHITE - return to ready");
    Serial.println("Plus vibration patterns: Bill=1x, Menu=2x, Service=3x");
}

void loop() {
    // Check buttons and provide combined feedback
    if (Buttons::isBillPressed()) {
        LEDs::billPressed();     // Blue cluster flash
        Vibration::billFeedback(); // Single vibration
    }
    
    if (Buttons::isMenuPressed()) {
        LEDs::menuPressed();     // Orange cluster flash
        Vibration::menuFeedback(); // Double vibration
    }
    
    if (Buttons::isServicePressed()) {
        LEDs::servicePressed();  // Green cluster flash
        Vibration::serviceFeedback(); // Triple vibration
    }
    
    delay(50); // Small delay for stability
}