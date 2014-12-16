#ifndef RGBMATRIX_H
#define RGBMATRIX_H

#include "Arduino.h"

class RGBmatrix {
  public:
     RGBmatrix();
    ~RGBmatrix();

    void flashNotification(String notification, String color, int duration, int num);
    void drawText(String notification, String color);
    void blankScreen(int ms);
    void drawIcons();
    void fillRect();
    void testText();
  private:
    const int DELAY =  250;
    const int RANGE = 1000;

    long _readDuration(int side);
};

#endif
