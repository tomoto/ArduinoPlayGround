#include "gateway/MeshEventRepeater.h"
#include "util/CloudUtil.h"
#include "Particle.h"

static void handleMeshEvent(const char *event, const char *data) {
    CloudUtil::publish(event, data);
}

void MeshEventRepeater::begin() {
    Mesh.subscribe("mesh/", handleMeshEvent);
}
