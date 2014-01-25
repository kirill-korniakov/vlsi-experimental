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

//event handlers
private: void NetsGrowEventHandler(int netsFrom, int netsTo);
private: void NetsSwapEventHandler(HNet first, HNet second);
private: void NetsAddEventHandler(HNet net);

ENDHCOLLECTION(HWires)

//properties specializations
PROPERTYADECL(HWires, double, HWire::Length, m_ld->Routing.netWL)
PROPERTYADECL(HWires, RoutingType, HWire::RoutingType, m_ld->Routing.netRoutingType)

//getters specializations
GETTER(HWires, HSteinerPoint, HWire::RootPoint);
GETTER(HWires, HNet, HWire::Net);

BEGINWRAPPER(HWireWrapper, HWires)

  //properties specializations
  PROPERTYWD(double, Length)
  PROPERTYWD(::RoutingType, RoutingType)

	//Getters
  GETTERW(HSteinerPoint, RootPoint, Root)
  GETTERWD(HNet, Net)

ENDWRAPPER(HWireWrapper)

#endif //__HIGH_WIRE_H__
