#include "gateway/MeshRemoteResetSender.h"
#include "util/MeshUtil.h"

void MeshRemoteResetSender::send(const char* deviceID) {
  char eventName[MeshUtil::EVENT_NAME_LEN+1];
  MeshUtil::getEventName(eventName, deviceID, "remoteReset");
  MeshUtil::publish(eventName);
}

