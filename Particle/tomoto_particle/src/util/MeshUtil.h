#ifndef MeshUtil_h
#define MeshUtil_h

#include <application.h>

class MeshUtil {
public:
  // Publish event to mesh network w/ LED flash
  static bool publish(const char* event, const char* data = NULL);
  
  // Create event name "mesh/{deviceID}/{suffix}"
  static String eventName(const char* body);
};

#endif
