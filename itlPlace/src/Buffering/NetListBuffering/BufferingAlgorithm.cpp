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
        ALERT("Simple netlist buffering");
        BufferingAllCriticalPath* bufferingAlgorithm = new BufferingAllCriticalPath(design);
        bufferingAlgorithm->Initialize();
        bufferingAlgorithm->BufferingCriticalPaths();
        delete bufferingAlgorithm;
    }
    else
    {
        ALERT("Path-based buffering");
        PathBasedBuffering* bufferingAlgorithm = new PathBasedBuffering(design);
        bufferingAlgorithm->Initialize();
        
        if (design.cfg.ValueOf("TypePathBased", 0) == 0)
            bufferingAlgorithm->BufferingNetlist();
        else
            bufferingAlgorithm->BufferingNetlist2();

        delete bufferingAlgorithm;
    }
}

void HVGAlgorithm::RemoveNewBuffering()
{
    RemoveBuffer removeBuffer = RemoveBuffer(design);
    removeBuffer.RemoveNewBuffering();
}