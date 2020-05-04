#include "util/SimpleTimeout.h"
#include "application.h"

SimpleTimeout::SimpleTimeout(unsigned long timeoutMillis) :
  m_startMillis(millis()),
  m_timeoutMillis(timeoutMillis)
{
}

void SimpleTimeout::wait() {
  long remainingMillis = m_startMillis + m_timeoutMillis - millis();
  if (remainingMillis > 0) {
    delay(remainingMillis);
  }
}
