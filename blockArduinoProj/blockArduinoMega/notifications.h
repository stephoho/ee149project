/*#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include "Arduino.h"

typedef struct Notification {
  char* name;
  uint32_t colors[2];
  bool operator==(Notification notif) const {
    return !strcmp(name, notif.name);
  }
} Notification;

static const Notification face = {"facebook", {blue}};
static const Notification line = {"line",     {green, white}};
static const Notification msgr = {"messenger",{cyan, white}};
static const Notification mail = {"gmail",    {red, white}};
static const Notification hang = {"hangouts", {green}};
static const Notification snap = {"snapchat", {yellow}};
static const Notification othr = {"other",    {white}};


class Notifications {
  public:
    Notifcation current[10];
    
     Notifications();
    ~Notifications();

    void append(char* app_name);
    void delete(char* app_name);
    
  private:
    const int DELAY =  250;
    const int RANGE = 1000;

    long _readDuration(int side);
};

#endif


typedef struct {
  Notifcation current[10];
  

} Notifications;
*/
