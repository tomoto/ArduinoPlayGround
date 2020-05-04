#ifndef _LightChangeTrigger_h_
#define _LightChangeTrigger_h_

#include "instrument/Voltage.h"

class LightChangeTrigger {
private:
  const Voltage& m_voltage;
  double m_threshold;
  const char* m_eventName;
  bool m_state;
  bool m_isEnabled;
  
public:
  LightChangeTrigger(
    const Voltage& voltage,
    double threshold = 0.1,
    const char* eventName = "lightChange",
    bool initialState = true);
  
  void begin();
  void process();
  
  bool isEnabled() const { return m_isEnabled; }
};

#endif
