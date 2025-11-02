#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Restaurant display states
enum DisplayState {
  BOOT,
  READY_FOR_SERVICE,
  TAKING_ORDER,
  ORDER_CONFIRMED,
  BILL_READY,
  THANK_YOU
};

DisplayState currentState = BOOT;
DisplayState lastState = THANK_YOU;
unsigned long stateChangeTime = 0;
unsigned long bootStartTime = 0;
int tableNumber = 12;
int bootProgress = 0;

// FUNCTION DECLARATIONS
void showBootScreen();
void showReadyDisplay();
void showOrderDisplay();
void showConfirmDisplay();
void showBillDisplay();
void showThankYouDisplay();
void drawSimpleLogo();
void drawProgressBar(int progress);
void drawCheckmark(int x, int y, int size);
void drawXmark(int x, int y, int size);

void setup() {
  Serial.begin(9600);
  Serial.println("LUMI Cube - Clean B&W&Red Design");
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  stateChangeTime = millis();
  bootStartTime = millis();
}

void loop() {
  // Handle boot sequence first
  if(currentState == BOOT) {
    if(millis() - bootStartTime < 6000) {  // 6 second boot
      bootProgress = map(millis() - bootStartTime, 0, 6000, 0, 100);
      showBootScreen();
      delay(100);
      return;
    } else {
      currentState = READY_FOR_SERVICE;
    }
  }
  
  // Normal state cycling after boot
  if(millis() - stateChangeTime > 4000) {
    // Skip BOOT in normal cycling
    do {
      currentState = (DisplayState)((currentState + 1) % 6);
    } while(currentState == BOOT);
    
    stateChangeTime = millis();
    Serial.print("Display state: ");
    Serial.println(currentState);
  }
  
  // Only redraw when state changes
  if(currentState != lastState) {
    switch(currentState) {
      case READY_FOR_SERVICE:
        showReadyDisplay();
        break;
      case TAKING_ORDER:
        showOrderDisplay();
        break;
      case ORDER_CONFIRMED:
        showConfirmDisplay();
        break;
      case BILL_READY:
        showBillDisplay();
        break;
      case THANK_YOU:
        showThankYouDisplay();
        break;
    }
    lastState = currentState;
  }
  
  delay(200);
}

void showBootScreen() {
  static int lastProgress = -1;
  
  // Only redraw if progress changed
  if(bootProgress == lastProgress) return;
  lastProgress = bootProgress;
  
  tft.fillScreen(TFT_BLACK);
  
  // LUMI Logo
  drawSimpleLogo();
  
  // Boot text
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("LUMI CUBE", 120, 100);
  
  tft.setTextSize(1);
  tft.drawString("Restaurant Service System", 120, 130);
  
  // Progress bar
  drawProgressBar(bootProgress);
  
  // Boot status text
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  
  if(bootProgress < 20) {
    tft.drawString("Initializing hardware...", 120, 200);
  } else if(bootProgress < 40) {
    tft.drawString("Connecting to network...", 120, 200);
  } else if(bootProgress < 60) {
    tft.drawString("Loading restaurant data...", 120, 200);
  } else if(bootProgress < 80) {
    tft.drawString("Configuring table settings...", 120, 200);
  } else {
    tft.drawString("Ready for service!", 120, 200);
  }
  
  // Progress percentage
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString(String(bootProgress) + "%", 120, 220);
}

