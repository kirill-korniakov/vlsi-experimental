#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HWires)

void HWires::NetsGrowEventHandler(int netsFrom, int netsTo)
{
	Grow(netsFrom == 0 ? 1 : netsFrom, netsTo);
}

void HWires::NetsSwapEventHandler(HNet first, HNet second)
{
	::Swap(m_ld->Routing.netWL + ::ToID(first), m_ld->Routing.netWL + ::ToID(second));
	::Swap(m_ld->Routing.netRoutingType + ::ToID(first), m_ld->Routing.netRoutingType + ::ToID(second));
}

void HWires::NetsAddEventHandler(HNet net)
{
  m_ld->Routing.netWL[::ToID(net)] = 0;
	m_ld->Routing.netRoutingType[::ToID(net)] = RoutingType_Default;
}

void HWires::Grow(int netsFrom, int netsTo)
{
	::Grow(&m_ld->Routing.netWL, netsFrom, netsTo);
	::Grow(&m_ld->Routing.netRoutingType, netsFrom, netsTo);

	OnGrow(netsFrom, netsTo);
}

void HWires::Initialize()
{
  CHECKIFINITIALIZED();

  Grow(0, m_ld->NetList.nNetsLimit == 0 ? 1 : m_ld->NetList.nNetsLimit);
	
  m_ld->Routing.netWL[0] = 0;
	m_ld->Routing.netRoutingType[0] = RoutingType_Default;

	m_hd->Nets.OnGrow += Event<2, int, int>::EventDelegate(this, &HWires::NetsGrowEventHandler);
	m_hd->Nets.BeforeSwap += Event<2, HNet, HNet>::EventDelegate(this, &HWires::NetsSwapEventHandler);
  m_hd->Nets.OnNetAllocation += Event<1, HNet>::EventDelegate(this, &HWires::NetsAddEventHandler);

	OnInitialize();
}