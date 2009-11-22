#include "STA.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Timing.h"
#include "Reporting.h"

LayersModel GetActualLayersModel(HDesign& design)
{
  switch (design.cfg.ValueOf("Timing.RCE.LayersUsed", 2))
  {
  case 0:
    return LayersModel_Lumped;
  case 1:
    return LayersModel_OneDirection;
  case 2:
    return LayersModel_TwoDirections;
  default:
    LOGERRORFORMAT(("Unsupported layers number: %d", design.cfg.ValueOf("Timing.RCE.LayersUsed", 2)));
    return LayersModel_TwoDirections;
  }
}

SignalModel GetActualSignalModel(HDesign& design)
{
  switch (design.cfg.ValueOf("Timing.SignalDirectionsUsed", 2))
  {
  case 1:
    return SignalModel_Universal;
  case 2:
    return SignalModel_RiseFall;
  default:
    LOGERRORFORMAT(("Unsupported number of signal directions: %d", design.cfg.ValueOf("Timing.SignalDirectionsUsed", 2)));
    return SignalModel_RiseFall;
  }
}

void STA(HDesign& design, bool doReport)
{
  //No timing == No STA
  if (!design.CanDoTiming()) return;

  //1. Route/Reroute
  AdaptiveRoute(design);
  //2. RC Extraction + delays calculation
  CalculateWireDelays(design, GetActualLayersModel(design), GetActualSignalModel(design));
  //3. Arrival & Required time propagation
  PropagateDelays(design);

  if (doReport) ReportNegativeSlacks(design);
}