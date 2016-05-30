#ifndef PopsParserBase_h
#define PopsParserBase_h

template<int N>
class PopsParserBase
{
public:
  static const int TOTAL_POPS = N;
  
private:
  int m_numberOfPops;
  int m_pops[TOTAL_POPS];

public:
  PopsParserBase() : m_numberOfPops(0) {}
  virtual ~PopsParserBase() {}
  
  bool found() const { return m_numberOfPops == TOTAL_POPS; }
  int numberOfPops() const { return m_numberOfPops; }
  int pop(int i) const { return m_pops[i]; }

  void next(char c)
  {
    if (!found()) {
      nextImpl(c);
    }
  }

protected:
  virtual void nextImpl(char c) = 0;
  void addPop(int p) { m_pops[m_numberOfPops++] = p; }
};

#endif

