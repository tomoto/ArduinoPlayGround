#ifndef EventUtil_h
#define EventUtil_h

#include <application.h>

class EventUtil {
public:
  static const int EVENT_NAME_LEN = 63;
  static const int EVENT_DATA_LEN = 255;
  
  static bool publish(const char* eventNameBody, const char* data = NULL);
  
  static bool message(const char* message) {
    return publish("message", message);
  }
};

#endif
