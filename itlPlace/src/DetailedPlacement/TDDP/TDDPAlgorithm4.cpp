/*  
* TDDPAlgorithm4.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include <math.h>
#include "TDDPAlgorithm4.h"
#include "Timing.h"

PinsStorage::~PinsStorage()
{
  Node* pv = head;
  Node* pt = head;

  while (pv != NULL)
  {
    pt = pv;
    pv = pv->next;
    delete pt;
    pt = NULL;
  }

  head = NULL;
}

void PinsStorage::InsertNode(HPinWrapper pin)
{
  Node* pv = new Node(pin);
  
  if (tail != NULL)
  {
    tail->next = pv;
    pv->prev   = tail;
    tail       = pv;
  }
  
  else //first node
  {
    head = tail = pv;
  }
}

void PinsStorage::ConsiderNewNode(HPinWrapper pin)
{
  if (pin.Direction() == PinDirection_INPUT) //sink
  {
    if (!::IsNull(pin.Cell()))
    {
      Node* pv = FindNode(pin.Cell());

      if (pv != NULL)
      {

        if (pv->IsSource()) //source was found
        {
          InsertNode(pin); //add to the end of the list
          tail->cellFOMByT = pv->cellFOMByT; //set pointer
        }

        else //sink
        {
          HTimingPointWrapper newPoint = design[design.TimingPoints[pin]];
          HTimingPointWrapper oldPoint = design[design.TimingPoints[pv->pin]];

          if (newPoint.Slack() < oldPoint.Slack())
            pv->pin = pin; //this pin is more critical than previous
        }
      }

      else //no sources, no sinks on the cell were not found
        InsertNode(pin);
    }
    
    else //this pin isn't on a cell
    {
      HTimingPointWrapper point = design[design.TimingPoints[pin]];
      
      if (point.Slack() < 0) //if it's critical
      {
        InsertNode(pin);
        tail->FOMByT = 1;
      }
    }
  }

  else //this pin is source
  {
    InsertNode(pin);
    Node* pv = FindNode(pin.Cell());

    if (pv != NULL)
    {
      if (pv->cellFOMByT != NULL)
        tail->cellFOMByT = pv->cellFOMByT;

      else
        tail->cellFOMByT = new unsigned int(0);
    }

    else
    {
      tail->cellFOMByT = new unsigned int(0);
    }
  }
}

void PinsStorage::CalculateFOMSensitivities()
{
  //first - fill pins storage
  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoPrevious()); )
  {
    HPinWrapper pin  = design[design.Get<HTimingPoint::Pin, HPin>(pt)];
    ConsiderNewNode(pin);
  }

  //calculate FOM sensitivities
  Node* pv = head;
  
  while (pv != NULL)
  {
    if (!(pv->IsSource()))
    {
      //Find node pt with source of a pin's net;
      HNetWrapper net = design[pv->pin.Net()];
      HPinWrapper src = design[net.Source()];
      Node* pt = FindSourceNode(src);

      if (pt == NULL)
        LOGERROR("no source for this pin");

      unsigned int fomByT = pv->FOMByT;

      if (pv->cellFOMByT != NULL)
        fomByT += *(pv->cellFOMByT);

      *(pt->cellFOMByT) += fomByT;
      pt->FOMByT        += fomByT;
    }
    
    pv = pv->next;
  }
}

unsigned int PinsStorage::FindFOMByTForSource(HPinWrapper src)
{
  Node* pv = head;

  while (pv != NULL)
  {
    if (pv->pin == src)
      break;

    pv = pv->next;
  }

  if (pv != NULL)
    return pv->FOMByT;
  
  return 0;
}

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

  double netHPWL = Utils::CalcNetHPWL(hd, net);
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
  double netHPWL     = Utils::CalcNetHPWL(hd, net);
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

void ProcessCriticalNets(HDesign& hd)
{
  double C = hd.cfg.ValueOf("NetWeighting.Placement4.C", 3000.0);  //constraint on total weight increasing
  double A = hd.cfg.ValueOf("NetWeighting.Placement4.alpha", 1.0); //balance coefficient between slack and
                                                                   //FOM sensitivities
  double minWeight = hd.cfg.ValueOf("NetWeighting.Placement4.minWeight", 1.0); //initial weight for each net

  double enumenator = 0.0; //enumerator for beta coefficient
  NetSensitivities netSensitivities; //for storing nets

  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext();)
  {
    if (net.Weight() <= 0)
    hd.Set<HNet::Weight>(net, minWeight);
  }

  PinsStorage pinsStorage(hd);
  double tmpKoef = 10000.0;
  
  //find all critical nets
  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext();)
  {
    HPinWrapper netSource = hd[net.Source()];
    HTimingPointWrapper point = hd[hd.TimingPoints[netSource]];

    if (point.Slack() >= 0)  //net is not critical
      continue;

    double slackByWeight = GetNetSensitivitySlacByWeight(net, hd);
    double FOMByWeight   = pinsStorage.FindFOMByTForSource(netSource) * slackByWeight;
    NetSensitivity netSensitivity(net, slackByWeight, FOMByWeight);
    netSensitivities.push_back(netSensitivity);

    double netIncrementSqrt = (0 - point.Slack()) * slackByWeight + A * FOMByWeight;
    enumenator += tmpKoef * netIncrementSqrt * netIncrementSqrt;
    //enumenator += (0 - point.Slack()) * (0 - point.Slack()) * (slackByWeight + A * FOMByWeight)
    //              * (slackByWeight + A * FOMByWeight);
  }

  if (enumenator < 0.0001)
      LOGERROR("Too small enumerator of beta");

  ALERTFORMAT(("C: %f", C));
  double B = sqrt(C * tmpKoef / enumenator);
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
    //ALERTFORMAT(("weight delta: %f", deltaW));
  }

  ALERTFORMAT(("num critical nets: %d", netSensitivities.size()));
}

//This function is used only for tests
//TODO: remove it later
void TestProcessCriticalNets(HDesign& hd)
{
}

void Placement4(HDesign& design)
{
  ProcessCriticalNets(design);
  ExportNetWeights(design, "my_weights.txt");
  design.cfg.SetCfgValue("NetWeighting.netWeightsImportFileName", "my_weights.txt");
}