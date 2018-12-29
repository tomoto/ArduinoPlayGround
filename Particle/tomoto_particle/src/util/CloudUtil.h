#ifndef CloudUtil_h
#define CloudUtil_h

#include <application.h>

class CloudUtil {
public:
  static const int EVENT_NAME_LEN = 63;
  static const int EVENT_DATA_LEN = 255;
  
  static bool publish(const char* event, const char* data = NULL);
};

#endif
