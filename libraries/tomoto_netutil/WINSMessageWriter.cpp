#include <WINSMessageWriter.h>

using namespace tomoto;

void WINSMessageWriter::writeName(const char* value)
{
  size_t len = strlen(value);
  
  if (len > 63) {
    return; // TODO error
  }
  
  m_udp->write(len);
  for (size_t i = 0; i < len; i++) {
    m_udp->write(value[i]);
  }
  m_udp->write((uint8_t) 0);
}
