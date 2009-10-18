#ifndef __HIGH_DESIGN_H__
#define __HIGH_DESIGN_H__

#pragma warning(disable:4355)

#include "HSteinerPoint.h"
#include "HCircuit.h"
#include "HMacroType.h"
#include "HPinType.h"
#include "HRoutingLayer.h"
#include "HTimingArcType.h"
#include "HCell.h"
#include "HPin.h"
#include "HNet.h"
#include "HWire.h"
#include "HSite.h"
#include "HPlacementRow.h"
#include "HTimingPoint.h"
#include "HPlotter.h"
#include "HCriticalPath.h"
#include "HCriticalPathPoint.h"

#define HDESIGNCOLLECTION(field_name, col_type) \
public: \
  col_type field_name; \
  col_type::WrapperType operator[](col_type::ItemType obj) \
    { return field_name.MakeWrapper(obj); } \
  template<int prop, class RetType> \
  RetType Get(col_type::ItemType item) const { return field_name.Get<prop, RetType>(item); } \
  template<int prop, class ArgType> \
  void Set(col_type::ItemType item, const ArgType& value) { field_name.Set<prop>(item, value); } \
  template<int prop> \
  double GetDouble(col_type::ItemType item) const { return field_name.Get<prop, double>(item); } \
  template<int prop> \
  int GetInt(col_type::ItemType item) const { return field_name.Get<prop, int>(item); } \
  template<int prop> \
  bool GetBool(col_type::ItemType item) const { return field_name.Get<prop, bool>(item); } \
  template<int prop> \
  string GetString(col_type::ItemType item) const { return field_name.Get<prop, string>(item); }

/* Attention!!!
 *  distance units - nm   (nanometers)
 *  capacitance    - pf   (picofarads)
 *  resistance     - kohm (kiloohms)
 *  time           - ns   (nanoseconds)
 */
class HDesign
{
public:
  LDesign _Design;

  HDesign(): _Design(),            Circuit(&_Design),    MacroTypes(this),
             PinTypes(this),       RoutingLayers(this),  TimingArcTypes(this),
             Cells(this),          Pins(this),           Nets(this),
             SteinerPoints(this),  Wires(this),          Sites(this),
             PlacementRows(this),  TimingPoints(this),   Plotter(*this),
             CriticalPaths(this),  CriticalPathPoints(this)
  {
    _Design.Tech = 0;
    isGlobalPlacementDone = false;
  }

  HCircuit Circuit;
  HPlotter Plotter;
  libconfig::ConfigExt cfg;
  HDESIGNCOLLECTION(Nets, HNets)
  HDESIGNCOLLECTION(MacroTypes, HMacroTypes)
  HDESIGNCOLLECTION(PinTypes, HPinTypes)
  HDESIGNCOLLECTION(RoutingLayers, HRoutingLayers)
  HDESIGNCOLLECTION(TimingArcTypes, HTimingArcTypes)
  HDESIGNCOLLECTION(Cells, HCells)
  HDESIGNCOLLECTION(Pins, HPins)
  HDESIGNCOLLECTION(SteinerPoints, HSteinerPoints)
  HDESIGNCOLLECTION(Wires, HWires)
  HDESIGNCOLLECTION(Sites, HSites)
  HDESIGNCOLLECTION(PlacementRows, HPlacementRows)
  HDESIGNCOLLECTION(TimingPoints, HTimingPoints)
  HDESIGNCOLLECTION(CriticalPaths, HCriticalPaths)
  HDESIGNCOLLECTION(CriticalPathPoints, HCriticalPathPoints)

  bool isGlobalPlacementDone;

  void Initialize();

  bool HasTechInfo() const {return _Design.Tech != 0;}
  void SetTechInfo(HDesign* design = 0)
  {
    if(!design)
      _Design.Tech = new LTech();
    else
      _Design.Tech = design->_Design.Tech;
  }
};

#include "HExternalMethods.h"

#endif //__HIGH_DESIGN_H__