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
#include <functional>

namespace tomoto {

class MQTTConfig;

class WiFiApp {
private:
  String m_appName;
  bool m_isAPEnabled;
  WiFiAppDeepSleepConfig m_deepSleepConfig;
  std::function<void(unsigned long)> m_waitForNetworkCallback;
  
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
  virtual ~WiFiApp();
  
  ESP8266WebServer* ws() { return &m_webServer; }
  bool isAPEnabled() const { return m_isAPEnabled; }
  
  virtual void init();
  void begin(const char* hostName, const char* apSSID, const char* apPassword, const IPAddress& apIP);
  void beginWS();
  virtual void loop();
  
  bool waitForNetwork(unsigned long timeoutMillis);
  void waitForNetworkOrDeepSleep(unsigned long timeoutMillis, unsigned long deepSleepMillis, std::function<void(void)> shutdownFunc);
  void goToDeepSleep();
  void goToDeepSleep(unsigned long deepSleepMillis, std::function<void(void)> shutdownFunc);
  void setWaitForNetworkCallback(std::function<void(unsigned long)> f) { m_waitForNetworkCallback = f; }
  
  virtual MQTTConfig* mqttConfig();

private:
  void waitForEnableAccessPoint();
  void handleRootDefault();
};

}

#endif
