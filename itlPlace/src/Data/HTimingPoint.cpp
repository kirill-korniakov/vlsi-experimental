#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HTimingPoints)

void HTimingPoints::PinsGrowEventHandler(int pinsFrom, int pinsTo)
{
	Grow(pinsFrom == 0 ? 1 : pinsFrom, pinsTo);
}

void HTimingPoints::Grow(int pointsFrom, int pointsTo)
{
	::Grow(&m_ld->Timing.tpFallArrivalTime, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpRiseArrivalTime, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpFallRequiredTime, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpRiseRequiredTime, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpArrivalPropagationOrder, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpRequiredPropagationOrder, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpFallRequiredAncestorPoint, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpFallArrivalAncestorPoint, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpRiseRequiredAncestorPoint, pointsFrom, pointsTo);
  ::Grow(&m_ld->Timing.tpRiseArrivalAncestorPoint, pointsFrom, pointsTo);
  
	OnGrow(pointsFrom, pointsTo);
}

void HTimingPoints::Initialize()
{
  CHECKIFINITIALIZED();

  Grow(0, m_ld->NetList.nPinsLimit == 0 ? 1 : m_ld->NetList.nPinsLimit);
	
  m_ld->Timing.tpFallArrivalTime[0] = 0;
	m_ld->Timing.tpRiseArrivalTime[0] = 0;
  m_ld->Timing.tpFallRequiredTime[0] = 0;
  m_ld->Timing.tpRiseRequiredTime[0] = 0;
  m_ld->Timing.tpArrivalPropagationOrder[0] = 0;
  m_ld->Timing.tpRequiredPropagationOrder[0] = 0;
  m_ld->Timing.tpFallRequiredAncestorPoint[0] = 0;
  m_ld->Timing.tpFallArrivalAncestorPoint[0] = 0;
  m_ld->Timing.tpRiseRequiredAncestorPoint[0] = 0;
  m_ld->Timing.tpRiseArrivalAncestorPoint[0] = 0;

  m_ld->Timing.nTimingStartPoints = 0;
  m_ld->Timing.nTimingEndPoints = 0;
  m_ld->Timing.tpFirstInternalPoint = 0;
  m_ld->Timing.tpLastInternalPoint = 0;

  m_hd->Pins.OnGrow += Event<2, int, int>::EventDelegate(this, &HTimingPoints::PinsGrowEventHandler);

	OnInitialize();
}

void HTimingPoints::CountStartAndEndPoints()
{
  HTimingPointWrapper sp = (*m_hd)[TopologicalOrderRoot()];
  m_ld->Timing.nTimingStartPoints = 0;
  sp.GoNext();
  while (sp.IsTimingStartPoint())
  {
    m_ld->Timing.nTimingStartPoints++;
    sp.GoNext();
  }
  m_ld->Timing.tpFirstInternalPoint = ::ToID(sp);

  HTimingPointWrapper ep = (*m_hd)[TopologicalOrderRoot()];
  m_ld->Timing.nTimingEndPoints = 0;
  ep.GoPrevious();
  while (ep.IsTimingEndPoint())
  {
    m_ld->Timing.nTimingEndPoints++;
    ep.GoPrevious();
  }
  m_ld->Timing.tpLastInternalPoint = ::ToID(ep);
}

void HTimingPoints::ClearTimingResults()
{
  memset(m_ld->Timing.tpFallArrivalTime, 0, m_ld->NetList.nPinsEnd * sizeof(double));
  memset(m_ld->Timing.tpRiseArrivalTime, 0, m_ld->NetList.nPinsEnd * sizeof(double));
  memset(m_ld->Timing.tpFallRequiredTime, 0, m_ld->NetList.nPinsEnd * sizeof(double));
  memset(m_ld->Timing.tpRiseRequiredTime, 0, m_ld->NetList.nPinsEnd * sizeof(double));
  memset(m_ld->Timing.tpFallRequiredAncestorPoint, 0, m_ld->NetList.nPinsEnd * sizeof(IDType));
  memset(m_ld->Timing.tpFallArrivalAncestorPoint, 0, m_ld->NetList.nPinsEnd * sizeof(IDType));
  memset(m_ld->Timing.tpRiseRequiredAncestorPoint, 0, m_ld->NetList.nPinsEnd * sizeof(IDType));
  memset(m_ld->Timing.tpRiseArrivalAncestorPoint, 0, m_ld->NetList.nPinsEnd * sizeof(IDType));
}