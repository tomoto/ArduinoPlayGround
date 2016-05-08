#ifndef WiFiAppDeepSleepConfig_h
#define WiFiAppDeepSleepConfig_h

#include <functional>

namespace tomoto {

struct WiFiAppDeepSleepConfig {
  static const unsigned long DEFAULT_INITIAL_TIMEOUT_MILLIS = 15000;
  static const unsigned long DEFAULT_IN_PROCESS_TIMEOUT_MILLIS = 120000;
  static const unsigned long DEFAULT_DEEP_SLEEP_MILLIS = 90000;
  
  unsigned long initialTimeoutMillis;
  unsigned long inProcessTimeoutMillis;
  unsigned long deepSleepMillis;
  std::function<void(void)> shutdownFunc;
  
  WiFiAppDeepSleepConfig(
      unsigned long initialTimeoutMillis,
      unsigned long inProcessTimeoutMillis,
      unsigned long deepSleepMillis,
      std::function<void(void)> shutdownFunc) :
    initialTimeoutMillis(initialTimeoutMillis),
    inProcessTimeoutMillis(inProcessTimeoutMillis),
    deepSleepMillis(deepSleepMillis),
    shutdownFunc(shutdownFunc)
  {
  }
  
  WiFiAppDeepSleepConfig(std::function<void(void)> shutdownFunc = NULL) :
    initialTimeoutMillis(DEFAULT_INITIAL_TIMEOUT_MILLIS),
    inProcessTimeoutMillis(DEFAULT_IN_PROCESS_TIMEOUT_MILLIS),
    deepSleepMillis(DEFAULT_DEEP_SLEEP_MILLIS),
    shutdownFunc(shutdownFunc)
  {
  }
};

}

#endif
