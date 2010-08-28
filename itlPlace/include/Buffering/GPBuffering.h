#ifndef __GPBuffering_H__
#define __GPBuffering_H__

#include "NetBufferingAlgorithm.h"

class GPBuffering: public NetBufferingAlgorithm
{
public:
  GPBuffering(HDesign& hd);
  double CalcBufferArea(AppCtx* context, int colIdx, int rowIdx, BufferPositions& bufferPositions);
  virtual int UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant);
  virtual int SetBinTableBuffer(AppCtx* context, double HPWL, double LHPWL);
};
#endif