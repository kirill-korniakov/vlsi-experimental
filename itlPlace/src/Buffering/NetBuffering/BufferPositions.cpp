#include "BufferPositions.h"

BufferPositions::BufferPositions()
{
  index = 0;
  position = NULL;
  bufferInfo = NULL;
}

BufferPositions::BufferPositions(VanGinnekenTreeNode* pos, BufferInfo* bufInfo, int i)
{
  index = i;
  position = pos;
  bufferInfo = bufInfo;
}

bool BufferPositions::operator > (BufferPositions& element)
{
  if (position->index > element.GetPosition()->index)
    return true;
  return false;
}

bool BufferPositions::operator < (BufferPositions& element)
{
  if (position->index < element.GetPosition()->index)
    return true;
  return false;
}

VanGinnekenTreeNode* BufferPositions::GetPosition()
{
  return position;
}

BufferInfo* BufferPositions::GetBufferInfo()
{
  return bufferInfo;
}

int BufferPositions::GetIndex()
{
  return index;
}

void BufferPositions::SetPosition(VanGinnekenTreeNode* pos)
{
  position = pos;
}

void BufferPositions::SetBufferInfo(BufferInfo* bufInfo)
{
  bufferInfo = bufInfo;
}

void BufferPositions::SetIndex(int i)
{
  index = i;
}