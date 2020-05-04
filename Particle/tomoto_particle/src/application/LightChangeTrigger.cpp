#include "application/LightChangeTrigger.h"
#include "util/EventUtil.h"
#include "util/LedUtil.h"

const double ENABLED_THRESHOLD = 0.01;

LightChangeTrigger::LightChangeTrigger(
  const Voltage& voltage,
  double threshold,
  const char* eventName,
  bool initialState) :
    m_voltage(voltage),
    m_threshold(threshold),
    m_eventName(eventName),
    m_state(initialState),
    m_isEnabled(false)
{
}

void LightChangeTrigger::begin() {
  double v = m_voltage.getVoltage();
  m_isEnabled = v >= ENABLED_THRESHOLD;
  
  char buf[64];
  sprintf(buf, "LightChangeTrigger %s at %fV", (m_isEnabled ? "enabled" : "disabled"), v);
  EventUtil::message(buf);
}

void LightChangeTrigger::process() {
  bool nextState =
    m_voltage.getVoltage() >= (m_state ? m_threshold : m_threshold * 1.1);
    
  if (m_state != nextState) {
    m_state = nextState;
    EventUtil::publish(m_eventName, nextState ? "1" : "0");
    LedUtil::setBrightness(m_state ? 50 : 5);
  }
}

