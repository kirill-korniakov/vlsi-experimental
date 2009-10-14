#include "Timing.h"

enum PathExtractionType
{
  PathExtractionType_Arrival,
  PathExtractionType_Required
};

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
  default:
    LOGCRITICAL("Unknown signal direction");
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
  default:
    LOGCRITICAL("Unknown signal direction");
    return HTimingPoint();
  }
}

template<PathExtractionType ext>
bool IsSignalInverted(HDesign& design, HTimingPoint current_point, HTimingPoint next_point, SignalDirection current_dir);

template<>
bool IsSignalInverted<PathExtractionType_Arrival>(HDesign& design,
                                                  HTimingPoint current_point,
                                                  HTimingPoint next_point,
                                                  SignalDirection current_dir)
{
  bool isInverted = false;
  HTimingArcType arc;
  double time;
  if (current_dir == SignalDirection_Rise)
    GetArrivalRisingArc(design, next_point, current_point, &arc, &time, &isInverted);
  else if (current_dir == SignalDirection_Fall)
    GetArrivalFallingArc(design, next_point, current_point, &arc, &time, &isInverted);
  else
    LOGCRITICAL("Unknown signal direction");
  return isInverted;
}

template<>
bool IsSignalInverted<PathExtractionType_Required>(HDesign& design,
                                                  HTimingPoint current_point,
                                                  HTimingPoint next_point,
                                                  SignalDirection current_dir)
{
  bool isInverted = false;
  HTimingArcType arc;
  double time;
  if (current_dir == SignalDirection_Rise)
    GetRequiredRisingArc(design, current_point, next_point, &arc, &time, &isInverted);
  else if (current_dir == SignalDirection_Fall)
    GetRequiredFallingArc(design, current_point, next_point, &arc, &time, &isInverted);
  else
    LOGCRITICAL("Unknown signal direction");
  return isInverted;
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
        if(IsSignalInverted<ext>(design, sPoint, next_point, current_direction))
          if(current_direction == SignalDirection_Rise)
            current_direction = SignalDirection_Fall;
          else
            current_direction = SignalDirection_Rise;
        //if(next_point.RiseArrivalTime() > next_point.FallArrivalTime())
        //  current_direction = SignalDirection_Rise;
        //else if(next_point.RiseArrivalTime() < next_point.FallArrivalTime())
        //  current_direction = SignalDirection_Fall;
      }

      last_point = design.CriticalPathPoints.AllocatePoint();
      last_point.SetTimingPoint(next_point);
      last_point.SetSignalDirection(current_direction);
      sPoint = next_point;
    }

    HCriticalPath path = design.CriticalPaths.AllocatePath();
    design.CriticalPathPoints.SetPoints(path, first_point, last_point, ext == PathExtractionType_Arrival);
  }
}

void FindArrivalCriticalPaths(HDesign& design)
{
  HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
  for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
    ep.GoPrevious() != endPointsEnd; )
  {
    ExtractPath<PathExtractionType_Arrival>(design, ep, SignalDirection_Fall);
    ExtractPath<PathExtractionType_Arrival>(design, ep, SignalDirection_Rise);
  }
}

void FindRequiredCriticalPaths(HDesign& design)
{
  HTimingPoint startPointsEnd = design.TimingPoints.FirstInternalPoint();
  for (HTimingPointWrapper sp = design[design.TimingPoints.TopologicalOrderRoot()];
    sp.GoNext() != startPointsEnd; )
  {
    ExtractPath<PathExtractionType_Required>(design, sp, SignalDirection_Fall);
    ExtractPath<PathExtractionType_Required>(design, sp, SignalDirection_Rise);
  }
}

void FindCriticalPaths(HDesign& design)
{
  int max_critical_paths = 2 * (design.TimingPoints.StartPointsCount() + design.TimingPoints.EndPointsCount());
  design.CriticalPaths.Initialize(max_critical_paths);
  design.CriticalPathPoints.Initialize(max_critical_paths 
    * design.cfg.ValueOf("Timing.pointsPerPathBase", 10));
  
  FindArrivalCriticalPaths(design);
  FindRequiredCriticalPaths(design);
}