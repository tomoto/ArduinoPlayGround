#ifndef EventUtil_h
#define EventUtil_h

#include <application.h>

class EventUtil {
public:
  static bool publish(const char* eventNameBody, const char* data = NULL);
  
  static bool message(const char* message) {
    publish("message", message);
  }
};

#endif
