#include "BufferingAlgorithm.h"
#include "NetListBufferingAlgorithm.h"
#include "HBaseDesignItem.h"

HVGAlgorithm::HVGAlgorithm(HDesign& hd): design(hd)
{
    typeNetlistBuffering = hd.cfg.ValueOf("TypeNetListBuffering", 0);
}

void HVGAlgorithm::NetlistBuffering()
{
    if (typeNetlistBuffering == 0)
    {
        BufferingAllCriticalPath* bufferingAlgorithm = new BufferingAllCriticalPath(design);
        bufferingAlgorithm->Initialize();
        bufferingAlgorithm->BufferingCriticalPaths();
        delete bufferingAlgorithm;
    }
    else
    {
        PathBasedBuffering* bufferingAlgorithm = new PathBasedBuffering(design);
        bufferingAlgorithm->Initialize();
        bufferingAlgorithm->BufferingNetlist();
        delete bufferingAlgorithm;
    }
}

void HVGAlgorithm::RemoveNewBuffering()
{
    RemoveBuffer removeBuffer = RemoveBuffer(design);
    removeBuffer.RemoveNewBuffering();
}