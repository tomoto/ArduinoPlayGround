#include "gateway/MeshEventRepeater.h"
#include "gateway/MeshRemoteResetSender.h"
#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/EventUtil.h"
#include "util/JsonObject.h"

ApplicationWatchdog wd(60000 * 5, System.reset);

MeshEventRepeater repeater;
MeshRemoteResetSender remoteResetSender;

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

static int resetSelfFunc(const char*) {
  System.reset();
  return 0;
}

static int sendResetFunc(const char* deviceID) {
  remoteResetSender.send(deviceID);
  return 0;
}

void setup() {
  Particle.function("resetSelf", resetSelfFunc);
  Particle.function("sendReset", sendResetFunc);
  
  repeater.begin();
  batt.begin();
  light.begin();
  climate.begin();
}

void loop() {
  updateStatusJson();
  EventUtil::publish("status", statusJson.c_str());
  
  delay(60000);
}
