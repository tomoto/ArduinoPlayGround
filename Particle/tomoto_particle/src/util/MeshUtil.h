#ifndef MeshUtil_h
#define MeshUtil_h

#include <application.h>

class MeshUtil {
public:
  static const int EVENT_NAME_LEN = 63;
  static const int EVENT_DATA_LEN = 255;
  
  // Publish event to mesh network w/ LED flash
  static bool publish(const char* event, const char* data = NULL);
  
  // Create event name "mesh/{this deviceID}/{body}"
  static void getEventName(char* buf, const char* body);

  // Create event name "mesh/{given deviceID}/{body}"
  static void getEventName(char* buf, const char* deviceID, const char* body);
};

#endif
