#include "gateway/MeshEventRepeater.h"
#include "util/CloudUtil.h"
#include "Particle.h"

MeshEventRepeater::MeshEventRepeater() {
}

static void handleMeshEvent(const char *event, const char *data) {
    CloudUtil::publish(event, data);
}

void MeshEventRepeater::begin() {
    Mesh.subscribe("mesh/", handleMeshEvent);
}

void MeshEventRepeater::process() {
    // Nothing to do
}
