#ifndef WiFiHTTPClient_h
#define WiFiHTTPClient_h

#include <functional>

namespace tomoto {

class WiFiHTTPClient
{
public:
  int get(const char* uri, std::function<bool(int)> consumer);
};

}

#endif
