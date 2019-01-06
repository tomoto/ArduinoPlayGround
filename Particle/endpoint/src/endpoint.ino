#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/EventUtil.h"
#include "util/JsonObject.h"
#include "endpoint/MeshRemoteResetReceiver.h"
#include "util/ExternalAntenna.h"

SYSTEM_THREAD(ENABLED);

ExternalAntenna ant(true); // specify if you use the external antenna or not

MeshRemoteResetReceiver remoteResetReceiver;

Voltage batt = BatteryVoltage::create();
Voltage light(A1, 1.0);
BME280 climate;

JsonObject<EventUtil::EVENT_DATA_LEN> statusJson;

static void updateStatusJson() {
  statusJson.
    clear().
    a("batt", batt.getJson()).
    a("light", light.getJson()).
    a("climate", climate.getJson());
}

void setup() {
  ant.begin();
  waitUntil(Mesh.ready);
  ant.sendStatus();
  
  remoteResetReceiver.begin();
  batt.begin();
  light.begin();
  climate.begin();
}

void loop() {
  updateStatusJson();
  EventUtil::publish("status", statusJson.c_str());
  
  delay(60000);
}
