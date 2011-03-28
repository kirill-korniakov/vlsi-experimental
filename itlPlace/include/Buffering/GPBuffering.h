#ifndef __GPBuffering_H__
#define __GPBuffering_H__

#include "NetBufferingAlgorithm.h"
#include "NetListBufferingAlgorithm.h"

class GPBuffering: public PathBasedBuffering
{
public:
    GPBuffering(HDesign& hd);
    virtual void SetBinTableBuffer(AppCtx* context, double HPWL, double LHPWL);
    virtual int SetBinTablePathBasedBuffer(AppCtx* context, double HPWL, double LHPWL);

private:
    double CalcBufferArea(AppCtx* context, int colIdx, int rowIdx, BufferPositions& bufferPositions);
    virtual int UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant);

    void FillBinTable(AppCtx* context, std::vector<HCriticalPath>& paths);
};

#endif