#include "gateway/MeshEventRepeater.h"
#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/CloudUtil.h"
#include "util/JsonObject.h"

MeshEventRepeater repeater;
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
  repeater.begin();
  battery.begin();
  brightness.begin();
  climate.begin();
}

void loop() {
  repeater.process();
  
  CloudUtil::publish("gateway/status", statusJson());
  
  delay(60000);
}
