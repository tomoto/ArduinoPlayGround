#include "EventUtil.h"
#include "CloudUtil.h"
#include "MeshUtil.h"

bool EventUtil::publish(const char* eventNameBody, const char* data) {
#if Wiring_WiFi // Argon
  return CloudUtil::publish(String("gateway/") + eventNameBody, data);
#else // Xenon
  return MeshUtil::publish(MeshUtil::eventName(eventNameBody), data);
#endif
}
