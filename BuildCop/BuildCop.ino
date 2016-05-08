#include "BuildCop.h"
#include <WiFiApp.h>
#include <ArduinoJson.h>

using namespace tomoto;

const int DEFAULT_DURATION = 10;
const int DEFAULT_POWER = 100;

const char* const CT_HTML = "text/html";
const char* const CT_JSON = "application/json";

class LightStatus {
private:
  unsigned long m_turnOffTime; // millis
  int m_power; // percent

public:
  LightStatus() : m_turnOffTime(0), m_power(0) {}

  void setStatus(unsigned long turnOffTime, int power)
  {
    if (power > 0 && turnOffTime > 0) {
      m_power = power;
      m_turnOffTime = turnOffTime;
    } else {
      m_power = 0;
      m_turnOffTime = 0;
    }
  }

  int power() const { return m_power; }
  int rawPower() const { return m_power / 100.0 * 1024.0; }

  bool shouldTurnOff() const { return m_turnOffTime > 0 && m_turnOffTime <= millis(); }
  
  unsigned long untilTurnOff() const
  {
    unsigned long currentTime = millis();
    return m_turnOffTime > currentTime ? m_turnOffTime - currentTime : 0L;
  }
} lightStatuses[PINS_MAX];

WiFiApp app("BuildCop");

inline ESP8266WebServer* ws() { return app.ws(); }

inline int getIntArg(ESP8266WebServer* ws, const String& name, int defaultValue, int min, int max)
{
  if (ws->hasArg(name)) {
    int value = ws->arg(name).toInt();
    if (value >= min && value <= max) {
      return value;
    }
  }
  return defaultValue;
}

void handleRoot()
{
  ws()->send(200, CT_HTML, renderRootView());
}

void sendStatus(int lightId)
{
  StaticJsonBuffer<100> jb;
  JsonObject& jo = jb.createObject();

  LightStatus& status = lightStatuses[lightId];
  jo["lightId"] = lightId;
  jo["power"] = status.power();
  jo["duration"] = status.untilTurnOff() / 1000.0;

  String json;
  jo.printTo(json);
  ws()->send(200, CT_JSON, json);
}

void turnLightOn(int lightId, int duration, int power)
{
  LightStatus& status = lightStatuses[lightId];
  status.setStatus(millis() + duration * 1000L, power);
  analogWrite(PINS[lightId], status.rawPower());
}

void handleLightOn(int lightId)
{
  int duration = getIntArg(ws(), "duration", DEFAULT_DURATION, 0, 60);
  int power = getIntArg(ws(), "power", DEFAULT_POWER, 0, 100);
  turnLightOn(lightId, duration, power);
  
  sendStatus(lightId);
}

void handleLight(int lightId)
{
  sendStatus(lightId);
}

void setup() {
  app.init();

  forEachPin([](int i, int p) { pinMode(p, OUTPUT); });
  
  app.begin("buildcop", "BuildCopWiFi", "buildfailure", IPAddress(192, 168, 52, 1));
  
  ws()->on("/", handleRoot);
  
  forEachPin([](int i, int p) {
    String path = "/light/";
    path += i;
    ws()->on(path.c_str(), std::bind(handleLight, i));
    path += "/on";
    ws()->on(path.c_str(), std::bind(handleLightOn, i));
  });
  
  app.beginWS();
}

void loop() {
  app.loop();

  forEachPin([](int i, int p) {
    LightStatus& status = lightStatuses[i];
    if (status.shouldTurnOff()) {
      analogWrite(p, 0);
      status.setStatus(0, 0);
    }
  });
  
  delay(100);
}

