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
    RiseArrivalAncestor,
    ArrivalAncestor,

    FallRequiredAncestor,
    RiseRequiredAncestor,
    RequiredAncestor,

    IsRiseArrivalInversed,
    IsFallArrivalInversed,
    IsRiseRequiredInversed,
    IsFallRequiredInversed
  };

ENDITEM(HTimingPoint)

BEGINHCOLLECTION(HTimingPoints, HTimingPoint)
private:
  static const IDType mc_InversedSignalFlag = 0xF0000000;
public:

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

  GETTER(HTimingPoint, HTimingPoint::FallRequiredAncestor)
    { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

  GETTER(HTimingPoint, HTimingPoint::RiseRequiredAncestor)
    { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

  GETTER(HTimingPoint, HTimingPoint::FallArrivalAncestor)
    { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

  GETTER(HTimingPoint, HTimingPoint::RiseArrivalAncestor)
    { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

  GETTER(HTimingPoint, HTimingPoint::RequiredAncestor)
    { return Get<HTimingPoint::RiseRequiredAncestor, HTimingPoint>(arg); }

  GETTER(HTimingPoint, HTimingPoint::ArrivalAncestor)
    { return Get<HTimingPoint::RiseArrivalAncestor, HTimingPoint>(arg); }

  GETTER(bool, HTimingPoint::IsRiseArrivalInversed)
    { return 0 != (m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] & mc_InversedSignalFlag); }

  GETTER(bool, HTimingPoint::IsFallArrivalInversed)
    { return 0 != (m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] & mc_InversedSignalFlag); }

  GETTER(bool, HTimingPoint::IsRiseRequiredInversed)
    { return 0 != (m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] & mc_InversedSignalFlag); }

  GETTER(bool, HTimingPoint::IsFallRequiredInversed)
    { return 0 != (m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] & mc_InversedSignalFlag); }

  //setters specialization
  SETTER(double, HTimingPoint::ArrivalTime)
  {
    Set<HTimingPoint::RiseArrivalTime>(arg, value);
    Set<HTimingPoint::FallArrivalTime>(arg, value);
  }

  SETTER(double, HTimingPoint::RequiredTime)
  {
    Set<HTimingPoint::RiseRequiredTime>(arg, value);
    Set<HTimingPoint::FallRequiredTime>(arg, value);
  }

  SETTERA2(HTimingPoint, HTimingPoint::FallRequiredAncestor, m_ld->Timing.tpFallRequiredAncestorPoint);
  SETTERA2(HTimingPoint, HTimingPoint::RiseRequiredAncestor, m_ld->Timing.tpRiseRequiredAncestorPoint);
  SETTERA2(HTimingPoint, HTimingPoint::FallArrivalAncestor, m_ld->Timing.tpFallArrivalAncestorPoint);
  SETTERA2(HTimingPoint, HTimingPoint::RiseArrivalAncestor, m_ld->Timing.tpRiseArrivalAncestorPoint);

  SETTER(HTimingPoint, HTimingPoint::RequiredAncestor)
  {
    Set<HTimingPoint::RiseRequiredAncestor>(arg, value);
    Set<HTimingPoint::FallRequiredAncestor>(arg, value);
  }

  SETTER(HTimingPoint, HTimingPoint::ArrivalAncestor)
  {
    Set<HTimingPoint::RiseArrivalAncestor>(arg, value);
    Set<HTimingPoint::FallArrivalAncestor>(arg, value);
  }

  SETTER(bool, HTimingPoint::IsRiseArrivalInversed)
  {
    if (value)
      m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] |= mc_InversedSignalFlag;
    else
      m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
  }

  SETTER(bool, HTimingPoint::IsFallArrivalInversed)
  {
    if (value)
      m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] |= mc_InversedSignalFlag;
    else
      m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
  }

  SETTER(bool, HTimingPoint::IsRiseRequiredInversed)
  {
    if (value)
      m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] |= mc_InversedSignalFlag;
    else
      m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
  }

  SETTER(bool, HTimingPoint::IsFallRequiredInversed)
  {
    if (value)
      m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] |= mc_InversedSignalFlag;
    else
      m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
  }

//event handlers
private: void PinsGrowEventHandler(int pinsFrom, int pinsTo);

ENDHCOLLECTION(HTimingPoints)

BEGINWRAPPER(HTimingPointWrapper, HTimingPoints)

  //properties specializations
  PROPERTYWD(double, FallArrivalTime)
  PROPERTYWD(double, RiseArrivalTime)
  PROPERTYWD(double, FallRequiredTime)
  PROPERTYWD(double, RiseRequiredTime)
  PROPERTYWD(double, ArrivalTime)
  PROPERTYWD(double, RequiredTime)
  PROPERTYW(HTimingPoint, NextPoint, Next)
  PROPERTYW(HTimingPoint, PreviousPoint, Previous)
  PROPERTYWD(HTimingPoint, FallRequiredAncestor)
  PROPERTYWD(HTimingPoint, RiseRequiredAncestor)
  PROPERTYWD(HTimingPoint, RequiredAncestor)
  PROPERTYWD(HTimingPoint, FallArrivalAncestor)
  PROPERTYWD(HTimingPoint, RiseArrivalAncestor)
  PROPERTYWD(HTimingPoint, ArrivalAncestor)
  PROPERTYWD(bool, IsFallArrivalInversed)
  PROPERTYWD(bool, IsRiseArrivalInversed)
  PROPERTYWD(bool, IsFallRequiredInversed)
  PROPERTYWD(bool, IsRiseRequiredInversed)

	//Getters
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