#ifndef __TIMINGHELPERS_H__
#define __TIMINGHELPERS_H__

#include "HDesign.h"
#include "NetInfo.h"
#include "BufferInfo.h"

class TimingHelper
{
private:
  HDesign& hd;

public:
  TimingHelper(HDesign& design):hd(design){}
  
  double GetBufferedNetMaxDelay(HNet net, NetInfo& ninf, BufferInfo& binf);
  void PrintNetPinsDetails(HNet net, BufferInfo& binf);
};

#endif //__TIMINGHELPERS_H__
