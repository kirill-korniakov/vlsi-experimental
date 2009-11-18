#include "STA.h"

void STA(HDesign& design, bool doReport)
{
  AdaptiveRoute(design);
  CalculateWireDelays(design, LayersModel_Lumped, SignalModel_RiseFall);
  PropagateDelays(design);
  if (doReport)
    ReportNegativeSlacks(design);
}