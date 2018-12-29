#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/EventUtil.h"
#include "util/JsonObject.h"

SYSTEM_THREAD(ENABLED);

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
  waitUntil(Mesh.ready);
  
  batt.begin();
  light.begin();
  climate.begin();
}

void loop() {
  updateStatusJson();
  EventUtil::publish("status", statusJson.c_str());
  
  delay(60000);
}
