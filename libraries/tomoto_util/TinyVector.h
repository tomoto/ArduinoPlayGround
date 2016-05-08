#ifndef TinyVector_h
#define TinyVector_h

#include <stddef.h>

namespace tomoto {

template <typename T> class TinyVector
{
private:
  T* m_buf;
  size_t m_size;
  size_t m_capacity;
  
public:
  TinyVector(size_t size = 0) : m_buf(NULL), m_size(0), m_capacity(0)
  {
    resize(size);
  }
  
  ~TinyVector()
  {
    clear();
  }
  
  size_t size() { return m_size; }
  size_t capacity() { return m_capacity; }
  
  operator T*() { return m_buf; }
  operator const T*() const { return m_buf; }
  
  T* begin() { return m_buf; }
  const T* begin() const { return m_buf; }
  T* end() { return m_buf + m_size; }
  const T* end() const { return m_buf + m_size; }
  
  void resize(size_t size, bool truncate = false)
  {
    if (truncate || size > m_capacity) {
      clear();
      m_buf = new T[size];
      m_capacity = size;
    }
    m_size = size;
  }
  
  void clear()
  {
    if (m_buf) {
      delete [] m_buf;
      m_buf = NULL;
      m_size = m_capacity = 0;
    }
  }

private:
  TinyVector(const TinyVector& other);
  TinyVector& operator=(const TinyVector& other);
};

}

#endif
