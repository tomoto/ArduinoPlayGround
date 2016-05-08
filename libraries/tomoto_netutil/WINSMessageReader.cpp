#include <WINSMessageReader.h>

using namespace tomoto;

String WINSMessageReader::readName()
{
  String result;
  
  if ((m_ptr[0] & 0xc0) == 0xc0) {
    size_t offset = readUInt16BE() & 0x3fff;
    WINSMessageReader innerReader;
    innerReader.reset(m_begin + offset, m_end);
    return innerReader.readName();
  } else {
    while (m_ptr < m_end) {
      size_t length = *m_ptr++;
      if (length == 0 || length > 63) break;
      if (result.length() > 0) result += '.';
      for (size_t i = 0; i < length; i++) {
        result += (char) *m_ptr++;
      }
    }
    return result;
  }
};

String WINSMessageReader::decodeNetbiosName(const char* rawName, uint8_t* pNetbiosSuffix)
{
  String result;
  
  const char* p = rawName;
  for (int i = 0; i < 16; i++) {
    char h = *p++;
    char l = *p++;
    if (h >= 'A' && h <= 'P' && l >= 'A' && l <= 'P') {
      char c = (((h - 'A') << 4) + (l - 'A'));
      if (i < 15) {
        result += c;
      } else {
        *pNetbiosSuffix = c;
      }
    }
  }
  
  result.trim();
  
  return result;
}
