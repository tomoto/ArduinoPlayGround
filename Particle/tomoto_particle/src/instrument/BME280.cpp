#include "BME280.h"
#include "util/EventUtil.h"

BME280::BME280() : m_active(false) {
}

void BME280::begin(int address) {
  // Start with retry
  int retry = 0;
  while (true) {
    if (m_bme.begin(address)) {
      m_active = true;
      break;
    } else {
      if (retry == 2) {
        m_active = false;
        EventUtil::message("BME280 was not found or could not be initialized.");
        return;
      } else {
        retry++;
        delay(500);
        continue;
      }
    }
  }
  
  m_bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::SAMPLING_X1,
                    Adafruit_BME280::FILTER_OFF);
  
  EventUtil::message("BME280 started at forced mode.");
}

const char* BME280::getJson() {
  if (m_active) {
    m_bme.takeForcedMeasurement();
    sprintf(m_buf, "{\"t\":\"%f\",\"h\":\"%f\",\"p\":\"%f\"}",
      m_bme.readTemperature(),
      m_bme.readHumidity(),
      m_bme.readPressure());
    return m_buf;
  } else {
    return "null";
  }
}
