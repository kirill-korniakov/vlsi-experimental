#ifndef __LAGRANGIAN_RELAXATION_DATA__
#define __LAGRANGIAN_RELAXATION_DATA__

#include "HDesign.h"

struct LRData
{
  double    alphaTWL;
  double    c;
  double    r;

  void Initialize(HDesign& hd)
  {
    alphaTWL = hd.cfg.ValueOf(".LagrangianRelaxation.alphaTWL", 1.0);
    c = hd.RoutingLayers.Physics.CPerDist;
    r = hd.RoutingLayers.Physics.RPerDist;
  }
};

#endif