#include "util/MeshUtil.h"
#include "util/LedUtil.h"
#include "Particle.h"

static String deviceID = System.deviceID(); // get only once

bool MeshUtil::publish(const char* event, const char* data) {
    LedUtil::sendToMeshBegin();
    return LedUtil::sendToMeshEnd(Mesh.publish(event, data));
}

void MeshUtil::getEventName(char* buf, const char* body) {
  sprintf(buf, "mesh/%s/%s", deviceID.c_str(), body);
}
