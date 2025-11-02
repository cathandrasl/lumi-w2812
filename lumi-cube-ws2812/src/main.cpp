#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// Restaurant display states
enum DisplayState {
  WELCOME,
  READY_FOR_SERVICE,
  TAKING_ORDER,
  ORDER_CONFIRMED,
  BILL_READY,
  THANK_YOU
};

DisplayState currentState = WELCOME;
unsigned long stateChangeTime = 0;
int tableNumber = 12;
String restaurantName = "LUMI BISTRO";

// FUNCTION DECLARATIONS (THIS FIXES ALL ERRORS!)
void showWelcomeDisplay();
void showReadyDisplay();
void showOrderDisplay();
void showConfirmDisplay();
void showBillDisplay();
void showThankYouDisplay();
void drawHeader();
void drawStatusBar(String status, uint16_t color);

void setup() {
  Serial.begin(9600);
  Serial.println("Restaurant Cube Display Demo");
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  stateChangeTime = millis();
}

void loop() {
  // Auto-cycle through different display states every 4 seconds
  if(millis() - stateChangeTime > 4000) {
    currentState = (DisplayState)((currentState + 1) % 6);
    stateChangeTime = millis();
    Serial.print("Display state: ");
    Serial.println(currentState);
  }
  
  // Show appropriate screen
  switch(currentState) {
    case WELCOME:
      showWelcomeDisplay();
      break;
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
  
  delay(100);
}

// FUNCTION IMPLEMENTATIONS
void showWelcomeDisplay() {
  tft.fillScreen(TFT_BLACK);
  
  // Header with restaurant branding
  tft.fillRoundRect(10, 20, 220, 80, 15, TFT_NAVY);
  
  // Restaurant logo/name
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("LUMI", 120, 45);
  
  tft.setTextSize(2);
  tft.drawString("BISTRO", 120, 75);
  
  // Welcome message
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(3);
  tft.drawString("Welcome!", 120, 130);
  
  // Table number
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(4);
  tft.drawString("Table " + String(tableNumber), 120, 170);
  
  // Instruction
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("Your smart dining assistant", 120, 210);
  
  drawStatusBar("INITIALIZING", TFT_BLUE);
}

void showReadyDisplay() {
  tft.fillScreen(TFT_BLACK);
  
  drawHeader();
  
  // Main ready message
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("READY", 120, 80);
  
  // Service options visual
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Available Services:", 120, 120);
  
  // Service options (simplified without emojis)
  tft.setTextSize(1);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("- Order Food", 120, 150);
  tft.drawString("- Call Waiter", 120, 170);
  tft.drawString("- Request Bill", 120, 190);
  tft.drawString("- Get Help", 120, 210);
  
  drawStatusBar("TOUCH TO START", TFT_GREEN);
}

void showOrderDisplay() {
  tft.fillScreen(TFT_PURPLE);
  
  drawHeader();
  
  // Voice/order taking animation
  static bool pulse = false;
  pulse = !pulse;
  
  uint16_t color = pulse ? TFT_WHITE : TFT_CYAN;
  
  tft.setTextColor(color, TFT_PURPLE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("LISTENING", 120, 70);
  
  // Microphone animation (circle)
  tft.fillCircle(120, 110, pulse ? 20 : 15, color);
  tft.setTextColor(TFT_PURPLE, color);
  tft.setTextSize(2);
  tft.drawString("MIC", 120, 110);
  
  // Sample order text
  tft.setTextColor(TFT_WHITE, TFT_PURPLE);
  tft.setTextSize(1);
  tft.drawString("Two Caesar salads and", 120, 150);
  tft.drawString("one grilled salmon please", 120, 170);
  
  // Processing indicator
  tft.setTextColor(TFT_YELLOW, TFT_PURPLE);
  tft.drawString("Processing your order...", 120, 200);
  
  drawStatusBar("VOICE ORDER ACTIVE", TFT_MAGENTA);
}

void showConfirmDisplay() {
  tft.fillScreen(TFT_DARKGREEN);
  
  drawHeader();
  
  // Confirmation message
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("ORDER", 120, 70);
  tft.drawString("CONFIRMED", 120, 100);
  
  // Checkmark (circle with check)
  tft.fillCircle(120, 140, 25, TFT_WHITE);
  tft.setTextColor(TFT_DARKGREEN, TFT_WHITE);
  tft.setTextSize(3);
  tft.drawString("OK", 120, 140);
  
  // Order details
  tft.setTextColor(TFT_LIGHTGREY, TFT_DARKGREEN);
  tft.setTextSize(1);
  tft.drawString("2x Caesar Salad", 120, 180);
  tft.drawString("1x Grilled Salmon", 120, 195);
  
  tft.setTextColor(TFT_YELLOW, TFT_DARKGREEN);
  tft.drawString("Estimated time: 15-20 min", 120, 215);
  
  drawStatusBar("SENT TO KITCHEN", TFT_GREEN);
}

void showBillDisplay() {
  tft.fillScreen(TFT_BLUE);
  
  drawHeader();
  
  // Bill ready message
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("BILL READY", 120, 70);
  
  // Amount
  tft.setTextSize(5);
  tft.setTextColor(TFT_YELLOW, TFT_BLUE);
  tft.drawString("$42.50", 120, 120);
  
  // Payment options
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextSize(1);
  tft.drawString("Payment Options:", 120, 170);
  tft.drawString("Card - Cash - Mobile", 120, 190);
  
  tft.setTextColor(TFT_CYAN, TFT_BLUE);
  tft.drawString("Tap to pay or call waiter", 120, 210);
  
  drawStatusBar("PAYMENT READY", TFT_NAVY);  // Fixed color
}

void showThankYouDisplay() {
  tft.fillScreen(TFT_BLACK);
  
  drawHeader();
  
  // Thank you message with animation
  static int sparkle = 0;
  sparkle++;
  
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);  // Changed from TFT_GOLD
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(4);
  tft.drawString("THANK", 120, 80);
  tft.drawString("YOU!", 120, 120);
  
  // Star animation (simple asterisks)
  if(sparkle % 20 < 10) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("*", 80, 60);
    tft.drawString("*", 160, 60);
    tft.drawString("*", 120, 160);
  }
  
  // Feedback request
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("Rate your experience:", 120, 180);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("* * * * *", 120, 200);
  
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString("Visit us again soon!", 120, 220);
  
  drawStatusBar("SESSION COMPLETE", TFT_DARKGREEN);
}

void drawHeader() {
  // Top header bar
  tft.fillRect(0, 0, 240, 30, TFT_NAVY);
  
  // Restaurant name
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextSize(1);
  tft.setTextDatum(TL_DATUM);
  tft.drawString("LUMI BISTRO", 5, 10);
  
  // Time (simulated)
  tft.setTextDatum(TR_DATUM);
  tft.drawString("7:45 PM", 235, 10);
  
  // Table number
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Table " + String(tableNumber), 120, 10);
}

void drawStatusBar(String status, uint16_t color) {
  // Bottom status bar
  tft.fillRect(0, 250, 240, 30, color);
  tft.setTextColor(TFT_WHITE, color);
  tft.setTextSize(1);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(status, 120, 265);
}