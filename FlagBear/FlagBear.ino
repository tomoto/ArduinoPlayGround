#include "FlagBear.h"
#include <WiFiApp.h>
#include <ArduinoJson.h>

using namespace tomoto;

const int WAVE_COUNT = 4;
const char* const CT_HTML = "text/html";
const char* const CT_TEXT = "text/plain";
const char* const CT_JSON = "application/json";
const char* const DEGREES_PARAM = "degrees";

void shutdown()
{
  shutdownServo();
}

WiFiApp app("Kuma", WiFiAppDeepSleepConfig(shutdown));

inline ESP8266WebServer* ws() { return app.ws(); }

void handleRoot()
{
  ws()->send(200, CT_HTML, rootView().c_str());
}

int getSpeed()
{
  String speed = ws()->arg("speed");
  if (speed.equalsIgnoreCase("fast")) return SPEED_FAST;
  if (speed.equalsIgnoreCase("slow")) return SPEED_SLOW;
  return SPEED_MODERATE;
}

void sendDegreesResponse(float degrees)
{
  StaticJsonBuffer<100> jb;
  JsonObject& jo = jb.createObject();
  jo[DEGREES_PARAM] = degrees;
  String result;
  jo.printTo(result);
  ws()->send(200, CT_JSON, result);
}

String getConfigInJSON();
void handleConfig()
{
  ws()->send(200, CT_JSON, getConfigInJSON());
}

void handleHandDown(int index)
{
  if (ws()->hasArg(DEGREES_PARAM)) {
    float degrees = ws()->arg(DEGREES_PARAM).toFloat();
    if (degrees < 0 || degrees > 180 || degrees > getConfigUpDegrees(index)) {
      ws()->send(400, CT_TEXT, "Invalid down degrees");
      return;
    }
    setConfigDownDegrees(index, degrees);
  }
  
  moveDown(index, getSpeed());
  sendDegreesResponse(getHand(index));
}

void handleRightDown() { handleHandDown(RIGHT_SERVO_INDEX); }
void handleLeftDown() { handleHandDown(LEFT_SERVO_INDEX); }

void handleHandUp(int index, int waveCount = 0)
{
  if (ws()->hasArg(DEGREES_PARAM)) {
    float degrees = ws()->arg(DEGREES_PARAM).toFloat();
    if (degrees < 0 || degrees > 180 || degrees < getConfigDownDegrees(index)) {
      ws()->send(400, CT_TEXT, "Invalid up degrees");
      return;
    }
    setConfigUpDegrees(index, degrees);
  }

  if (waveCount == 0) {
    moveUp(index, getSpeed());
  } else {
    waveHand(index, getSpeed(), waveCount);
  }
  
  sendDegreesResponse(getHand(index));
}

void handleRightUp() { handleHandUp(RIGHT_SERVO_INDEX); }
void handleLeftUp() { handleHandUp(LEFT_SERVO_INDEX); }
void handleRightWave() { handleHandUp(RIGHT_SERVO_INDEX, WAVE_COUNT); }
void handleLeftWave() { handleHandUp(LEFT_SERVO_INDEX, WAVE_COUNT); }

void handleHand(int index)
{
  if (ws()->hasArg(DEGREES_PARAM)) {
    moveTo(index, getSpeed(), ws()->arg(DEGREES_PARAM).toFloat());
  }
  sendDegreesResponse(getHand(index));
}

void handleRight() { handleHand(RIGHT_SERVO_INDEX); }
void handleLeft() { handleHand(LEFT_SERVO_INDEX); }

void setup()
{
  app.init();
  app.begin("kuma", "KumaWiFi", "kumakuma", IPAddress(192, 168, 90, 1));

  initServo();

  ws()->on("/", handleRoot);
  ws()->on("/right", handleRight);
  ws()->on("/right/up", handleRightUp);
  ws()->on("/right/down", handleRightDown);
  ws()->on("/right/wave", handleRightWave);
  ws()->on("/left", handleLeft);
  ws()->on("/left/up", handleLeftUp);
  ws()->on("/left/down", handleLeftDown);
  ws()->on("/left/wave", handleLeftWave);
  ws()->on("/config", handleConfig);

  app.beginWS();
}

void loop()
{
  app.loop();
  delay(100);
}

