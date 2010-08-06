#include "Timing.h"

template<PathExtractionType ext>
inline HTimingPoint GetAncestor(const HTimingPointWrapper& point, SignalDirection dir);

template<>
inline HTimingPoint GetAncestor<PathExtractionType_Arrival>(const HTimingPointWrapper& point, SignalDirection dir)
{
  switch(dir)
  {
  case SignalDirection_Fall:
    return point.FallArrivalAncestor();
  case SignalDirection_Rise:
    return point.RiseArrivalAncestor();
  case SignalDirection_None:
    return point.ArrivalAncestor();
  default:
    LOGCRITICAL("Unsupported signal direction");
    return HTimingPoint();
  }
}

template<>
inline HTimingPoint GetAncestor<PathExtractionType_Required>(const HTimingPointWrapper& point, SignalDirection dir)
{
  switch(dir)
  {
  case SignalDirection_Fall:
    return point.FallRequiredAncestor();
  case SignalDirection_Rise:
    return point.RiseRequiredAncestor();
  case SignalDirection_None:
    return point.RequiredAncestor();
  default:
    LOGCRITICAL("Unsupported signal direction");
    return HTimingPoint();
  }
}

template<PathExtractionType ext>
bool IsSignalInverted(HDesign& design, HTimingPoint current_point, SignalDirection current_dir);

template<>
bool IsSignalInverted<PathExtractionType_Arrival>(HDesign& design,
                                                  HTimingPoint current_point,
                                                  SignalDirection current_dir)
{
  switch(current_dir)
  {
  case SignalDirection_Fall:
    return design.GetBool<HTimingPoint::IsFallArrivalInversed>(current_point);
  case SignalDirection_Rise:
    return design.GetBool<HTimingPoint::IsRiseArrivalInversed>(current_point);
  case SignalDirection_None:
    return false;
  default:
    LOGCRITICAL("Unsupported signal direction");
    return false;
  }
}

template<>
bool IsSignalInverted<PathExtractionType_Required>(HDesign& design,
                                                  HTimingPoint current_point,
                                                  SignalDirection current_dir)
{
  switch(current_dir)
  {
  case SignalDirection_Fall:
    return design.GetBool<HTimingPoint::IsFallRequiredInversed>(current_point);
  case SignalDirection_Rise:
    return design.GetBool<HTimingPoint::IsRiseRequiredInversed>(current_point);
  case SignalDirection_None:
    return false;
  default:
    LOGCRITICAL("Unsupported signal direction");
    return false;
  }
}

template<PathExtractionType ext>
void ExtractPath(HDesign& design, HTimingPoint extractionStartPoint, SignalDirection extractionStartDirection)
{
  HTimingPointWrapper sPoint = design[extractionStartPoint];
  if (sPoint.Slack() < 0.0)
  {
    HCriticalPathPointWrapper first_point = design[design.CriticalPathPoints.AllocatePoint()];
    first_point.SetSignalDirection(extractionStartDirection);
    first_point.SetTimingPoint(sPoint);
    HCriticalPathPointWrapper last_point = first_point;
    SignalDirection current_direction = extractionStartDirection;
    HTimingPointWrapper next_point = sPoint;
    while(!::IsNull(next_point = GetAncestor<ext>(sPoint, current_direction)))
    {
      if(design.Get<HPin::Cell, HCell>(next_point.Pin())
        == design.Get<HPin::Cell, HCell>(sPoint.Pin()))
      {//same cell - signal inversion is possible
        if (ext == PathExtractionType_Arrival)
          if (::IsNull(next_point.Next()) && ::IsNull(next_point.Previous()))//skip clock pin
            break;

        if(IsSignalInverted<ext>(design, sPoint, current_direction))
          if(current_direction == SignalDirection_Rise)
            current_direction = SignalDirection_Fall;
          else if(current_direction == SignalDirection_Fall)
            current_direction = SignalDirection_Rise;
      }

      last_point = design.CriticalPathPoints.AllocatePoint();
      last_point.SetTimingPoint(next_point);
      last_point.SetSignalDirection(current_direction);
      sPoint = next_point;
    }

    HCriticalPath path = design.CriticalPaths.AllocatePath();
    design.Set<HCriticalPath::ExtractionType>(path, ext);
    design.CriticalPathPoints.SetPoints(path, first_point, last_point, ext == PathExtractionType_Arrival);
  }
}

void FindArrivalCriticalPaths(HDesign& design, int signalDirections)
{
  HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
  for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
    ep.GoPrevious() != endPointsEnd; )
  {
    if (signalDirections == 2)
    {
      ExtractPath<PathExtractionType_Arrival>(design, ep, SignalDirection_Fall);
      ExtractPath<PathExtractionType_Arrival>(design, ep, SignalDirection_Rise);
    }
    else if (signalDirections == 1)
    {
      ExtractPath<PathExtractionType_Arrival>(design, ep, SignalDirection_None);
    }
    else
    {
      LOGCRITICAL("Invalid argument signalDirections");
    }
  }
}

void FindRequiredCriticalPaths(HDesign& design, int signalDirections)
{
  HTimingPoint startPointsEnd = design.TimingPoints.FirstInternalPoint();
  for (HTimingPointWrapper sp = design[design.TimingPoints.TopologicalOrderRoot()];
    sp.GoNext() != startPointsEnd; )
  {
    if (signalDirections == 2)
    {
      ExtractPath<PathExtractionType_Required>(design, sp, SignalDirection_Fall);
      ExtractPath<PathExtractionType_Required>(design, sp, SignalDirection_Rise);
    }
    else if (signalDirections == 1)
    {
      ExtractPath<PathExtractionType_Required>(design, sp, SignalDirection_None);
    }
    else
    {
      LOGCRITICAL("Invalid argument signalDirections");
    }
  }
}

void FindCriticalPaths(HDesign& design)
{
  int signalDirections = design.cfg.ValueOf("Timing.SignalDirectionsUsed", 2);
  if (signalDirections < 1 || signalDirections > 2)
    signalDirections = 2;
  int max_critical_paths = signalDirections * (design.TimingPoints.StartPointsCount() + design.TimingPoints.EndPointsCount());
  design.CriticalPaths.Initialize(max_critical_paths);
  design.CriticalPathPoints.Initialize(max_critical_paths 
    * design.cfg.ValueOf("Timing.pointsPerPathBase", 10));
  
  FindArrivalCriticalPaths(design, signalDirections);
  FindRequiredCriticalPaths(design, signalDirections);
}