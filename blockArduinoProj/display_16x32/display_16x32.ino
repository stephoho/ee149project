// testcolors demo for Adafruit RGBmatrixPanel library.
// Renders 512 colors on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420
// Library supports 4096 colors, but there aren't that many pixels!  :)

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A5
#define OE  9
#define A   A2
#define B   A3
#define C   A4
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

uint8_t r=0, g=0, b=0;

void setup() {
  matrix.begin();
  Serial.begin(9600);
}

void topHalf() {
  // Draw top half
  for (uint8_t y=0; y < 8; y++) {      
    for (uint8_t x=0; x < 32; x++) {  
      matrix.drawPixel(x, y, matrix.Color333(0, 7, 0));
    }
  }
}

void bottomHalf() {
  // Draw bottom half
  for (uint8_t x=0; x < 32; x++) {      
    for (uint8_t y=8; y < 16; y++) {  
      matrix.drawPixel(x, y, matrix.Color333(5, 5, 0));
    }
  }
}


void loop() {
  //topHalf();
  //bottomHalf();
  //matrix.fillRect(0, 0, 32, 16, matrix.Color333(7, 0, 0));
  for (int y=0; y < 16; y++) {
    matrix.drawPixel(0, y, matrix.Color333(7, 7, 0));
  }
}

