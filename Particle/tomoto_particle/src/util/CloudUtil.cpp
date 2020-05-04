#include "util/CloudUtil.h"
#include "util/LedUtil.h"
#include "util/SimpleTimeout.h"
#include "Particle.h"

static system_tick_t failureStartTime = 0;

bool CloudUtil::publish(const char* event, const char* data) {
  // throttling
  bool result;
  int challenge = 0;
  while (true) {
    SimpleTimeout timeout(50);
    LedUtil::setUserSignal(true);
    result = Particle.publish(event, data);
    timeout.wait();
    LedUtil::setUserSignal(false);
    if (result || ++challenge == 3) break;
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
  
  return result;
}
