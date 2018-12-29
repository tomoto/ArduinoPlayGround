#include "EventUtil.h"
#include "CloudUtil.h"
#include "MeshUtil.h"

bool EventUtil::publish(const char* eventNameBody, const char* data) {
#if Wiring_WiFi // Argon
  char eventName[CloudUtil::EVENT_NAME_LEN+1];
  sprintf(eventName, "gateway/%s", eventNameBody);
  return CloudUtil::publish(eventName, data);
#else // Xenon
  char eventName[MeshUtil::EVENT_NAME_LEN+1];
  MeshUtil::getEventName(eventName, eventNameBody);
  return MeshUtil::publish(eventName, data);
#endif
}
