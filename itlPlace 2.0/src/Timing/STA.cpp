#include "STA.h"

void STA(HDesign& design, bool doReport)
{
  AdaptiveRoute(design);
  CalculateWireDelays(design, LayersModel_Lumped, SignalModel_Universal);
  PropagateDelays(design);
  if (doReport)
    ReportNegativeSlacks(design);
}