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
  
  void InsertNextPoint(HDesign& design, HTimingPoint pointToPlace, HTimingPoint afterThisPoint)
  {
    if (design.GetBool<HTimingPoint::IsTimingStartPoint>(pointToPlace)
      || design.GetBool<HTimingPoint::IsTimingEndPoint>(pointToPlace))
    {
      LOGERROR("Unable to insert start/end timing point\n");
      return;
    }
    if (!::IsNull(design.Get<HTimingPoint::NextPoint, HTimingPoint>(pointToPlace))
      || !::IsNull(design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(pointToPlace)))
    {//remove from old position if needed
      HTimingPoint prev = design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(pointToPlace);
      HTimingPoint next = design.Get<HTimingPoint::NextPoint, HTimingPoint>(pointToPlace);
      if (design.Get<HTimingPoint::NextPoint, HTimingPoint>(prev) == pointToPlace)
        design.Set<HTimingPoint::NextPoint>(prev, next);
      if (design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(next) == pointToPlace)
        design.Set<HTimingPoint::PreviousPoint>(next, prev);
    }

    bool needRecalc = false;
    if (design.GetBool<HTimingPoint::IsTimingStartPoint>(afterThisPoint))
    {
      afterThisPoint = design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(design.TimingPoints.FirstInternalPoint());
      needRecalc = true;
    } else if (design.GetBool<HTimingPoint::IsTimingEndPoint>(afterThisPoint)
      || afterThisPoint == design.TimingPoints.LastInternalPoint())
    {
      afterThisPoint = design.TimingPoints.LastInternalPoint();
      needRecalc = true;
    }

    design.Set<HTimingPoint::PreviousPoint>(pointToPlace, afterThisPoint);
    design.Set<HTimingPoint::NextPoint>(pointToPlace, design.Get<HTimingPoint::NextPoint, HTimingPoint>(afterThisPoint));
    design.Set<HTimingPoint::PreviousPoint>(design.Get<HTimingPoint::NextPoint, HTimingPoint>(afterThisPoint), pointToPlace);
    design.Set<HTimingPoint::NextPoint>(afterThisPoint, pointToPlace);

    if (needRecalc)
      design.TimingPoints.CountStartAndEndPoints();//we need only adjust first & last internal points
  }

  void DeletePointInList(HDesign& design, HTimingPoint pointDelete)
  {
    bool needRecalc = design.GetBool<HTimingPoint::IsTimingStartPoint>(pointDelete)
      || design.GetBool<HTimingPoint::IsTimingEndPoint>(pointDelete);

    HTimingPoint previousPoint = design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(pointDelete);
    HTimingPoint nextPoint = design.Get<HTimingPoint::NextPoint, HTimingPoint>(pointDelete);

    design.Set<HTimingPoint::PreviousPoint>(nextPoint, previousPoint);
    design.Set<HTimingPoint::NextPoint>(previousPoint, nextPoint);

    design.Set<HTimingPoint::PreviousPoint>(pointDelete, design.TimingPoints.Null());
    design.Set<HTimingPoint::NextPoint>(pointDelete, design.TimingPoints.Null());

    if (needRecalc) design.TimingPoints.CountStartAndEndPoints();
  }
}