#include "SignalOutput.h"
#include "SignalState.h"
#include <UtilityFunctions.h>
#include <FixedArray.h>
#include <ArduinoJson.h>

#include <WiFiApp.h>

using namespace tomoto;

const char* const CT_JSON = "application/json";
const char* const JSON_PARAM = "json";
const char* const AFTER_PARAM = "after";

SignalState signalStatesContents[] = {
  SignalState(12, "blue"),
  SignalState(13, "green"),
  SignalState(14, "yellow"),
  SignalState(15, "red")
};

FixedArray<SignalState> signalStates(signalStatesContents, 4);

struct RequestQueue {
  struct Request {
    String json;
    unsigned long runAt;
  } head;
  void enqueue(const String& json, unsigned long runAt) { head.json = json, head.runAt = runAt; }
  void dequeue() { head.json = "", head.runAt = 0; }
  bool isEmpty() const { return head.runAt == 0; }
} requestQueue;

WiFiApp app("SignalTower");

void processJson(const String& json)
{
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

void processStates()
{
  if (app.ws()->hasArg(JSON_PARAM)) {
    processJson(app.ws()->arg(JSON_PARAM));
  }
  
  sendStatesResponse();
}

void sendQueueResponse()
{
  StaticJsonBuffer<100> jb;
  JsonObject& jo = jb.createObject();
  jo["json"] = requestQueue.head.json;
  jo["runAt"] = requestQueue.head.runAt;
  String result;
  jo.printTo(result);
  app.ws()->send(200, CT_JSON, result);
}

void processQueue()
{
  if (app.ws()->hasArg(JSON_PARAM) && app.ws()->hasArg(AFTER_PARAM)) {
    int after = app.ws()->arg(AFTER_PARAM).toInt();
    requestQueue.enqueue(app.ws()->arg(JSON_PARAM), millis() + after * 1000);
  }
  
  sendQueueResponse();
}

void setup() {
  app.init();
  
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    s->init();
  }
  analogWriteFreq(4000);
  
  app.begin("signaltower", "SignalTowerWiFi", "signal108", IPAddress(192, 168, 108, 1));
  
  app.ws()->on("/states", processStates);
  app.ws()->on("/queue", processQueue);
  app.beginWS();
}

void loop() {
  app.loop();

  if (!requestQueue.isEmpty() && millis() > requestQueue.head.runAt) {
    processJson(requestQueue.head.json);
    requestQueue.dequeue();
  }
  
  ISignalOutput::resolution_t resolution = 100;
  for (SignalState* s = signalStates.begin(); s != signalStates.end(); s++) {
    resolution = min(s->output(), resolution);
  }
  
  delay(resolution);
}
