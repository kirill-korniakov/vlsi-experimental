#include "PinUtils.h"

using namespace Utils;

HPin Utils::FindCellPinByName(HDesign& design, HCell cell, const string& name)
{
  int type_id = design._Design.NetList.cellType[::ToID(cell)];
  int pins_start = design._Design.Tech->macroPinStart[type_id];
  for(int i = pins_start; i < design._Design.Tech->macroPinEnd[type_id]; i++)
    if(design._Design.Tech->pinName[i] == name)
      return ::__ConstructPtr<HPin>(i - pins_start + design._Design.NetList.cellPinStartIdx[::ToID(cell)]);
  return design.Pins.Null();
}

HPin Utils::FindPrimaryPinByName(HDesign& design, const string& name)
{
  for(HPins::PrimariesEnumerator pin = design.Pins.GetEnumerator(); pin.MoveNext(); )
    if(design.GetString<HPin::Name>(pin) == name)
      return pin;
  return design.Pins.Null();
}

void Utils::AllocatePinsForAllCells(HDesign& design, double reservePercent)
{
  int pins_needed = 0;

  for(HCells::CellsEnumerator i = design.Cells.GetEnumerator(); i.MoveNext(); )
    pins_needed += design.GetInt<HMacroType::PinsCount>(design.Get<HCell::MacroType, HMacroType>(i));

  int required_size = (int)((pins_needed + design._Design.NetList.nPinsStart) * reservePercent);
  if(required_size > design._Design.NetList.nPinsLimit)
    design.Pins.Grow(required_size);

  for(HCells::CellsEnumerator i = design.Cells.GetEnumerator(); i.MoveNext(); )
    design.Pins.AllocatePins(i);
}

string Utils::MakePinFullName(HDesign& design, HPin pin)
{
  if (design.GetBool<HPin::IsPrimary>(pin))
    return string("PIN ") + design.GetString<HPin::Name>(pin);
  else
    return design.GetString<HCell::Name>(design.Get<HPin::Cell, HCell>(pin)) + " " + design.GetString<HPin::Name>(pin);
}