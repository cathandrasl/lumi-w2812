#include "leds.h"

// Static variable definitions
Adafruit_NeoPixel LEDs::strip = Adafruit_NeoPixel(LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
bool LEDs::initialized = false;

void LEDs::init() {
    strip.begin();
    strip.setBrightness(LED_BRIGHTNESS);
    strip.clear();
    strip.show();
    initialized = true;
    
    if (DEBUG_LEDS) {
        Serial.printf("[LEDS] Initialized %d LEDs on GPIO%d\n", LED_COUNT, LED_STRIP_PIN);
    }
    
    // Boot sequence
    bootSequence();
}

void LEDs::bootSequence() {
    if (!initialized) return;
    
    if (DEBUG_LEDS) {
        Serial.println("[LEDS] Boot sequence - all white");
    }
    
    // All LEDs white for 1 second
    setAll(COLOR_WHITE);
    delay(1000);
    
    // Clear all LEDs
    clear();
    
    // Set default states
    setPowerLED(COLOR_GREEN);      // Power LED green (device is on)
    setWiFiLED(COLOR_RED);         // WiFi LED red (not connected yet)
    setButtonCluster(COLOR_WHITE); // Button cluster white (ready state)
}

void LEDs::setLED(int index, uint32_t color) {
    if (!initialized || index < 0 || index >= LED_COUNT) return;
    
    strip.setPixelColor(index, color);
    strip.show();
}

void LEDs::setPowerLED(uint32_t color) {
    setLED(LED_POWER, color);
}

void LEDs::setWiFiLED(uint32_t color) {
    setLED(LED_WIFI, color);
}

void LEDs::setBatteryLED(uint32_t color) {
    setLED(LED_BATTERY, color);
}

void LEDs::setButtonLED(int buttonIndex, uint32_t color) {
    // Map button index to LED index
    switch (buttonIndex) {
        case 0: setLED(LED_BILL, color); break;     // Bill button
        case 1: setLED(LED_MENU, color); break;     // Menu button
        case 2: setLED(LED_SERVICE, color); break;  // Service button
    }
}

void LEDs::setButtonCluster(uint32_t color) {
    if (!initialized) return;
    
    for (int i = LED_BUTTON_START; i < LED_BUTTON_START + LED_BUTTON_COUNT; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void LEDs::setPowerStatus(bool on) {
    setPowerLED(on ? COLOR_GREEN : COLOR_RED);
}

void LEDs::setWiFiStatus(int status) {
    switch (status) {
        case 0: setWiFiLED(COLOR_OFF); break;      // Off
        case 1: setWiFiLED(COLOR_ORANGE); break;   // Connecting
        case 2: setWiFiLED(COLOR_GREEN); break;    // Connected
        case 3: setWiFiLED(COLOR_RED); break;      // Error
    }
}

void LEDs::setBatteryStatus(int level) {
    switch (level) {
        case 0: setBatteryLED(COLOR_RED); break;    // Low
        case 1: setBatteryLED(COLOR_ORANGE); break; // Medium
        case 2: setBatteryLED(COLOR_GREEN); break;  // Good
    }
}

void LEDs::billPressed() {
    if (DEBUG_LEDS) {
        Serial.println("[LEDS] Bill button feedback - green press, blue processing");
    }
    // Green flash for button press
    setButtonCluster(COLOR_GREEN);
    delay(1000);  // 1 second green
    // Blue for processing
    setButtonCluster(COLOR_BLUE);
    delay(1000);  // 1 second blue
    setButtonCluster(COLOR_WHITE); // Return to ready state
}

void LEDs::menuPressed() {
    if (DEBUG_LEDS) {
        Serial.println("[LEDS] Menu button feedback - green press, blue processing");
    }
    // Green flash for button press
    setButtonCluster(COLOR_GREEN);
    delay(1000);  // 1 second green
    // Blue for processing
    setButtonCluster(COLOR_BLUE);
    delay(1000);  // 1 second blue
    setButtonCluster(COLOR_WHITE); // Return to ready state
}

void LEDs::servicePressed() {
    if (DEBUG_LEDS) {
        Serial.println("[LEDS] Service button feedback - green press, blue processing");
    }
    // Green flash for button press
    setButtonCluster(COLOR_GREEN);
    delay(1000);  // 1 second green
    // Blue for processing
    setButtonCluster(COLOR_BLUE);
    delay(1000);  // 1 second blue
    setButtonCluster(COLOR_WHITE); // Return to ready state
}

void LEDs::clearButtonFeedback() {
    setButtonCluster(COLOR_OFF);
}

void LEDs::setAll(uint32_t color) {
    if (!initialized) return;
    
    for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void LEDs::clear() {
    setAll(COLOR_OFF);
}

void LEDs::show() {
    if (initialized) {
        strip.show();
    }
}

void LEDs::setBrightness(uint8_t brightness) {
    if (initialized) {
        strip.setBrightness(brightness);
        strip.show();
    }
}

void LEDs::testSequence() {
    if (!initialized) {
        Serial.println("[LEDS] Not initialized!");
        return;
    }
    
    Serial.println("[LEDS] Starting test sequence...");
    
    // Test each LED individually
    for (int i = 0; i < LED_COUNT; i++) {
        Serial.printf("[LEDS] Testing LED %d\n", i);
        clear();
        setLED(i, COLOR_WHITE);
        delay(500);
    }
    
    // Test colors
    Serial.println("[LEDS] Testing colors...");
    setAll(COLOR_RED); delay(500);
    setAll(COLOR_GREEN); delay(500);
    setAll(COLOR_BLUE); delay(500);
    setAll(COLOR_ORANGE); delay(500);
    
    // Test button cluster
    Serial.println("[LEDS] Testing button cluster...");
    clear();
    setButtonCluster(COLOR_WHITE);
    delay(1000);
    
    clear();
    Serial.println("[LEDS] Test complete!");
}

uint32_t LEDs::wheel(byte wheelPos) {
    wheelPos = 255 - wheelPos;
    if (wheelPos < 85) {
        return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
    }
    if (wheelPos < 170) {
        wheelPos -= 85;
        return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
    wheelPos -= 170;
    return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

void LEDs::rainbowCycle(int cycles) {
    if (!initialized) return;
    
    for (int cycle = 0; cycle < cycles; cycle++) {
        for (int j = 0; j < 256; j++) {
            for (int i = 0; i < LED_COUNT; i++) {
                strip.setPixelColor(i, wheel(((i * 256 / LED_COUNT) + j) & 255));
            }
            strip.show();
            delay(10);
        }
    }
    clear();
}