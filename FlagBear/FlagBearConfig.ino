#include <WiFiAppConfigBase.h>

const char* const RIGHT_DOWN_DEGREES_PROPERTY = "rightDownDegrees";
const char* const RIGHT_UP_DEGREES_PROPERTY = "rightUpDegrees";
const char* const LEFT_DOWN_DEGREES_PROPERTY = "leftDownDegrees";
const char* const LEFT_UP_DEGREES_PROPERTY = "leftUpDegrees";

const float DEFAULT_DOWN_DEGREES = 10;
const float DEFAULT_UP_DEGREES = 100;

class Config : public WiFiAppConfigBase
{
public:
  float downDegrees[2];
  float upDegrees[2];

  Config() : WiFiAppConfigBase("Kuma") {}
  Config* operator->() { ensureLoad(); return this; }

protected:
  virtual void read(JsonObject& jo)
  {
    downDegrees[RIGHT_SERVO_INDEX] = getProperty(jo, RIGHT_DOWN_DEGREES_PROPERTY, DEFAULT_DOWN_DEGREES);
    downDegrees[LEFT_SERVO_INDEX] = getProperty(jo, LEFT_DOWN_DEGREES_PROPERTY, DEFAULT_DOWN_DEGREES);
    upDegrees[RIGHT_SERVO_INDEX] = getProperty(jo, RIGHT_UP_DEGREES_PROPERTY, DEFAULT_UP_DEGREES);
    upDegrees[LEFT_SERVO_INDEX] = getProperty(jo, LEFT_UP_DEGREES_PROPERTY, DEFAULT_UP_DEGREES);
  }

  virtual void write(JsonObject& jo)
  {
    jo[RIGHT_DOWN_DEGREES_PROPERTY] = downDegrees[RIGHT_SERVO_INDEX];
    jo[LEFT_DOWN_DEGREES_PROPERTY] = downDegrees[LEFT_SERVO_INDEX];
    jo[RIGHT_UP_DEGREES_PROPERTY] = upDegrees[RIGHT_SERVO_INDEX];
    jo[LEFT_UP_DEGREES_PROPERTY] = upDegrees[LEFT_SERVO_INDEX];
  }
} config;

float getConfigDownDegrees(int index)
{
  return config->downDegrees[index];
}

float getConfigUpDegrees(int index)
{
  return config->upDegrees[index];
}

void setConfigDownDegrees(int index, float degrees)
{
  config->downDegrees[index] = degrees;
  config->save();
}

void setConfigUpDegrees(int index, float degrees)
{
  config->upDegrees[index] = degrees;
  config->save();
}

String getConfigInJSON()
{
  return config->getConfigInJSON();
}

