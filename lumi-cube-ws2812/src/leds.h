#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

class LEDs {
public:
    // Initialize LED strip
    static void init();
    
    // Boot sequence - all LEDs white
    static void bootSequence();
    
    // Individual LED control
    static void setPowerLED(uint32_t color);
    static void setWiFiLED(uint32_t color);
    static void setBatteryLED(uint32_t color);
    static void setButtonLED(int buttonIndex, uint32_t color);  // 0=Bill, 1=Menu, 2=Service
    
    // Button cluster control (LEDs 5-7)
    static void setButtonCluster(uint32_t color);
    
    // Convenience status methods
    static void setPowerStatus(bool on);        // Green=on, Red=off
    static void setWiFiStatus(int status);      // 0=off, 1=connecting(orange), 2=connected(green), 3=error(red)
    static void setBatteryStatus(int level);    // 0=red, 1=orange, 2=green
    
    // Button feedback
    static void billPressed();
    static void menuPressed();
    static void servicePressed();
    static void clearButtonFeedback();
    static void showRequestError();  // Red error feedback
    
    // Utility methods
    static void setAll(uint32_t color);
    static void clear();
    static void show();
    static void setBrightness(uint8_t brightness);
    
    // Test methods
    static void testSequence();
    static void rainbowCycle(int cycles = 1);

private:
    static Adafruit_NeoPixel strip;
    static bool initialized;
    
    // Helper methods
    static uint32_t wheel(byte wheelPos);
    static void setLED(int index, uint32_t color);
    static void flowingBlueEffect();  // 5-second flowing blue animation
};

#endif // LEDS_H