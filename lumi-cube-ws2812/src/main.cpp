#include <Arduino.h>
#include "config.h"
#include "buttons.h"
#include "vibration.h"
#include "leds.h"
#include "display.h"

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("=== Lumi Cube MVP - Synchronized States ===");
    
    // Initialize components
    Display::init();      // Display first (shows boot screen + table info)
    LEDs::init();         // LEDs second (boot sequence)
    Buttons::init();
    Vibration::init();
    
    Serial.printf("Table %d ready for service!\n", TABLE_NUMBER);
}

void handleButtonPress(const char* buttonName, void (*vibrationFunc)(), void (*ledFunc)()) {
    Serial.printf("Processing %s request...\n", buttonName);
    
    // STATE 1: Button Pressed - Blue LEDs + Display + Vibration (synchronized)
    Display::showButtonPressed(buttonName);
    LEDs::setButtonCluster(COLOR_BLUE);  // Immediate blue
    vibrationFunc();  // Haptic feedback
    delay(1000);  // Hold blue state
    
    // STATE 2: Processing - Display + LED Animation (synchronized)
    Display::showProcessing(buttonName);
    LEDs::flowingBlueEffect();  // 5-second animation matches processing display
    
    // STATE 3: Success - Green LEDs + Display (synchronized)
    Display::showSuccess("received");
    LEDs::setButtonCluster(COLOR_GREEN);  // Green confirmation
    delay(2000);  // Hold success state
    
    // STATE 4: Return to Ready - White LEDs + Display (synchronized)
    Display::showReady();
    LEDs::setButtonCluster(COLOR_WHITE);  // Return to ready state
    
    Serial.printf("%s request completed!\n", buttonName);
}

void loop() {
    // Check buttons and provide synchronized feedback
    if (Buttons::isBillPressed()) {
        handleButtonPress("BILL", Vibration::billFeedback, LEDs::billPressed);
    }
    
    if (Buttons::isMenuPressed()) {
        handleButtonPress("MENU", Vibration::menuFeedback, LEDs::menuPressed);
    }
    
    if (Buttons::isServicePressed()) {
        handleButtonPress("SERVICE", Vibration::serviceFeedback, LEDs::servicePressed);
    }
    
    delay(50); // Small delay for stability
}