void showReadyDisplay() {
  tft.fillScreen(TFT_WHITE);
  
  // Header
  tft.fillRect(0, 0, 240, 50, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("LUMI", 120, 25);
  
  // Table number
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(4);
  tft.drawString("TABLE " + String(tableNumber), 120, 90);
  
  // Status
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("READY", 120, 140);
  
  // Instructions
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(1);
  tft.drawString("Touch screen or press button", 120, 180);
  tft.drawString("for service", 120, 200);
  
  // Simple service icons
  tft.fillRect(0, 230, 240, 50, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("SERVICE | ORDER | BILL | HELP", 120, 255);
}

void showOrderDisplay() {
  tft.fillScreen(TFT_BLACK);
  
  // Header
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("VOICE ORDER", 120, 40);
  
  // Microphone graphic (simple circles)
  tft.fillCircle(120, 100, 30, TFT_WHITE);
  tft.fillCircle(120, 100, 25, TFT_BLACK);
  tft.fillCircle(120, 100, 15, TFT_RED);
  
  // Sound waves
  tft.drawCircle(120, 100, 40, TFT_WHITE);
  tft.drawCircle(120, 100, 50, TFT_WHITE);
  tft.drawCircle(120, 100, 60, TFT_WHITE);
  
  // Status
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("LISTENING", 120, 170);
  
  // Sample text
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("Speak your order clearly", 120, 200);
  
  // Footer
  tft.fillRect(0, 230, 240, 50, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawString("RECORDING", 120, 255);
}

void showConfirmDisplay() {
  tft.fillScreen(TFT_WHITE);
  
  // Header
  tft.fillRect(0, 0, 240, 50, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("ORDER STATUS", 120, 25);
  
  // Large checkmark
  drawCheckmark(120, 110, 40);
  
  // Confirmation text
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("CONFIRMED", 120, 160);
  
  // Order details
  tft.setTextSize(1);
  tft.drawString("2x Caesar Salad", 120, 190);
  tft.drawString("1x Grilled Salmon", 120, 205);
  
  // Time estimate
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("15-20 MIN", 120, 230);
  
  // Footer
  tft.fillRect(0, 250, 240, 30, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("SENT TO KITCHEN", 120, 265);
}

void showBillDisplay() {
  tft.fillScreen(TFT_BLACK);
  
  // Header
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("PAYMENT", 120, 30);
  
  // Amount box
  tft.fillRect(20, 70, 200, 80, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(5);
  tft.drawString("$42.50", 120, 110);
  
  // Payment methods
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("PAYMENT OPTIONS", 120, 170);
  
  // Simple payment icons (rectangles)
  tft.fillRect(30, 190, 50, 30, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(1);
  tft.drawString("CARD", 55, 205);
  
  tft.fillRect(95, 190, 50, 30, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawString("CASH", 120, 205);
  
  tft.fillRect(160, 190, 50, 30, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawString("MOBILE", 185, 205);
  
  // Footer
  tft.fillRect(0, 250, 240, 30, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawString("TAP TO PAY", 120, 265);
}

void showThankYouDisplay() {
  tft.fillScreen(TFT_WHITE);
  
  // Header
  tft.fillRect(0, 0, 240, 50, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("THANK YOU", 120, 25);
  
  // Large text
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(4);
  tft.drawString("VISIT", 120, 80);
  tft.drawString("AGAIN", 120, 120);
  
  // Rating stars (simple rectangles)
  int starY = 170;
  for(int i = 0; i < 5; i++) {
    tft.fillRect(60 + i*24, starY, 20, 20, TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.setTextSize(1);
    tft.drawString("*", 70 + i*24, starY + 10);
  }
  
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(1);
  tft.drawString("Rate your experience", 120, 210);
  
  // Footer
  tft.fillRect(0, 240, 240, 40, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("TABLE " + String(tableNumber) + " SESSION COMPLETE", 120, 260);
}

void drawSimpleLogo() {
  // Simple LUMI logo using geometric shapes
  
  // L
  tft.fillRect(80, 50, 5, 25, TFT_RED);
  tft.fillRect(80, 70, 15, 5, TFT_RED);
  
  // U
  tft.fillRect(100, 50, 5, 20, TFT_WHITE);
  tft.fillRect(110, 50, 5, 20, TFT_WHITE);
  tft.fillRect(100, 70, 15, 5, TFT_WHITE);
  
  // M
  tft.fillRect(120, 50, 5, 25, TFT_RED);
  tft.fillRect(135, 50, 5, 25, TFT_RED);
  tft.fillRect(125, 55, 5, 10, TFT_RED);
  
  // I
  tft.fillRect(145, 50, 5, 25, TFT_WHITE);
}

void drawProgressBar(int progress) {
  // Progress bar background
  tft.fillRect(40, 160, 160, 10, TFT_WHITE);
  
  // Progress fill
  int fillWidth = map(progress, 0, 100, 0, 160);
  tft.fillRect(40, 160, fillWidth, 10, TFT_RED);
}

void drawCheckmark(int x, int y, int size) {
  // Simple checkmark using lines
  tft.fillCircle(x, y, size, TFT_BLACK);
  tft.fillCircle(x, y, size-3, TFT_WHITE);
  
  // Checkmark lines
  for(int i = 0; i < 3; i++) {
    tft.drawLine(x-15+i, y+i, x-5+i, y+10+i, TFT_BLACK);
    tft.drawLine(x-5+i, y+10+i, x+15+i, y-10+i, TFT_BLACK);
  }
}

void drawXmark(int x, int y, int size) {
  tft.fillCircle(x, y, size, TFT_RED);
  tft.fillCircle(x, y, size-3, TFT_WHITE);
  
  // X mark lines
  for(int i = 0; i < 3; i++) {
    tft.drawLine(x-10+i, y-10+i, x+10+i, y+10+i, TFT_RED);
    tft.drawLine(x-10+i, y+10+i, x+10+i, y-10+i, TFT_RED);
  }
}