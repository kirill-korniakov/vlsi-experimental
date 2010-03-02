#include "LagrangianRelaxationData.h"
#include "Utils.h"

double CalcSourceAFactor(HDesign& design, HPinType sourceType, std::vector<double>& muSource)
{
  double sourceAFactor = 0.0;

  int i = 0;
  for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
    if (arc.TimingType() == TimingType_Combinational)
    {
      sourceAFactor += 0.5 * muSource[i] * (arc.ResistanceFall() + arc.ResistanceRise());
      i++;
    }

  return sourceAFactor;
}

void LRData::AssignNetLRData(HDesign& hd, HNetWrapper net, int netIdx)
{
  std::vector<double> cellArcMus;
  actualMus->GetNetMus(hd, net, cellArcMus, netLRData[netIdx].netArcMus);

  HPinType sourceType = hd.Get<HPin::Type, HPinType>(net.Source());
  //ALERT("Net %s", net.Name().c_str()));
  netLRData[netIdx].sourceAFactor = CalcSourceAFactor(hd, sourceType, cellArcMus);
}

void LRData::UpdateMultipliers(HDesign& hd)
{
  actualMus->UpdateMus(hd);

  int netIdx = 0;
  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
  {
    AssignNetLRData(hd, net, netIdx);
    netIdx++;
  }
}

void LRData::InitializeNets(HDesign& hd)
{
  netLRData.resize(hd.Nets.Count());

  //init sink load
  int netIdx = 0;
  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
  {
    for (HNet::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      netLRData[netIdx].sinkLoad.push_back(hd.GetDouble<HPinType::Capacitance>(sink.Type()));
    }
    netIdx++;
  }

  UpdateMultipliers(hd);
}

void LRData::Initialize(HDesign& hd) 
{
  alphaTWL = hd.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.alphaTWL", 1.0);
  c = hd.RoutingLayers.Physics.LinearC;
  r = hd.RoutingLayers.Physics.RPerDist;
  actualMus = new TimingPointMus(hd);

  InitializeNets(hd);
}

LRData::LRData()
{
  actualMus = 0;
}

LRData::~LRData()
{
  if (actualMus)
  {
    delete actualMus;
  }
}