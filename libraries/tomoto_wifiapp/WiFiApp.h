#ifndef WiFiApp_h
#define WiFiApp_h

#include <WString.h>
#include <WiFiAppAccessPoint.h>
#include <WiFiAppStation.h>
#include <WiFiAppNetworkConfigurator.h>
#include <WiFiAppNetworkConfiguratorView.h>
#include <WiFiAppDeepSleepConfig.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

namespace tomoto {

class WiFiApp {
private:
  String m_appName;
  bool m_isAPEnabled;
  WiFiAppDeepSleepConfig m_deepSleepConfig;
  
  String m_hostName;
  WiFiAppAccessPoint m_accessPoint;
  WiFiAppStation m_station;
  WiFiAppNetworkConfigurator m_networkConfigurator;
  WiFiAppNetworkConfiguratorView m_networkConfiguratorView;
  ESP8266WebServer m_webServer;
  
public:
  WiFiApp(const char* appName,
          const WiFiAppDeepSleepConfig &deepSleepConfig = WiFiAppDeepSleepConfig(),
          bool isAPEnabledByDefault = false);
  
  ESP8266WebServer* ws() { return &m_webServer; }
  
  void init();
  void begin(const char* hostName, const char* apSSID, const char* apPassword, const IPAddress& apIP);
  void beginWS();
  void loop();
  
  bool waitForNetwork(unsigned long timeoutMillis);
  void waitForNetworkOrDeepSleep(unsigned long timeoutMillis, unsigned long deepSleepMillis, std::function<void(void)> shutdownFunc);

private:
  void waitForEnableAccessPoint();
  void handleRootDefault();
};

}

#endif
