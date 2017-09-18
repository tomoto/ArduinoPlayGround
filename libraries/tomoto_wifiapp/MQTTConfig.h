#ifndef MQTTConfig_h
#define MQTTConfig_h

#include <WiFiAppConfigBase.h>
#include <IPAddressUtil.h>

namespace tomoto {

class MQTTConfig : public WiFiAppConfigBase
{
private:
  String m_defaultTopic;
  
public:
  bool enabled;
  String host;
  int port;
  String topic;
  String user;
  String password;
  
  ErrorInfo ex;
  
  MQTTConfig(const char* defaultTopic) : WiFiAppConfigBase("MQTT"), m_defaultTopic(defaultTopic) {}
  MQTTConfig* operator->() { ensureLoad(); return this; }
  
  bool validate()
  {
    ex.clear();
    if (enabled) {
      if (host.length() == 0) ex.add("Host should not be empty.");
      if (port <= 0) ex.add("Port should be positive.");
      if (topic.length() == 0) ex.add("Topic should not be empty.");
    }
    return !ex;
  }

protected:
  virtual void read(JsonObject& jo)
  {
    enabled = getProperty(jo, "enabled", false);
    host = getProperty(jo, "host", IPAddressUtil::toString(WiFi.gatewayIP()).c_str());
    port = getProperty(jo, "port", 1883);
    topic = getProperty(jo, "topic", m_defaultTopic.c_str());
    user = getProperty(jo, "user", (const char*)"");
    password = getProperty(jo, "password", (const char*)"");
  }
  
  virtual void write(JsonObject& jo)
  {
    jo["enabled"] = enabled;
    jo["host"] = host.c_str();
    jo["port"] = port;
    jo["topic"] = topic.c_str();
    jo["user"] = user.c_str();
    jo["password"] = password.c_str();
  }
};

}
#endif
