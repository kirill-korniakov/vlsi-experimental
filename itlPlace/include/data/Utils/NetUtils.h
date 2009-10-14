#ifndef __NETUTILS_H__
#define __NETUTILS_H__

#include "HDesign.h"

namespace Utils
{
  double CalcNetHPWL(HDesign& hd, HNet net);

  double CalculateHPWL(HDesign& hd, bool updateCachedValues);
  double CalculateWeightedHPWL(HDesign& hd, bool updateCachedValues);
  double CalculateHPWLDiff(HDesign& hd, const HNet* nets, int nNets, bool updateCachedValues);
  double CalculateHPWLDiff(HDesign& hd, const HCell* cells, int nCells, bool updateCachedValues);

  void SkipNet(HDesign& hd, HNet net);
  void SkipSpecialNets(HDesign& hd);
  void RemoveNet(HDesign& hd, HNet net);

  void CalculateLNets(HDesign& hd);

};

#endif //__NETUTILS_H__