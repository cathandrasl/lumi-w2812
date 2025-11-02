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
String tableStatus = "READY";

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
  Serial.println("LUMI Cube - Landscape Display");
  
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
  
  // Large LUMI logo - centered in landscape
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(6);
  tft.drawString("LUMI", 140, 80);  // Centered at 280/2 = 140
  
  // Progress bar - wider for landscape
  drawProgressBar(bootProgress);
  
  // Large percentage
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(4);
  tft.drawString(String(bootProgress) + "%", 140, 180);
}

void showDefaultDisplay() {
  tft.fillScreen(TFT_WHITE);
  
  // Left side - LUMI and time
  tft.fillRect(0, 0, 140, 240, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("LUMI", 70, 60);
  
  // Large time
  tft.setTextSize(3);
  tft.drawString(formatTime(), 70, 120);
  
  // Status
  tft.setTextSize(2);
  tft.drawString(tableStatus, 70, 180);
  
  // Right side - Table info
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("TABLE", 210, 60);
  
  // Huge table number
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextSize(8);
  tft.drawString(String(tableNumber), 210, 120);
  
  // Instructions at bottom
  tft.fillRect(0, 200, 280, 40, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(2);
  tft.drawString("PRESS BUTTON FOR SERVICE", 140, 220);
}

void showQRMenu() {
  tft.fillScreen(TFT_WHITE);
  
  // Left side - QR Code
  String menuURL = "https://lumi.restaurant/menu/table" + String(tableNumber);
  drawQRCode(menuURL, 70, 120, 4);
  
  // Right side - Info
  tft.fillRect(140, 0, 140, 240, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("MENU", 210, 50);
  
  tft.setTextSize(3);
  tft.drawString("SCAN", 210, 100);
  tft.drawString("WITH", 210, 130);
  tft.drawString("PHONE", 210, 160);
  
  // Table number
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("TABLE " + String(tableNumber), 210, 200);
}

void showQRPayment() {
  tft.fillScreen(TFT_BLACK);
  
  // Left side - Amount and QR
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("PAY", 70, 30);
  
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(4);
  tft.drawString("$42.50", 70, 60);
  
  // QR Code
  String paymentURL = "https://pay.lumi.restaurant/table" + String(tableNumber) + "/amount/4250";
  drawQRCode(paymentURL, 70, 150, 3);
  
  // Right side - Instructions
  tft.fillRect(140, 0, 140, 240, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("SCAN", 210, 60);
  tft.drawString("TO", 210, 90);
  tft.drawString("PAY", 210, 120);
  
  tft.setTextSize(2);
  tft.drawString("SECURE", 210, 160);
  tft.drawString("PAYMENT", 210, 180);
}

void showServiceScreen() {
  tft.fillScreen(TFT_BLACK);
  
  // Left side - Service symbol
  tft.fillCircle(70, 120, 60, TFT_WHITE);
  tft.fillCircle(70, 120, 55, TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(8);
  tft.drawString("S", 70, 120);
  
  // Right side - Message
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(4);
  tft.drawString("SERVICE", 210, 80);
  
  tft.setTextSize(3);
  tft.drawString("WAITER", 210, 120);
  tft.drawString("COMING", 210, 150);
  
  // Table number
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("TABLE " + String(tableNumber), 210, 190);
}

void showBillScreen() {
  tft.fillScreen(TFT_WHITE);
  
  // Left side - Amount
  tft.fillRect(0, 0, 140, 240, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("BILL", 70, 50);
  
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(5);
  tft.drawString("$42.50", 70, 120);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("TABLE " + String(tableNumber), 70, 180);
  
  // Right side - QR Code
  String billURL = "https://pay.lumi.restaurant/bill/table" + String(tableNumber);
  drawQRCode(billURL, 210, 80, 3);
  
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("SCAN TO", 210, 140);
  tft.drawString("VIEW & PAY", 210, 160);
  
  // Footer
  tft.fillRect(140, 200, 140, 40, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawString("FAST PAYMENT", 210, 220);
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
  
  // Draw QR code
  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      uint16_t color = qrcode_getModule(&qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      tft.fillRect(startX + x * scale, startY + y * scale, scale, scale, color);
    }
  }
  
  Serial.println("Landscape QR Code generated for: " + data);
}

void drawProgressBar(int progress) {
  // Wider progress bar for landscape
  tft.fillRect(40, 140, 200, 20, TFT_WHITE);
  int fillWidth = map(progress, 0, 100, 0, 200);
  tft.fillRect(40, 140, fillWidth, 20, TFT_RED);
  tft.drawRect(40, 140, 200, 20, TFT_BLACK);
}

String formatTime() {
  String hour = String(currentHour);
  String minute = String(currentMinute);
  
  if(currentHour < 10) hour = "0" + hour;
  if(currentMinute < 10) minute = "0" + minute;
  
  return hour + ":" + minute;
}