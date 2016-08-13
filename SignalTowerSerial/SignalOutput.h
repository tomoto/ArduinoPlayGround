#ifndef SignalOutput_h
#define SignalOutput_h

#include <FixedArray.h>

class ISignalOutput
{
public:
  typedef unsigned long resolution_t;
  
  virtual const char* name() const = 0;
  virtual resolution_t output(int pin) const = 0;
  
  // no virtual destructor intentionally defined
};

const ISignalOutput::resolution_t UNDEFINED_RESOLUTION = 1000000L;

extern const ISignalOutput* onSignalOutput;
extern const ISignalOutput* offSignalOutput;
extern const ISignalOutput* blinkSignalOutput;
extern const tomoto::FixedConstArray<const ISignalOutput*> supportedSignalOutputs;

#endif
