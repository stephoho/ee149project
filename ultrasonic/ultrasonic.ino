// include the library code:
#include <LiquidCrystal.h>

#define trigR 13
#define echoR 12
#define trigL 11
#define echoL 10

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 5, 4, 3, 2);

int val = 0;

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);

}

void loop() {
  long durationR, distanceR;
  long durationL, distanceL;  

  digitalWrite(trigR, LOW);
  delayMicroseconds(5);
  digitalWrite(trigR, HIGH);
  delayMicroseconds(10);
  durationR = pulseIn(echoR, HIGH);

  digitalWrite(trigL, LOW);
  delayMicroseconds(5);    
  digitalWrite(trigL, HIGH);  
  delayMicroseconds(10);  
  durationL = pulseIn(echoL, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Right: ");
  lcd.print(durationR);  
  lcd.setCursor(0, 1);
  lcd.print("Left: ");
  lcd.print(durationL);  

}


