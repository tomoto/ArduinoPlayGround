#ifndef BuildCop_h
#define BuildCop_h

#include <Arduino.h>
#include <functional>

const int PINS_MAX = 2;
const int PINS[PINS_MAX] = { 13, 14 };

inline void forEachPin(std::function<void(int, int)> f)
{
  for (int i = 0; i < PINS_MAX; i++) f(i, PINS[i]);
}

#endif

