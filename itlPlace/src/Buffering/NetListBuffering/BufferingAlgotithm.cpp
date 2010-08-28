#include "BufferingAlgotithm.h"
#include "NetListBufferingAlgorithm.h"
#include "HBaseDesignItem.h"

HVGAlgorithm::HVGAlgorithm(HDesign& hd): design(hd)
{
  typeNetListBuffering = hd.cfg.ValueOf("TypeNetListBuffering", 0);
}

void HVGAlgorithm::NetListBuffering()
{
  if (typeNetListBuffering == 0)
  {
    BufferingAllCriticalPath* bufferingAlgorithm = new BufferingAllCriticalPath(design);
    bufferingAlgorithm->Initialize();
    bufferingAlgorithm->BufferingCriticalPath();
  }
}