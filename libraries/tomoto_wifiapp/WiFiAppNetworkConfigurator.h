#ifndef WiFiAppNetworkConfigurator_h
#define WiFiAppNetworkConfigurator_h

#include <WiFiAccessPointInfo.h>
#include <TinyVector.h>
#include <ErrorInfo.h>

namespace tomoto {

class WiFiAppNetworkConfigurator
{
public:
  struct SwitchWiFiPayload {
    String ssid;
    String password;
  };
  
  void switchWiFi(const SwitchWiFiPayload& payload, ErrorInfo& ex);
  void validateSwitchWiFiPayload(const SwitchWiFiPayload& payload, ErrorInfo& ex);
  
  void scanAccessPoints(TinyVector<WiFiAccessPointInfo>* outAccessPoints);
};

}

#endif
