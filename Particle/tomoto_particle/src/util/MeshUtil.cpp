#include "util/MeshUtil.h"
#include "util/LedUtil.h"
#include "util/SimpleTimeout.h"
#include "Particle.h"

static String thisDeviceID = System.deviceID(); // get only once

bool MeshUtil::publish(const char* event, const char* data) {
  SimpleTimeout timeout(50);
  LedUtil::setUserSignal(true);
  Mesh.publish(event, data);
  timeout.wait();
  LedUtil::setUserSignal(false);
  return true;
}

void MeshUtil::getEventName(char* buf, const char* body) {
  getEventName(buf, thisDeviceID.c_str(), body);
}

void MeshUtil::getEventName(char* buf, const char* deviceID, const char* body) {
  sprintf(buf, "mesh/%s/%s", deviceID, body);
}
