#include <Arduino.h>
#include "config.h"
#include "buttons.h"
#include "vibration.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("=== Lumi Cube MVP - Buttons + Vibration Test ===");
    
    // Initialize components
    Buttons::init();
    Vibration::init();
    
    Serial.println("Ready! Press buttons to test vibration feedback:");
    Serial.println("  BILL button → Single vibration");
    Serial.println("  MENU button → Double vibration"); 
    Serial.println("  SERVICE button → Triple vibration");
}

void loop() {
    // Check buttons and provide vibration feedback
    if (Buttons::isBillPressed()) {
        Vibration::billFeedback();
    }
    
    if (Buttons::isMenuPressed()) {
        Vibration::menuFeedback();
    }
    
    if (Buttons::isServicePressed()) {
        Vibration::serviceFeedback();
    }
    
    delay(500); // Small delay for stability
}