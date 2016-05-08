#ifndef WINSMessageReader_h
#define WINSMessageReader_h

#include <stddef.h>
#include <stdint.h>
#include <WString.h>

namespace tomoto {

class WINSMessageReader
{
private:
  const uint8_t* m_begin;
  const uint8_t* m_end;
  const uint8_t* m_ptr;

public:
  WINSMessageReader() : m_begin(NULL), m_end(NULL), m_ptr(NULL)
  {
  }
  
  void reset(const uint8_t* begin, size_t length)
  {
    reset(begin, begin + length);
  }
  
  void reset(const uint8_t* begin, const uint8_t* end)
  {
    m_begin = begin;
    m_end = end;
    m_ptr = begin;
  }
  
  int checkPointerValidity()
  {
    // 1: not fully consumed yet
    // 0: fully consumed
    // -1: exceeded
    return (m_ptr < m_end) ? 1 : (m_ptr == m_end) ? 0 : -1;
  }
  
  uint16_t readUInt16BE()
  {
    uint16_t i = (m_ptr[0] << 8) + m_ptr[1];
    m_ptr += 2;
    return i;
  }
  
  uint32_t readUInt32BE()
  {
    uint32_t i = (m_ptr[0] << 24) + (m_ptr[1] << 16) + (m_ptr[2] << 8) + m_ptr[3];
    m_ptr += 4;
    return i;
  }
  
  String readName();
  
  static String decodeNetbiosName(const char* rawName, uint8_t* pNetbiosSuffix);
};

}

#endif
