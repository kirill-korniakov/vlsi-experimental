#ifndef __BufferPositions_H__
#define __BufferPositions_H__

#include "BufferInfo.h"
#include "VanGinnekenTreeNode.h"

class BufferPositions
{
public:
  BufferPositions();
  BufferPositions(VanGinnekenTreeNode* pos, BufferInfo* bufInfo, int i);

  bool operator > (BufferPositions& element);
  bool operator < (BufferPositions& element);

  VanGinnekenTreeNode* GetPosition();
  BufferInfo* GetBufferInfo();
  int GetIndex();

  void SetPosition(VanGinnekenTreeNode* pos);
  void SetBufferInfo(BufferInfo* bufInfo);
  void SetIndex(int i);

  ~BufferPositions()
  {
    position = NULL;
    bufferInfo = NULL;
  }

protected:
  VanGinnekenTreeNode* position;
  BufferInfo* bufferInfo;
  int index;
};

#endif