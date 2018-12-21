#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/MeshUtil.h"
#include "util/JsonObject.h"

String statusTopic = MeshUtil::eventName("status");

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
  battery.begin();
  brightness.begin();
  climate.begin();
}

void loop() {
  MeshUtil::publish(statusTopic, statusJson());
  
  delay(60000);
}
