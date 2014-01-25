#ifndef __HIGH_MACROTYPE_H__
#define __HIGH_MACROTYPE_H__

#include "HBaseDesignItem.h"
#include "HPinType.h"

BEGINITEM(HMacroType, HMacroTypeWrapper)

  enum Properties
  {
    Name,
    OriginalName,
    Type,
    OriginX,
    OriginY,
    SizeX,
    SizeY,
    PinsCount,
    PinTypesEnumerator
  };

  typedef HEnumerator<HPinType> PinsEnumerator;
  typedef HEnumeratorW<HPinTypeWrapper, HPinTypes> PinsEnumeratorW;

ENDITEM(HMacroType)

BEGINHCOLLECTION(HMacroTypes, HMacroType)

  int Count() const {return m_ld->Tech->nMacro - 1;}

  ///Enumerator
  typedef HConstEnumerator<HMacroType,1> Enumerator;
  typedef HEnumeratorW<HMacroTypeWrapper, HMacroTypes> EnumeratorW;
  Enumerator GetEnumerator() const {return Enumerator(m_ld->Tech->nMacro);}
  EnumeratorW GetEnumeratorW();

  ///Events
  Event<2, int, int> OnGrow; ///int nMacrosLimit_before, int nMacrosLimit_after
  Event<1, int> OnInitialize; ///int nMacrosLimit
  Event<1, HMacroType> OnMacroTypeAllocation;
  
  ///Operations
  void Initialize(int macrosLimit);
  void Grow(int macrosLimit);
  HMacroType AllocateMacroType();

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

ENDHCOLLECTION(HMacroTypes)

//properties specializations
PROPERTYADECL(HMacroTypes, string, HMacroType::Name, m_ld->Tech->macroName)
PROPERTYADECL(HMacroTypes, string, HMacroType::OriginalName, m_ld->Tech->macroOriginalName)
PROPERTYADECL(HMacroTypes, MacroType, HMacroType::Type, m_ld->Tech->macroType)
PROPERTYADECL(HMacroTypes, double, HMacroType::SizeX, m_ld->Tech->macroSizeX)
PROPERTYADECL(HMacroTypes, double, HMacroType::SizeY, m_ld->Tech->macroSizeY)
PROPERTYADECL(HMacroTypes, double, HMacroType::OriginX, m_ld->Tech->macroOriginX)
PROPERTYADECL(HMacroTypes, double, HMacroType::OriginY, m_ld->Tech->macroOriginY)

//getters specializations
GETTER(HMacroTypes, int, HMacroType::PinsCount);
GETTER(HMacroTypes, HMacroType::PinsEnumerator, HMacroType::PinTypesEnumerator);
GETTER(HMacroTypes, HMacroType::PinsEnumeratorW, HMacroType::PinTypesEnumerator); //implemented in HExternalMethods.h

BEGINWRAPPER(HMacroTypeWrapper, HMacroTypes)

  //properties
  PROPERTYWD(string, Name)
  PROPERTYWD(string, OriginalName)
  PROPERTYWD(MacroType, Type)
  PROPERTYWD(double, SizeX)
  PROPERTYWD(double, SizeY)
  PROPERTYWD(double, OriginX)
  PROPERTYWD(double, OriginY)

  ///Getters
  GETTERWD(int, PinsCount)
  GETTERW(PinsEnumerator, PinTypesEnumerator, GetEnumerator)
  GETTERW(PinsEnumeratorW, PinTypesEnumerator, GetEnumeratorW)

ENDWRAPPER(HMacroTypeWrapper)

inline HMacroTypes::EnumeratorW HMacroTypes::GetEnumeratorW()
  { return EnumeratorW(this, 1, m_ld->Tech->nMacro); }


#endif //__HIGH_MACROTYPE_H__
