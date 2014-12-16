#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Arduino.h"

struct Notification {
  char* name;
  uint32_t colors[2];

  bool operator==(Notification notif) const {
    return !strcmp(name, notif.name);
  }

};

#endif

