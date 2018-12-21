#ifndef Voltage_h
#define Voltage_h

#include <application.h>

class Voltage {
private:
  int m_pin;
  double m_multiplier;
  
public:
  Voltage(int pin, double multiplier);
  void begin();
  double getVoltage() const;
  String str() const;
};


class BatteryVoltage {
public:
  static Voltage create();
};

#endif
