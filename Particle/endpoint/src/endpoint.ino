#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/EventUtil.h"
#include "util/JsonObject.h"

SYSTEM_THREAD(ENABLED);

Voltage battery = BatteryVoltage::create();
Voltage brightness(A1, 1.0);
BME280 climate;

String statusJson() {
  return JsonObject().
    a("battery", battery.str()).
    a("brightness", brightness.str()).
    a("climate", climate.str()).
    str();
}

void setup() {
  waitUntil(Mesh.ready);
  
  battery.begin();
  brightness.begin();
  climate.begin();
}

void loop() {
  EventUtil::publish("status", statusJson());
  
  delay(60000);
}
