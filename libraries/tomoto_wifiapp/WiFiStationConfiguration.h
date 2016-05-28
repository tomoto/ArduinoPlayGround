#ifndef WiFiStationConfiguration_h
#define WiFiStationConfiguration_h

extern "C" {
#include "user_interface.h"
}

namespace tomoto {

class WiFiStationConfiguration {
private:
  struct station_config m_stationConfig;
  
public:
  WiFiStationConfiguration(bool persistent)
  {
    if (persistent) {
      getPersistent();
    } else {
      getCurrent();
    }
  }
  
  void getPersistent() { wifi_station_get_config_default(&m_stationConfig); }
  void getCurrent() { wifi_station_get_config(&m_stationConfig); }
  void setPresistent() { wifi_station_set_config(&m_stationConfig); }
  void setCurrent() { wifi_station_set_config_current(&m_stationConfig); }
  
  const char* ssid() const { return reinterpret_cast<const char*>(m_stationConfig.ssid); }
  const char* password() const { return reinterpret_cast<const char*>(m_stationConfig.password); }
};

}

#endif
