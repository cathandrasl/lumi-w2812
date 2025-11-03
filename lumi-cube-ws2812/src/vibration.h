#ifndef VIBRATION_H
#define VIBRATION_H

#include <Arduino.h>
#include "config.h"

class Vibration {
public:
    // Initialize vibration motor
    static void init();
    
    // Basic vibration control
    static void pulse(int duration = VIBRATION_DURATION);
    static void stop();
    
    // Button feedback patterns
    static void billFeedback();     // Single pulse
    static void menuFeedback();     // Double pulse
    static void serviceFeedback();  // Triple pulse
    
    // System feedback patterns
    static void bootFeedback();     // Welcome pulse
    static void successFeedback();  // Short confirmation
    static void errorFeedback();    // Rapid pulses
    
    // Test function
    static void test();
    
    // Status check
    static bool isActive();

private:
    static bool initialized;
    static unsigned long pulseStartTime;
    static bool currentlyVibrating;
    
    // Helper methods
    static void startPulse(int duration);
    static void pattern(int pulses, int onTime, int offTime);
};

#endif // VIBRATION_H