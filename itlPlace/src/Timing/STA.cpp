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

void STA(HDesign& design, bool doReport, bool reroute)
{
  //No timing == No STA
  if (!design.CanDoTiming()) return;

  //1. Route/Reroute
  if (reroute)
    AdaptiveRoute(design);
  //2. RC Extraction + delays calculation
  CalculateWireDelays(design, GetActualLayersModel(design), GetActualSignalModel(design));
  //3. Arrival & Required time propagation
  PropagateDelays(design);

  if (doReport) ReportNegativeSlacks(design);
}

template<SignalModel sm>
void CalculateDelaysNoWires(HDesign& design);

template<>
void CalculateDelaysNoWires<SignalModel_Universal>(HDesign& design)
{
  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
  {
    double cLoad = 0.0;
    for (HNet::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      double C = design.GetDouble<HPinType::Capacitance>(sink.Type());
      cLoad += C;
      HSteinerPoint pt = design.SteinerPoints[sink];
      design.Set<HSteinerPoint::ObservedC>(pt, C);
      design.Set<HSteinerPoint::PathDelay>(pt, 0.0);
    }
    HSteinerPoint src = design.SteinerPoints[net.Source()];
    design.Set<HSteinerPoint::ObservedC>(src, cLoad);
    design.Set<HSteinerPoint::PathDelay>(src, 0.0);
  }
}

template<>
void CalculateDelaysNoWires<SignalModel_RiseFall>(HDesign& design)
{
  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
  {
    double cLoadR = 0.0;
    double cLoadF = 0.0;
    for (HNet::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      double CR = design.GetDouble<HPinType::RiseCapacitance>(sink.Type());
      double CF = design.GetDouble<HPinType::FallCapacitance>(sink.Type());
      cLoadR += CR;
      cLoadF += CF;
      HSteinerPoint pt = design.SteinerPoints[sink];
      design.Set<HSteinerPoint::RiseObservedC>(pt, CR);
      design.Set<HSteinerPoint::FallObservedC>(pt, CF);
      design.Set<HSteinerPoint::RisePathDelay>(pt, 0.0);
      design.Set<HSteinerPoint::FallPathDelay>(pt, 0.0);
    }
    HSteinerPoint src = design.SteinerPoints[net.Source()];
    design.Set<HSteinerPoint::RiseObservedC>(src, cLoadR);
    design.Set<HSteinerPoint::FallObservedC>(src, cLoadF);
    design.Set<HSteinerPoint::RisePathDelay>(src, 0.0);
    design.Set<HSteinerPoint::FallPathDelay>(src, 0.0);
  }
}

void CalculateDelaysNoWires(HDesign& design, SignalModel sm)
{
  switch (sm)
  {
  case SignalModel_Universal:
    CalculateDelaysNoWires<SignalModel_Universal>(design);
    return;
  case SignalModel_RiseFall:
    CalculateDelaysNoWires<SignalModel_RiseFall>(design);
    return;
  default:
    LOGERRORFORMAT(("Unsupported signal direction: %d", sm));
    CalculateDelaysNoWires<SignalModel_RiseFall>(design);
    return;
  }
}

void NoWireDelaySTA(HDesign& design, bool doReport)
{
  //No timing == No STA
  if (!design.CanDoTiming()) return;

  //2. RC Extraction + delays calculation
  CalculateDelaysNoWires(design, GetActualSignalModel(design));
  //3. Arrival & Required time propagation
  PropagateDelays(design);

  if (doReport) ReportNegativeSlacks(design);
}