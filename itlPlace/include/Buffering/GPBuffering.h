#pragma once

#include "NetListBufferingAlgorithm.h"

class GPBuffering: public PathBasedBuffering
{
public:
    GPBuffering(HDesign& hd);
    void DoBuffering(AppCtx& context, double HPWL, double LHPWL);

private:
    void Plot(AppCtx* context);

    void Init2(AppCtx* context, double HPWL, double LHPWL);

    void FillBinTable(AppCtx* context, std::vector<HCriticalPath>& paths);
    void FillBinTablePathBased(AppCtx* context, std::vector<HCriticalPath>& paths);

    void UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant);    
    double CalcBufferArea(AppCtx* context, int colIdx, int rowIdx, BufferPositions& bufferPositions);
    bool TODO_GiveMeProperName(HNetWrapper net);

    RemoveBuffer* removeBuffer;
    std::vector<HCriticalPath> paths;
};
