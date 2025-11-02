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
  Serial.println("LUMI Cube - Large Font Display");
  
  tft.init();
  tft.setRotation(0);
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
  
  // Demo: cycle through states every 6 seconds (longer for readability)
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
  
  // Large LUMI logo
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(6);  // Very large
  tft.drawString("LUMI", 120, 80);
  
  // Progress bar
  drawProgressBar(bootProgress);
  
  // Large percentage
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(4);
  tft.drawString(String(bootProgress) + "%", 120, 220);
}

void showDefaultDisplay() {
  tft.fillScreen(TFT_WHITE);
  
  // Large LUMI header
  tft.fillRect(0, 0, 240, 60, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("LUMI", 120, 30);
  
  // Large time display
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(5);
  tft.drawString(formatTime(), 120, 80);
  
  // Large table number
  tft.setTextSize(6);
  tft.drawString("TABLE", 120, 130);
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.setTextSize(8);
  tft.drawString(String(tableNumber), 120, 170);
  
  // Footer
  tft.fillRect(0, 220, 240, 60, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("PRESS BUTTON", 120, 240);
  tft.drawString("FOR SERVICE", 120, 260);
}

void showQRMenu() {
  tft.fillScreen(TFT_WHITE);
  
  // Large header
  tft.fillRect(0, 0, 240, 50, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("MENU", 120, 25);
  
  // QR Code for menu - larger
  String menuURL = "https://lumi.restaurant/menu/table" + String(tableNumber);
  drawQRCode(menuURL, 120, 120, 4);  // Bigger QR code
  
  // Large instructions
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("SCAN", 120, 180);
  tft.drawString("WITH PHONE", 120, 200);
  
  // Footer
  tft.fillRect(0, 230, 240, 50, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(2);
  tft.drawString("TABLE " + String(tableNumber), 120, 255);
}

void showQRPayment() {
  tft.fillScreen(TFT_BLACK);
  
  // Large payment header
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("PAY", 120, 25);
  
  // Large amount
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(6);
  tft.drawString("$42.50", 120, 60);
  
  // Large QR Code for payment
  String paymentURL = "https://pay.lumi.restaurant/table" + String(tableNumber) + "/amount/4250";
  drawQRCode(paymentURL, 120, 140, 4);
  
  // Large scan instruction
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.drawString("SCAN TO PAY", 120, 200);
  
  // Footer
  tft.fillRect(0, 230, 240, 50, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(2);
  tft.drawString("SECURE PAYMENT", 120, 255);
}

void showServiceScreen() {
  tft.fillScreen(TFT_BLACK);
  
  // Large service header
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("SERVICE", 120, 40);
  
  // Large service indicator
  tft.fillCircle(120, 110, 50, TFT_WHITE);
  tft.fillCircle(120, 110, 45, TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(6);
  tft.drawString("S", 120, 110);
  
  // Large status message
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.drawString("WAITER", 120, 170);
  tft.drawString("COMING", 120, 200);
  
  // Footer
  tft.fillRect(0, 230, 240, 50, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(2);
  tft.drawString("TABLE " + String(tableNumber), 120, 255);
}

void showBillScreen() {
  tft.fillScreen(TFT_WHITE);
  
  // Large bill header
  tft.fillRect(0, 0, 240, 50, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("BILL", 120, 25);
  
  // Large amount
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(6);
  tft.drawString("$42.50", 120, 70);
  
  // Large QR Code for bill
  String billURL = "https://pay.lumi.restaurant/bill/table" + String(tableNumber);
  drawQRCode(billURL, 120, 140, 4);
  
  // Large instruction
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("SCAN TO", 120, 190);
  tft.drawString("VIEW & PAY", 120, 210);
  
  // Footer
  tft.fillRect(0, 240, 240, 40, TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.setTextSize(2);
  tft.drawString("TABLE " + String(tableNumber), 120, 260);
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
  
  // Draw QR code with larger pixels
  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      uint16_t color = qrcode_getModule(&qrcode, x, y) ? TFT_BLACK : TFT_WHITE;
      tft.fillRect(startX + x * scale, startY + y * scale, scale, scale, color);
    }
  }
  
  Serial.println("Large QR Code generated for: " + data);
}

void drawProgressBar(int progress) {
  // Larger progress bar
  tft.fillRect(20, 160, 200, 25, TFT_WHITE);
  int fillWidth = map(progress, 0, 100, 0, 200);
  tft.fillRect(20, 160, fillWidth, 25, TFT_RED);
  tft.drawRect(20, 160, 200, 25, TFT_BLACK);
}

String formatTime() {
  String hour = String(currentHour);
  String minute = String(currentMinute);
  
  if(currentHour < 10) hour = "0" + hour;
  if(currentMinute < 10) minute = "0" + minute;
  
  return hour + ":" + minute;
}