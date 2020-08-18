#pragma once

#include <Arduino.h>

class CameraController {
 public:
  typedef void (*SendSignal)();
  typedef void (*TimeFeedback)(int remainingMillis, bool shutterOpen);

 private:
  SendSignal m_sendSignal;
  TimeFeedback m_timeFeedback;
  int m_bulbMillis;
  int m_bufferMillis;
  uint64_t m_startedMillis;
  bool m_shutterOpen;

 public:
  CameraController(SendSignal sendSignal, TimeFeedback timeFeedback)
      : m_sendSignal(sendSignal),
        m_timeFeedback(timeFeedback),
        m_bulbMillis(0),
        m_bufferMillis(5000),
        m_startedMillis(0),
        m_shutterOpen(0) {}

  void release() {
    m_sendSignal();
    if (m_bulbMillis > 0) {
      m_startedMillis = millis();
      m_shutterOpen = true;
      m_timeFeedback(m_bulbMillis, m_shutterOpen);
    }
  }
  
  void stop() {
    if (m_shutterOpen) {
      m_sendSignal();
    }
    m_startedMillis = 0;
    m_shutterOpen = false;
    m_timeFeedback(0, false);
  }

  void setBulbSeconds(int seconds) {
    // stop();
    m_bulbMillis = seconds * 1000;
    // m_timeFeedback(m_bulbMillis, true);
    // delay(1000);
    // m_timeFeedback(0, false);
  }

  void process() {
    if (m_startedMillis == 0) {
      return;
    }

    int d = millis() - m_startedMillis;
    if (d < m_bulbMillis) {
      m_timeFeedback(m_bulbMillis - d, m_shutterOpen);
    } else if (d < m_bulbMillis + m_bufferMillis) {
      if (m_shutterOpen) {
        m_sendSignal();
        m_shutterOpen = false;
      }
      m_timeFeedback(m_bulbMillis + m_bufferMillis - d, m_shutterOpen);
    } else {
      release();
    }
  }

  bool isBulbRunning() const {
    return !!m_startedMillis;
  }
};