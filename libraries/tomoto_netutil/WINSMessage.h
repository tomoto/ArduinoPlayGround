#ifndef WINSMessage_h
#define WINSMessage_h

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <WString.h>

namespace tomoto {

const int DNS_TYPE_A = 1;
const int DNS_TYPE_PTR = 12;
const int DNS_TYPE_AAAA = 28;
const int DNS_TYPE_NB = 32;

const int DNS_CLASS_IN = 1;

struct dns_a_record_t {
  uint32_t addr;
};

struct dns_header_t {
  uint16_t id;
  uint16_t flags;
  uint16_t qdcount;
  uint16_t ancount;
  uint16_t nscount;
  uint16_t arcount;
};

struct question_t {
  String rawName;
  String name;
  uint8_t netbiosSuffix;
  uint16_t type;
  uint16_t clazz;
};

}

#endif
