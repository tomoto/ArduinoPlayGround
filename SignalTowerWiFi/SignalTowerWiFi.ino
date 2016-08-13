#include "SignalOutput.h"
#include "SignalState.h"
#include <UtilityFunctions.h>
#include <FixedArray.h>
#include <ArduinoJson.h>

#include <WiFiApp.h>

using namespace tomoto;

const char* const CT_JSON = "application/json";
const char* const JSON_PARAM = "json";

SignalState signalStatesContents[] = {
  SignalState(12, "blue"),
  SignalState(13, "green"),
  SignalState(14, "yellow"),
  SignalState(15, "red")
};

FixedArray<SignalState> signalStates(signalStatesContents, 4);

WiFiApp app("SignalTower");

void processJson(const String& json) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json.c_str());
  
  if (!root.success()) {
    Serial.print("Message parsing error: ");
    Serial.println(json);
    return;
  }
  
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    const char* mode = root.containsKey(s->name()) ? root[s->name()] : root["all"];
    if (mode) {
      for (const ISignalOutput* const* o = supportedSignalOutputs.begin(); o != supportedSignalOutputs.end(); o++) {
        if (strcmp(mode, (*o)->name()) == 0) {
          s->signalOutput(*o);
          break;
        }
      }
    }
  }
}

void sendStatesResponse()
{
  StaticJsonBuffer<100> jb;
  JsonObject& jo = jb.createObject();
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    jo[s->name()] = s->signalOutput()->name();
  }
  String result;
  jo.printTo(result);
  app.ws()->send(200, CT_JSON, result);
}

void processStates() {
  if (app.ws()->hasArg(JSON_PARAM)) {
    processJson(app.ws()->arg(JSON_PARAM));
  }
  
  sendStatesResponse();
}

void setup() {
  app.init();
  app.begin("signaltower", "SignalTowerWiFi", "signal108", IPAddress(192, 168, 108, 1));
  
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    s->init();
  }
  analogWriteFreq(4000);
  
  app.ws()->on("/states", processStates);
  app.beginWS();
}

void loop() {
  app.loop();
  
  ISignalOutput::resolution_t resolution = 100;
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    resolution = min(s->output(), resolution);
  }
  
  delay(resolution);
}
