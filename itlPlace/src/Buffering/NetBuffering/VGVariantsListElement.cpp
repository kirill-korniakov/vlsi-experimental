#include "VGVariantsListElement.h"
#include "BufferPositions.h"

VGVariantsListElement::VGVariantsListElement()
{
  RAT = 0;
  c = 0;
  positionCount = 0;
  index = 0;
}

bool VGVariantsListElement::operator > (VGVariantsListElement& element)
{
  if ((c > element.c) && (RAT > element.RAT))
    return true;
  else
    return false;
}

bool VGVariantsListElement::operator < (VGVariantsListElement& element)
{
  if ((c < element.c) && (RAT < element.RAT))
    return false;
  else
    return true;
}

bool VGVariantsListElement::operator == (VGVariantsListElement& element)
{
  if (index == element.index)
    return true;
  else
    return false;
}

VGVariantsListElement& VGVariantsListElement::operator = (VGVariantsListElement& element)
{
  if(&element == this )
    return *this;
  RAT = element.GetRAT();
  c = element.GetC();
  bufferPositions = element.bufferPositions;
  positionCount = element.GetPositionCount();
  index = element.index;
  return *this;
}

void VGVariantsListElement::SortBufferPosition()
{
  bufferPositions.sort();
}

double VGVariantsListElement::GetRAT()
{
  return RAT;
}

double VGVariantsListElement::GetC()
{
  return c;
}

BufferPositions VGVariantsListElement::GetStartBufferPosition()
{
  if (positionCount > 0)
  {
    index = 0;
    return *bufferPositions.begin();
  }
  BufferPositions pos;
  return pos;
}

BufferPositions VGVariantsListElement::GetEndBufferPosition()
{
  if (positionCount > 0)
  {
    index = positionCount - 1;
    return *bufferPositions.end();
  }
  BufferPositions pos;
  return pos;
}

BufferPositions VGVariantsListElement::GetNextBufferPosition()
{
  if ((positionCount > 0) && (index < (positionCount )))
  {
    index++;
    return GetbufferPosition(index);
  }
  BufferPositions pos;
  return pos;
}

BufferPositions VGVariantsListElement::GetbufferPosition(int i)
{
  if ((positionCount > 0) && (i < (positionCount - 1)) && (i >= 0))
  {
    index = i;
    TemplateTypes<BufferPositions>::list::iterator iter = bufferPositions.begin();
    while (iter->GetIndex() != i)
      iter++;
    return *iter;
  }
  BufferPositions pos;
  return pos;
}

int VGVariantsListElement::GetPositionCount()
{
  return positionCount;
}

TemplateTypes<BufferPositions>::list* VGVariantsListElement::GetBufferPosition()
{
  return &bufferPositions;
}

int VGVariantsListElement::GetIndex()
{
  return index;
}

void VGVariantsListElement::SetBufferPosition(BufferPositions position)
{
  bufferPositions.push_back(position);
  positionCount = int (bufferPositions.size());
}
void VGVariantsListElement::SetBufferPosition(TemplateTypes<BufferPositions>::list position)
{
  bufferPositions = position;
  positionCount = int (bufferPositions.size());
}

void VGVariantsListElement::SetRAT(double rat)
{
  RAT = rat;
}
void VGVariantsListElement::SetC(double capacity)
{
  c = capacity;
}

void VGVariantsListElement::SetIndex(int i)
{
  index = i;
}