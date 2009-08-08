#ifndef __TIMINGARCUTILS_H__
#define __TIMINGARCUTILS_H__

#include "HDesign.h"

namespace Utils
{
  TimingSense GetTimingSense(const char* sense);
  const char* GetTimingSenseStr(TimingSense sense);

  TimingType GetTimingType(const char* typeStr);
  const char* GetTimingTypeStr(TimingType ttype);
};

#endif //__TIMINGARCUTILS_H__