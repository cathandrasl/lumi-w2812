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
        Serial.printf("[DISPLAY] Initialized %dx%d ST7789V display\n", TFT_WIDTH, TFT_HEIGHT);
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
    
    // Simple boot screen
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(4);
    displayCenteredText("LUMI", 80);
    
    delay(2000);
    showReady();
}

void Display::showReady() {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.println("[DISPLAY] Showing ready screen");
    }
    
    clear();
    
    // Table number
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    displayText("TABLE", 20, 30);
    
    tft.setTextSize(4);
    char tableStr[10];
    sprintf(tableStr, "%d", TABLE_NUMBER);
    displayText(tableStr, 140, 20);
    
    // Reservation status
    tft.setTextSize(2);
    if (RESERVATION_STATUS) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        displayCenteredText("RESERVED", 100);
    } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        displayCenteredText("AVAILABLE", 100);
    }
    
    // Ready status
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    displayCenteredText("READY", 160);
}

void Display::showWiFiStatus(const char* status) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] WiFi status: %s\n", status);
    }
    
    // Update status line at bottom
    tft.fillRect(0, 260, 240, 20, TFT_BLACK);  // Clear status area
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(1);
    displayText(status, 10, 265);
}

void Display::showBatteryLevel(int level) {
    if (!initialized) return;
    
    // Draw battery icon in top right
    int x = 200, y = 10;
    tft.drawRect(x, y, 30, 15, TFT_WHITE);
    tft.drawRect(x + 30, y + 3, 3, 9, TFT_WHITE);
    
    // Fill battery based on level
    uint16_t color = (level > 50) ? TFT_GREEN : (level > 20) ? TFT_YELLOW : TFT_RED;
    int fillWidth = (level * 28) / 100;
    tft.fillRect(x + 1, y + 1, fillWidth, 13, color);
}

void Display::showButtonPressed(const char* buttonName) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Button pressed: %s\n", buttonName);
    }
    
    clear();
    
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(4);
    displayCenteredText(buttonName, 80);
}

void Display::showProcessing(const char* requestType) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Processing: %s\n", requestType);
    }
    
    clear();
    
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    displayCenteredText("REQUESTING", 80);
}

void Display::showSuccess(const char* message) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Success: %s\n", message);
    }
    
    clear();
    
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    displayCenteredText("RECEIVED", 80);
}

void Display::showError(const char* errorMessage) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Error: %s\n", errorMessage);
    }
    
    clear();
    
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(2);
    displayCenteredText("PLEASE TRY", 70);
    displayCenteredText("AGAIN", 110);
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
    
    // Calculate text width for centering (landscape: 280 width)
    int textWidth = tft.textWidth(text);
    int x = (280 - textWidth) / 2;  // Center on 280px width
    
    tft.setCursor(x, y);
    tft.print(text);
}

void Display::drawHeader(const char* title) {
    if (!initialized) return;
    
    // Draw header bar
    tft.fillRect(0, 0, TFT_WIDTH, 30, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
    tft.setTextSize(2);
    
    // Center title in header
    int textWidth = tft.textWidth(title);
    int x = (TFT_WIDTH - textWidth) / 2;
    tft.setCursor(x, 8);
    tft.print(title);
}

void Display::drawStatus(const char* status, uint16_t color) {
    if (!initialized) return;
    
    // Draw status at bottom
    tft.fillRect(0, TFT_HEIGHT - 20, TFT_WIDTH, 20, TFT_BLACK);
    tft.setTextColor(color, TFT_BLACK);
    tft.setTextSize(1);
    displayCenteredText(status, TFT_HEIGHT - 15);
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