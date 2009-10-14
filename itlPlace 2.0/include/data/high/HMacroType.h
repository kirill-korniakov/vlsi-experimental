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

  //properties specializations
  PROPERTYA(string, HMacroType::Name, m_ld->Tech->macroName)
  PROPERTYA(string, HMacroType::OriginalName, m_ld->Tech->macroOriginalName)
  PROPERTYA(MacroType, HMacroType::Type, m_ld->Tech->macroType)
  PROPERTYA(double, HMacroType::SizeX, m_ld->Tech->macroSizeX)
  PROPERTYA(double, HMacroType::SizeY, m_ld->Tech->macroSizeY)
  PROPERTYA(double, HMacroType::OriginX, m_ld->Tech->macroOriginX)
  PROPERTYA(double, HMacroType::OriginY, m_ld->Tech->macroOriginY)

  //getters specializations
  GETTER(int, HMacroType::PinsCount)
    { return m_ld->Tech->macroPinEnd[ARGID]
           - m_ld->Tech->macroPinStart[ARGID]; }

  GETTER(HMacroType::PinsEnumerator, HMacroType::PinTypesEnumerator)
    { return HMacroType::PinsEnumerator(m_ld->Tech->macroPinStart[ARGID],
                                       m_ld->Tech->macroPinEnd[ARGID]); }

  GETTER(HMacroType::PinsEnumeratorW, HMacroType::PinTypesEnumerator); //implemented in HExternalMethods.h

ENDHCOLLECTION(HMacroTypes)

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