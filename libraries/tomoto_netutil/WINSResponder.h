// reference: https://github.com/soramimi/esp8266naming/blob/master/naming.

#ifndef WINSResponder_h
#define WINSResponder_h

#include <UDP.h>
#include <WINSMessage.h>
#include <TinyVector.h>

namespace tomoto {

class WINSResponder {
private:
  UDP* m_udp;
  bool m_verbose;
  String m_hostName;
  IPAddress m_ipAddress;
  TinyVector<uint8_t> m_buf;

public:
  WINSResponder(UDP* udp, bool verbose = false);
  void begin(const char* hostName, const IPAddress& ipAddress);
  void processRequest();
  IPAddress lookUpIPAddressByHostName(const char* hostName);

private:
  void sendBackResponse(const dns_header_t& header, const question_t& q, const IPAddress& ipAddress);
};

}

#endif
