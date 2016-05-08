#ifndef WiFiAccessPointInfo_h
#define WiFiAccessPointInfo_h

#include <stddef.h>
#include <WString.h>

namespace tomoto {

struct WiFiAccessPointInfo
{
  String ssid;
  uint8_t encryptionType;
  int32_t rssi;
  uint8_t* bssid;
  int32_t ch;
  bool isHidden;
};

}

#endif
