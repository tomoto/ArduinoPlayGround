#include "gateway/MeshEventRepeater.h"
#include "util/CloudUtil.h"
#include "util/MeshUtil.h"
#include "Particle.h"

static const char* fs_ack = NULL;

static void handleMeshEvent(const char *event, const char *data) {
  if (fs_ack) {
    char eventName[MeshUtil::EVENT_NAME_LEN+1];
    sprintf(eventName, "%s/ack", event);
    MeshUtil::publish(eventName, fs_ack);
  }
  
  CloudUtil::publish(event, data);
}

void MeshEventRepeater::begin() {
  Mesh.subscribe("mesh/", handleMeshEvent);
}

void MeshEventRepeater::setAck(const char* ack) {
  fs_ack = ack;
}

const char* MeshEventRepeater::getAck() const {
  return fs_ack;
}

