#include <TFT_eSPI.h> // Hardware-specific library
#include "logo.h"       
#include <SPI.h>
#include <string>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setup() {
  tft.init(); // Initialize with reversed resolution
  tft.setRotation(3); // Experiment with rotation
  tft.setTextSize(2);
  Serial.begin(115200); // For debug
  tft.fillScreen(TFT_BLACK); // Test color
  tft.setTextColor(TFT_PINK);
  tft.println("mrepp miaoww :3");
  tft.drawXBitmap(245, 177, logo, logoWidth, logoHeight, TFT_BLACK, TFT_RED);

}

void loop() {
  delay(1000);
}
