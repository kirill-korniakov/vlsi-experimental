#ifndef __HIGH_CRITICAL_PATH_POINT__H__
#define __HIGH_CRITICAL_PATH_POINT__H__

#include "HBaseDesignItem.h"
#include "HTimingPoint.h"

class HCriticalPath;

BEGINITEM(HCriticalPathPoint, HCriticalPathPointWrapper)

  enum Properties
  {
    SignalDirection,
    TimingPoint,
  };

ENDITEM(HCriticalPathPoint)

BEGINHCOLLECTION(HCriticalPathPoints, HCriticalPathPoint)

  ///Events
  Event<1, int> OnInitialize;
  Event<2, int, int> OnGrow; ///int nPointsLimit_before, int nPointsLimit_after
  Event<1, HCriticalPathPoint> OnPointAllocation;
  Event<2, HCriticalPathPoint, HCriticalPathPoint> BeforeSwap; ///HCriticalPathPoint first, HCriticalPathPoint second

  ///Operations
  void Initialize(int pointsLimit);
  void Grow(int pointsLimit);
  HCriticalPathPoint AllocatePoint();
  void Swap(HCriticalPathPoint& first, HCriticalPathPoint& second);
  void SetPoints(HCriticalPath path, HCriticalPathPoint first, HCriticalPathPoint last, bool inverse = false);

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(::SignalDirection, HCriticalPathPoint::SignalDirection, m_ld->Timing.cppSigDirection)
  PROPERTYA2(HTimingPoint, HCriticalPathPoint::TimingPoint, m_ld->Timing.cppTimingPointID)

ENDHCOLLECTION(HCriticalPathPoints)

BEGINWRAPPER(HCriticalPathPointWrapper, HCriticalPathPoints)

  //properties specializations
  PROPERTYWD(::SignalDirection, SignalDirection)
  PROPERTYWD(HTimingPoint, TimingPoint)

ENDWRAPPER(HCriticalPathPointWrapper)

#endif //__HIGH_CRITICAL_PATH_POINT__H__