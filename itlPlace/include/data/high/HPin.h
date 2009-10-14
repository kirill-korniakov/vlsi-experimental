#ifndef __HIGH_PIN_H__
#define __HIGH_PIN_H__

#include "HBaseDesignItem.h"
#include "HPinType.h"

class HCell;
class HNet;

BEGINITEM(HPin, HPinWrapper)

  enum Properties
  {
    Name,
    OffsetX,
    OffsetY,
    Type,
    Direction,
    X,
    Y,
    Net,
    OriginalNet,
    Cell,
    IsPrimary,
    IsPrimaryInput,
    IsPrimaryOutput
  };

ENDITEM(HPin)

BEGINHCOLLECTION(HPins, HPin)

  int PrimariesCount() const { return m_ld->NetList.nPrimariesEnd - 1; }

  //enumerators
  typedef HEnumerator<HPin> PrimariesEnumerator;
  typedef HEnumeratorW<HPinWrapper, HPins> PrimariesEnumeratorW;

  PrimariesEnumerator GetEnumerator();
  PrimariesEnumeratorW GetEnumeratorW();

  //events
  Event<2, int, int> OnGrow;  //int nPinsLimit_before, int nPinsLimit_after
  Event<1, int> OnInitialize; //int nPinsLimit
  Event<1, HPin> OnPinAllocation;
  
  ///operations
  void Initialize(int primariesNum, int pinsLimit);
  void Grow(int pinsLimit);
  HPin AllocatePrimaryPin();
  void AllocatePins(HCell cell);

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(double, HPin::OffsetX, m_ld->NetList.pinOffsetX)
  PROPERTYA(double, HPin::OffsetY, m_ld->NetList.pinOffsetY)
  PROPERTYA(PinDirection, HPin::Direction, m_ld->NetList.pinDirection)
  PROPERTYA2(HPinType, HPin::Type, m_ld->NetList.pinType)

  //getters specializations
  GETTER(bool, HPin::IsPrimary)
    { return ARGID < m_ld->NetList.nPrimariesEnd && ARGID > 0; }
  GETTER(bool, HPin::IsPrimaryInput)
    { return ARGID < m_ld->NetList.nPrimariesEnd && ARGID > 0
      && m_ld->NetList.pinDirection[ARGID] == PinDirection_OUTPUT; }
  GETTER(bool, HPin::IsPrimaryOutput)
    { return ARGID < m_ld->NetList.nPrimariesEnd && ARGID > 0
      && m_ld->NetList.pinDirection[ARGID] == PinDirection_INPUT; }
  GETTER(double, HPin::X)
    { return m_ld->NetList.cellX[m_ld->NetList.pinCellIdx[ARGID]]
      + m_ld->NetList.pinOffsetX[ARGID]; }
  GETTER(double, HPin::Y)
    { return m_ld->NetList.cellY[m_ld->NetList.pinCellIdx[ARGID]]
      + m_ld->NetList.pinOffsetY[ARGID]; }
  GETTER(string, HPin::Name)
    {
      return ARGID < m_ld->NetList.nPinsStart
        ? m_ld->NetList.pinName[ARGID]
        : m_ld->Tech->pinName[m_ld->NetList.pinType[ARGID]];
    }

  GETTER(HCell, HPin::Cell);       //implemented in HExternalMethods.h
  GETTER(HNet, HPin::Net);         //implemented in HExternalMethods.h
  GETTER(HNet, HPin::OriginalNet); //implemented in HExternalMethods.h

  //setters specialization
  SETTER(HNet, HPin::Net); //implemented in HExternalMethods.h

  SETTER(string, HPin::Name)
  { 
    if (ARGID < m_ld->NetList.nPinsStart)
			m_ld->NetList.pinName[ARGID] = value;
		else
      LOGERROR("Unable to set name for non primary arg.");
  }

ENDHCOLLECTION(HPins)

BEGINWRAPPER(HPinWrapper, HPins)

  //properties
  PROPERTYWD(double, OffsetX)
  PROPERTYWD(double, OffsetY)
  PROPERTYWD(HPinType, Type)
  PROPERTYWD(PinDirection, Direction)
  PROPERTYWD(string, Name)

  //getters
  GETTERWD(double, X)
  GETTERWD(double, Y)
  GETTERWD(bool, IsPrimary)
  GETTERWD(bool, IsPrimaryInput)
  GETTERWD(bool, IsPrimaryOutput)
  GETTERWEX(HCell, Cell);       //implemented in HExternalMethods.h
  GETTERWEX(HNet, Net);         //implemented in HExternalMethods.h
  GETTERWEX(HNet, OriginalNet); //implemented in HExternalMethods.h

  //seters
  SETTERWEX(HNet, Net);

ENDWRAPPER(HPinWrapper)

inline HPins::PrimariesEnumerator HPins::GetEnumerator()
{ return PrimariesEnumerator(1, m_ld->NetList.nPrimariesEnd); }
inline HPins::PrimariesEnumeratorW HPins::GetEnumeratorW()
{ return PrimariesEnumeratorW(this, 1, m_ld->NetList.nPrimariesEnd); }

#endif //__HIGH_PIN_H__