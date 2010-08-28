#ifndef __BufferingAlgotithm_H__
#define __BufferingAlgotithm_H__

#include "NetBufferingAlgorithm.h"

class HVGAlgorithm
{
private:
  int typeNetListBuffering;
  HDesign& design;
public:
  HVGAlgorithm(HDesign& hd);
  void NetListBuffering();
};

#endif