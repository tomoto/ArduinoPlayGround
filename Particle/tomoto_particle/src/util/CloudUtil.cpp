#include "util/CloudUtil.h"
#include "util/LedUtil.h"
#include "Particle.h"

bool CloudUtil::publish(const char* event, const char* data) {
  LedUtil::sendToCloudBegin();
  
  bool result;
  for (int i = 0; i < 3; i++) {
    result = Particle.publish(event, data);
    if (result) break;
    delay(250);
  }
  
  return LedUtil::sendToCloudEnd(result);
}
