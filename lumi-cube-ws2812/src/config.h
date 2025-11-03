#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// LUMI CUBE MVP - CONFIGURATION
// ============================================================================

// Button Pin Definitions
#define BUTTON_BILL_PIN     18    // Button 1: Bill request
#define BUTTON_MENU_PIN     19    // Button 2: Menu request  
#define BUTTON_SERVICE_PIN  21    // Button 3: Service request

// Display Pin Definitions (ST7789V - 1.69" 240x280)
#define TFT_MOSI    13
#define TFT_SCLK    14
#define TFT_CS      15
#define TFT_DC      27
#define TFT_RST     12
#define TFT_BL      32   // Backlight

// LED Strip Pin Definition (WS2812)
#define LED_STRIP_PIN       2
#define LED_COUNT           7

// LED Assignments (0-based indexing)
#define LED_POWER           0    // LED 1: Power status
#define LED_WIFI            1    // LED 2: WiFi status  
#define LED_BATTERY         2    // LED 3: Battery status (future)
#define LED_RESERVED_1      3    // LED 4: Reserved

// Button Feedback LED Cluster (LEDs 5-7 operate as a group)
#define LED_BUTTON_START    4    // LED 5: Start of button cluster
#define LED_BUTTON_COUNT    3    // LEDs 5,6,7 for button feedback
#define LED_BILL            4    // LED 5: Bill button feedback
#define LED_MENU            5    // LED 6: Menu button feedback  
#define LED_SERVICE         6    // LED 7: Service button feedback

// LED Clusters
#define BUTTON_LED_START    4    // LEDs 5-7 (indices 4-6) work as cluster
#define BUTTON_LED_COUNT    3    // 3 LEDs in button cluster

// Vibration Motor Pin Definition
#define VIBRATION_PIN       4

// LED Colors (RGB values for WS2812)
#define COLOR_WHITE         0xFFFFFF
#define COLOR_GREEN         0x00FF00
#define COLOR_ORANGE        0xFF8000
#define COLOR_RED           0xFF0000
#define COLOR_BLUE          0x0000FF
#define COLOR_OFF           0x000000

// System Settings
#define SERIAL_BAUD         9600
#define BUTTON_DEBOUNCE_MS  50
#define LED_BRIGHTNESS      50      // 0-255
#define VIBRATION_DURATION  300     // milliseconds (increased from 200)

// Vibration Settings
#define VIBRATION_STRONG    400     // Strong pulse duration
#define VIBRATION_MEDIUM    400     // Medium pulse duration  
#define VIBRATION_SHORT     400     // Short pulse duration
#define VIBRATION_GAP       300     // Gap between pulses

// WiFi Settings (update with your credentials)
#define WIFI_SSID           "YOUR_WIFI_SSID"
#define WIFI_PASSWORD       "YOUR_WIFI_PASSWORD"
#define API_ENDPOINT        "http://192.168.1.100:8000/api/requests"

// Display Settings
#define DISPLAY_ROTATION    0       // 0, 1, 2, or 3
#define BACKLIGHT_PWM       200     // 0-255

// Debug Settings
#define DEBUG_ENABLED       true
#define DEBUG_BUTTONS       true
#define DEBUG_LEDS          true
#define DEBUG_DISPLAY       true
#define DEBUG_NETWORK       true

#endif // CONFIG_H