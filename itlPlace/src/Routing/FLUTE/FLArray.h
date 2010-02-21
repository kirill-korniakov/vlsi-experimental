#include "Grow.h"

template<class T, int initialD, int D = initialD>
class FLArray
{
private:
  T* m_Data;
  int m_Capacity;
public:
  FLArray()
  {
    m_Capacity = initialD + D;
    m_Data = 0;
    ::Grow(&m_Data, 0, m_Capacity);
  }

  FLArray(int initCap)
  {
    m_Capacity = initCap + D;
    m_Data = 0;
    ::Grow(&m_Data, 0, m_Capacity);
  }

  ~FLArray()
  {
    ::Grow(&m_Data, m_Capacity, 0);
    m_Capacity = 0;
  }

  T operator[](int idx) const
  {
    return m_Data[idx];
  }

  T& operator[](int idx)
  {
    if (idx + D < m_Capacity)
      return m_Data[idx];
    else
    {
      int nCap = m_Capacity + D + D;
      if (idx > nCap)
        nCap = idx + 1;
      ::Grow(&m_Data, m_Capacity, nCap);
      m_Capacity = nCap;
      return m_Data[idx];
    }
  }

  T* AsArray()
  {
    return m_Data;
  }

  void Reserve(int requiredCapacity)
  {
    if (requiredCapacity > m_Capacity)
    {
      ::Grow(&m_Data, m_Capacity, requiredCapacity);
      m_Capacity = requiredCapacity;
    }
  }
};