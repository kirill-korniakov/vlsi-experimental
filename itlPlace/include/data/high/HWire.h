#ifndef __HIGH_WIRE_H__
#define __HIGH_WIRE_H__

#include "HBaseDesignItem.h"
#include "HSteinerPoint.h"
#include "HNet.h"

BEGINITEM(HWire, HWireWrapper)

  enum Properties
  {
    Length,
    RoutingType,
    RootPoint,
    Net
  };

ENDITEM(HWire)

BEGINHCOLLECTION(HWires, HWire)

  HWire operator[] (const HNet& net) const
  {
    return ::__ConstructPtr<HWire>(net);
  }

	///Events
	Event<0> OnInitialize;
	Event<2, int,int> OnGrow; //int netsFrom, int netsTo

	///Operations
	void Initialize();
  void Grow(int netsFrom, int netsTo);

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(double, HWire::Length, m_ld->Routing.netWL)
  PROPERTYA(RoutingType, HWire::RoutingType, m_ld->Routing.netRoutingType)

  //getters specializations
  GETTER(HSteinerPoint, HWire::RootPoint)
    { return ::__ConstructPtr<HSteinerPoint>(m_ld->NetList.netPins[m_ld->NetList.netPinStart[ARGID]]); }

  GETTER(HNet, HWire::Net)
    { return ::__ConstructPtr<HNet>(arg); }

//event handlers
private: void NetsGrowEventHandler(int netsFrom, int netsTo);
private: void NetsSwapEventHandler(HNet first, HNet second);
private: void NetsAddEventHandler(HNet net);

ENDHCOLLECTION(HWires)

BEGINWRAPPER(HWireWrapper, HWires)

  //properties specializations
  PROPERTYWD(double, Length)
  PROPERTYWD(::RoutingType, RoutingType)

	//Getters
  GETTERW(HSteinerPoint, RootPoint, Root)
  GETTERWD(HNet, Net)

ENDWRAPPER(HWireWrapper)

#endif //__HIGH_WIRE_H__