#ifndef __TIMINGPOINTUTILS_H__
#define __TIMINGPOINTUTILS_H__

#include "HDesign.h"

namespace Utils
{
  double TNS(HDesign& design);
  double WNS(HDesign& design);

  void InsertNextPoint(HDesign& design, HTimingPoint pointToPlace, HTimingPoint afterThisPoint);
};

#endif //__TIMINGPOINTUTILS_H__