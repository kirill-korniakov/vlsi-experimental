#ifndef __GROW_H__
#define __GROW_H__

#include <memory.h>
#include "Configuration.h"

#pragma warning( push )
#pragma warning(disable:4345)

template<class T>
void Grow(T** data, int _from, int _to)
{
  if(_from == _to) return;

  T* new_data = _to != 0 ? (T*)(new char[_to * sizeof(T)]) : 0;
  int up = _from < _to ? _from : _to;
  memcpy(new_data, *data, up * sizeof(T));
  for(int i = up; i < _to; i++)
    new ((void*)(new_data+i)) T();
  for(int j = up; j < _from; j++)
    (*data)[j].~T();
  if (_from != 0)
    delete [] (char*)(*data);
  *data = new_data;
}

template<class T>
void GrowFromMiddle(T** data, int _fromLeft, int _fromRight, int _toLeft, int _toRight)
{
  if(_fromLeft == _toLeft && _fromRight == _toRight) return;
  if(_toLeft == 0) _toLeft = 1;
  if(_toRight == 0) _toRight = 1;

  T* new_data = (T*)(new char[(_toLeft + _toRight - 1) * sizeof(T)]) + _toLeft - 1;
  int down = _fromLeft < _toLeft ? _fromLeft : _toLeft;
  int up = _fromRight < _toRight ? _fromRight : _toRight;
  if(up == 0 && down == 0)
    up = 1;
  memcpy(new_data - down + 1, *data - down + 1, (down + up - 1) * sizeof(T));
  for(int i1 = -_toLeft + 1; i1 <= -down; i1++)
    new ((void*)(new_data+i1)) T();
  for(int i2 = up; i2 < _toRight; i2++)
    new ((void*)(new_data+i2)) T();
  for(int j1 = -_fromLeft + 1; j1 <= -down; j1++)
    (*data)[j1].~T();
  for(int j2 = up; j2 < _fromRight; j2++)
    (*data)[j2].~T();
  delete [] (char*)(*data - (_fromLeft == 0 ? 1 : _fromLeft) + 1);
  *data = new_data;
}

#pragma warning( pop )


template<class T>
inline void Swap(T* first, T* second)
{
  T tmp = *first;
  *first = *second;
  *second = tmp;
}

template<class T>
inline void Swap(T& first, T& second)
{
  T tmp = first;
  first = second;
  second = tmp;
}

inline int GetNewSize(
                      libconfig::ConfigExt& cfg,
                      int currentSize,
                      const string& configSectionName,
                      int minAjustment,
                      int minSize,
                      double defGrowPercent)
{
  double growPercent = configSectionName.empty()
    ? defGrowPercent
    : cfg.ValueOf(configSectionName + ".percent", defGrowPercent);
  int minIncrement = configSectionName.empty()
    ? 0
    : cfg.ValueOf(configSectionName + ".min", (int)0);
  if(minIncrement < minAjustment)
    minIncrement = minAjustment;
  int new_size = (int)(currentSize * growPercent);
  if(new_size - currentSize < minIncrement)
    new_size = currentSize + minIncrement;
  return new_size >= minSize ? new_size : minSize;
}

#endif //__GROW_H__