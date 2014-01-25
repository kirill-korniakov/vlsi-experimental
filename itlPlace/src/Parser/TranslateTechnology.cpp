#include "HDesign.h"

void TranslateTechnology(HDesign& design)
{
  if (design.cfg.ValueOf("tech_mapping.enabled", false))
  {
    design.Circuit.SetXScaleFactor(design.cfg.ValueOf("tech_mapping.XScaleFactor", 1.0));
    design.Circuit.SetYScaleFactor(design.cfg.ValueOf("tech_mapping.YScaleFactor", 1.0));

    for (HMacroTypes::EnumeratorW mt = design.MacroTypes.GetEnumeratorW(); mt.MoveNext(); )
    {
      string msection = "tech_mapping." + mt.Name();
      if (design.cfg.Exists(msection))
      {
        msection += '.';
        string macro_name = design.cfg.ValueOf(msection + mt.Name(), mt.Name().c_str());
        if (!macro_name.empty())
          mt.SetName(macro_name);
        for (HMacroType::PinsEnumeratorW pin = mt.GetEnumeratorW(); pin.MoveNext(); )
        {
          string pin_name = design.cfg.ValueOf(msection + pin.Name(), pin.Name().c_str());
          if (!pin_name.empty())
            pin.SetName(pin_name);
        }
      }
    }
  }
  else
  {
    design.Circuit.SetXScaleFactor(1.0);
    design.Circuit.SetYScaleFactor(1.0);
  }
}
