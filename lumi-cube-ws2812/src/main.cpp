#include <Arduino.h>
#include "config.h"
#include "buttons.h"
#include "vibration.h"
#include "leds.h"
#include "display.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("=== Lumi Cube MVP - Full Integration ===");
    
    // Initialize components
    Display::init();      // Display first (shows boot screen + table info)
    LEDs::init();         // LEDs second (boot sequence)
    Buttons::init();
    Vibration::init();
    
    Serial.printf("Table %d ready for service!\n", TABLE_NUMBER);
}

void loop() {
    // Check buttons and provide complete feedback
    if (Buttons::isBillPressed()) {
        // Show button pressed
        Display::showButtonPressed("BILL");
        
        // LED and vibration feedback
        LEDs::billPressed();     // Blue -> flowing -> green -> white
        Vibration::billFeedback(); // Single vibration
        
        // Show processing
        Display::showProcessing("BILL");
        delay(4000);  // Show during LED animation
        
        // Show success
        Display::showSuccess("Bill received");
        delay(2000);
        
        // Return to ready
        Display::showReady();
    }
    
    if (Buttons::isMenuPressed()) {
        Display::showButtonPressed("MENU");
        
        LEDs::menuPressed();     
        Vibration::menuFeedback(); 
        
        Display::showProcessing("MENU");
        delay(4000);
        
        Display::showSuccess("Menu received");
        delay(2000);
        
        Display::showReady();
    }
    
    if (Buttons::isServicePressed()) {
        Display::showButtonPressed("SERVICE");
        
        LEDs::servicePressed();  
        Vibration::serviceFeedback(); 
        
        Display::showProcessing("SERVICE");
        delay(4000);
        
        Display::showSuccess("Service received");
        delay(2000);
        
        Display::showReady();
    }
    
    delay(50); // Small delay for stability
}