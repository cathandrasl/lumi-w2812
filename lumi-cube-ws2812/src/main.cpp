#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    Serial.println("=== LUMI Display Width Test ===");
    
    tft.init();
    tft.setRotation(1);  // Landscape mode (280x240)
    tft.fillScreen(TFT_BLACK);
    
    // Set backlight
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 200);
    
    Serial.println("Testing full 280px width...");
    
    // Test: Copy exact working showDefaultDisplay but with red/black/white
    tft.fillScreen(TFT_BLACK);  // Black background instead of white
    
    // Top bar - should span full 280px width
    tft.fillRect(0, 0, 280, 60, TFT_RED);  // Red top bar
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(5);
    tft.drawString("TABLE", 140, 30);
    
    // Big table number - red on black
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(8);
    tft.drawString(String(TABLE_NUMBER), 140, 120);
    
    // Status text - white on black
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    if (RESERVATION_STATUS) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("RESERVED", 140, 160);
    } else {
        tft.drawString("AVAILABLE", 140, 160);
    }
    
    // Bottom bar - should span full 280px width
    tft.fillRect(0, 210, 280, 30, TFT_RED);  // Red bottom bar
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setTextSize(2);
    tft.drawString("READY", 140, 225);
    
    Serial.println("Display test complete!");
    Serial.println("You should see:");
    Serial.println("- Red bars at top and bottom spanning FULL width");
    Serial.println("- Big red table number in center");
    Serial.println("- White 'AVAILABLE' text");
    Serial.println("- If bars don't span full width, there's a config issue");
}

void loop() {
    // Static display test
    delay(1000);
}