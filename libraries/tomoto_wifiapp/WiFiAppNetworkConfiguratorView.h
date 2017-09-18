#ifndef WiFiAppNetworkConfiguratorView_h
#define WiFiAppNetworkConfiguratorView_h

#include <WString.h>

class ESP8266WebServer;

namespace tomoto {

class WiFiAppNetworkConfigurator;
class MQTTConfig;

class WiFiAppNetworkConfiguratorView
{
private:
  ESP8266WebServer* m_webServer;
  WiFiAppNetworkConfigurator* m_configurator;
  MQTTConfig* m_mqttConfig;
  
public:
  void init(ESP8266WebServer* webServer, WiFiAppNetworkConfigurator* configurator, MQTTConfig* mqttConfig);
  
private:
  ESP8266WebServer* ws() { return m_webServer; }
  
  void handleStatus();
  void handleChangeGet();
  void handleChangePost();
  void handleScanAndChange();
  void handleReconnect();
  void handleDisconnect();
  void handleMqttPost();
  
  String renderWiFiOptions();
  String renderChangeView();
  String renderStatusView();
  String renderScanAndChangeView();
  String renderWaitForStatusView();
};

}

#endif
