#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HNets)

//properties specializations
PROPERTYA(HNets, string, HNet::Name, m_ld->NetList.netName)
PROPERTYA(HNets, double, HNet::HPWL, m_ld->NetList.netHPWL)
PROPERTYA(HNets, double, HNet::Weight, m_ld->NetList.netWeight)
PROPERTYA(HNets, double, HNet::LNet, m_ld->NetList.netLnet)

//getters specializations
GETTERA(HNets, NetKind, HNet::Kind, m_ld->NetList.netKind)

GETTER(HNets, HPin, HNet::Source)
  { return ::__ConstructPtr<HPin>(m_ld->NetList.netPins[m_ld->NetList.netPinStart[ARGID]]); }

GETTER(HNets, HPin, HNet::LastSink)
  { return ::__ConstructPtr<HPin>(m_ld->NetList.netPins[m_ld->NetList.netPinEnd[ARGID] - 1]); }

GETTER(HNets, int, HNet::PinsCount)
  { return m_ld->NetList.netPinEnd[ARGID]
         - m_ld->NetList.netPinStart[ARGID]; }

GETTER(HNets, int, HNet::SinksCount)
  { return m_ld->NetList.netPinEnd[ARGID]
         - m_ld->NetList.netPinStart[ARGID] - 1; }

GETTER(HNets, HNet::PinsEnumeratorW, HNet::Pins);
GETTER(HNets, HNet::SinksEnumeratorW, HNet::Sinks);

//setters specializations
SETTER(HNets, HPin, HNet::Source)
  { AssignPin(arg, m_ld->NetList.netPinStart[ARGID], value); }

SETTER(HNets, NetKind, HNet::Kind)
{
#pragma warning( push )
#pragma warning(disable:6385)
  if (m_ld->NetList.netKind[ARGID] != value)
  {
    m_ld->NetList.nNetsByKind[value] += 1;
    m_ld->NetList.nNetsByKind[m_ld->NetList.netKind[ARGID]] -= 1;
    m_ld->NetList.netKind[ARGID] = value;
  }
#pragma warning( pop )
}

void HNets::PinsGrowEventHandler(int pinsFrom, int pinsTo)
{
  if(m_ld->NetList.nNetPinLimit < pinsTo)
  {
    ::Grow(&m_ld->NetList.netPins, m_ld->NetList.nNetPinLimit, pinsTo);
    m_ld->NetList.nNetPinLimit = pinsTo;
  }
}

void HNets::Initialize(int netsLimit)
{
  CHECKIFINITIALIZED();

  Grow(netsLimit);

  m_ld->NetList.netName[0] = "NULL";
  m_ld->NetList.netHPWL[0] = 0.0;
  m_ld->NetList.netWeight[0] = 1.0;
  m_ld->NetList.netKind[0] = NetKind_Default;
  m_ld->NetList.netPinEnd[0] = 0;
  m_ld->NetList.netPinStart[0] = 0;
  m_ld->NetList.netLnet[0] = 0;

  m_ld->NetList.nNetsBaseEnd = 1;
  m_ld->NetList.nNetsEnd = 1;
  
  PinsGrowEventHandler(0, m_ld->NetList.nPinsLimit > 0 ? m_ld->NetList.nPinsLimit : 1);
  m_ld->NetList.netPins[0] = 0;
  m_ld->NetList.nNetPinEnd = 1;

  m_hd->Pins.OnGrow += Event<2, int, int>::EventDelegate(this, &HNets::PinsGrowEventHandler);

  OnInitialize(netsLimit);
}

void HNets::Grow(int netsLimit)
{
  int old_size = m_ld->NetList.nNetsLimit;
  ::Grow(&m_ld->NetList.netName, old_size, netsLimit);
  ::Grow(&m_ld->NetList.netHPWL, old_size, netsLimit);
  ::Grow(&m_ld->NetList.netWeight, old_size, netsLimit);
  ::Grow(&m_ld->NetList.netKind, old_size, netsLimit);
  ::Grow(&m_ld->NetList.netPinEnd, old_size, netsLimit);
  ::Grow(&m_ld->NetList.netPinStart, old_size, netsLimit);
  ::Grow(&m_ld->NetList.netLnet, old_size, netsLimit);
  m_ld->NetList.nNetsLimit = netsLimit;
  OnGrow(old_size, netsLimit);
}

