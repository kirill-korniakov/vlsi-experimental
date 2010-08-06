#ifndef __CELLUTILS_H__
#define __CELLUTILS_H__

#include "HDesign.h"

namespace Utils
{
  void NormalizeCellsOrder(HDesign& design);
  HCell FindCellByName(HDesign& design, const string& name);
  CellOrientation GetCellOrient(const char* orient);
  const char* GetCellOrientStr(CellOrientation orient);
};

#endif //__CELLUTILS_H__