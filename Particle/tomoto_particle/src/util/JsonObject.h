#ifndef JsonObject_h
#define JsonObject_h

#include "application.h"

class JsonObject {
private:
  String m_buf;

public:
  JsonObject();
  JsonObject& a(const char* name, const char* value);
  JsonObject& a(const char* name, const String& value);
  String str();
  const char* c_str();
};

#endif
