#include "endpoint/MeshAckReceiver.h"
#include "util/MeshUtil.h"
#include "Particle.h"

static bool fs_keepAwake = false;

static void handleAckEvent(const char *event, const char *data) {
  fs_keepAwake = (strcmp(data, "awake") == 0);
}

void MeshAckReceiver::begin(const char* body) {
  char eventName[MeshUtil::EVENT_NAME_LEN+1];
  MeshUtil::getEventName(eventName, body);
  Mesh.subscribe(eventName, handleAckEvent);
}

bool MeshAckReceiver::isKeepAwake() const {
  return fs_keepAwake;
}
