/*  
* ConstraintsController.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2010, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include <float.h>
#include "ConstraintsController.h"
#include "Utils.h"

bool HPWLChecker::CheckCriteria(HCell* cells, int nCells, double bestDiff)
{
  if (Utils::CalculateHPWLDiff(grid.Design(), cells, nCells, false)
      < bestDiff)
      return true;

  else
    return false;
}

HippocrateChecker::HippocrateChecker(HDPGrid& _grid): HPWLChecker(_grid) 
{
  HDesign& hd = grid.Design();
  CalculateGamma();

  nNets = hd.Nets.Count(NetKind_Active);
  nets  = new ExtNet[nNets];
  int i = 0;

  for (HNets::ActiveNetsEnumeratorW netw = hd.Nets.GetActiveNetsEnumeratorW(); netw.MoveNext();)
  {
    nets[i].net   = netw;
    nets[i].nPins = netw.PinsCount();
    nets[i].pins  = new ExtPin [nets[i].nPins];
    nets[i].alpha = CalculateAlpha(netw, hd);

    HPinWrapper srcw     = hd[netw.Source()];
    nets[i].pins[0].pin  = srcw;
    int j = 1;

    for (HNet::SinksEnumeratorW sinkw = hd.Get<HNet::Sinks, HNet::SinksEnumeratorW>(netw);
         sinkw.MoveNext(); )
    {
      nets[i].pins[j].pin          = sinkw;
      nets[i].pins[j].oldMDist     = fabs(sinkw.X() - srcw.X()) + fabs(sinkw.Y() - srcw.Y());
      nets[i].pins[j].betta        = CalculateBetta(sinkw, nets[i].pins[j].oldMDist, hd);
      nets[i].pins[j].deltaArrTime = CalculateDeltaArrivalTime(sinkw, hd);
      j++;
    }

    i++;
  }

  ALERT("active nets num: %d", nNets);
}

void HippocrateChecker::CalculateGamma()
{
  HDesign& hd = grid.Design();
  double r    = hd.RoutingLayers.Physics.RPerDist; //удельное сопротивление
  double с    = hd.RoutingLayers.Physics.LinearC;  //удельная электроёмкость
  gamma       = CONST_Kd * r * с * 0.5;
}

bool HippocrateChecker::CheckConstraintsOnNet(ExtNet& net)
{  
  HDesign& hd       = grid.Design();
  double deltaHPWL  = 0.0;

  HPinWrapper src = hd[net.pins[0].pin];
  double srcNewX  = src.X();
  double srcNewY  = src.Y();

  for (int i = 1; i < net.nPins; i++)
  {
    double sinkNewX = hd.GetDouble<HPin::X>(net.pins[i].pin);
    double sinkNewY = hd.GetDouble<HPin::Y>(net.pins[i].pin);
      
    double deltaMDist = fabs(sinkNewX - srcNewX) + fabs(sinkNewY - srcNewY) - net.pins[i].oldMDist;
    deltaHPWL += deltaMDist;
  }

  //check constraints for all sinks of this net
  for (int i = 1; i < net.nPins; i++)
  {
    double sinkNewX = hd.GetDouble<HPin::X>(net.pins[i].pin);
    double sinkNewY = hd.GetDouble<HPin::Y>(net.pins[i].pin);
      
    double deltaMDist   = fabs(sinkNewX - srcNewX) + fabs(sinkNewY - srcNewY) -
                                                          net.pins[i].oldMDist;

    HPinWrapper pin     = hd[net.pins[i].pin];
    double estimatedTimeDelta = net.alpha * deltaHPWL + net.pins[i].betta * deltaMDist +
                                gamma * deltaMDist * deltaMDist;

    //if constraints are not satisfied
    if (estimatedTimeDelta > net.pins[i].deltaArrTime)
       return false;
  }

  //constraints are satisfied
  return true;
}

bool HippocrateChecker::CheckConstraints(HCell* cells, int nCells)
{
  for (int i = 0; i < nCells; i++)
  {
    //find nets connected to this cell
    for (HCell::PinsEnumerator pe = grid.Design().Get<HCell::Pins, HCell::PinsEnumerator>(cells[i]);
                                    pe.MoveNext(); )
    {
      HNet current_net = grid.Design().Get<HPin::Net, HNet>(pe);

      if (!::IsNull(current_net))
      {
        int j = 0;
        for (j = 0; j < nNets; j++)
        {
          //find this net in storage
          if (nets[j].net == current_net)
          {
            //and check constraints on it
            if (CheckConstraintsOnNet(nets[j]) == false)
            {
              return false; //constraints are not satisfied
            }

            break;
          }
        }
      }
    }
  }

  return true; //constraints are satisfied
}

bool HippocrateChecker::CheckCriteria(HCell* cells, int nCells, double bestDiff)
{
  if (HPWLChecker::CheckCriteria(cells, nCells, bestDiff))
    if (CheckConstraints(cells, nCells))
      return true;

  return false;
}

double CalculateAlpha(HNet changedNet, HDesign& hd)
{
  HPin source    = hd.Get<HNet::Source, HPin>(changedNet);
  double Rdriver = Utils::GetDriverWorstPhisics(hd, source, SignalDirection_None).R;
  double c       = hd.RoutingLayers.Physics.LinearC;  //удельная электроёмкость
  return (Rdriver * c);
}

double CalculateBetta(HPinWrapper sink, double oldMDist, HDesign& hd)
{
  double r     = hd.RoutingLayers.Physics.RPerDist; //удельное сопротивление
  double c     = hd.RoutingLayers.Physics.LinearC;  //удельная электроёмкость
  double csink = Utils::GetSinkCapacitance(hd, sink, SignalDirection_None);
  return (CONST_Kd * r * (c * oldMDist + csink));
}

double CalculateEstimatedTime(HPinWrapper sink, double oldMDist, HDesign& hd)
{
  double r     = hd.RoutingLayers.Physics.RPerDist; //удельное сопротивление
  double c     = hd.RoutingLayers.Physics.LinearC;  //удельная электроёмкость
  double csink = Utils::GetSinkCapacitance(hd, sink, SignalDirection_None);
  return (CONST_Kd * r * (c * oldMDist / 2 + csink));
}

double CalculateDeltaArrivalTime(HPinWrapper sink, HDesign& hd)
{
	HCell cell = sink.Cell();
	double cellAT; //AT для элемента
	double sinkAT; //AT для sink
	HTimingPointWrapper tpsink = hd[hd.TimingPoints[sink]];
	sinkAT = tpsink.ArrivalTime();
	cellAT = sinkAT;
	std::vector<HPin> inpPin;	//входные пины элемента

	for (HCell::PinsEnumeratorW pin = hd.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell);
       pin.MoveNext(); )
  {
		HNet net = hd.Get<HPin::Net,HNet>(pin);
    
    if (::IsNull(net))
      continue;

		HPin source = hd.Get<HNet::Source, HPin>(net);

		if (hd[source].Cell() != cell)
			inpPin.push_back(pin);
	}

	//рассматриваем все входные пины элемента
	for (int i = 0; i < inpPin.size(); i++)
  {
		HTimingPointWrapper tp = hd[hd.TimingPoints[inpPin[i]]];
		
    if (cellAT < tp.ArrivalTime())
    {
			cellAT = tp.ArrivalTime();
		}
	}

	return (cellAT - sinkAT);
}