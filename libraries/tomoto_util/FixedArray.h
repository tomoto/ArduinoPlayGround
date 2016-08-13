#ifndef FixedArray_h
#define FixedArray_h

namespace tomoto {

#include <stddef.h>

template<typename T>
class FixedConstArray
{
protected:
  T* m_contents;
  size_t m_length;
  
public:
  FixedConstArray(const T* contents, size_t length) :
    m_contents(const_cast<T*>(contents)), m_length(length) {}
  
  size_t length() const { return m_length; }
  
  const T* begin() const { return m_contents; }
  const T* end() const { return m_contents + m_length; }
  operator const T*() const { return m_contents; }
};

template<typename T>
class FixedArray : public FixedConstArray<T>
{
public:
  FixedArray(T* contents, size_t length) : FixedConstArray<T>(contents, length) {}
  
  T* begin() { return FixedConstArray<T>::m_contents; }
  T* end() { return FixedConstArray<T>::m_contents + FixedConstArray<T>::m_length; }
  operator T*() { return FixedConstArray<T>::m_contents; }
};

}

#endif
