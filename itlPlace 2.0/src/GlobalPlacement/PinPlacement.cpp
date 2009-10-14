#include "GlobalPlacement.h"

void PinPlacement(HDesign& design)
{
  int piCount = 0;
  int poCount = 0;

  for (HPins::PrimariesEnumeratorW pin = design.Pins.GetEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.X() == 0.0 && pin.Y() == 0.0)
    {
      if (pin.IsPrimaryInput())
        piCount++;
      else
        poCount++;
    }
  }

  int piIdx = 0;
  int poIdx = 0;

  for (HPins::PrimariesEnumeratorW pin = design.Pins.GetEnumeratorW(); pin.MoveNext(); )
  {
    if (pin.X() == 0.0 && pin.Y() == 0.0)
    {
      //ALERTFORMAT(("Not placed primary pin"));
      if (pin.IsPrimaryInput())
      {
        pin.SetOffsetX(design.Circuit.PlacementMinX());
        pin.SetOffsetY(design.Circuit.PlacementMinY() + 
          design.Circuit.PlacementHeight() * (double)piIdx/(double)(piCount-1));
        piIdx++;
      }
      else
      {
        pin.SetOffsetX(design.Circuit.PlacementMaxX());
        pin.SetOffsetY(design.Circuit.PlacementMinY() + 
          design.Circuit.PlacementHeight() * (double)poIdx/(double)(poCount-1));
        poIdx++;
      }
    }
  }
}