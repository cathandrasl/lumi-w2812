#include "buttons.h"

// Static variable definitions
unsigned long Buttons::lastBillPress = 0;
unsigned long Buttons::lastMenuPress = 0;
unsigned long Buttons::lastServicePress = 0;

void Buttons::init() {
    pinMode(BUTTON_BILL_PIN, INPUT_PULLUP);
    pinMode(BUTTON_MENU_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SERVICE_PIN, INPUT_PULLUP);
    
    if (DEBUG_BUTTONS) {
        Serial.println("[BUTTONS] Initialized");
        Serial.printf("[BUTTONS] Bill: GPIO%d, Menu: GPIO%d, Service: GPIO%d\n", 
                     BUTTON_BILL_PIN, BUTTON_MENU_PIN, BUTTON_SERVICE_PIN);
    }
}

void Buttons::update() {
    if (isBillPressed()) {
        Serial.println("[BUTTONS] BILL pressed!");
    }
    
    if (isMenuPressed()) {
        Serial.println("[BUTTONS] MENU pressed!");
    }
    
    if (isServicePressed()) {
        Serial.println("[BUTTONS] SERVICE pressed!");
    }
}

bool Buttons::isBillPressed() {
    return checkButton(BUTTON_BILL_PIN, lastBillPress);
}

bool Buttons::isMenuPressed() {
    return checkButton(BUTTON_MENU_PIN, lastMenuPress);
}

bool Buttons::isServicePressed() {
    return checkButton(BUTTON_SERVICE_PIN, lastServicePress);
}

bool Buttons::checkButton(int pin, unsigned long &lastPress) {
    if (digitalRead(pin) == LOW) {  // Button pressed (pull-up)
        unsigned long now = millis();
        if (now - lastPress > BUTTON_DEBOUNCE_MS) {
            lastPress = now;
            return true;
        }
    }
    return false;
}

void Buttons::test() {
    Serial.println("[BUTTONS] Starting test...");
    Serial.println("[BUTTONS] Press each button to test");
    
    while (true) {
        update();
        delay(50);
    }
}