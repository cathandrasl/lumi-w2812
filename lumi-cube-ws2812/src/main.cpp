#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);  // Using 9600 baud as requested
  Serial.println("Simple LUMI Display Test - 9600 baud");
  
  // Initialize display
  tft.init();
  tft.setRotation(0);  // Try 0, 1, 2, or 3 if display is rotated wrong
  tft.fillScreen(TFT_BLACK);
  
  // Display LUMI in big letters
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(50, 120);  // Adjust position as needed
  tft.print("LUMI");
  
  Serial.println("LUMI displayed on screen!");
}

void loop() {
  // Change colors every 2 seconds
  static uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_WHITE};
  static int colorIndex = 0;
  
  Serial.print("Changing to color index: ");
  Serial.println(colorIndex);
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(colors[colorIndex], TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(50, 120);
  tft.print("LUMI");
  
  colorIndex = (colorIndex + 1) % 6;
  delay(2000);
}