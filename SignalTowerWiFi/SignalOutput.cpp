#include <Arduino.h>
#include "SignalOutput.h"

using namespace tomoto;

class OffSignalOutput : public ISignalOutput
{
public:
  const char* name() const { return "off"; }
  
  resolution_t output(int pin) const {
    analogWrite(pin, 0);
    return UNDEFINED_RESOLUTION;
  }
};

class OnSignalOutput : public ISignalOutput
{
public:
  const char* name() const { return "on"; }
  
  resolution_t output(int pin) const {
    analogWrite(pin, 1023);
    return UNDEFINED_RESOLUTION;
  }
};

class BlinkSignalOutput : public ISignalOutput
{
public:
  const char* name() const { return "blink"; }
  
  resolution_t output(int pin) const {
    int phase = millis() % 4000L;
    if (phase < 2000) {
      analogWrite(pin, 1023);
      return UNDEFINED_RESOLUTION;
    } else if (phase < 3023) {
      long x = 3023 - phase;
      analogWrite(pin, x*x/1024);
      return 10;
    } else {
      analogWrite(pin, 0);
      return UNDEFINED_RESOLUTION;
    }
  }
};

static OffSignalOutput s_offSignalOutput;
static OnSignalOutput s_onSignalOutput;
static BlinkSignalOutput s_blinkSignalOutput;

const ISignalOutput* offSignalOutput = &s_offSignalOutput;
const ISignalOutput* onSignalOutput = &s_onSignalOutput;
const ISignalOutput* blinkSignalOutput = &s_blinkSignalOutput;

const ISignalOutput* supportedSignalOutputsContents[] = {
  offSignalOutput,
  onSignalOutput,
  blinkSignalOutput
};

const FixedConstArray<const ISignalOutput*> supportedSignalOutputs(supportedSignalOutputsContents, 3);
