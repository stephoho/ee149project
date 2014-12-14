#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include "Adafruit_BLE_UART.h"

// pins and objecs
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2 //add to ultra
#define ADAFRUITBLE_RST 9
Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);


// ultrasonic constants
#define trigR 7
#define echoR 6
#define trigL 5
#define echoL 4

#define SWIPE_L 42
#define SWIPE_R 43
#define DELAY  250
#define RANGE 1000

// neopixel pins
#define PIN 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);
int irSensor = A5;

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


// tracking stuff
int const notif_size = 6;
uint32_t notifs[notif_size] = {red, yellow, green, cyan, blue, magenta};
int tot = 0;
int cur = 0;


void setup() {
  Serial.begin(9600);
  
  ultra_setup();
  
  while(!Serial); // Leonardo/Micro should wait for serial init
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
  displayNotification();
//  int swipe = readSwipe();
//  if (swipe == SWIPE_R) {
//    int old = cur;
//    cur = (cur + 1) % tot;
//    if (cur < 0) {
//      cur += tot;
//    }
//    Serial.println("from " + String(old) + " to " + String(cur));    
//    colorWipe(notifs[cur], 50);
//  } else if (swipe == SWIPE_L) {
//    int old = cur;
//    cur = (cur - 1) % tot;
//    if (cur < 0) {
//      cur += tot;
//    }
//    Serial.println("from " + String(old) + " to " + String(cur));
//    colorWipe(notifs[cur], 50);
//  }
//  delay(500);  
  uart.pollACI();  
}

// Changing LED color based on notification
void displayNotif(){
  //rainbow(20); // idle
  theaterChase(strip.Color(127, 127, 127), 50); // White
  if (notifChar == "f") {
    colorWipe(blue, 50); // Blue
  } 
  else if (notifChar == "l" or notifChar == "h") {
    colorWipe(green, 50); // Green
  }
  else if (notifChar == "g") {
    colorAlt2(red, white, 50); // Red and White
  }
  else if (notifChar == "m") {
    colorWipe(cyan, 50); // Cyan
  } 
}

void displayNotification() {
  theaterChase(strip.Color(127, 127, 127), 50); // Whites
  if (uart.available()) {
    Serial.println("displaying notification: ");
    Serial.println(notifChar.substring(notifChar.length()));
    Serial.println(notifChar.length());
    if (notifChar.substring(0, 5) == "gmail") {
      colorAlt2(red, white, 50); // Red and White
    }
    else if (notifChar.substring(0, 8) == "hangouts") {
      colorWipe(green, 50); // Red and White
    }
    else if (notifChar.substring(0,8) == "facebook") {
      colorWipe(blue, 50);
    }
   }
  notifChar = (String)"";
}


/**********************************************w****************************/
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




// ####################################################################
// ####################################################################
// ############            ULTRASONIC STUFF                ############
// ####################################################################
// ####################################################################

/* Setup IO pins for each of the sensors. */
void ultra_setup() {
  pinMode(trigR, OUTPUT);
  pinMode(echoR,  INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL,  INPUT);
}


/** Returns:
 *    -1	if   no  swipe 
 *  SWIPE_R	if right swipe
 *  SWIPE_L	if  left swipe
 */
int readSwipe() {
  boolean detectR, detectL;
  long read_R, last_R;
  long read_L, last_L;
  
  read_R = readDuration(SWIPE_R); 
  read_L = readDuration(SWIPE_L); 
  detectR = read_R && (read_R < RANGE);
  detectL = read_L && (read_L < RANGE);
  
  if (detectR && !detectL) {
    while(detectR || detectL) {
//  Serial.println("Left: \t" + String(detectL) + "\tRight:\t" + String(detectR));  
      
      last_R = read_R;
      last_L = read_L;
      read_R = readDuration(SWIPE_R);
      read_L = readDuration(SWIPE_L);   
      detectR = read_R && (read_R < RANGE);
      detectL = read_L && (read_L < RANGE);
    }
    detectR = last_R && (last_R < RANGE);
    detectL = last_L && (last_L < RANGE);
    if (!detectR && detectL) {
    Serial.println(">>>>>>>>>>>>>>>>>>>RIGHT>>>>>>>>>>>>>>>>>>>");
      
      return SWIPE_R;
    }      
  } else if (!detectR && detectL) {
    while(detectR || detectL) {
//  Serial.println("Left: \t" + String(detectL) + "\tRight:\t" + String(detectR));  
      
      last_R = read_R;
      last_L = read_L;
      read_R = readDuration(SWIPE_R); 
      read_L = readDuration(SWIPE_L);   
      detectR = read_R && (read_R < RANGE);
      detectL = read_L && (read_L < RANGE);
    }
    detectR = last_R && (last_R < RANGE);
    detectL = last_L && (last_L < RANGE);
    if (detectR && !detectL) {
    Serial.println("<<<<<<<<<<<<<<<<<<<<<LEFT<<<<<<<<<<<<<<<<<<<<<");
      
      return SWIPE_L;  
    }
  }
  return -1;
}

/* Returns the reading from the @side sensor. */
long readDuration(int side) {
  int trig, echo;
  if (side == SWIPE_L) {
    trig = trigL;
    echo = echoL;
  } else if (side == SWIPE_R) {
    trig = trigR;
    echo = echoR;
  }
  digitalWrite(trig, LOW);
  delayMicroseconds(5);    
  digitalWrite(trig, HIGH);  
  delayMicroseconds(10);  
  return pulseIn(echo, HIGH, 10000);
}
