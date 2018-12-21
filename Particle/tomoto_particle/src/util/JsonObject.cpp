#include "JsonObject.h"

JsonObject::JsonObject() {
}

JsonObject& JsonObject::a(const char* name, const char* value) {
  m_buf = m_buf + (m_buf.length() == 0 ? "{" : ",") + "\"" + name + "\":" + value;
  return *this;
}

JsonObject& JsonObject::a(const char* name, const String& value) {
  return a(name, value.c_str());
}

String JsonObject::str() {
  return m_buf + "}";
}

const char* JsonObject::c_str() {
  return str().c_str();
}
