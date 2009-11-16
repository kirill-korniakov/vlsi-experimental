#ifndef __PINUTILS_H__
#define __PINUTILS_H__

#include "HDesign.h"

namespace Utils
{
  HPin FindCellPinByName(HDesign& design, HCell cell, const string& name);
  HPin FindPrimaryPinByName(HDesign& design, const string& name);
  void AllocatePinsForAllCells(HDesign& design, double reservePercent = 1.25);

  string MakePinFullName(HDesign& design, HPin pin);
};

#endif //__PINUTILS_H__