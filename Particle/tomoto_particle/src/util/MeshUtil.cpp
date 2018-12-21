#include "util/MeshUtil.h"
#include "util/LedUtil.h"
#include "Particle.h"

bool MeshUtil::publish(const char* event, const char* data) {
    LedUtil::sendToMeshBegin();
    LedUtil::sendToMeshEnd(Mesh.publish(event, data));
}

String MeshUtil::eventName(const char* body) {
  return String("mesh/") + System.deviceID() + "/" + body;
}
