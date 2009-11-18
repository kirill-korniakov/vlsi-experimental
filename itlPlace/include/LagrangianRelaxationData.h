#ifndef __LAGRANGIAN_RELAXATION_DATA__
#define __LAGRANGIAN_RELAXATION_DATA__

#include "HDesign.h"
#include "TimingPointMus.h"

struct NetLRData
{
  double sourceAFactor;
  std::vector<double> sinkLoad;
  std::vector<double> netArcMus;
};

class LRData
{
 public:
  double alphaTWL;
  double c;
  double r;
  std::vector<NetLRData> netLRData;
  TimingPointMus* actualMus;

  LRData();
  ~LRData();

  void Initialize(HDesign& hd);
  void UpdateMultipliers(HDesign& hd);

 private:
  void InitializeNets(HDesign& hd);
  void AssignNetLRData(HDesign& hd, HNetWrapper net, int netIdx);
};

#endif