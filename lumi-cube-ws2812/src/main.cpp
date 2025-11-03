#include <Arduino.h>

/*
 * Simple ESP32 Vibration Motor Test
 * Just press the BOOT button to make it vibrate!
 */

const int VIBRATION_PIN = 2;  // Connect motor IN pin to GPIO2
const int BUTTON_PIN = 0;     // Built-in BOOT button

void setup() {
  Serial.begin(115200);
  Serial.println("Simple Vibration Test");
  Serial.println("Press BOOT button to vibrate!");
  
  pinMode(VIBRATION_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  digitalWrite(VIBRATION_PIN, LOW);  // Motor off
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {  // Button pressed
    Serial.println("Vibrating...");
    
    digitalWrite(VIBRATION_PIN, HIGH);   // Motor on
    delay(5000);                          // Vibrate for 300ms
    digitalWrite(VIBRATION_PIN, LOW);    // Motor off
    
    delay(500);  // Wait before next press
  }
}