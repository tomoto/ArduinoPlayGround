#include <WINSResponder.h>
#include <WINSProcessor.h>
#include <WINSMessageWriter.h>
#include <HardwareSerial.h>

using namespace tomoto;

const int MAX_REQUEST_SIZE = 4096;
const int WINS_PORT = 137;
const int TTL = 60;

WINSResponder::WINSResponder(UDP* udp, bool verbose) : m_udp(udp), m_verbose(verbose)
{
}

void WINSResponder::begin(const char* hostName, const IPAddress& ipAddress)
{
  m_hostName = hostName;
  m_ipAddress = ipAddress;
  m_udp->begin(WINS_PORT);
}

void WINSResponder::processRequest()
{
  int len = m_udp->parsePacket();
  if (len > 0 && len <= MAX_REQUEST_SIZE) {
    m_buf.resize(len);
    m_udp->read(m_buf, len);
    m_udp->flush();
    
    WINSProcessor p(m_verbose);
    p.process(m_buf, len);
    
    while (p.moveToNextQuestion()) {
      if (p.q()->clazz == DNS_CLASS_IN && p.q()->type == DNS_TYPE_NB) {
        IPAddress ipAddress = lookUpIPAddressByHostName(p.q()->name.c_str());
        if (ipAddress != INADDR_NONE) {
          if (m_verbose) {
            Serial.print("WINS: Found name=");
            Serial.print(p.q()->name);
            Serial.print(", ipAddress=");
            Serial.print(ipAddress);
            Serial.println();
          }
          sendBackResponse(*p.h(), *p.q(), ipAddress);
        }
      }
    }
  }
}

void WINSResponder::sendBackResponse(const dns_header_t& h, const question_t& q, const IPAddress& ipAddress)
{
  m_udp->beginPacket(m_udp->remoteIP(), m_udp->remotePort());

  WINSMessageWriter out(m_udp);
  
  out.writeUInt16BE(h.id); // id
  out.writeUInt16BE(0x8580); // flags
  out.writeUInt16BE(0); // qdcount
  out.writeUInt16BE(1); // ancount
  out.writeUInt16BE(0); // nscount
  out.writeUInt16BE(0); // arcount
  
  out.writeName(q.rawName.c_str());
  out.writeUInt16BE(q.type);
  out.writeUInt16BE(q.clazz);
  out.writeUInt32BE(TTL);
  out.writeUInt16BE(6);
  out.writeUInt16BE(0); // nameflags
  out.writeUInt8(ipAddress[0]);
  out.writeUInt8(ipAddress[1]);
  out.writeUInt8(ipAddress[2]);
  out.writeUInt8(ipAddress[3]);
  
  m_udp->endPacket();
}

IPAddress WINSResponder::lookUpIPAddressByHostName(const char* hostName)
{
  if (stricmp(hostName, m_hostName.c_str()) == 0 ||
      stricmp(hostName, (m_hostName + ".local").c_str()) == 0) {
    return m_ipAddress;
  } else {
    return INADDR_NONE;
  }
}
