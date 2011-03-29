#pragma once

#include "NetListBufferingAlgorithm.h"

class GPBuffering: public PathBasedBuffering
{
public:
    GPBuffering(HDesign& hd);
    void DoBuffering(AppCtx& context, double HPWL, double LHPWL);

private:
    virtual void SetBinTableBuffer(AppCtx* context, double HPWL, double LHPWL);
    virtual void SetBinTablePathBasedBuffer(AppCtx* context, double HPWL, double LHPWL);
    virtual int UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant);
    
    double CalcBufferArea(AppCtx* context, int colIdx, int rowIdx, BufferPositions& bufferPositions);
    void FillBinTable(AppCtx* context, std::vector<HCriticalPath>& paths);

    RemoveBuffer* removeBuffer;
};