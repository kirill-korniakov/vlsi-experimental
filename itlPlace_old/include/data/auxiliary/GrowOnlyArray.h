#ifndef __GROWONLYARRAY_H__
#define __GROWONLYARRAY_H__

#include "Grow.h"

template<class T, int StartIndex = 1>
class GrowOnlyArray
{
private:
  T* m_Data;
  int m_Size;
  int m_Count;
  int m_MinAjustment;
  double m_DefGrowPercent;

  int Grow(int currentSize, int minSize)
  {
    int newSize = ::GetNewSize(::gCfg, currentSize, "", m_MinAjustment, minSize, m_DefGrowPercent);
    ::Grow(&m_Data, currentSize, newSize);
    return newSize;
  }

public:

  GrowOnlyArray(int minSize, int minAjustment, double defGrowPercent)
  {
    if (minSize < StartIndex) minSize = StartIndex;

    m_DefGrowPercent = defGrowPercent;
    m_MinAjustment = minAjustment;
    m_Size = Grow(0, minSize);
    m_Count = StartIndex;
  }

  ~GrowOnlyArray()
  {
    ::Grow(&m_Data, m_Size, 0);
    m_Size = m_Count = 0;
  }

  int New()
  {
    if (m_Count >= m_Size)
      m_Size = Grow(m_Size, m_Size + 1);
    return m_Count++;
  }

  void Clear() { m_Count = StartIndex;}

  T& operator[] (int index) { return m_Data[index]; }
  const T& operator[] (int index) const { return m_Data[index]; }

  int GetIndex(const T* item) const { return (int)(item - m_Data);}
};

//#ifndef FIELD_OFFSET
//#define FIELD_OFFSET(type, field)    ((long)(long*)&(((type *)0)->field))
//#endif
//
//template<class T>
//class ArrayList
//{
//  struct Node
//  {
//    int NextNode;
//    T Item;
//  }
//
//  int m_FreeHead;
//  GrowOnlyArray<Node, 0> m_Data;
//};

#endif //__GROWONLYARRAY_H__