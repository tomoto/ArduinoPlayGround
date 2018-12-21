#include "Voltage.h"

Voltage::Voltage(int pin, double multiplier) :
  m_pin(pin), m_multiplier(multiplier)
{
  // Nothing to do
}

void Voltage::begin() {
  pinMode(m_pin, INPUT);
}

double Voltage::getVoltage() const {
  return analogRead(m_pin) / 4095.0 * 3.3 * m_multiplier;
}

String Voltage::str() const {
  char buf[16];
  sprintf(buf, "{\"v\":%f}", getVoltage());
  return String(buf);
}

Voltage BatteryVoltage::create() {
  const double R1 = 806.0;
  const double R2 = 2000.0;
  return Voltage(BATT, (R1+R2)/R2);
}
