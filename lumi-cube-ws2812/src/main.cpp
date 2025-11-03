#include <Arduino.h>
#include "config.h"
#include "buttons.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("=== Lumi Cube MVP - Button Test ===");
    
    // Initialize buttons
    Buttons::init();
    
    Serial.println("Ready! Press buttons to test.");
}

void loop() {
    // Update buttons (handles debouncing and detection)
    Buttons::update();
    
    delay(50); // Small delay for stability
}