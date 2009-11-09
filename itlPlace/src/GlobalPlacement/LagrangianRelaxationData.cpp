#include "LagrangianRelaxationData.h"
#include "Utils.h"
#include "MuCalculations.h"

double CalcSourceAFactor(HDesign& design, HPinType sourceType, std::vector<double>& muSource)
{
  double sourceAFactor = 0.0;

  int i = 0;
  for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
    if (arc.TimingType() == TimingType_Combinational || arc.TimingType() == TimingType_RisingEdge)
    {
      sourceAFactor += 0.5 * muSource[i] * (arc.ResistanceFall() + arc.ResistanceRise());
      i++;
    }

  return sourceAFactor;
}

int CalcNumberOfLRArc(HDesign& design, HPinType sourceType)
{
  int i = 0;
  for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
    if (arc.TimingType() == TimingType_Combinational || arc.TimingType() == TimingType_RisingEdge)
      i++;

  return i;
}

void LRData::AssignLRData(HDesign &hd, HNetWrapper net, int netIdx)
{
  HPinType sourceType = hd.Get<HPin::Type, HPinType>(net.Source());

  std::vector<double> muSourceVector;
  InitializeMuSourceVector(hd, muSourceVector, CalcNumberOfLRArc(hd, sourceType));
  netLRData[netIdx].sourceAFactor = CalcSourceAFactor(hd, sourceType, muSourceVector);

  InitializeMuNetVector(hd, netLRData[netIdx].muNetVector, net.SinksCount());
  for (HNet::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext(); )
    netLRData[netIdx].sinkLoad.push_back(hd.GetDouble<HPinType::Capacitance>(sink.Type()));
}

void LRData::InitializeNets(HDesign& hd)
{
  netLRData.resize(hd.Nets.Count());

  //put nets
  int netIdx = 0;
  for (HNets::NetsEnumeratorW net = hd.Nets.GetNetsEnumeratorW(); net.MoveNext(); )
  {
    AssignLRData(hd, net, netIdx);
    netIdx++;
  }
}

void LRData::Initialize(HDesign& hd)
{
  alphaTWL = hd.cfg.ValueOf(".LagrangianRelaxation.alphaTWL", 1.0);
  c = hd.RoutingLayers.Physics.LinearC;
  r = hd.RoutingLayers.Physics.RPerDist;

  InitializeNets(hd);
}