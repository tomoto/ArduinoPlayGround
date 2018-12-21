#include "BME280.h"
#include "util/MeshUtil.h"

BME280::BME280() : m_active(false) {
}

void BME280::begin(int address) {
  int retry = 0;
  while (true) {
    if (m_bme.begin(address)) {
      m_active = true;
      MeshUtil::message("BME280 started.");
      return;
    } else {
      if (retry == 2) {
        m_active = false;
        MeshUtil::message("BME280 was not found or could not be initialized.");
        return;
      } else {
        retry++;
        delay(500);
        continue;
      }
    }
  }
}

String BME280::str() {
  if (m_active) {
    char data[64];
    sprintf(data, "{\"t\":\"%f\",\"h\":\"%f\",\"p\":\"%f\"}", m_bme.readTemperature(), m_bme.readHumidity(), m_bme.readPressure());
    return String(data);
  } else {
    return String("null");
  }
}
