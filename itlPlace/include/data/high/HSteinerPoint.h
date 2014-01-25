#ifndef __HIGH_STEINERPOINT_H__
#define __HIGH_STEINERPOINT_H__

#include "HBaseDesignItem.h"
#include "HPin.h"

#define nullSP m_hd.SteinerPoints.Null()

BEGINITEM(HSteinerPoint, HSteinerPointWrapper)

  enum Properties
  {
    X,
    Y,
    Left,
    Right,
    HasLeft,
    HasRight,
    IsInternal,
    ExtractedR,     //R for following segment
    ExtractedC,     //C for following segment
    RisePointC,     //own point capacitance
    FallPointC,
    PointC,         // equals to RisePointC for now
    RiseObservedC,  //Observed C including this point capacitance
    FallObservedC,
    ObservedC,      // equals to ObservedRiseC for now
    RisePathDelay,
    FallPathDelay,
    PathDelay,      // equals to RisePathDelay for now
    Pin
  };

ENDITEM(HSteinerPoint)

BEGINHCOLLECTION(HSteinerPoints, HSteinerPoint)

  HSteinerPoint operator[] (const HPin& pin) const
  {
    return ::__ConstructPtr<HSteinerPoint>(pin);
  }

  ///Events
  Event<0> OnInitialize;
  Event<4, int, int, int, int> OnGrow; 
    ///int internalPointsCount_before, int internalPointsCount_after
    ///int pinsPointsCount_before, int pinsPointsCount_after
  Event<1, HSteinerPoint> OnSteinerPointAllocation;
  Event<1, HSteinerPoint> OnRemoveSteinerPoint;

  ///Operations
  void Initialize();
  void Grow(int pinsFrom, int internalTo, int pinsTo);
  HSteinerPoint AllocateSteinerPoint();
	void RemoveSteinerTree(HSteinerPoint root);

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //special setters
  void InheritPinCoordinates(HSteinerPoint point); //implemented in HExternalMethods.h

private: void PinsGrowEventHandler(int pinsFrom, int pinsTo);
private: void PinAllocatedEventHandler(HPin pin);

ENDHCOLLECTION(HSteinerPoints)

//properties specializations
PROPERTYA2DECL(HSteinerPoints, HSteinerPoint, HSteinerPoint::Left, m_ld->Routing.rpLeft)
PROPERTYA2DECL(HSteinerPoints, HSteinerPoint, HSteinerPoint::Right, m_ld->Routing.rpRight)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::X, m_ld->Routing.rpX)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::Y, m_ld->Routing.rpY)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::RisePointC, m_ld->RCExtraction.rceRisePointC)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::FallPointC, m_ld->RCExtraction.rceFallPointC)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::PointC, m_ld->RCExtraction.rceRisePointC)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::RiseObservedC, m_ld->RCExtraction.rceRiseObservedC)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::FallObservedC, m_ld->RCExtraction.rceFallObservedC)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::ObservedC, m_ld->RCExtraction.rceRiseObservedC)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::RisePathDelay, m_ld->RCExtraction.rceRisePathDelay)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::FallPathDelay, m_ld->RCExtraction.rceFallPathDelay)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::PathDelay, m_ld->RCExtraction.rceRisePathDelay)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::ExtractedC, m_ld->RCExtraction.rceExtractedC)
PROPERTYADECL(HSteinerPoints, double, HSteinerPoint::ExtractedR, m_ld->RCExtraction.rceExtractedR)

//getters specializations
GETTER(HSteinerPoints, bool, HSteinerPoint::HasLeft);
GETTER(HSteinerPoints, bool, HSteinerPoint::HasRight);
GETTER(HSteinerPoints, bool, HSteinerPoint::IsInternal);
GETTER(HSteinerPoints, HPin, HSteinerPoint::Pin);

BEGINWRAPPER(HSteinerPointWrapper, HSteinerPoints)

  //properties specializations
  PROPERTYWD(HSteinerPoint, Left)
  PROPERTYWD(HSteinerPoint, Right)
  PROPERTYWD(double, X)
  PROPERTYWD(double, Y)
  PROPERTYWD(double, RisePointC)
  PROPERTYWD(double, FallPointC)
  PROPERTYWD(double, PointC)
  PROPERTYWD(double, RiseObservedC)
  PROPERTYWD(double, FallObservedC)
  PROPERTYWD(double, ObservedC)
  PROPERTYWD(double, RisePathDelay)
  PROPERTYWD(double, FallPathDelay)
  PROPERTYWD(double, PathDelay)
  PROPERTYWD(double, ExtractedC)
  PROPERTYWD(double, ExtractedR)

  ///Getters
  GETTERWD(bool, HasLeft)
  GETTERWD(bool, HasRight)
  GETTERWD(bool, IsInternal)
  GETTERWD(HPin, Pin)

  //special setter
  void InheritPinCoordinates() { m_Parent->InheritPinCoordinates(*this); }

  ///In place navigation
  HSteinerPointWrapper& GoLeft() { ::FromID(*this, ::ToID(Left())); return *this; }
  HSteinerPointWrapper& GoRight() { ::FromID(*this, ::ToID(Right())); return *this; }

ENDWRAPPER(HSteinerPointWrapper)

#endif //__HIGH_STEINERPOINT_H__
