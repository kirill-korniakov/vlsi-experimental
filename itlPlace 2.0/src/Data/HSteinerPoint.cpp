#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HSteinerPoints)

void HSteinerPoints::PinsGrowEventHandler(int pinsFrom, int pinsTo)
{
  int internalTo = (int)(m_ld->NetList.nPinsLimit 
    * m_hd->cfg.ValueOf("data.steinerPointsToPinsRatio", 1.0));
  if(internalTo < m_ld->Routing.nInternalRPLimit)
    internalTo = m_ld->Routing.nInternalRPLimit;
  Grow(pinsFrom, internalTo, pinsTo);
}

void HSteinerPoints::PinAllocatedEventHandler(HPin pin)
{
	m_ld->Routing.rpLeft[::ToID(pin)] = 0;
	m_ld->Routing.rpRight[::ToID(pin)] = 0;
  m_ld->Routing.rpX[::ToID(pin)] = m_hd->GetDouble<HPin::X>(pin);
	m_ld->Routing.rpY[::ToID(pin)] = m_hd->GetDouble<HPin::Y>(pin);
  m_ld->RCExtraction.rceFallObservedC[::ToID(pin)] = 0;
  m_ld->RCExtraction.rceRiseObservedC[::ToID(pin)] = 0;
  m_ld->RCExtraction.rceRisePointC[::ToID(pin)] = 0;
  m_ld->RCExtraction.rceFallPointC[::ToID(pin)] = 0;
  m_ld->RCExtraction.rceExtractedC[::ToID(pin)] = 0;
  m_ld->RCExtraction.rceExtractedR[::ToID(pin)] = 0;
  m_ld->RCExtraction.rceFallPathDelay[::ToID(pin)] = 0;
  m_ld->RCExtraction.rceRisePathDelay[::ToID(pin)] = 0;
	
  OnSteinerPointAllocation(m_hd->SteinerPoints[pin]);
}

void HSteinerPoints::Initialize()
{
  CHECKIFINITIALIZED();

  m_ld->Routing.nInternalRPCount = 0;
  m_ld->Routing.nFreeRPIndex = 0;
  m_ld->Routing.nInternalRPLimit = 0;

  Grow(
    0,
    (int)(m_ld->NetList.nPinsLimit * m_hd->cfg.ValueOf("data.steinerPointsToPinsRatio", 1.0)),
    m_ld->NetList.nPinsLimit == 0 ? 1 : m_ld->NetList.nPinsLimit
    );

  m_ld->Routing.rpLeft[0] = 0;
  m_ld->Routing.rpRight[0] = 0;
  m_ld->Routing.rpX[0] = 0;
  m_ld->Routing.rpY[0] = 0;
  m_ld->RCExtraction.rceFallObservedC[0] = 0;
  m_ld->RCExtraction.rceRiseObservedC[0] = 0;
  m_ld->RCExtraction.rceRisePointC[0] = 0;
  m_ld->RCExtraction.rceFallPointC[0] = 0;
  m_ld->RCExtraction.rceExtractedC[0] = 0;
  m_ld->RCExtraction.rceExtractedR[0] = 0;
  m_ld->RCExtraction.rceFallPathDelay[0] = 0;
  m_ld->RCExtraction.rceRisePathDelay[0] = 0;

  for (int i = 1; i < m_ld->NetList.nPinsEnd; i++)
  {
	  m_ld->Routing.rpLeft[i] = 0;
	  m_ld->Routing.rpRight[i] = 0;
    OnSteinerPointAllocation(::__ConstructPtr<HSteinerPoint>(i));
  }

  m_hd->Pins.OnGrow += Event<2, int, int>::EventDelegate(this, &HSteinerPoints::PinsGrowEventHandler);
  m_hd->Pins.OnPinAllocation += Event<1, HPin>::EventDelegate(this, &HSteinerPoints::PinAllocatedEventHandler);

  OnInitialize();
}

void HSteinerPoints::Grow(int pinsFrom, int internalTo, int pinsTo)
{
  int internalFrom = m_ld->Routing.nInternalRPLimit;
  ::GrowFromMiddle(&m_ld->Routing.rpLeft, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->Routing.rpRight, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->Routing.rpX, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->Routing.rpY, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceFallObservedC, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceRiseObservedC, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceRisePointC, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceFallPointC, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceExtractedC, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceExtractedR, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceFallPathDelay, internalFrom, pinsFrom, internalTo, pinsTo);
  ::GrowFromMiddle(&m_ld->RCExtraction.rceRisePathDelay, internalFrom, pinsFrom, internalTo, pinsTo);
  m_ld->Routing.nInternalRPLimit = internalTo;

  if(internalFrom < internalTo)
  {
    if(internalFrom == 0) internalFrom++;
    for(int i = -internalTo + 2; i <= -internalFrom; i++)
      m_ld->Routing.rpLeft[i] = i - 1;
    m_ld->Routing.rpLeft[-internalTo + 1] = m_ld->Routing.nFreeRPIndex;
    m_ld->Routing.nFreeRPIndex = -internalFrom;
  }

  OnGrow(internalFrom, internalTo, pinsFrom, pinsTo);
}

HSteinerPoint HSteinerPoints::AllocateSteinerPoint()
{
  while (m_ld->Routing.nFreeRPIndex == 0)
    Grow(m_ld->NetList.nPinsLimit,
      GetNewSize(m_hd->cfg, m_ld->Routing.nInternalRPLimit, "data.internalSteinerPointsGrow", 128, 0, 1.25),
      m_ld->NetList.nPinsLimit);


	HSteinerPoint ret = ::__ConstructPtr<HSteinerPoint>(m_ld->Routing.nFreeRPIndex);
	m_ld->Routing.nFreeRPIndex = m_ld->Routing.rpLeft[m_ld->Routing.nFreeRPIndex];
  m_ld->Routing.rpLeft[::ToID(ret)] = 0;
	m_ld->Routing.nInternalRPCount++;
	
	OnSteinerPointAllocation(ret);

	return ret;
}

void HSteinerPoints::RemoveSteinerTree(HSteinerPoint root)
{
	TemplateTypes<int>::stack candidates;
  candidates.push(m_ld->Routing.rpLeft[::ToID(root)]);
	m_ld->Routing.rpLeft[::ToID(root)] = 0;
	candidates.push(m_ld->Routing.rpRight[::ToID(root)]);
	m_ld->Routing.rpRight[::ToID(root)] = 0;
	while(!candidates.empty())
	{
		int id = candidates.top();
		candidates.pop();
		while(id < 0)
		{
      OnRemoveSteinerPoint(::__ConstructPtr<HSteinerPoint>(id));
			m_ld->Routing.nInternalRPCount--;
			int next = m_ld->Routing.rpLeft[id];
			if(m_ld->Routing.rpRight[id] < 0)
				candidates.push(m_ld->Routing.rpRight[id]);
			m_ld->Routing.rpRight[id] = 0;
			m_ld->Routing.rpLeft[id] = m_ld->Routing.nFreeRPIndex;
			m_ld->Routing.nFreeRPIndex = id;
			id = next;
		}
	} 
}