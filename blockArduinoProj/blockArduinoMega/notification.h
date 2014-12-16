#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Arduino.h"

"appname; <0, 1>; [RGB, RGB, ...]"
class Notification {

  public:
     Notification(int id, uint32_t[] colors);
    ~Notification();

    int id;
    uint32_t[] colors;

    long readSwipe();

    
  private:

};

#endif
