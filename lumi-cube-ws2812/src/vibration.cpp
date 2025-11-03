#include "vibration.h"

// Static variable definitions
bool Vibration::initialized = false;
unsigned long Vibration::pulseStartTime = 0;
bool Vibration::currentlyVibrating = false;

void Vibration::init() {
    pinMode(VIBRATION_PIN, OUTPUT);
    digitalWrite(VIBRATION_PIN, LOW);
    initialized = true;
    
    if (DEBUG_ENABLED) {
        Serial.printf("[VIBRATION] Initialized on GPIO%d\n", VIBRATION_PIN);
    }
    
    // Welcome pulse
    bootFeedback();
}

void Vibration::pulse(int duration) {
    if (!initialized) return;
    
    digitalWrite(VIBRATION_PIN, HIGH);
    delay(duration);
    digitalWrite(VIBRATION_PIN, LOW);
    
    if (DEBUG_ENABLED) {
        Serial.printf("[VIBRATION] Pulse %dms\n", duration);
    }
}

void Vibration::stop() {
    if (!initialized) return;
    
    digitalWrite(VIBRATION_PIN, LOW);
    currentlyVibrating = false;
}

void Vibration::billFeedback() {
    if (DEBUG_ENABLED) {
        Serial.println("[VIBRATION] Bill feedback - single strong pulse");
    }
    pulse(VIBRATION_STRONG);  // 400ms strong pulse
}

void Vibration::menuFeedback() {
    if (DEBUG_ENABLED) {
        Serial.println("[VIBRATION] Menu feedback - double pulse");
    }
    pattern(2, VIBRATION_MEDIUM, VIBRATION_GAP);  // 250ms pulses, 100ms gap
}

void Vibration::serviceFeedback() {
    if (DEBUG_ENABLED) {
        Serial.println("[VIBRATION] Service feedback - triple pulse");
    }
    pattern(3, VIBRATION_SHORT, VIBRATION_GAP);   // 150ms pulses, 100ms gap
}

void Vibration::bootFeedback() {
    if (DEBUG_ENABLED) {
        Serial.println("[VIBRATION] Boot feedback");
    }
    pulse(150);
}

void Vibration::successFeedback() {
    if (DEBUG_ENABLED) {
        Serial.println("[VIBRATION] Success feedback");
    }
    pulse(100);
}

void Vibration::errorFeedback() {
    if (DEBUG_ENABLED) {
        Serial.println("[VIBRATION] Error feedback");
    }
    pattern(4, 50, 50);
}

void Vibration::pattern(int pulses, int onTime, int offTime) {
    if (!initialized) return;
    
    for (int i = 0; i < pulses; i++) {
        digitalWrite(VIBRATION_PIN, HIGH);
        delay(onTime);
        digitalWrite(VIBRATION_PIN, LOW);
        
        if (i < pulses - 1) {  // Don't delay after last pulse
            delay(offTime);
        }
    }
}

void Vibration::test() {
    if (!initialized) {
        Serial.println("[VIBRATION] Not initialized!");
        return;
    }
    
    Serial.println("[VIBRATION] Starting test sequence...");
    
    Serial.println("  1. Single pulse (Bill)");
    billFeedback();
    delay(1000);
    
    Serial.println("  2. Double pulse (Menu)");
    menuFeedback();
    delay(1000);
    
    Serial.println("  3. Triple pulse (Service)");
    serviceFeedback();
    delay(1000);
    
    Serial.println("  4. Success pulse");
    successFeedback();
    delay(1000);
    
    Serial.println("  5. Error pattern");
    errorFeedback();
    delay(1000);
    
    Serial.println("[VIBRATION] Test complete!");
}

bool Vibration::isActive() {
    return currentlyVibrating;
}