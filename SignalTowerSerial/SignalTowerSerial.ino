#include "SignalOutput.h"
#include "SignalState.h"
#include <UtilityFunctions.h>
#include <FixedArray.h>
#include <ArduinoJson.h>

using namespace tomoto;

SignalState signalStatesContents[] = {
  SignalState(5, "blue"),
  SignalState(6, "green"),
  SignalState(9, "yellow"),
  SignalState(10, "red")
};

FixedArray<SignalState> signalStates(signalStatesContents, 4);

void setup() {
  Serial.begin(9600);
  
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    s->init();
  }
  
  Serial.println("Serial port ready.");
}

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
      
      Serial.print(s->name());
      Serial.print(" set to ");
      Serial.println(mode);
      
      for (const ISignalOutput* const* o = supportedSignalOutputs.begin(); o != supportedSignalOutputs.end(); o++) {
        if (strcmp(mode, (*o)->name()) == 0) {
          s->signalOutput(*o);
          break;
        }
      }
    }
  }
}

void loop() {
  while (Serial.available()) {
    delay(10);
    String json = Serial.readStringUntil('\n');
    processJson(json);
  }
  
  ISignalOutput::resolution_t resolution = 100;
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    resolution = min(s->output(), resolution);
  }
  
  delay(resolution);
}
