#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "Adafruit_BLE_UART.h"
#include <Adafruit_GFX.h>
#include <gamma.h>
#include <RGBmatrixPanel.h>

#include "ultrasonic.h"

// RGB Matrix Panel Pins
#define CLK 11 // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// BT pins and objects
#define ADAFRUITBLE_REQ 49
#define ADAFRUITBLE_RDY 21 //add to ultra 
#define ADAFRUITBLE_RST 48
Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

// LED Display pins
uint8_t r=0, g=0, b=0;

// neopixel pins
#define PIN 13
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// global variables
float colorOutput = 0;
int startColor = 0;
int flag = 0;
int pixelDiff = 0;
int maxPixel = 0;
int wait = 10;
uint32_t blue    = strip.Color(  0,   0, 255);
uint32_t red     = strip.Color(255,   0,   0);
uint32_t green   = strip.Color(  0, 255,   0);
uint32_t yellow  = strip.Color(255, 255,   0);
uint32_t cyan    = strip.Color(  0, 255, 255);
uint32_t magenta = strip.Color(255,   0, 255);
uint32_t white   = strip.Color(255, 255, 255);
uint32_t noColor = strip.Color(  0,   0,   0);
String notifChar = "";

// sensors instances
Ultrasonic ultrasonic;

// actuator instances
RGBmatrix panel;

// notification array and values
/*struct Notification notifications[10];*/


// tracking stuff
int const notif_size = 6;
uint32_t notifs[notif_size] = {red, yellow, green, cyan, blue, magenta};
int tot = 0;
int cur = 0;


void setup() {
  Serial.begin(9600);
  matrix.begin();
  // Leonardo/Micro should wait for serial init    
  while(!Serial); 
  Serial.println(F("Adafruit Bluefruit Low Energy nRF8001"));
  uart.setRXcallback(rxCallback);
  uart.setACIcallback(aciCallback);
  uart.setDeviceName("[Block]"); /* 7 characters max! */
  uart.begin();
  
  // NeoPixel LEDs
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(notifs[cur], 50);
  Serial.println("setup complete");
  tot = notif_size;
}

void loop() {
  uart.pollACI();  
  handleSwipe();
  displayNotification();
}




void flashNotification(String notification, String color, int duration, int num) {
  for (int i=0; i<num; i++) {
    drawText(notification, color);
    blankScreen(duration);
  }
}

void drawText(String notification, String color) {
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

void testText() {
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


/**************************************************************************/
/*
 *   RGBMatrix Panel Functions (Modified from Example)
 */
/**************************************************************************/


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
  delay(ms);
}


void displayNotification() {
  //theaterChase(strip.Color(127, 0, 127), 50); // Whites
  if (uart.available()) {
    Serial.println("displaying notification: ");
    Serial.println(notifChar.substring(notifChar.length()));
    Serial.println(notifChar.length());
    if (notifChar.substring(0, 5) == "gmail") {
      colorAlt2(red, white, 50); // Red and White
      panel.flashNotification("GMAIL", "red", 500, 3);
      panel.drawText("GMAIL", "red");
      panel.blankScreen(2000);
    }
    else if (notifChar.substring(0, 8) == "hangouts") {
      colorWipe(green, 50); // Red and White
      panel.flashNotification("GChat", "green", 500, 3);
      panel.drawText("GChat", "green");
      panel.blankScreen(2000);
    }
    else if (notifChar.substring(0,8) == "facebook") {
      colorWipe(blue, 50);
      panel.flashNotification("FB", "blue", 500, 3);
      panel.drawText("FB", "blue");
      panel.blankScreen(2000);
    }
   }
  notifChar = (String)"";
}

/**************************************************************************/
/*!
    NeoPixel Functions (Modified from Example)
*/
/**************************************************************************/

// strandTest
void strandTest() {
   // NeoPixels
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorAlt2(uint32_t c, uint32_t c2, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if (i%2 == 0) {
      strip.setPixelColor(i, c);
    } else {
      strip.setPixelColor(i, c2);
    }
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


/**************************************************************************/
/*!
    BLE Code (from callbackEcho)
*/
/**************************************************************************/
//This function is called whenever select ACI events happen
void aciCallback(aci_evt_opcode_t event) {
  switch(event)
  {
    case ACI_EVT_DEVICE_STARTED:
      Serial.println(F("Advertising started"));
      break;
    case ACI_EVT_CONNECTED:
      Serial.println(F("Connected!"));
      theaterChase(strip.Color(0, 127, 0), 50); // Green
      break;
    case ACI_EVT_DISCONNECTED:
      Serial.println(F("Disconnected or advertising timed out"));
      theaterChase(strip.Color(127, 0, 0), 50); // Green
      break;
    default:
      break;
  }
}

//This function is called whenever data arrives on the RX channel
void rxCallback(uint8_t *buffer, uint8_t len) {
  Serial.print(F("Received "));
  Serial.print(len);
  Serial.print(F(" bytes: "));
  for(int i=0; i<len; i++) {
   Serial.print((char)buffer[i]); 
   notifChar += ((char)buffer[i]);
  }
  Serial.println("");
  Serial.print("Next: ");
  Serial.println(notifChar);
  
//  Serial.print(F(" ["));
//  for(int i=0; i<len; i++)
//  {
//    Serial.print(" 0x"); Serial.print((char)buffer[i], HEX); 
//  }
//  Serial.println(F(" ]"));
  /* Echo the same data back! */
//  uart.write(buffer, len);

  Serial.println("end of rxCallback function!");
}

// Check if a gesture was made; if so, take an action.
void handleSwipe() {
  int swipe = ultrasonic.readSwipe();
  Serial.println("swipe: "  + String(swipe));
  if (swipe == ultrasonic.SWIPE_R) {
    int old = cur;
    cur = (cur + 1) % tot;
    if (cur < 0) {
      cur += tot;
    }
    Serial.println("from " + String(old) + " to " + String(cur));
  colorWipe(notifs[cur], 50);
      delay(500);  
  } else if (swipe == ultrasonic.SWIPE_L) {
    int old = cur;
    cur = (cur - 1) % tot;
    if (cur < 0) {
      cur += tot;
    }
    Serial.println("from " + String(old) + " to " + String(cur));
    colorWipe(notifs[cur], 50);
  delay(500);  
  }

}
