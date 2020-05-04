#include "util/LedUtil.h"
#include "application.h"

void LedUtil::setUserSignal(bool value) {
  pinMode(D7, OUTPUT);
  digitalWrite(D7, value ? HIGH : LOW);
}

void LedUtil::setBrightness(int value) {
  RGB.control(true);
  RGB.brightness(value);
  RGB.control(false);
}

