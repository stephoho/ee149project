// testcolors demo for Adafruit RGBmatrixPanel library.
// Renders 512 colors on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420
// Library supports 4096 colors, but there aren't that many pixels!  :)

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8 // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
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
      matrix.drawPixel(x, y, matrix.Color333(0, 0, 5));
    }
  }
}

void bottomHalf() {
  // Draw bottom half
  for (uint8_t y=8; y < 16; y++) {  
    for (uint8_t x=0; x < 32; x++) {      
      matrix.drawPixel(x, y, matrix.Color333(0, 0, 5));
    }
  }
}

void drawText(String notification, String color) {
  // draw some text!
  matrix.setCursor(2, 5);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  
  for (int i = 0; i < notification.length(); i++) {
    if (color == "red") {
      matrix.setTextColor(matrix.Color333(7,0,0));
    }
    if (color == "green") {
      matrix.setTextColor(matrix.Color333(0,7,0));
    }
    if (color == "blue") {
      matrix.setTextColor(matrix.Color333(0,0,7));
    }
    matrix.print(notification[i]);
  }
}

void testText() {
  // draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high

  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('1');
  matrix.setTextColor(matrix.Color333(7,4,0)); 
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0)); 
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(0,7,0));  
  matrix.print('2'); 
 
  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0,7,7)); 
  matrix.print('*');
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.print("B");
  matrix.setTextColor(matrix.Color333(7,0,4)); 
  matrix.print("*");
}

void fillRect() {
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(7, 0, 0));
}

void drawIcons() {
  for (uint8_t y=5; y < 11; y++) {
    matrix.drawPixel(2, y, matrix.Color333(0, 0, 5));
    matrix.drawPixel(7, y, matrix.Color333(0, 0, 5));
  }
  for (uint8_t x=2; x < 7; x++) {
    matrix.drawPixel(x, 5, matrix.Color333(0, 0, 5));
    matrix.drawPixel(x, 10, matrix.Color333(0, 0, 5));
  }
}

void blankScreen(int ms) {
  delay(ms);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
}

void loop() {
//  topHalf();
//  bottomHalf();
  drawText("GMAIL", "red");
  blankScreen(2000);
  drawText("FB", "blue");
  blankScreen(2000);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  drawText("GChat", "green");
  blankScreen(2000);
}

