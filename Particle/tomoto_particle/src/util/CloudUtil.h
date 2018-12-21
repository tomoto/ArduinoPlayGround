#ifndef CloudUtil_h
#define CloudUtil_h

#include <application.h>

class CloudUtil {
public:
  static bool publish(const char* event, const char* data = NULL);
};

#endif
