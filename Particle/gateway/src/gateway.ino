#include "gateway/MeshEventRepeater.h"
#include "instrument/Voltage.h"
#include "instrument/BME280.h"
#include "util/EventUtil.h"
#include "util/JsonObject.h"

MeshEventRepeater repeater;
Voltage battery = BatteryVoltage::create();
Voltage brightness(A1, 1.0);
BME280 climate;

static String statusJson() {
  return JsonObject().
    a("battery", battery.str()).
    a("brightness", brightness.str()).
    a("climate", climate.str()).
    str();
}

static int resetFunc(const char*) {
  System.reset();
}

void setup() {
  Particle.function("reset", resetFunc);
  
  repeater.begin();
  battery.begin();
  brightness.begin();
  climate.begin();
}

void loop() {
  repeater.process();
  
  EventUtil::publish("status", statusJson());
  
  delay(60000);
}
