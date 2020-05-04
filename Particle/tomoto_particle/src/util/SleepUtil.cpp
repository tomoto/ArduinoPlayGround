#include "util/SleepUtil.h"
#include "util/LedUtil.h"
#include "application.h"

void SleepUtil::sleep(int seconds) {
  LedUtil::setUserSignal(false);
  System.sleep({}, 0, RISING, seconds);
  waitUntil(Mesh.ready);
  waitFor(Particle.connected, 2000);
}
