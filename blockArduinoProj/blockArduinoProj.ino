#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "Adafruit_BLE_UART.h"

// pins and objecs
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9
Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

#define PIN 5
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);
int irSensor = A5;

// global variables
float irValue = 0;
float colorOutput = 0;
int startColor = 0;
int flag = 0;
int pixelDiff = 0;
int maxPixel = 0;
int wait = 10;
uint32_t blue = strip.Color(0, 0, 255);
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t white = strip.Color(255, 255, 255);
uint32_t cyan = strip.Color(0, 255, 255);
uint32_t noColor = strip.Color(0, 0, 0);
char notifChar;

void setup() {
  Serial.begin(9600);
  
  while(!Serial); // Leonardo/Micro should wait for serial init
  Serial.println(F("Adafruit Bluefruit Low Energy nRF8001 Callback Echo demo"));
  uart.setRXcallback(rxCallback);
  uart.setACIcallback(aciCallback);
  // uart.setDeviceName("NEWNAME"); /* 7 characters max! */
  uart.begin();
  
  pinMode(irSensor, INPUT);
  // NeoPixel LEDs
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  uart.pollACI();
  displayNotif();
  if (notifChar == 'p')  {
    irValue = analogRead(irSensor);
    irValue = 255*(irValue / 1000.0);
    potentiometerDemo(irValue);
  } 
//  Serial.print("Output Value: ");
//  Serial.print(irValue);
//  Serial.print("\n"); 
}

// Changing LED color based on notification
void displayNotif(){
  rainbow(20); // idle
  if (notifChar == 'f' or notifChar == 'F') {
    colorWipe(blue, 50); // Blue
  } 
  if (notifChar == 'l' or notifChar == 'L') {
    colorWipe(green, 50); // Green
  }
  if (notifChar == 'g' or notifChar == 'G') {
    colorAlt2(red, white, 50); // Red and White
  }
  if (notifChar == 'm' or notifChar == 'M' or notifChar == 't' or notifChar == 'T') {
    colorWipe(cyan, 50); // Cyan
  }
}

// Mini Project Update Demo: Potentiometer (11-19-14)
void potentiometerDemo(int colorOutput) {
  // set startColor
  if (flag == 0) {
    startColor = colorOutput;
    flag = 1;
  } 
  // initialize 1st pixel
//  strip.setPixelColor(0, blue);
//  strip.show();
//  delay(wait);
  
  // add pixels
  if (colorOutput > startColor & (colorOutput - startColor) >= pixelDiff) {
    pixelDiff = colorOutput - startColor;
    maxPixel = pixelDiff;
    for (int i=0; i<pixelDiff; i++) {
      if (i%2 == 0) {
        strip.setPixelColor(i, blue);
      } else {
        strip.setPixelColor(i, yellow);
      }
      strip.show();
      delay(wait);
    }
  }
  else { // remove pixels
    pixelDiff = colorOutput - startColor;
    for (int i=maxPixel; i>pixelDiff; i--) {
      strip.setPixelColor(i, noColor);
      strip.show();
      delay(wait);
    }
  }
}

/**************************************************************************/
/*!
    BLE Code (from callbackEcho)
*/
/**************************************************************************/
//This function is called whenever select ACI events happen
void aciCallback(aci_evt_opcode_t event)
{
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
void rxCallback(uint8_t *buffer, uint8_t len)
{
  Serial.print(F("Received "));
  Serial.print(len);
  Serial.print(F(" bytes: "));
  for(int i=0; i<len; i++)
   Serial.print((char)buffer[i]); 
   notifChar = (char)buffer[0];
  Serial.print(F(" ["));

  for(int i=0; i<len; i++)
  {
    Serial.print(" 0x"); Serial.print((char)buffer[i], HEX); 
  }
  Serial.println(F(" ]"));

  /* Echo the same data back! */
  uart.write(buffer, len);
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

