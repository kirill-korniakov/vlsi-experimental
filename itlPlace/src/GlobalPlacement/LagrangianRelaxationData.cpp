#include "LagrangianRelaxationData.h"
#include "Utils.h"
#include "MuCalculations.h"

HMacroType GetMacrotype(HDesign &hd, HPin pin)
{
  HCell cell = hd.Pins.Get<HPin::Cell, HCell>(pin);
  //ALERTFORMAT(("%s", hd.GetString<HPin::Name>(source).c_str()));
  HMacroType type = hd.Cells.Get<HCell::MacroType, HMacroType>(cell);

  return type;
}

void LRData::AssignLRData(HDesign &hd, HNets::NetsEnumeratorW net, int netIdx)
{
  HPin source = hd.Nets.Get<HNet::Source, HPin>(net);
  HMacroType type = GetMacrotype(hd, source);

  std::vector<double> muSourceVector;
  InitializeMuSourceVector(hd, muSourceVector, Utils::GetNOutArcs(hd, type));
  netLRData[netIdx].sourceAFactor = 
    Utils::CalcSourceAFactor(hd, type, muSourceVector);

  InitializeMuNetVector(hd, netLRData[netIdx].muNetVector, net.SinksCount());
  HNet::PinsEnumeratorW pin = net.GetPinsEnumeratorW(); pin.MoveNext();
  for (; pin.MoveNext(); )
  {
    netLRData[netIdx].sinkLoad.push_back(Utils::GetSinkLoad(hd, GetMacrotype(hd, pin)));
  }
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
  c = hd.RoutingLayers.Physics.CPerDist;
  r = hd.RoutingLayers.Physics.RPerDist;

  InitializeNets(hd);
}