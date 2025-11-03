#include "display.h"

// Static variable definitions
TFT_eSPI Display::tft = TFT_eSPI();
bool Display::initialized = false;

void Display::init() {
    tft.init();
    tft.setRotation(DISPLAY_ROTATION);
    tft.fillScreen(TFT_BLACK);
    
    // Set backlight
    pinMode(TFT_BL, OUTPUT);
    setBacklight(BACKLIGHT_PWM);
    
    initialized = true;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Initialized %dx%d ST7789V display\n", 280, 240);
    }
    
    // Show boot screen
    showBootScreen();
}

void Display::showBootScreen() {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.println("[DISPLAY] Showing boot screen");
    }
    
    clear();
    
    // Simple boot screen - red/black/white only
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(6);
    displayCenteredText("LUMI", 120);
    
    delay(2000);
    showReady();
}

void Display::showReady() {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.println("[DISPLAY] Showing ready screen");
    }
    
    clear();  // Black background
    
    // Left side: Big table number
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(10);  // Very large
    char tableStr[10];
    sprintf(tableStr, "%d", TABLE_NUMBER);
    displayText(tableStr, 20, 60);
    
    // Right side: Status info
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    displayText("TABLE", 180, 50);
    
    // Reservation status
    tft.setTextSize(3);
    if (RESERVATION_STATUS) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        displayText("RESERVED", 180, 80);
    } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        displayText("AVAILABLE", 180, 80);
    }
    
    // Ready status at bottom
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    displayCenteredText("READY", 200);
}

void Display::showWiFiStatus(const char* status) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] WiFi status: %s\n", status);
    }
    
    // Update status line at bottom
    tft.fillRect(0, 220, 280, 20, TFT_BLACK);  // Clear status area
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    displayText(status, 10, 225);
}

void Display::showBatteryLevel(int level) {
    if (!initialized) return;
    
    // Draw battery icon in top right
    int x = 240, y = 10;
    tft.drawRect(x, y, 30, 15, TFT_WHITE);
    tft.drawRect(x + 30, y + 3, 3, 9, TFT_WHITE);
    
    // Fill battery based on level
    uint16_t color = (level > 50) ? TFT_WHITE : (level > 20) ? TFT_WHITE : TFT_RED;
    int fillWidth = (level * 28) / 100;
    tft.fillRect(x + 1, y + 1, fillWidth, 13, color);
}

void Display::showButtonPressed(const char* buttonName) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Button pressed: %s\n", buttonName);
    }
    
    clear();  // Black background
    
    // Large button name centered in landscape
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(6);
    displayCenteredText(buttonName, 100);
}

void Display::showProcessing(const char* requestType) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Processing: %s\n", requestType);
    }
    
    clear();  // Black background
    
    // Processing message centered in landscape
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(4);
    displayCenteredText("REQUESTING", 100);
}

void Display::showSuccess(const char* message) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Success: %s\n", message);
    }
    
    clear();  // Black background
    
    // Success message centered in landscape
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(4);
    displayCenteredText("RECEIVED", 100);
}

void Display::showError(const char* errorMessage) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Error: %s\n", errorMessage);
    }
    
    clear();  // Black background
    
    // Error message in red
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(3);
    displayCenteredText("PLEASE TRY", 90);
    // displayCenteredText("AGAIN", 130);
}

void Display::clear() {
    if (initialized) {
        tft.fillScreen(TFT_BLACK);
    }
}

void Display::setBrightness(uint8_t brightness) {
    setBacklight(brightness);
}

void Display::displayText(const char* text, int x, int y, uint16_t color) {
    if (!initialized) return;
    
    tft.setTextColor(color, TFT_BLACK);
    tft.setCursor(x, y);
    tft.print(text);
}

void Display::displayCenteredText(const char* text, int y, uint16_t color) {
    if (!initialized) return;
    
    tft.setTextColor(color, TFT_BLACK);
    
    // Calculate text width for centering (landscape: 280px width)
    int textWidth = tft.textWidth(text);
    int x = (280 - textWidth) / 2;  // Center on 280px width
    
    tft.setCursor(x, y);
    tft.print(text);
}

void Display::drawHeader(const char* title) {
    if (!initialized) return;
    
    // Draw header bar
    tft.fillRect(0, 0, 280, 30, TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(2);
    
    // Center title in header
    int textWidth = tft.textWidth(title);
    int x = (280 - textWidth) / 2;
    tft.setCursor(x, 8);
    tft.print(title);
}

void Display::drawStatus(const char* status, uint16_t color) {
    if (!initialized) return;
    
    // Draw status at bottom
    tft.fillRect(0, 220, 280, 20, TFT_BLACK);
    tft.setTextColor(color, TFT_BLACK);
    tft.setTextSize(1);
    displayCenteredText(status, 230);
}

void Display::setBacklight(uint8_t brightness) {
    analogWrite(TFT_BL, brightness);
}

void Display::test() {
    if (!initialized) {
        Serial.println("[DISPLAY] Not initialized!");
        return;
    }
    
    Serial.println("[DISPLAY] Starting test sequence...");
    
    Serial.println("  1. Boot screen");
    showBootScreen();
    delay(2000);
    
    Serial.println("  2. Ready screen");
    showReady();
    delay(2000);
    
    Serial.println("  3. Button pressed");
    showButtonPressed("BILL");
    delay(2000);
    
    Serial.println("  4. Processing");
    showProcessing("BILL REQUEST");
    delay(3000);
    
    Serial.println("  5. Success");
    showSuccess("Bill request sent");
    delay(2000);
    
    Serial.println("  6. Error test");
    showError("Network error");
    delay(2000);
    
    Serial.println("  7. Return to ready");
    showReady();
    
    Serial.println("[DISPLAY] Test complete!");
}