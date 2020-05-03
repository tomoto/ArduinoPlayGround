#include <Arduino.h>

// expects
extern String loginTk;
extern String loginAccountId;
extern bool switchState;

// provides
bool setSwitch(bool state, int retry = 0);
bool getSwitch(int retry = 0);
