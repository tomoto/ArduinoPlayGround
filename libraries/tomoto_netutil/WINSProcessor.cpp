#include <WINSProcessor.h>
#include <HardwareSerial.h>
#include <stdio.h>

using namespace tomoto;

WINSProcessor::WINSProcessor(bool verbose) : m_verbose(verbose)
{
}

void WINSProcessor::process(const uint8_t* message, size_t messageLength)
{
  if (m_verbose) {
    char buf[256];
    sprintf(buf, "WINS: Parsing message. length=%d", messageLength);
    Serial.println(buf);
  }
  
  m_reader.reset(message, messageLength);
  
  m_questionIndex = 0;
  
  parseHeader();
}

void WINSProcessor::parseHeader()
{
  m_header.id = m_reader.readUInt16BE();
  m_header.flags = m_reader.readUInt16BE();
  m_header.qdcount = m_reader.readUInt16BE();
  m_header.ancount = m_reader.readUInt16BE();
  m_header.nscount = m_reader.readUInt16BE();
  m_header.arcount = m_reader.readUInt16BE();
  
  if (m_verbose) {
    char buf[256];
    sprintf(buf, "WINS: header id=%d, flags=0x%04x, qdcount=%d, ancount=%d, nscount=%d, arcount=%d",
      m_header.id, m_header.flags, m_header.qdcount, m_header.ancount, m_header.nscount, m_header.arcount);
    Serial.println(buf);
  }
}

void WINSProcessor::parseQuestion()
{
  question_t q;
  q.rawName = m_reader.readName();
  q.type = m_reader.readUInt16BE();
  q.clazz = m_reader.readUInt16BE();
  if (q.clazz == DNS_CLASS_IN && q.type == DNS_TYPE_NB) {
    q.name = WINSMessageReader::decodeNetbiosName(q.rawName.c_str(), &q.netbiosSuffix);
  } else {
    q.name = q.rawName;
  }
  
  m_question = q;
  
  if (m_verbose) {
    char buf[256];
    sprintf(buf, "WINS: question rawName=%s, name=%s, type=%d, clazz=%d, netbiosSuffix=%d",
      q.rawName.c_str(), q.name.c_str(), q.type, q.clazz, q.netbiosSuffix);
    Serial.println(buf);
  }
}

bool WINSProcessor::moveToNextQuestion()
{
  if (m_questionIndex++ < m_header.qdcount) {
    parseQuestion();
    return true;
  } else {
    return false;
  }
}
