#ifndef JsonObject_h
#define JsonObject_h

#include "application.h"

class JsonObjectBase {
private:
  char* const m_begin;
  const int m_capacity;

protected:
  JsonObjectBase(char* begin, int capacity);
  
public:
  JsonObjectBase& clear();
  JsonObjectBase& a(const char* name, const char* value);
  
  const char* c_str() const { return m_begin; }

private:
  JsonObjectBase(const JsonObjectBase&); // forbidden
};

template<int L>
class JsonObject : public JsonObjectBase {
private:
  char m_buf[L+1];
  
public:
  JsonObject() : JsonObjectBase(m_buf, L+1) {}
};

#endif
