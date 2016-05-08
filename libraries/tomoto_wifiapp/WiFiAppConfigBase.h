#ifndef WiFiAppConfigBase_h
#define WiFiAppConfigBase_h

#include <WString.h>
#include <ArduinoJson.h>

namespace tomoto {

class WiFiAppConfigBase
{
private:
  String m_file;
  bool m_isLoaded;
  
public:
  WiFiAppConfigBase(const char* appName);
  virtual ~WiFiAppConfigBase();
  
  void save();
  
  void ensureLoad();
  
  String getConfigInJSON();
  
protected:
  virtual void read(JsonObject& jo) = 0;
  
  virtual void write(JsonObject& jo) = 0;
  
protected:
  template<typename T> T getProperty(JsonObject& jo, const char* property, const T& defaultValue)
  {
    return jo.containsKey(property) ? T(jo.get(property)) : defaultValue;
  }
};

}

#endif
