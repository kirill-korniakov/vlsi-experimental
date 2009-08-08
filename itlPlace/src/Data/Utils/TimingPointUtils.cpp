#include "TimingPointUtils.h"

namespace Utils
{
  double TNS(HDesign& design)
  {
    double tns = 0.0;
    HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
    for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
      ep.GoPrevious() != endPointsEnd; )
    {
      tns += ep.NegativeSlack();
    }

    return tns;
  }

  double WNS(HDesign& design)
  {
    double wns = 0.0;
    HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
    for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
      ep.GoPrevious() != endPointsEnd; )
    {
      double ns = ep.NegativeSlack();
      if (ns > wns)
        wns = ns;
    }

    return wns;
  }
}