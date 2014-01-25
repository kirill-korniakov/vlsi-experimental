#ifndef __HIGH_TIMINGARCTYPE_H__
#define __HIGH_TIMINGARCTYPE_H__

#include "HBaseDesignItem.h"

class HPinType;
class HPin;

BEGINITEM(HTimingArcType, HTimingArcTypeWrapper)

  enum Properties
  {
    Type,
    TimingSense,
    TIntrinsicRise,
    TIntrinsicFall,
    ResistanceRise,
    ResistanceFall,
  };

ENDITEM(HTimingArcType)

BEGINHCOLLECTION(HTimingArcTypes, HTimingArcType)

  int Count() const {return m_ld->Tech->nArcs - 1;}

  ///Events
  Event<2, int, int> OnGrow;  ///int nArcsLimit_before, int nArcsLimit_after
  Event<1, int> OnInitialize; ///int nArcsLimit
  Event<1, HTimingArcType> OnTimingArcTypeAllocation;
  
  ///Operations
  void Initialize(int arcsLimit);
  void Grow(int arcsLimit);
  HTimingArcType AddNextArcToPinType(HPinType startType, HPinType endType);

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //special getters
  HPinType GetStartPinType(HTimingArcType arcType, HPinType endPinType) const;
  HPin GetStartPin(HTimingArcType arcType, HPin endPin) const;
  HPinType GetEndPinType(HTimingArcType arcType, HPinType startPinType) const;
  HPin GetEndPin(HTimingArcType arcType, HPin startPin) const;

  //Event handlers
private: void PinTypesGrowEventHandler(int aFrom, int aTo);
private: void PinTypeAddedEventHandler(HPinType type);

ENDHCOLLECTION(HTimingArcTypes)

//properties specializations
PROPERTYADECL(HTimingArcTypes, TimingType, HTimingArcType::Type, m_ld->Tech->arcType)
PROPERTYADECL(HTimingArcTypes, TimingSense, HTimingArcType::TimingSense, m_ld->Tech->arcSense)
PROPERTYADECL(HTimingArcTypes, double, HTimingArcType::ResistanceRise, m_ld->Tech->arcResistanceRise)
PROPERTYADECL(HTimingArcTypes, double, HTimingArcType::ResistanceFall, m_ld->Tech->arcResistanceFall)
PROPERTYADECL(HTimingArcTypes, double, HTimingArcType::TIntrinsicRise, m_ld->Tech->arcTIntrinsicRise)
PROPERTYADECL(HTimingArcTypes, double, HTimingArcType::TIntrinsicFall, m_ld->Tech->arcTIntrinsicFall)

BEGINWRAPPER(HTimingArcTypeWrapper, HTimingArcTypes)

  //properties
  PROPERTYW(::TimingType, Type, TimingType)
  PROPERTYWD(::TimingSense, TimingSense)
  PROPERTYWD(double, ResistanceRise)
  PROPERTYWD(double, ResistanceFall)
  PROPERTYWD(double, TIntrinsicRise)
  PROPERTYWD(double, TIntrinsicFall)

  //special getters
  HPinType GetStartPinType(HPinType endPinType) const;
  HPin GetStartPin(HPin endPin) const;
  HPinType GetEndPinType(HPinType startPinType) const;
  HPin GetEndPin(HPin startPin) const;

ENDWRAPPER(HTimingArcTypeWrapper)

#endif //__HIGH_TIMINGARCTYPE_H__
