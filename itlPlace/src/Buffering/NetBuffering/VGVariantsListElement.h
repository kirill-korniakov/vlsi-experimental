#ifndef __VGVariantsListElement_H__
#define __VGVariantsListElement_H__

#include "stdTypes.h"
class BufferPositions;

class VGVariantsListElement
{
protected:
  double RAT;
  double c;
  TemplateTypes<BufferPositions>::list bufferPositions;
  int positionCount;
  int index;

public:
  VGVariantsListElement();

  bool operator > (VGVariantsListElement& element);
  bool operator < (VGVariantsListElement& element);
  bool operator == (VGVariantsListElement& element);
  VGVariantsListElement& operator = (VGVariantsListElement& element);
  void SortBufferPosition();

  double GetRAT();
  double GetC();
  BufferPositions GetStartBufferPosition();
  BufferPositions GetEndBufferPosition();
  BufferPositions GetNextBufferPosition();
  BufferPositions GetbufferPosition(int i);
  int GetPositionCount();
  TemplateTypes<BufferPositions>::list* GetBufferPosition();
  int GetIndex();

  void SetBufferPosition(BufferPositions position);
  void SetBufferPosition(TemplateTypes<BufferPositions>::list position);
  void SetRAT(double rat);
  void SetC(double capacity);
  void SetIndex(int i);
};

#endif