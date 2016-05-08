#ifndef WiFiAppAccessPoint_h
#define WiFiAppAccessPoint_h

#include <IPAddress.h>

namespace tomoto {

class WiFiAppAccessPoint {
public:
  void begin(const char* apSSID, const char* apPassword, const IPAddress& apIP, const IPAddress& apSubnetMask, const char* hostName);
  void loop();
};

}

#endif
