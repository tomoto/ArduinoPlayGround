#include "util/ExternalAntenna.h"
#include "util/EventUtil.h"

ExternalAntenna::ExternalAntenna(bool enabled) : m_enabled(enabled) {
  // Nothing to do yet
}

void ExternalAntenna::begin() {
  enable(m_enabled);
}

void ExternalAntenna::enable(bool enabled) {
  // https://community.particle.io/t/mesh-range-external-antenna/45595/37
#if PLATFORM_ID == PLATFORM_ARGON
  // ANTSW1(31)=0, ANTSW2(32)=1 by default (= disabled)
  digitalWrite(ANTSW1, enabled);
  digitalWrite(ANTSW2, !enabled);
#elif PLATFORM_ID == PLATFORM_XENON
  // ANTSW1(29)=1, ANTSW2(30)=0 by default (= disabled)
  digitalWrite(ANTSW1, !enabled);
  digitalWrite(ANTSW2, enabled);
#else // PLATFORM_BORON
  digitalWrite(ANTSW1, !enabled);
#endif

  m_enabled = enabled;
}

void ExternalAntenna::sendStatus() {
  char buf[EventUtil::EVENT_DATA_LEN+1];
  sprintf(buf, "ANTSW1(%d)=%d, ANTSW2(%d)=%d", ANTSW1, digitalRead(ANTSW1), ANTSW2, digitalRead(ANTSW2));
  EventUtil::message(buf);
}
