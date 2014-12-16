  #include "RGBmatrix.h"


// RGB Matrix Panel Pins
#define CLK 11 // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);


// constructor
RGBmatrix::RGBmatrix() {
  matrix.begin();
}

// destructor 
RGBmatrix::~RGBmatrix(){/* empty */}

void RGBmatrix::flashNotification(String notification, String color, int duration, int num) {
  for (int i=0; i<num; i++) {
    drawText(notification, color);
    blankScreen(duration);
  }
}

void RGBmatrix::drawText(String notification, String color) {
  // draw some text!
  matrix.setCursor(2, 4);   // start at top left, with one pixel of spacing
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

void RGBmatrix::testText() {
  Serial.println("hihi");
  // draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high

  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('E');
  matrix.setTextColor(matrix.Color333(7,4,0)); 
  matrix.print('E');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('1');
  matrix.setTextColor(matrix.Color333(4,7,0)); 
  matrix.print('4');
  matrix.setTextColor(matrix.Color333(0,7,0));  
  matrix.print('9'); 
 
  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0,7,7)); 
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('0');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('1');
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.print("4");
//  matrix.setTextColor(matrix.Color333(7,0,4)); 
//  matrix.print("*");
Serial.println("done");
delay(500);
}

void RGBmatrix::fillRect() {
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(7, 0, 0));
}

void RGBmatrix::drawIcons() {
  for (uint8_t y=5; y < 11; y++) {
    matrix.drawPixel(2, y, matrix.Color333(0, 0, 5));
    matrix.drawPixel(7, y, matrix.Color333(0, 0, 5));
  }
  for (uint8_t x=2; x < 7; x++) {
    matrix.drawPixel(x, 5, matrix.Color333(0, 0, 5));
    matrix.drawPixel(x, 10, matrix.Color333(0, 0, 5));
  }
}

void RGBmatrix::blankScreen(int ms) {
  delay(ms);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  delay(ms);
}
