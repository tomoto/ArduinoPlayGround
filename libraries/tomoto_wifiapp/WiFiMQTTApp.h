#ifndef WiFiMQTTApp_h
#define WiFiMQTTApp_h

#include <WiFiApp.h>
#include <MQTTConfig.h>
#include <WiFiClient.h>
#include <Adafruit_MQTT_Client.h>

namespace tomoto {

class WiFiMQTTApp : public WiFiApp {
private:
  MQTTConfig m_mqttConfig;
  WiFiClient* m_wifiClient;
  Adafruit_MQTT_Client* m_mqttClient;
  unsigned long m_lastPingTimeMillis;
  unsigned long m_pingIntervalMillis;
  unsigned long m_lastConnectionFailedMillis;
  unsigned long m_reconnectIntervalMillis;
  
public:
  WiFiMQTTApp(const char* appName,
              const WiFiAppDeepSleepConfig &deepSleepConfig = WiFiAppDeepSleepConfig(),
              bool isAPEnabledByDefault = false);
  virtual ~WiFiMQTTApp();
  
  virtual void init();
  virtual void loop();
  virtual MQTTConfig* mqttConfig();
  
  Adafruit_MQTT_Subscribe* subscribe(const char* topic);
  Adafruit_MQTT_Subscribe* subscribeDefaultTopic();
  void beginMQTT();
  
  Adafruit_MQTT_Subscribe* readSubscription(int16_t timeout = 0);
  
  Adafruit_MQTT_Client* mqttClient() { return m_mqttClient; }
  
private:
  void connectMQTT();
};

}

#endif
