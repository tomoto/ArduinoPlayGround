#ifndef SignalState_h
#define SignalState_h

class SignalState
{
private:
  int m_pin;
  String m_name;
  const ISignalOutput* m_signalOutput;
  
public:
  SignalState(int pin, const char* name) :
    m_pin(pin), m_name(name), m_signalOutput(offSignalOutput) {}
  
  int pin() const { return m_pin; }
  
  const char* name() const { return m_name.c_str(); }
  
  const ISignalOutput* signalOutput() const { return m_signalOutput; }
  void signalOutput(const ISignalOutput* signalOutput) { m_signalOutput = signalOutput; }
  
  void init() const {
    pinMode(m_pin, OUTPUT);
  }
  
  ISignalOutput::resolution_t output() const {
    return m_signalOutput->output(m_pin);
  }
};

#endif
