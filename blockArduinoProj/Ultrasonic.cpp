/*
 * Ultrasonic.h - Library for reading swipe gestures using HC-SR04 ultrasonic sensors. 
 * Created by Johnny Shen, December 13, 2014.
 */
#include "Arduino.h"
#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int trigR, int echoR, int trigL, int echoL) {
  _trigR = trigR;
  _echoR = echoR;  
  _trigL = trigL;  
  _echoL = echoL;  
  
  pinMode(trigR, OUTPUT);
  pinMode(echoR,  INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL,  INPUT);
}

long Ultrasonic::readSwipe() {
  boolean detectR, detectL;
  long read_R, last_R;
  long read_L, last_L;
  
  read_R = _readDuration(SWIPE_R); 
  read_L = _readDuration(SWIPE_L); 
  detectR = read_R && (read_R < RANGE);
  detectL = read_L && (read_L < RANGE);
  
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

long Ultrasonic::_readDuration(int side) {
  int trig, echo;
  if (side == SWIPE_L) {
    trig = _trigL;
    echo = _echoL;
  } else if (side == SWIPE_R) {
    trig = _trigR;
    echo = _echoR;
  }
  digitalWrite(trig, LOW);
  delayMicroseconds(5);    
  digitalWrite(trig, HIGH);  
  delayMicroseconds(10);  
  return pulseIn(echo, HIGH, 10000);
}

