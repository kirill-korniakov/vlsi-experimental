#include "Reporting.h"

void ReportBufferingPhysics(VanGinneken& vg)
{
  if (vg.GetBufferInfo() != NULL)
  {
    ALERTFORMAT(("Wire Resistance\t= %.10f", vg.GetPhysical().RPerDist));
    ALERTFORMAT(("Wire Capacitance\t= %.10f", vg.GetPhysical().LinearC));
    ALERTFORMAT(("buffer Resistance\t= %.10f", vg.GetBufferInfo()->Resistance()));
    ALERTFORMAT(("buffer Capacitance\t= %.10f", vg.GetBufferInfo()->Capacitance()));
  }
}