#ifndef __INDEX_H__
#define __INDEX_H__

#include "stdTypes.h"
#include "Grow.h"

class HIndex
{
private:
  int m_AllocatedSize;
  int m_Size;
  int m_GrowStep;
  IDType* m_Index;

public:

  HIndex(int size = 16, int growStep = 16)
  {
    m_Index = 0;
    m_GrowStep = growStep;
    m_Size = 0;
    m_AllocatedSize = 0;
    Reserve(size);
  }

  void Reserve(int size)
  {
    if(m_Size > size) return;
    ::Grow(&m_Index, m_AllocatedSize, size);
    m_AllocatedSize = size;
  }

  void Clear()
  {
    m_Size = 0;
  }

  void Add(IDType id)
  {
    if(m_Size >= m_AllocatedSize)
      Reserve(m_Size + m_GrowStep);
    m_Index[m_Size++] = id;
  }

  int Size() const
  {
    return m_Size;
  }

  IDType operator[] (int pos) const { return m_Index[pos]; }
};

#endif //__INDEX_H__