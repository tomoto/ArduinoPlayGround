#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

void setup(void) 
{
  Serial.begin(115200);

  ina219.begin();
}

void loop(void)
{
  long timestamp = millis();
  float current = ina219.getCurrent_mA();
  Serial.write('$');
  Serial.write(reinterpret_cast<char*>(&timestamp), 4);
  Serial.write(reinterpret_cast<char*>(&current), 4);
}

