#include "endpoint/MeshRemoteResetReceiver.h"
#include "util/MeshUtil.h"
#include "Particle.h"

static void handleRemoteResetEvent(const char *event, const char *data) {
  System.reset();
}

void MeshRemoteResetReceiver::begin() {
  char eventName[MeshUtil::EVENT_NAME_LEN+1];
  MeshUtil::getEventName(eventName, "remoteReset");
  Mesh.subscribe(eventName, handleRemoteResetEvent);
}

