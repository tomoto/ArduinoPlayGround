#ifndef BME280_h
#define BME280_h

#include <application.h>
#include <Adafruit_BME280.h>

class BME280 {
private:
  Adafruit_BME280 m_bme;
  bool m_active;
  
public:
  BME280();
  void begin(int address = 0x76);
  String str();
};

#endif
