#ifndef Voltage_h
#define Voltage_h

#include <application.h>

class Voltage {
private:
  int m_pin;
  double m_multiplier;
  char m_buf[10];
  
public:
  Voltage(int pin, double multiplier = 1.0);
  void begin();
  double getVoltage() const;
  const char* getJson();
};


class BatteryVoltage {
public:
  static Voltage create();
};

#endif
