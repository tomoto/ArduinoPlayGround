#ifndef WiFiStatusUtil_h
#define WiFiStatusUtil_h

#include <ESP8266WiFi.h>
#include <WString.h>

namespace tomoto {

class WiFiStatusUtil {
public:
  static String toString(wl_status_t status);
};

}

#endif
