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

//event handlers
private: void PinsGrowEventHandler(int pinsFrom, int pinsTo);

ENDHCOLLECTION(HTimingPoints)

//properties specializations
PROPERTYADECL(HTimingPoints, double, HTimingPoint::FallArrivalTime, m_ld->Timing.tpFallArrivalTime)
PROPERTYADECL(HTimingPoints, double, HTimingPoint::RiseArrivalTime, m_ld->Timing.tpRiseArrivalTime)
PROPERTYADECL(HTimingPoints, double, HTimingPoint::FallRequiredTime, m_ld->Timing.tpFallRequiredTime)
PROPERTYADECL(HTimingPoints, double, HTimingPoint::RiseRequiredTime, m_ld->Timing.tpRiseRequiredTime)
PROPERTYA2DECL(HTimingPoints, HTimingPoint, HTimingPoint::NextPoint, m_ld->Timing.tpArrivalPropagationOrder);
PROPERTYA2DECL(HTimingPoints, HTimingPoint, HTimingPoint::PreviousPoint, m_ld->Timing.tpRequiredPropagationOrder);

//getters specializations
GETTER(HTimingPoints, double, HTimingPoint::ArrivalTime);
GETTER(HTimingPoints, double, HTimingPoint::RequiredTime);
GETTER(HTimingPoints, HPin, HTimingPoint::Pin);
GETTER(HTimingPoints, bool, HTimingPoint::IsTimingStartPoint);
GETTER(HTimingPoints, bool, HTimingPoint::IsTimingEndPoint);
GETTER(HTimingPoints, double, HTimingPoint::FallSlack);
GETTER(HTimingPoints, double, HTimingPoint::RiseSlack);
GETTER(HTimingPoints, double, HTimingPoint::Slack);
GETTER(HTimingPoints, double, HTimingPoint::FallNegativeSlack);
GETTER(HTimingPoints, double, HTimingPoint::RiseNegativeSlack);
GETTER(HTimingPoints, double, HTimingPoint::NegativeSlack);
GETTER(HTimingPoints, HTimingPoint, HTimingPoint::FallRequiredAncestor);
GETTER(HTimingPoints, HTimingPoint, HTimingPoint::RiseRequiredAncestor);
GETTER(HTimingPoints, HTimingPoint, HTimingPoint::FallArrivalAncestor);
GETTER(HTimingPoints, HTimingPoint, HTimingPoint::RiseArrivalAncestor);
GETTER(HTimingPoints, HTimingPoint, HTimingPoint::RequiredAncestor);
GETTER(HTimingPoints, HTimingPoint, HTimingPoint::ArrivalAncestor);
GETTER(HTimingPoints, bool, HTimingPoint::IsRiseArrivalInversed);
GETTER(HTimingPoints, bool, HTimingPoint::IsFallArrivalInversed);
GETTER(HTimingPoints, bool, HTimingPoint::IsRiseRequiredInversed);
GETTER(HTimingPoints, bool, HTimingPoint::IsFallRequiredInversed);

//setters specialization
SETTER(HTimingPoints, double, HTimingPoint::ArrivalTime);
SETTER(HTimingPoints, double, HTimingPoint::RequiredTime);
SETTERA2DECL(HTimingPoints, HTimingPoint, HTimingPoint::FallRequiredAncestor, m_ld->Timing.tpFallRequiredAncestorPoint);
SETTERA2DECL(HTimingPoints, HTimingPoint, HTimingPoint::RiseRequiredAncestor, m_ld->Timing.tpRiseRequiredAncestorPoint);
SETTERA2DECL(HTimingPoints, HTimingPoint, HTimingPoint::FallArrivalAncestor, m_ld->Timing.tpFallArrivalAncestorPoint);
SETTERA2DECL(HTimingPoints, HTimingPoint, HTimingPoint::RiseArrivalAncestor, m_ld->Timing.tpRiseArrivalAncestorPoint);
SETTER(HTimingPoints, HTimingPoint, HTimingPoint::RequiredAncestor);
SETTER(HTimingPoints, HTimingPoint, HTimingPoint::ArrivalAncestor);
SETTER(HTimingPoints, bool, HTimingPoint::IsRiseArrivalInversed);
SETTER(HTimingPoints, bool, HTimingPoint::IsFallArrivalInversed);
SETTER(HTimingPoints, bool, HTimingPoint::IsRiseRequiredInversed);
SETTER(HTimingPoints, bool, HTimingPoint::IsFallRequiredInversed);

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
