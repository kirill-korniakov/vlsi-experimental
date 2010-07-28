#include "TimingPointUtils.h"
#include "PinUtils.h"

namespace Utils
{
  double TNS(HDesign& design)
  {
    if (!design.TimingPoints.IsInitialized()) return 0.0;
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
    if (!design.TimingPoints.IsInitialized()) return 0.0;
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

  double AverageNS(HDesign& design)
  {
      double tns = Utils::TNS(design);
      int nTEP = design.TimingPoints.EndPointsCount();

      return tns / nTEP;
  }

  double ALL_TNS(HDesign& design)
  {
      if (!design.TimingPoints.IsInitialized()) return 0.0;

      double allTNS = 0.0;
      for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; !IsNull(tp.GoNext()); )
      {
          allTNS += tp.NegativeSlack();
      }

      return allTNS;
  }
  
  void InsertNextPoint(HDesign& design, HTimingPoint pointToPlace, HTimingPoint afterThisPoint)
  {
    //bool isStart = design.GetBool<HTimingPoint::IsTimingStartPoint>(afterThisPoint);
    //bool isEnd = design.GetBool<HTimingPoint::IsTimingEndPoint>(afterThisPoint);
    //string name = Utils::MakePointFullName(design, afterThisPoint);
    //string name1 = Utils::MakePointFullName(design, pointToPlace);
    //ALERT("Inserting [%s] after [%s (%s)]",name1.c_str(), name.c_str(), isStart ? "s" : (isEnd ? "e" : "-")));

    ASSERT(!::IsNull(pointToPlace));
    ASSERT(!::IsNull(afterThisPoint));
    if (design.GetBool<HTimingPoint::IsTimingStartPoint>(pointToPlace)
      || design.GetBool<HTimingPoint::IsTimingEndPoint>(pointToPlace))
    {
      LOGERROR("Unable to insert start/end timing point\n");
      return;
    }
    if (!::IsNull(design.Get<HTimingPoint::NextPoint, HTimingPoint>(pointToPlace))
      || !::IsNull(design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(pointToPlace)))
    {//remove from old position if needed
      Utils::DeletePointInList(design, pointToPlace);
    }

    if (design.GetBool<HTimingPoint::IsTimingStartPoint>(afterThisPoint))
    {
      afterThisPoint = design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(design.TimingPoints.FirstInternalPoint());
      design._Design.Timing.tpFirstInternalPoint = ::ToID(pointToPlace);
    }
    else if (design.GetBool<HTimingPoint::IsTimingEndPoint>(afterThisPoint)
      || afterThisPoint == design.TimingPoints.LastInternalPoint())
    {
      afterThisPoint = design.TimingPoints.LastInternalPoint();
      design._Design.Timing.tpLastInternalPoint = ::ToID(pointToPlace);
    }

    design.Set<HTimingPoint::PreviousPoint>(pointToPlace, afterThisPoint);
    design.Set<HTimingPoint::NextPoint>(pointToPlace, design.Get<HTimingPoint::NextPoint, HTimingPoint>(afterThisPoint));
    design.Set<HTimingPoint::PreviousPoint>(design.Get<HTimingPoint::NextPoint, HTimingPoint>(afterThisPoint), pointToPlace);
    design.Set<HTimingPoint::NextPoint>(afterThisPoint, pointToPlace);
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

    if (pointDelete == design.TimingPoints.FirstInternalPoint())
      design._Design.Timing.tpFirstInternalPoint = ::ToID(nextPoint);

    if (pointDelete == design.TimingPoints.LastInternalPoint())
      design._Design.Timing.tpLastInternalPoint = ::ToID(previousPoint);
  }

  string MakePointFullName(HDesign& design, HTimingPoint point)
  {
    return Utils::MakePinFullName(design, design.Get<HTimingPoint::Pin, HPin>(point));
  }

  void PrintTopologicalOrder(HDesign& design)
  {
    WRITELINE("=================================");
    WRITELINE("=================================");
    for (HTimingPointWrapper point = design[design.TimingPoints.TopologicalOrderRoot()];
      !::IsNull(point.GoNext()); )
    {
      WRITELINE(Utils::MakePointFullName(design, point).c_str());
    }
    WRITELINE("=================================");
    WRITELINE("=================================");
    WRITELINE("");
    WRITELINE("");
  }
}