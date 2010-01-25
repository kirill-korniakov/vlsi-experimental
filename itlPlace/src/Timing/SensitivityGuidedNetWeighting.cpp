/*  
* SensitivityGuidedNetWeighting.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include <math.h>
#include "SensitivityGuidedNetWeighting.h"
#include "Timing.h"
#include "HPinsStorage.h"

//functions for net weighting
HPinWrapper FindMostCriticalSink(HNetWrapper net, HDesign &hd)
{
  //find the most critical sink of this net
  //(i.e. sink with the worst slack)
  double worstSinkSlack = 0.0;
  HPinWrapper mostCriticalPin = hd[net.Source()];

  for (HNetWrapper::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext();)
  {
    HTimingPointWrapper point = hd[hd.TimingPoints[sink]];

    if (point.Slack() < worstSinkSlack)
    {
      worstSinkSlack  = point.Slack();
      mostCriticalPin = sink;
    }
  }

  if (worstSinkSlack >= 0)
    LOGERROR("net isn't critical");

  return mostCriticalPin;
}

double GetNetSensitivityTByL(HNetWrapper net, HDesign& hd)
{
  //FIX ME: need to find rPerDist, cPerDist, rDriver, cReceiver
  double r = hd.RoutingLayers.Physics.RPerDist;
  double c = hd.RoutingLayers.Physics.LinearC;

  HPinWrapper receiver = FindMostCriticalSink(net, hd);
  double Cr = Utils::GetSinkCapacitance(hd, receiver, SignalDirection_None);
  double Rs = Utils::GetDriverWorstPhisics(hd, net.Source(), SignalDirection_None).R;

  double netHPWL = net.HPWL();
  return (r * c * netHPWL + c * Rs + r * Cr);
}

double GetCellWeight(HCellWrapper cell, HDesign& hd)
{
  //cell's weight == sum of weights of all nets connected to this cell
  double cellWeight = 0.0;
  
  for (HCell::PinsEnumeratorW pin = cell.GetPinsEnumeratorW(); pin.MoveNext();)
  {
    HNet net = pin.Net();
    cellWeight += hd.GetDouble<HNet::Weight>(net);
  }

  if (cellWeight == 0)
    LOGCRITICAL("cell weight == 0! (may be cell is null?)");

  return cellWeight;
}

double GetSinksWeight(HNetWrapper net, HDesign &hd)
{
  HPinWrapper  criticalSink = FindMostCriticalSink(net, hd);
  HCellWrapper receiverCell = hd[criticalSink.Cell()];

  if (::IsNull(receiverCell))
    return net.Weight();

  return GetCellWeight(receiverCell, hd);
}

double GetNetSensitivityLByWeight(HNetWrapper net, HDesign& hd)
{
  double netHPWL     = net.HPWL();
  HPinWrapper source = hd[net.Source()];
  HCellWrapper cell  = hd[source.Cell()];
  double wSrc        = 0.0;

  if (::IsNull(cell))
    wSrc = net.Weight();

  else
    wSrc = GetCellWeight(cell, hd);

  double wSinks = GetSinksWeight(net, hd);
  return ((-1) * netHPWL * (wSrc + wSinks - 2 * net.Weight()) / (wSrc * wSinks));
}

double GetNetSensitivitySlacByWeight(HNetWrapper net, HDesign& hd)
{
  double delayByL  = GetNetSensitivityTByL(net, hd);
  double lenghtByW = GetNetSensitivityLByWeight(net, hd);
  return ((-1) * delayByL * lenghtByW);
}

void SensitivityGuidedNetWeighting(HDesign& hd)
{
  //constraint on total weight increasing
  double C = hd.cfg.ValueOf("NetWeighting.SensitivityGuidedNetWeighting.C", 3227.0);

  //balance coefficient between slack and FOM sensitivities
  double A = hd.cfg.ValueOf("NetWeighting.SensitivityGuidedNetWeighting.alpha", 0.6);

  //initial weight for each net
  double minWeight = hd.cfg.ValueOf("NetWeighting.SensitivityGuidedNetWeighting.minWeight", 1.0);

  double denominator = 0.0; //denominator for beta coefficient
  NetSensitivities netSensitivities; //for storing nets

  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext();)
  {
    if (net.Weight() <= 0)
      hd.Set<HNet::Weight>(net, minWeight);
  }

  Utils::CalculateHPWL(hd, true); //in order to find HPWL for each net and to use net.HPWL()
  PinsStorage pinsStorage(hd);
  double multiplier = 10000.0; //for more precise calculations
  
  //find all critical nets
  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext();)
  {
    HPinWrapper netSource = hd[net.Source()];
    HTimingPointWrapper point = hd[hd.TimingPoints[netSource]];

    if (point.Slack() >= 0) //net is not critical
      continue;

    double slackByWeight = GetNetSensitivitySlacByWeight(net, hd);
    double FOMByWeight   = pinsStorage.FindFOMByTForSource(netSource) * slackByWeight;
    NetSensitivity netSensitivity(net, slackByWeight, FOMByWeight);
    netSensitivities.push_back(netSensitivity);

    double netIncrementSqrt = (0 - point.Slack()) * slackByWeight + A * FOMByWeight;
    denominator += multiplier * netIncrementSqrt * netIncrementSqrt;
  }

  if (denominator < 0.0001)
      LOGERROR("Too small denominator of beta");

  ALERTFORMAT(("C: %f", C));
  double B = sqrt(C * multiplier / denominator);
  ALERTFORMAT(("B: %f", B));

  //now calculate new net weights
  for (NetSensitivities::iterator i = netSensitivities.begin();
       i != netSensitivities.end(); i++)
  {
    HPin netSrc          = i->net.Source();
    double slackByWeight = i->slackByWeight;
    double FOMByWeight   = i->FOMByWeight;

    HTimingPointWrapper point = hd[hd.TimingPoints[netSrc]];
    double deltaW = B * ((-1) * point.Slack() * slackByWeight + A * FOMByWeight);

    //set new weight
    hd.Set<HNet::Weight>(i->net, i->net.Weight() + deltaW);
  }

  ALERTFORMAT(("num critical nets: %d", netSensitivities.size()));
}