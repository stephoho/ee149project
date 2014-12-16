#include "ultrasonic.h"

// sensor pins
#define trigR 39
#define echoR 41
#define trigL 38
#define echoL 40

// constructor
Ultrasonic::Ultrasonic() {
  pinMode(trigR, OUTPUT);
  pinMode(echoR,  INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL,  INPUT);
}

// destructor 
Ultrasonic::~Ultrasonic(){/* empty */}


/** Returns:
 *    -1	if   no  swipe 
 *  SWIPE_R	if right swipe
 *  SWIPE_L	if  left swipe
 */
long Ultrasonic::readSwipe() {
  boolean detectR, detectL;
  long read_R, last_R;
  long read_L, last_L;
  
  read_R = _readDuration(SWIPE_R); 
  read_L = _readDuration(SWIPE_L); 
  detectR = read_R && (read_R < RANGE);
  detectL = read_L && (read_L < RANGE);
  //Serial.println(read_R);
  
  if (detectR && !detectL) {
    while(detectR || detectL) {
      last_R = read_R;
      last_L = read_L;
      read_R = _readDuration(SWIPE_R);
      read_L = _readDuration(SWIPE_L);   
      detectR = read_R && (read_R < RANGE);
      detectL = read_L && (read_L < RANGE);
    }
    detectR = last_R && (last_R < RANGE);
    detectL = last_L && (last_L < RANGE);
    if (!detectR && detectL) {
      return SWIPE_R;
    }      
  } else if (!detectR && detectL) {
    while(detectR || detectL) {
      last_R = read_R;
      last_L = read_L;
      read_R = _readDuration(SWIPE_R); 
      read_L = _readDuration(SWIPE_L);   
      detectR = read_R && (read_R < RANGE);
      detectL = read_L && (read_L < RANGE);
    }
    detectR = last_R && (last_R < RANGE);
    detectL = last_L && (last_L < RANGE);
    if (detectR && !detectL) {
      return SWIPE_L;  
    }
  }
  return -1;
}


/** Returns:
 *    duration of ping up to 10,000 ms
 *  Given:
 *    select ultrasonic sensor side
 */
long Ultrasonic::_readDuration(int side) {
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
