#ifndef StreamProcessUtil_h
#define StreamProcessUtil_h

using namespace tomoto;

class StringFinder {
private:
  const char* m_pattern;
  const char* m_ptr;

public:
  StringFinder(const char* pattern) : m_pattern(pattern) { reset(); }

  bool found() const { return *m_ptr == 0; }
  void reset() { m_ptr = m_pattern; }
  
  void next(char c)
  {
    if (found()) reset();
    m_ptr = (*m_ptr == c) ? m_ptr+1 : m_pattern;
  }
};

class IntParser {
private:
  int m_value;
  int m_state;

public:
  IntParser() { reset(); }

  bool found() const { return m_state == 2; }
  int value() const { return m_value; }
  void reset() { m_value = 0, m_state = 0; }
  
  void next(char c)
  {
    if (found()) reset();
    
    switch (m_state) {
    case 0: // looking for an integer
      if (isdigit(c)) {
        m_value = c - '0';
        m_state = 1;
      } else {
        // just skip
      }
      break;
    case 1: // parsing integer
      if (isdigit(c)) {
        m_value = m_value * 10 + (c - '0');
      } else {
        m_state = 2;
      }
    }
  }
};

#endif

