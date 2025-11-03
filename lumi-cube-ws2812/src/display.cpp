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
        Serial.printf("[DISPLAY] Initialized ST7789V display\n");
    }
    
    // Show boot screen
    showBootScreen();
}

void Display::showBootScreen() {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.println("[DISPLAY] Showing boot screen");
    }
    
    tft.fillScreen(TFT_BLACK);
    
    // LUMI text centered
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(8);
    tft.drawString("LUMI", 140, 120);
    
    delay(2000);
    showReady();
}

void Display::showReady() {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.println("[DISPLAY] Showing ready screen");
    }
    
    tft.fillScreen(TFT_BLACK);
    
    // Top bar
    tft.fillRect(0, 0, 280, 60, TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(5);
    tft.drawString("TABLE", 140, 30);
    
    // Big table number
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(8);
    char tableStr[10];
    sprintf(tableStr, "%d", TABLE_NUMBER);
    tft.drawString(tableStr, 140, 120);
    
    // Reservation status
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    if (RESERVATION_STATUS) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("RESERVED", 140, 170);
    } else {
        tft.drawString("AVAILABLE", 140, 170);
    }
    
    // Bottom bar
    tft.fillRect(0, 210, 280, 30, TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("READY", 140, 225);
}

void Display::showWiFiStatus(const char* status) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] WiFi status: %s\n", status);
    }
    
    // Update status line at bottom
    tft.fillRect(0, 220, 280, 20, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(ML_DATUM);
    tft.setTextSize(1);
    tft.drawString(status, 10, 230);
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
    
    tft.fillScreen(TFT_BLACK);
    
    // Large button name centered
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(6);
    tft.drawString(buttonName, 140, 120);
}

void Display::showProcessing(const char* requestType) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Processing: %s\n", requestType);
    }
    
    tft.fillScreen(TFT_BLACK);
    
    // Processing message centered
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(4);
    tft.drawString("REQUESTING", 140, 120);
}

void Display::showSuccess(const char* message) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Success: %s\n", message);
    }
    
    tft.fillScreen(TFT_BLACK);
    
    // Success message centered
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(4);
    tft.drawString("RECEIVED", 140, 120);
}

void Display::showError(const char* errorMessage) {
    if (!initialized) return;
    
    if (DEBUG_DISPLAY) {
        Serial.printf("[DISPLAY] Error: %s\n", errorMessage);
    }
    
    tft.fillScreen(TFT_BLACK);
    
    // Error message in red
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(3);
    tft.drawString("PLEASE TRY", 140, 100);
    tft.drawString("AGAIN", 140, 140);
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
    tft.setTextDatum(TL_DATUM);
    tft.drawString(text, x, y);
}

void Display::displayCenteredText(const char* text, int y, uint16_t color) {
    if (!initialized) return;
    
    tft.setTextColor(color, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(text, 140, y);  // Center at x=140 for 280px width
}

void Display::drawHeader(const char* title) {
    if (!initialized) return;
    
    // Draw header bar
    tft.fillRect(0, 0, 280, 30, TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(2);
    tft.drawString(title, 140, 15);
}

void Display::drawStatus(const char* status, uint16_t color) {
    if (!initialized) return;
    
    // Draw status at bottom
    tft.fillRect(0, 220, 280, 20, TFT_BLACK);
    tft.setTextColor(color, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);
    tft.drawString(status, 140, 230);
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