#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(9600);
  Serial.println("Auto-Centered LUMI");
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  static uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_WHITE};
  static int colorIndex = 0;
  
  tft.fillScreen(TFT_BLACK);
  
  // Use built-in centering
  tft.setTextColor(colors[colorIndex], TFT_BLACK);
  tft.setTextSize(4);
  tft.setTextDatum(MC_DATUM);  // Middle Center
  
  // Draw at screen center
  tft.drawString("LUMI", tft.width()/2, tft.height()/2);
  
  Serial.print("Auto-centered with color index: ");
  Serial.println(colorIndex);
  
  colorIndex = (colorIndex + 1) % 6;
  delay(2000);
}