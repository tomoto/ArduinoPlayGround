#ifndef WiFiAppStation_h
#define WiFiAppStation_h

namespace tomoto {

class WiFiAppStation
{
public:
  bool isConnected();
  
  void begin(const char* hostName);
  void loop();
};

}

#endif
