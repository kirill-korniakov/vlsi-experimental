#include "SumOfDelaysData.h"
#include "Utils.h"
#include "BufferInfo.h"

void SODData::InitializeNets(HDesign& hd)
{
  netSODData.resize(hd.Nets.Count());

  //put nets
  int netIdx = 0;
  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetNetsEnumeratorW(); net.MoveNext(); )
  {
    netSODData[netIdx].Lnet = net.LNet();
    netIdx++;
  }
}

void SODData::Initialize(HDesign& hd)
{
  BufferInfo bi = BufferInfo::Create(hd);
  Lbuf = bi.Lbuf();
  Dbuf = bi.Dbuf();
  DbufLbufDifferenceOfSquares = Dbuf*Dbuf - Lbuf*Lbuf;

  gradientBalance = hd.cfg.ValueOf("TAOOptions.gradientBalance", 1.0);

  InitializeNets(hd);
}