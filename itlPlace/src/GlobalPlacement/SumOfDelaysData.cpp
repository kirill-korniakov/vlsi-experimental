#include "SumOfDelaysData.h"
#include "Utils.h"

void SODData::InitializeNets(HDesign& hd)
{
  netSODData.resize(hd.Nets.Count());

  //put nets
  int netIdx = 0;
  for (HNets::NetsEnumeratorW net = hd.Nets.GetNetsEnumeratorW(); net.MoveNext(); )
  {
    netSODData[netIdx].Lnet = net.LNet();
    netIdx++;
  }
}

void SODData::Initialize(HDesign& hd)
{
  double L, D;
  Utils::CalcBufferLD(hd, 0, &L, &D);
  Lbuf = L;
  Dbuf = D;
  DbufLbufDifferenceOfSquares = D*D - L*L;

  gradientBalance = hd.cfg.ValueOf("TAOOptions.gradientBalance", 1.0);

  InitializeNets(hd);
}