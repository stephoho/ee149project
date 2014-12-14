/*
 * Ultrasonic.h - Library for reading swipe gestures using HC-SR04 ultrasonic sensors. 
 * Created by Johnny Shen, December 13, 2014.
 */
#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"

class Ultrasonic {
  public:
    Ultrasonic(int trigR, int echoR, int trigL, int echoL);
    int const SWIPE_R = 42;
    int const SWIPE_L = 43;
    long readSwipe();
  private:
    int _trigR;
    int _echoR;
    int _trigL;
    int _echoL;
    
    int const RANGE = 1000;
    
    long _readDuration(int side);
};

#endif
