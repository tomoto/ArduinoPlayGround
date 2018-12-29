#include "util/CloudUtil.h"
#include "util/LedUtil.h"
#include "Particle.h"

static system_tick_t failureStartTime = 0;

bool CloudUtil::publish(const char* event, const char* data) {
  LedUtil::sendToCloudBegin();
  
  bool result;
  
  // throttling
  int retry = 0;
  while (true) {
    result = Particle.publish(event, data);
    if (result || retry++ == 2) break;
    delay(500);
  }
  
  // Reboot itself if cannot reach to the cloud for 10 minutes
  if (result) {
    failureStartTime = 0;
  } else {
    system_tick_t t = Time.now();
    if (failureStartTime == 0) {
       failureStartTime = t;
    }
    
    if (t - failureStartTime >= 60 * 10) {
      System.reset();
    }
  }
  
  return LedUtil::sendToCloudEnd(result);
}
