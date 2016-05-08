#ifndef IPAddressUtil_h
#define IPAddressUtil_h

#include <IPAddress.h>
#include <WString.h>

namespace tomoto {

class IPAddressUtil
{
public:
  static String toString(const IPAddress& ip)
  {
    return String(ip[0]) + "." + ip[1] + "." + ip[2] + "." + ip[3];
  }
};

}

#endif
