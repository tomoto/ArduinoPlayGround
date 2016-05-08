#ifndef WiFiAppNetworkConfiguratorView_h
#define WiFiAppNetworkConfiguratorView_h

#include <WString.h>

class ESP8266WebServer;

namespace tomoto {

class WiFiAppNetworkConfigurator;

class WiFiAppNetworkConfiguratorView
{
private:
  ESP8266WebServer* m_webServer;
  WiFiAppNetworkConfigurator* m_configurator;
  
public:
  void init(ESP8266WebServer* webServer, WiFiAppNetworkConfigurator* configurator);
  
private:
  ESP8266WebServer* ws() { return m_webServer; }
  
  void handleStatusGet();
  void handleChangeGet();
  void handleChangePost();
  
  String renderWiFiOptions();
  String renderChangeView();
  String renderStatusView();
};

}

#endif
