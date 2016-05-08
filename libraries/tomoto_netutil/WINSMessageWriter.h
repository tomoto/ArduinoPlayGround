#ifndef WINSMessageWriter_h
#define WINSMessageWriter_h

#include <UDP.h>

namespace tomoto {

class WINSMessageWriter {
private:
  UDP* m_udp;
  
public:
  WINSMessageWriter(UDP* udp) : m_udp(udp)
  {
  }
  
  void writeUInt8(uint8_t value)
  {
    m_udp->write(value);
  }
  
  void writeUInt16BE(uint16_t value)
  {
    m_udp->write(value >> 8);
    m_udp->write(value);
  }
  
  void writeUInt32BE(uint32_t value)
  {
    m_udp->write(value >> 24);
    m_udp->write(value >> 16);
    m_udp->write(value >> 8);
    m_udp->write(value);
  }
  
  void writeName(const char* value);
};

}

#endif
