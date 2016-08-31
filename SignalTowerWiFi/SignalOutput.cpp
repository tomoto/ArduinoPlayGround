#include <Arduino.h>
#include "SignalOutput.h"

using namespace tomoto;

const int ANALOG_RES = 1024;
const int ANALOG_MAX = ANALOG_RES - 1;

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
    analogWrite(pin, ANALOG_MAX);
    return UNDEFINED_RESOLUTION;
  }
};

class BlinkSignalOutput : public ISignalOutput
{
public:
  const char* name() const { return "blink"; }
  
  resolution_t output(int pin) const {
    int phase = millis() % 3000L;
    if (phase < 1500) {
      analogWrite(pin, ANALOG_MAX);
      return UNDEFINED_RESOLUTION;
    } else if (phase < 1500 + ANALOG_RES) {
      long x = ANALOG_MAX - (phase - 1500);
      analogWrite(pin, x*x/ANALOG_RES);
      return 10;
    } else {
      analogWrite(pin, 0);
      return UNDEFINED_RESOLUTION;
    }
  }
};

class FlashSignalOutput : public ISignalOutput
{
public:
  const char* name() const { return "flash"; }
  
  resolution_t output(int pin) const {
    int phase = millis() % 500L;
    analogWrite(pin, phase < 300 ? ANALOG_MAX : 0);
    return UNDEFINED_RESOLUTION;
  }
};

static OffSignalOutput s_offSignalOutput;
static OnSignalOutput s_onSignalOutput;
static BlinkSignalOutput s_blinkSignalOutput;
static FlashSignalOutput s_flashSignalOutput;

const ISignalOutput* offSignalOutput = &s_offSignalOutput;
const ISignalOutput* onSignalOutput = &s_onSignalOutput;
const ISignalOutput* blinkSignalOutput = &s_blinkSignalOutput;
const ISignalOutput* flashSignalOutput = &s_flashSignalOutput;

const ISignalOutput* supportedSignalOutputsContents[] = {
  offSignalOutput,
  onSignalOutput,
  blinkSignalOutput,
  flashSignalOutput
};

const FixedConstArray<const ISignalOutput*> supportedSignalOutputs(supportedSignalOutputsContents, 4);