void HNets::Swap(HNet& first, HNet& second)
{
  BeforeSwap(first, second);

  ::Swap(m_ld->NetList.netName + ::ToID(first), m_ld->NetList.netName + ::ToID(second));
  ::Swap(m_ld->NetList.netHPWL + ::ToID(first), m_ld->NetList.netHPWL + ::ToID(second));
  ::Swap(m_ld->NetList.netWeight + ::ToID(first), m_ld->NetList.netWeight + ::ToID(second));
  ::Swap(m_ld->NetList.netKind + ::ToID(first), m_ld->NetList.netKind + ::ToID(second));
  ::Swap(m_ld->NetList.netPinEnd + ::ToID(first), m_ld->NetList.netPinEnd + ::ToID(second));
  ::Swap(m_ld->NetList.netPinStart + ::ToID(first), m_ld->NetList.netPinStart + ::ToID(second));
  ::Swap(m_ld->NetList.netLnet + ::ToID(first), m_ld->NetList.netLnet + ::ToID(second));

  ::Swap(&first, &second);
}

HNet HNets::AllocateNet(bool isBase)
{
  int ind = m_ld->NetList.nNetsEnd;
  while(ind >= m_ld->NetList.nNetsLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->NetList.nNetsLimit, "data.netsLimitGrow", 1, 0, 1.25));

  m_ld->NetList.nNetsEnd++;
  HNet res = ::__ConstructPtr<HNet>(ind);
  if(isBase)
  {
    if(m_ld->NetList.nNetsBaseEnd != ::ToID(res))
    {
      HNet base_net = ::__ConstructPtr<HNet>(m_ld->NetList.nNetsBaseEnd);
      Swap(res, base_net);
    }
    m_ld->NetList.nNetsBaseEnd++;
  }

  m_ld->NetList.netKind[ind] = NetKind_Default;
  m_ld->NetList.nNetsByKind[NetKind_Default]++;
  m_ld->NetList.netWeight[ind] = 1.0;
  m_ld->NetList.netLnet[ind] = 0.0;

  OnNetAllocation(res);

  return res;
}

void HNets::AllocatePins(HNet net, int count)
{
  if (m_ld->NetList.nNetPinEnd + count > m_ld->NetList.nNetPinLimit)
  {
    int npinsLimit = ::GetNewSize(m_hd->cfg, m_ld->NetList.nNetPinLimit, "data.netPinsLimitGrow", 1, m_ld->NetList.nNetPinEnd + count, 1.25); 
    ::Grow(&m_ld->NetList.netPins, m_ld->NetList.nNetPinLimit, npinsLimit);
    m_ld->NetList.nNetPinLimit = npinsLimit;
  }

  m_ld->NetList.netPinStart[::ToID(net)] = m_ld->NetList.nNetPinEnd;
  m_ld->NetList.nNetPinEnd += count;
  m_ld->NetList.netPinEnd[::ToID(net)] = m_ld->NetList.nNetPinEnd;
}

void HNets::AssignPin(HNet net, HNet::PinsEnumeratorW& netPin, HPin pin)
{
  AssignPin(net, netPin.NetPinID(), pin);
}

void HNets::AssignPin(HNet net, IDType netPinID, HPin pin)
{
  int ind = ::ToID(pin);
  m_ld->NetList.pinNetIdx[ind] = ::ToID(net);
  if (m_ld->NetList.pinOriginalNetIdx[ind] == 0)
    m_ld->NetList.pinOriginalNetIdx[ind] = ::ToID(net);
  m_ld->NetList.netPins[netPinID] = ind;
}
