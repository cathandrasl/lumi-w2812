#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"

class Display {
public:
    // Initialize display
    static void init();
    
    // Boot sequence
    static void showBootScreen();
    
    // Status displays
    static void showReady();
    static void showWiFiStatus(const char* status);
    static void showBatteryLevel(int level);
    
    // Button feedback displays
    static void showButtonPressed(const char* buttonName);
    static void showProcessing(const char* requestType);
    static void showSuccess(const char* message);
    static void showError(const char* errorMessage);
    
    // Utility methods
    static void clear();
    static void setBrightness(uint8_t brightness);
    static void displayText(const char* text, int x = 10, int y = 50, uint16_t color = TFT_WHITE);
    static void displayCenteredText(const char* text, int y = 120, uint16_t color = TFT_WHITE);
    
    // Test methods
    static void test();
    
private:
    static TFT_eSPI tft;
    static bool initialized;
    
    // Helper methods
    static void drawHeader(const char* title);
    static void drawStatus(const char* status, uint16_t color);
    static void setBacklight(uint8_t brightness);
};

#endif // DISPLAY_H