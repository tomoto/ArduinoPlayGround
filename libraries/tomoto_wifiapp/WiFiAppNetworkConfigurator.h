#ifndef WiFiAppNetworkConfigurator_h
#define WiFiAppNetworkConfigurator_h

#include <WiFiAccessPointInfo.h>
#include <TinyVector.h>
#include <ErrorInfo.h>

namespace tomoto {

class WiFiAppNetworkConfigurator
{
private:
  TinyVector<WiFiAccessPointInfo> m_scannedAccessPoints;

public:
  struct SwitchWiFiPayload {
    String ssid;
    String password;
  };
  
  void switchWiFi(const SwitchWiFiPayload& payload, ErrorInfo& ex);
  void validateSwitchWiFiPayload(const SwitchWiFiPayload& payload, ErrorInfo& ex);
  
  void scanAccessPoints();
  const TinyVector<WiFiAccessPointInfo>& getScannedAccessPoints() const { return m_scannedAccessPoints; }
};

}

#endif
