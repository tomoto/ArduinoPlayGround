#ifndef _SimpleTimeout_h_
#define _SimpleTimeout_h_

class SimpleTimeout {
private:
  unsigned long m_startMillis;
  unsigned long m_timeoutMillis;
  
public:
  SimpleTimeout(unsigned long timeoutMillis);
  void wait();
};

#endif
