#ifndef __HIGH_TIMINGPOINT_H__
#define __HIGH_TIMINGPOINT_H__

#include "HBaseDesignItem.h"
#include "HPin.h"

BEGINITEM(HTimingPoint, HTimingPointWrapper)

  enum Properties
  {
    FallArrivalTime,
    RiseArrivalTime,
    ArrivalTime,
    FallRequiredTime,
    RiseRequiredTime,
    RequiredTime,
    NextPoint,
    PreviousPoint,
    Pin,
    IsTimingStartPoint,
    IsTimingEndPoint,
    FallSlack,
    RiseSlack,
    Slack,
    FallNegativeSlack,
    RiseNegativeSlack,
    NegativeSlack,
    FallArrivalAncestor,
    FallRequiredAncestor,
    RiseArrivalAncestor,
    RiseRequiredAncestor
  };

ENDITEM(HTimingPoint)

BEGINHCOLLECTION(HTimingPoints, HTimingPoint)

  HTimingPoint operator[] (const HPin& pin) const
  {
    return ::__ConstructPtr<HTimingPoint>(pin);
  }

  int StartPointsCount() const { return m_ld->Timing.nTimingStartPoints; }
  int EndPointsCount() const { return m_ld->Timing.nTimingEndPoints; }
  HTimingPoint TopologicalOrderRoot() const { return Null(); }
  HTimingPoint FirstInternalPoint() const { return ::__ConstructPtr<HTimingPoint>(m_ld->Timing.tpFirstInternalPoint); }
  HTimingPoint LastInternalPoint() const { return ::__ConstructPtr<HTimingPoint>(m_ld->Timing.tpLastInternalPoint); }

	///Events
	Event<0> OnInitialize;
	Event<2, int,int> OnGrow; //int pinsFrom, int pinsTo

	///Operations
	void Initialize();
  void Grow(int pointsFrom, int pointsTo);
  void CountStartAndEndPoints();
  void ClearTimingResults();

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(double, HTimingPoint::FallArrivalTime, m_ld->Timing.tpFallArrivalTime)
  PROPERTYA(double, HTimingPoint::RiseArrivalTime, m_ld->Timing.tpRiseArrivalTime)
  PROPERTYA(double, HTimingPoint::FallRequiredTime, m_ld->Timing.tpFallRequiredTime)
  PROPERTYA(double, HTimingPoint::RiseRequiredTime, m_ld->Timing.tpRiseRequiredTime)
  PROPERTYA2(HTimingPoint, HTimingPoint::NextPoint, m_ld->Timing.tpArrivalPropagationOrder);
  PROPERTYA2(HTimingPoint, HTimingPoint::PreviousPoint, m_ld->Timing.tpRequiredPropagationOrder);
  PROPERTYA2(HTimingPoint, HTimingPoint::FallRequiredAncestor, m_ld->Timing.tpFallRequiredAncestorPoint);
  PROPERTYA2(HTimingPoint, HTimingPoint::RiseRequiredAncestor, m_ld->Timing.tpRiseRequiredAncestorPoint);
  PROPERTYA2(HTimingPoint, HTimingPoint::FallArrivalAncestor, m_ld->Timing.tpFallArrivalAncestorPoint);
  PROPERTYA2(HTimingPoint, HTimingPoint::RiseArrivalAncestor, m_ld->Timing.tpRiseArrivalAncestorPoint);

  //getters specializations
  GETTER(double, HTimingPoint::ArrivalTime)
    { return max(GetDouble<HTimingPoint::FallArrivalTime>(arg), GetDouble<HTimingPoint::RiseArrivalTime>(arg)); }

  GETTER(double, HTimingPoint::RequiredTime)
    { return min(GetDouble<HTimingPoint::FallRequiredTime>(arg), GetDouble<HTimingPoint::RiseRequiredTime>(arg)); }

  GETTER(HPin, HTimingPoint::Pin)
    { return ::__ConstructPtr<HPin>(arg); }

  GETTER(bool, HTimingPoint::IsTimingStartPoint);
  GETTER(bool, HTimingPoint::IsTimingEndPoint);

  GETTER(double, HTimingPoint::FallSlack)
    { return GetDouble<HTimingPoint::FallRequiredTime>(arg) - GetDouble<HTimingPoint::FallArrivalTime>(arg); }

  GETTER(double, HTimingPoint::RiseSlack)
    { return GetDouble<HTimingPoint::RiseRequiredTime>(arg) - GetDouble<HTimingPoint::RiseArrivalTime>(arg); }

  GETTER(double, HTimingPoint::Slack)
    { return GetDouble<HTimingPoint::RequiredTime>(arg) - GetDouble<HTimingPoint::ArrivalTime>(arg); }

  GETTER(double, HTimingPoint::FallNegativeSlack)
    { return max(0.0, -GetDouble<HTimingPoint::FallSlack>(arg)); }

  GETTER(double, HTimingPoint::RiseNegativeSlack)
    { return max(0.0, -GetDouble<HTimingPoint::RiseSlack>(arg)); }

  GETTER(double, HTimingPoint::NegativeSlack)
    { return max(0.0, -GetDouble<HTimingPoint::Slack>(arg)); }

//event handlers
private: void PinsGrowEventHandler(int pinsFrom, int pinsTo);

ENDHCOLLECTION(HTimingPoints)

BEGINWRAPPER(HTimingPointWrapper, HTimingPoints)

  //properties specializations
  PROPERTYWD(double, FallArrivalTime)
  PROPERTYWD(double, RiseArrivalTime)
  PROPERTYWD(double, FallRequiredTime)
  PROPERTYWD(double, RiseRequiredTime)
  PROPERTYW(HTimingPoint, NextPoint, Next)
  PROPERTYW(HTimingPoint, PreviousPoint, Previous)
  PROPERTYWD(HTimingPoint, FallRequiredAncestor)
  PROPERTYWD(HTimingPoint, RiseRequiredAncestor)
  PROPERTYWD(HTimingPoint, FallArrivalAncestor)
  PROPERTYWD(HTimingPoint, RiseArrivalAncestor)

	//Getters
  GETTERWD(double, ArrivalTime)
  GETTERWD(double, RequiredTime)
  GETTERWD(HPin, Pin)
  GETTERWD(bool, IsTimingStartPoint)
  GETTERWD(bool, IsTimingEndPoint)
  GETTERWD(double, FallSlack)
  GETTERWD(double, RiseSlack)
  GETTERWD(double, Slack)
  GETTERWD(double, FallNegativeSlack)
  GETTERWD(double, RiseNegativeSlack)
  GETTERWD(double, NegativeSlack)

  ///In place navigation
  HTimingPointWrapper& GoNext() { ::FromID(*this, ::ToID(Next())); return *this; }
  HTimingPointWrapper& GoPrevious() { ::FromID(*this, ::ToID(Previous())); return *this; }

ENDWRAPPER(HTimingPointWrapper)

#endif //__HIGH_TIMINGPOINT_H__