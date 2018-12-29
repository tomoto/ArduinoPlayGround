#ifndef BME280_h
#define BME280_h

#include <application.h>
#include <Adafruit_BME280.h>

class BME280 {
private:
  Adafruit_BME280 m_bme;
  bool m_active;
  char m_buf[64];
  
public:
  BME280();
  void begin(int address = 0x76);
  const char* getJson();
};

#endif
