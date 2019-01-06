#ifndef ExternalAntenna_h
#define ExternalAntenna_h

#include <application.h>

class ExternalAntenna {
private:
  bool m_enabled;
  
public:
  ExternalAntenna(bool enabled=false);
  
  void begin();
  void enable(bool enabled);
  void sendStatus(); // for testings
};

#endif

