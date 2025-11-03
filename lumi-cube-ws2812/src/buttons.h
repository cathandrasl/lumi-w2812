#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "config.h"

class Buttons {
public:
    // Initialize all buttons
    static void init();
    
    // Check button states (call in main loop)
    static void update();
    
    // Check individual button states
    static bool isBillPressed();
    static bool isMenuPressed(); 
    static bool isServicePressed();
    
    // Test function
    static void test();

private:
    static unsigned long lastBillPress;
    static unsigned long lastMenuPress;
    static unsigned long lastServicePress;
    
    static bool checkButton(int pin, unsigned long &lastPress);
};

#endif // BUTTONS_H