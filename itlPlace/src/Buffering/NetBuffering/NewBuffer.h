#ifndef __NewBuffer_H__
#define __NewBuffer_H__

#include "BufferPositions.h"

class NewBuffer
{
public:
  HCell cell;
  HPin source;
  HPin sink;
  BufferPositions Positions;
  bool operator > (NewBuffer& element)
  {
    if(Positions > element.Positions)
      return true;
    return false;
  }

  bool operator < (NewBuffer& element)
  {
    if(Positions < element.Positions)
      return true;
    return false;
  }
};

#endif