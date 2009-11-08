#ifndef __LAGRANGIAN_RELAXATION_DATA__
#define __LAGRANGIAN_RELAXATION_DATA__

#include "HDesign.h"

struct NetLRData
{
  double sourceAFactor;
  std::vector<double> sinkLoad;
  std::vector<double> muNetVector;
};

class LRData
{
 public:
  double    alphaTWL;
  double    c;
  double    r;
  std::vector<NetLRData> netLRData;

  void Initialize(HDesign& hd);

 private:
  void InitializeNets(HDesign& hd);
  void AssignLRData(HDesign &hd, HNets::NetsEnumeratorW net, int netIdx);
};

#endif