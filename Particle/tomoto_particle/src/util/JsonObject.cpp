#include "JsonObject.h"

JsonObjectBase::JsonObjectBase(char* begin, int capacity) : m_begin(begin), m_capacity(capacity) {
  clear();
}

JsonObjectBase& JsonObjectBase::clear() {
  strncpy(m_begin, "{}", m_capacity);
  return *this;
}

JsonObjectBase& JsonObjectBase::a(const char* name, const char* value) {
  char* p = m_begin + strlen(m_begin) - 1;
  bool isFirst = (p == m_begin + 1);
  
  sprintf(p, "%s\"%s\":%s}", (isFirst ? "" : ","), name, value); // TODO: check overrun
  
  return *this;
}

