#include <Arduino.h>
#include <TFT_eSPI.h>
#include "qrcode.h"

TFT_eSPI tft = TFT_eSPI();

// Restaurant display states
enum DisplayState {
  BOOT,
  DEFAULT_DISPLAY,
  QR_MENU,
  QR_PAYMENT,
  SERVICE_REQUESTED,
  BILL_REQUESTED
};

DisplayState currentState = BOOT;
DisplayState lastState = BILL_REQUESTED;
unsigned long stateChangeTime = 0;
unsigned long bootStartTime = 0;
int bootProgress = 0;
int tableNumber = 12;

// Simulated time
int currentHour = 14;
int currentMinute = 32;

// FUNCTION DECLARATIONS
void showBootScreen();
void showDefaultDisplay();
void showQRMenu();
void showQRPayment();
void showServiceScreen();
void showBillScreen();
void drawQRCode(String data, int x, int y, int scale);
void drawProgressBar(int progress);
String formatTime();

void setup() {
  Serial.begin(9600);
  Serial.println("LUMI Cube - Minimal Design");
  
  tft.init();
  tft.setRotation(1);  // Landscape mode (280x240)
  tft.fillScreen(TFT_BLACK);
  
  stateChangeTime = millis();
  bootStartTime = millis();
}

void loop() {
  // Handle boot sequence
  if(currentState == BOOT) {
    if(millis() - bootStartTime < 4000) {
      bootProgress = map(millis() - bootStartTime, 0, 4000, 0, 100);
      showBootScreen();
      delay(100);
      return;
    } else {
      currentState = DEFAULT_DISPLAY;
    }
  }
  
  // Demo: cycle through states every 6 seconds
  if(millis() - stateChangeTime > 6000) {
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
      case DEFAULT_DISPLAY:
        showDefaultDisplay();
        break;
      case QR_MENU:
        showQRMenu();
        break;
      case QR_PAYMENT:
        showQRPayment();
        break;
      case SERVICE_REQUESTED:
        showServiceScreen();
        break;
      case BILL_REQUESTED:
        showBillScreen();
        break;
    }
    lastState = currentState;
  }
  
  delay(200);
}

void showBootScreen() {
  static int lastProgress = -1;
  
  if(bootProgress == lastProgress) return;
  lastProgress = bootProgress;
  
  tft.fillScreen(TFT_BLACK);
  
  // Just LUMI
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(8);
  tft.drawString("LUMI", 140, 80);
  
  // Progress bar
  drawProgressBar(bootProgress);
  
  // Percentage only
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(5);
  tft.drawString(String(bootProgress) + "%", 140, 180);
}

void showDefaultDisplay() {
  tft.fillScreen(TFT_WHITE);
  
  // Top bar
  tft.fillRect(0, 0, 280, 60, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(5);
  tft.drawString("LUMI", 140, 30);
  
  // Time
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(6);
  tft.drawString(formatTime(), 140, 100);
  
  // Table number only
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextSize(8);
  tft.drawString(String(tableNumber), 140, 160);
  
  // Bottom bar
  tft.fillRect(0, 210, 280, 30, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("PRESS BUTTON", 140, 225);
}

void showQRMenu() {
  tft.fillScreen(TFT_WHITE);
  
  // Header
  tft.fillRect(0, 0, 280, 50, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(5);
  tft.drawString("MENU", 140, 25);
  
  // QR code only
  String menuURL = "https://lumi.restaurant/menu/table" + String(tableNumber);
  drawQRCode(menuURL, 140, 130, 4);
  
  // Bottom
  tft.fillRect(0, 210, 280, 30, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(3);
  tft.drawString(String(tableNumber), 140, 225);
}

void showQRPayment() {
  tft.fillScreen(TFT_BLACK);
  
  // Amount only
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(7);
  tft.drawString("$42.50", 140, 40);
  
  // QR code
  String paymentURL = "https://pay.lumi.restaurant/table" + String(tableNumber) + "/amount/4250";
  drawQRCode(paymentURL, 140, 130, 4);
  
  // Bottom
  tft.fillRect(0, 210, 280, 30, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(3);
  tft.drawString("PAY", 140, 225);
}

void showServiceScreen() {
  tft.fillScreen(TFT_BLACK);
  
  // Large S symbol
  tft.fillRect(90, 50, 100, 100, TFT_WHITE);
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(8);
  tft.drawString("S", 140, 100);
  
  // Status
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(5);
  tft.drawString("WAITER", 140, 170);
  
  // Bottom
  tft.fillRect(0, 210, 280, 30, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(3);
  tft.drawString(String(tableNumber), 140, 225);
}

void showBillScreen() {
  tft.fillScreen(TFT_WHITE);
  
  // Header
  tft.fillRect(0, 0, 280, 50, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(5);
  tft.drawString("BILL", 140, 25);
  
  // Amount
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextSize(6);
  tft.drawString("$42.50", 140, 80);
  
  // QR code
  String billURL = "https://pay.lumi.restaurant/bill/table" + String(tableNumber);
  drawQRCode(billURL, 140, 150, 3);
  
  // Bottom
  tft.fillRect(0, 210, 280, 30, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(3);
  tft.drawString(String(tableNumber), 140, 225);
}

void drawQRCode(String data, int x, int y, int scale) {
  // Create QR code
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, data.c_str());
  
  // Calculate position to center the QR code
  int qrSize = qrcode.size * scale;
  int startX = x - (qrSize / 2);
  int startY = y - (qrSize / 2);
  
  // Draw QR code - no rounded corners
  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      uint16_t color = qrcode_getModule(&qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      tft.fillRect(startX + x * scale, startY + y * scale, scale, scale, color);
    }
  }
  
  Serial.println("Minimal QR Code: " + data);
}

void drawProgressBar(int progress) {
  // Simple rectangle progress - no rounded corners
  tft.fillRect(40, 140, 200, 20, TFT_WHITE);
  int fillWidth = map(progress, 0, 100, 0, 200);
  tft.fillRect(40, 140, fillWidth, 20, TFT_RED);
  tft.drawRect(40, 140, 200, 20, TFT_WHITE);
}

String formatTime() {
  String hour = String(currentHour);
  String minute = String(currentMinute);
  
  if(currentHour < 10) hour = "0" + hour;
  if(currentMinute < 10) minute = "0" + minute;
  
  return hour + ":" + minute;
}