#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include "Adafruit_BLE_UART.h"

#include "ultrasonic.h"

// pins and objects
#define ADAFRUITBLE_REQ 49
#define ADAFRUITBLE_RDY 21 //add to ultra 
#define ADAFRUITBLE_RST 48
Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

// LED Display pins
#define CLK 11 // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
uint8_t r=0, g=0, b=0;

// neopixel pins
#define PIN 13
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// global variables
float colorOutput = 0;
int startColor = 0;
int flag = 0;
int pixelDiff = 0;
int maxPixel = 0;
int wait = 10;
int introFlag = 0;
uint32_t blue    = strip.Color(  0,   0, 255);
uint32_t red     = strip.Color(255,   0,   0);
uint32_t green   = strip.Color(  0, 255,   0);
uint32_t yellow  = strip.Color(255, 255,   0);
uint32_t cyan    = strip.Color(  0, 255, 255);
uint32_t magenta = strip.Color(255,   0, 255);
uint32_t white   = strip.Color(255, 255, 255);
uint32_t noColor = strip.Color(  0,   0,   0);
String notifChar = "";




// ultrasonic instance
Ultrasonic ultrasonic;


// NOTIFICATION STUFF
typedef struct Notification {
  String name;
  uint32_t colors[2];
  bool operator==(Notification notif) const {
    return name == notif.name;
  }
} Notification;
static const Notification face = {"FaceB", {blue}};
static const Notification line = {"line",     {green, white}};
static const Notification msgr = {"msngr",{cyan, white}};
static const Notification mail = {"gmail",    {red, white}};
static const Notification hang = {"gchat", {green}};
static const Notification snap = {"snapC", {yellow}};
static const Notification other ={"other",    {white}};
static const Notification empty ={"", {noColor}};

int const notif_size = 10;
Notification notifications[notif_size];
int total = 0;
int current = 0;

int base_time = 0;
String string = "";


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
  Serial.println("setup complete");
}

void loop() {
  if (introFlag == 0) {
    demoIntroduction();
    introFlag = 1;
  }
  uart.pollACI();  
  handleSwipe();  
  handleNotification();
  drawTimeIfIdle();
}

void handleNotification() {
  if (notifChar != "") { // && uart.available()
    int split = notifChar.indexOf(";");
    if (split == -1) {
        split = notifChar.indexOf(":");
        String device = notifChar.substring(0, split);
        base_time = notifChar.substring(split + 1).toInt();
	drawText(device, white);
	delay(2000);
        drawTimeIfIdle();
        notifChar = "";
	return;
    }
    int old = current;
    String name = notifChar.substring(0, split);
    String action  = notifChar.substring(split + 1);
    Serial.println(action + "ing notification for " + name);
    int index = existingApp(name);
    if (action == "post") {
      if (index == -1) { // new notification
	current = total;
        if (name == "facebook") {
	  notifications[current] = face;
        } else if (name == "line") {
	  notifications[current] = line;
        } else if (name == "messenger") {
	  notifications[current] = msgr;
        } else if (name == "gmail") {
	  notifications[current] = mail;
        } else if (name == "hangouts") {
	  notifications[current] = hang;
        } else if (name == "snapchat") {
	  notifications[current] = snap;
        } else {
	  notifications[current] = other;
        }
	total++;
      } else { // existing notification
	current = index;
      }
    } else if (action == "remove" && index >= 0) {
      for (int i = index; i < total; i++) {
      	  notifications[i] = notifications[i+1];
      }
      total--;
      if (total > 0) {
        if (current == index) {
      	  current = (index - 1) % total;
  	  if (current < 0) {
  	    current += total;
	  }
        } else if (current == total) {
          current = (index - 1) % total;
	  if (current < 0) {
	    current += total;
          }
        }
      }
    }
    Serial.println("old: "  + String(old) + " to new: " + String(current) + " out of " + String(total));
    notifChar = (String)"";    
    displayNotification();
  }
}

void displayNotification() {
  Notification notif = notifications[current];
  if (notif.name == "line" || notif.name == "gchat") {
      drawText(notif.name, matrix.Color333(0, 7, 0));
  } else if (notif.name == "gmail") {
      drawText(notif.name, matrix.Color333(7, 0, 0));
  } else {
    drawText(notif.name, notif.colors[0]);
  }
  colorAlt2(notif.colors[0], notif.colors[1], 50);
}

/**************************************************************************/
/*!
    RGBmatrix Panel Functions
*/
/**************************************************************************/

void drawText(String notification, uint16_t color) {
  if (string == notification) {
    return;
  }
  string = notification;
  blankScreen();
  // draw some text!
  matrix.setCursor(2, 4);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  
  for (int i = 0; i < notification.length(); i++) {
    matrix.setTextColor(color);
    matrix.print(notification[i]);
  }
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

void blankScreen() {
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
}

void demoIntroduction() {
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(7, 0, 0));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(7, 4, 0));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(7, 7, 0));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(4, 7, 0));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 7, 0));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 4, 7));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 7));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(4, 0, 7));
  delay(500);
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(7, 0, 4));
  delay(500);
  blankScreen();
  delay(500);
  
  // draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  // print each letter with a rainbow color
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
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('0');
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.print("1");
  matrix.setTextColor(matrix.Color333(7,0,4)); 
  matrix.print("4");
  delay(3000);
  
  drawText("This", matrix.Color333(4,0,7));
  delay(600);
  drawText("is", matrix.Color333(4,0,7));
  delay(600);
  blankScreen();
  delay(600);

  drawText("BLOCK", matrix.Color333(0,4,7));
  delay(3000);
  blankScreen();
  delay(1000);
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
      for (int i = 0; i < total; i++) {
        notifications[i] = empty;
      }
      total = 0;
      current = 0;
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
   // CRASHED HERE WHEN RECEIVING LINE POST
  
  Serial.println("------>>>>>end of rxCallback function!");
}

// Check if a gesture was made; if so, take an action.
void handleSwipe() {
  if (total > 0) {
    int swipe = ultrasonic.readSwipe();
    if (swipe == ultrasonic.SWIPE_R) {
      int old = current;
      if (total > 0) {
        current = (current + 1) % total;
        if (current < 0) {
          current += total;
        }
      }
      Serial.println("from " + String(old) + " to " + String(current));
      displayNotification();
      delay(500);  
    } else if (swipe == ultrasonic.SWIPE_L) {
      int old = current;
      if (total > 0) {
        current = (current - 1) % total; 
        if (current < 0) {
          current += total;
        }
      }
        Serial.println("from " + String(old) + " to " + String(current));
      displayNotification();
      delay(500);  
    }
  }
}
 
int existingApp(String name) {
  for (int i = 0; i < total; i++) {
    String letter = notifications[i].name.substring(0, 1);
    letter.toLowerCase();
    if (letter == name.substring(0, 1)) {
      return i;
    }
  } 
  return -1;
}

void drawTimeIfIdle() {
  if (total == 0) {
    String time = "";
    int hours = ((base_time / 60) + millis() /3600000)%24;
    if (hours < 10) {
      time = "0" + String(hours);   
    } else {
      time = String(hours);
    }
    int minutes = ((base_time % 60) + (millis() % 3600000)/60000)%60;
    if (minutes < 10) {
      time += ":0" + String(minutes);
    } else {
      time += ":" + String(minutes);
    }
    drawText(time, white);
  }
}
