#ifndef __HIGH_PINTYPE_H__
#define __HIGH_PINTYPE_H__

#include "HBaseDesignItem.h"
#include "HTimingArcType.h"

class HMacroType;

BEGINITEM(HPinType, HPinTypeWrapper)

  enum Properties
  {
    Name,
    OriginalName,
    Function,
    Direction,
    OriginX,
    OriginY,
    FallCapacitance,
    RiseCapacitance,
    Capacitance,
    TimingArcsCount,
    ArcTypesEnumerator
  };

  typedef HEnumerator<HTimingArcType> ArcsEnumerator;
  typedef HEnumeratorW<HTimingArcTypeWrapper, HTimingArcTypes> ArcsEnumeratorW;

ENDITEM(HPinType)

BEGINHCOLLECTION(HPinTypes, HPinType)

  int Count() const {return m_ld->Tech->nPins-1;}

  ///Events
  Event<2, int, int> OnGrow;  ///int nPinsLimit_before, int nPinsLimit_after
  Event<1, int> OnInitialize; ///int nPinsLimit
  Event<1, HPinType> OnPinTypeAllocation;

  ///Operations
  void Initialize(int pinsLimit);
  void Grow(int pinsLimit);
  HPinType AllocatePinType();
  void SetPinTypes(HMacroType macro, HPinType first, HPinType last);

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(string, HPinType::Name, m_ld->Tech->pinName)
  PROPERTYA(string, HPinType::OriginalName, m_ld->Tech->pinOriginalName)
  PROPERTYA(PinFunction, HPinType::Function, m_ld->Tech->pinFunction)
  PROPERTYA(PinDirection, HPinType::Direction, m_ld->Tech->pinDirection)
  PROPERTYA(double, HPinType::OriginX, m_ld->Tech->pinOriginX)
  PROPERTYA(double, HPinType::OriginY, m_ld->Tech->pinOriginY)
  PROPERTYA(double, HPinType::FallCapacitance, m_ld->Tech->pinFallCapacitance)
  PROPERTYA(double, HPinType::RiseCapacitance, m_ld->Tech->pinRiseCapacitance)
  PROPERTYA(double, HPinType::Capacitance, m_ld->Tech->pinCapacitance)

  //getters specializations
  GETTER(int, HPinType::TimingArcsCount)
    { return m_ld->Tech->pinArcEndIdx[ARGID]
           - m_ld->Tech->pinArcStartIdx[ARGID]; }

  GETTER(HPinType::ArcsEnumerator, HPinType::ArcTypesEnumerator)
    { return HPinType::ArcsEnumerator(m_ld->Tech->pinArcStartIdx[ARGID],
                                      m_ld->Tech->pinArcEndIdx[ARGID]); }

  GETTER(HPinType::ArcsEnumeratorW, HPinType::ArcTypesEnumerator); //implemented in HExternalMethods.h

ENDHCOLLECTION(HPinTypes)

BEGINWRAPPER(HPinTypeWrapper, HPinTypes)

  //properties
  PROPERTYWD(string, Name)
  PROPERTYWD(string, OriginalName)
  PROPERTYWD(PinFunction, Function)
  PROPERTYWD(PinDirection, Direction)
  PROPERTYWD(double, OriginX)
  PROPERTYWD(double, OriginY)
  PROPERTYWD(double, FallCapacitance)
  PROPERTYWD(double, RiseCapacitance)
  PROPERTYWD(double, Capacitance)

  //Getters
  GETTERWD(int, TimingArcsCount)
  GETTERW(ArcsEnumerator, ArcTypesEnumerator, GetArcsEnumerator)
  GETTERW(ArcsEnumeratorW, ArcTypesEnumerator, GetArcsEnumeratorW);

ENDWRAPPER(HPinTypeWrapper)

#endif //__HIGH_PINTYPE_H__