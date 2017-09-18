#ifndef ErrorInfor_h
#define ErrorInfor_h

#include <WString.h>

class ErrorInfo
{
private:
  String m_message;
  
public:
  ErrorInfo(const char* message = "") : m_message(message) {}
  void add(const char* message) { addSeparator(); m_message += message; }
  void clear() { m_message = ""; }
  
  const char* message() const { return m_message.c_str(); }
  
  operator bool() const { return m_message.length() > 0; }
  
private:
  void addSeparator() { if (m_message.length() > 0) m_message += "\n"; }
};

#endif
