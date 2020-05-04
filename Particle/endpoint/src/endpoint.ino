#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/EventUtil.h"
#include "util/LedUtil.h"
#include "util/JsonObject.h"
#include "endpoint/MeshRemoteResetReceiver.h"
#include "endpoint/MeshAckReceiver.h"
#include "util/ExternalAntenna.h"
#include "util/CloudUtil.h"
#include "util/SleepUtil.h"
#include "application/LightChangeTrigger.h"

SYSTEM_THREAD(ENABLED);

ExternalAntenna ant(true); // specify if you use the external antenna or not

MeshRemoteResetReceiver remoteResetReceiver;
MeshAckReceiver statusAckReceiver;

Voltage batt = BatteryVoltage::create();
Voltage light(A1, 1.0);
BME280 climate;
LightChangeTrigger lightChangeTrigger(light);

JsonObject<EventUtil::EVENT_DATA_LEN> statusJson;

static void updateStatusJson() {
  statusJson.
    clear().
    a("batt", batt.getJson()).
    a("light", light.getJson()).
    a("climate", climate.getJson()).
    a("keepAwake", statusAckReceiver.isKeepAwake() ? "1" : "0");
}

void setup() {
  LedUtil::setBrightness(50);
  
  ant.begin();
  waitUntil(Mesh.ready);
  ant.sendStatus();
  
  remoteResetReceiver.begin();
  statusAckReceiver.begin("status/ack");
  batt.begin();
  light.begin();
  climate.begin();
  
  delay(100);
  lightChangeTrigger.begin();
}

void loop() {
  updateStatusJson();
  EventUtil::publish("status", statusJson.c_str());
  CloudUtil::publish("hello", "still on?");
  
  if (lightChangeTrigger.isEnabled()) {
    for (int i = 0; i < 60; i++) {
      lightChangeTrigger.process();
      delay(1000);
    }
  } else {
    if (statusAckReceiver.isKeepAwake() ||
        BatteryVoltage::isPowerSupplied(batt.getVoltage())) {
      delay(60000);
    } else {
      delay(2000); // wait for ack
      SleepUtil::sleep(55);
    }
  }
}
