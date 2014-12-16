#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

class Ultrasonic {

  public:
    const int SWIPE_R = 123;
    const int SWIPE_L = 456;  
  
     Ultrasonic();
    ~Ultrasonic();

    long readSwipe();

    
  private:
    const int DELAY =  250;
    const int RANGE = 1000;

    long _readDuration(int side);
};

#endif
