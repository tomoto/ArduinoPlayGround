#include <WiFiAppConfigBase.h>
#include <TinyVector.h>
#include <ArduinoJson.h>

using namespace tomoto;

#define REGION_PROPERTY "region"
#define LAST_TIME_PROPERTY "lastTime"
#define LAST_MODIFIED_PROPERTY "lastModified"
#define SHORT_FORECAST_PROPERTY "shortForecast"
#define WEEKLY_FORECAST_PROPERTY "weeklyForecast"

#define DEFAULT_REGION "319" // Tokyo

class Config : public WiFiAppConfigBase
{
public:
  String region;
  String lastTime;
  String lastModified;
  TinyVector<int> shortForecast;
  TinyVector<int> weeklyForecast;

  Config() : WiFiAppConfigBase("amedes") {}
  Config* operator->() { ensureLoad(); return this; }

protected:
  static void copy(const TinyVector<int>& from, JsonArray& to)
  {
    for (const int* p = from.begin(); p != from.end(); p++) to.add(*p);
  }

  static void copy(JsonArray& from, TinyVector<int>& to)
  {
    to.resize(from.size());
    for (size_t i = 0; i < from.size(); i++) to[i] = from[i];
  }

  virtual void read(JsonObject& jo)
  {
    region = getProperty<const char*>(jo, REGION_PROPERTY, DEFAULT_REGION);
    lastTime = getProperty<const char*>(jo, LAST_TIME_PROPERTY, "");
    lastModified = getProperty<const char*>(jo, LAST_MODIFIED_PROPERTY, "");
    copy(jo[SHORT_FORECAST_PROPERTY], shortForecast);
    copy(jo[WEEKLY_FORECAST_PROPERTY], weeklyForecast);
  }

  virtual void write(JsonObject& jo)
  {
    jo[REGION_PROPERTY] = region;
    jo[LAST_TIME_PROPERTY] = lastTime;
    jo[LAST_MODIFIED_PROPERTY] = lastModified;
    copy(shortForecast, jo.createNestedArray(SHORT_FORECAST_PROPERTY));
    copy(weeklyForecast, jo.createNestedArray(WEEKLY_FORECAST_PROPERTY));
  }
} config;

const char* getConfigRegion() { return config->region.c_str(); }
const char* getConfigLastTime() { return config->lastTime.c_str(); }
const char* getConfigLastModified() { return config->lastModified.c_str(); }
const TinyVector<int>& getConfigShortForecast() { return config->shortForecast; }
const TinyVector<int>& getConfigWeeklyForecast() { return config->weeklyForecast; }

void setConfigRegion(const char* region)
{
  config->region = region;
  config->save();
}

void setConfigLastTime(const char* lastTime, const char* lastModified)
{
  config->lastTime = lastTime;
  config->lastModified = lastModified;
  config->save();
}

void setConfigForecast(const TinyVector<int>& shortForecast, const TinyVector<int>& weeklyForecast)
{
  config->shortForecast.copyFrom(shortForecast);
  config->weeklyForecast.copyFrom(weeklyForecast);
  config->save();
}

String getConfigInJSON()
{
  return config->getConfigInJSON();
}


