#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HTimingPoints)

//properties specializations
PROPERTYA(HTimingPoints, double, HTimingPoint::FallArrivalTime, m_ld->Timing.tpFallArrivalTime)
PROPERTYA(HTimingPoints, double, HTimingPoint::RiseArrivalTime, m_ld->Timing.tpRiseArrivalTime)
PROPERTYA(HTimingPoints, double, HTimingPoint::FallRequiredTime, m_ld->Timing.tpFallRequiredTime)
PROPERTYA(HTimingPoints, double, HTimingPoint::RiseRequiredTime, m_ld->Timing.tpRiseRequiredTime)
PROPERTYA2(HTimingPoints, HTimingPoint, HTimingPoint::NextPoint, m_ld->Timing.tpArrivalPropagationOrder);
PROPERTYA2(HTimingPoints, HTimingPoint, HTimingPoint::PreviousPoint, m_ld->Timing.tpRequiredPropagationOrder);

//getters specializations
GETTER(HTimingPoints, double, HTimingPoint::ArrivalTime)
  { return std::max(GetDouble<HTimingPoint::FallArrivalTime>(arg), GetDouble<HTimingPoint::RiseArrivalTime>(arg)); }

GETTER(HTimingPoints, double, HTimingPoint::RequiredTime)
  { return std::min(GetDouble<HTimingPoint::FallRequiredTime>(arg), GetDouble<HTimingPoint::RiseRequiredTime>(arg)); }

GETTER(HTimingPoints, HPin, HTimingPoint::Pin)
  { return ::__ConstructPtr<HPin>(arg); }

GETTER(HTimingPoints, bool, HTimingPoint::IsTimingStartPoint);
GETTER(HTimingPoints, bool, HTimingPoint::IsTimingEndPoint);

GETTER(HTimingPoints, double, HTimingPoint::FallSlack)
  { return GetDouble<HTimingPoint::FallRequiredTime>(arg) - GetDouble<HTimingPoint::FallArrivalTime>(arg); }

GETTER(HTimingPoints, double, HTimingPoint::RiseSlack)
  { return GetDouble<HTimingPoint::RiseRequiredTime>(arg) - GetDouble<HTimingPoint::RiseArrivalTime>(arg); }

GETTER(HTimingPoints, double, HTimingPoint::Slack)
  { return GetDouble<HTimingPoint::RequiredTime>(arg) - GetDouble<HTimingPoint::ArrivalTime>(arg); }

GETTER(HTimingPoints, double, HTimingPoint::FallNegativeSlack)
  { return std::max(0.0, -GetDouble<HTimingPoint::FallSlack>(arg)); }

GETTER(HTimingPoints, double, HTimingPoint::RiseNegativeSlack)
  { return std::max(0.0, -GetDouble<HTimingPoint::RiseSlack>(arg)); }

GETTER(HTimingPoints, double, HTimingPoint::NegativeSlack)
  { return std::max(0.0, -GetDouble<HTimingPoint::Slack>(arg)); }

GETTER(HTimingPoints, HTimingPoint, HTimingPoint::FallRequiredAncestor)
  { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

GETTER(HTimingPoints, HTimingPoint, HTimingPoint::RiseRequiredAncestor)
  { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

GETTER(HTimingPoints, HTimingPoint, HTimingPoint::FallArrivalAncestor)
  { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

GETTER(HTimingPoints, HTimingPoint, HTimingPoint::RiseArrivalAncestor)
  { return ::__ConstructPtr<HTimingPoint, IDType>(m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] & (~mc_InversedSignalFlag)); }

GETTER(HTimingPoints, HTimingPoint, HTimingPoint::RequiredAncestor)
  { return Get<HTimingPoint::RiseRequiredAncestor, HTimingPoint>(arg); }

GETTER(HTimingPoints, HTimingPoint, HTimingPoint::ArrivalAncestor)
  { return Get<HTimingPoint::RiseArrivalAncestor, HTimingPoint>(arg); }

GETTER(HTimingPoints, bool, HTimingPoint::IsRiseArrivalInversed)
  { return 0 != (m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] & mc_InversedSignalFlag); }

GETTER(HTimingPoints, bool, HTimingPoint::IsFallArrivalInversed)
  { return 0 != (m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] & mc_InversedSignalFlag); }

GETTER(HTimingPoints, bool, HTimingPoint::IsRiseRequiredInversed)
  { return 0 != (m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] & mc_InversedSignalFlag); }

GETTER(HTimingPoints, bool, HTimingPoint::IsFallRequiredInversed)
  { return 0 != (m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] & mc_InversedSignalFlag); }

//setters specialization
SETTER(HTimingPoints, double, HTimingPoint::ArrivalTime)
{
  Set<HTimingPoint::RiseArrivalTime>(arg, value);
  Set<HTimingPoint::FallArrivalTime>(arg, value);
}

SETTER(HTimingPoints, double, HTimingPoint::RequiredTime)
{
  Set<HTimingPoint::RiseRequiredTime>(arg, value);
  Set<HTimingPoint::FallRequiredTime>(arg, value);
}

SETTERA2(HTimingPoints, HTimingPoint, HTimingPoint::FallRequiredAncestor, m_ld->Timing.tpFallRequiredAncestorPoint);
SETTERA2(HTimingPoints, HTimingPoint, HTimingPoint::RiseRequiredAncestor, m_ld->Timing.tpRiseRequiredAncestorPoint);
SETTERA2(HTimingPoints, HTimingPoint, HTimingPoint::FallArrivalAncestor, m_ld->Timing.tpFallArrivalAncestorPoint);
SETTERA2(HTimingPoints, HTimingPoint, HTimingPoint::RiseArrivalAncestor, m_ld->Timing.tpRiseArrivalAncestorPoint);

SETTER(HTimingPoints, HTimingPoint, HTimingPoint::RequiredAncestor)
{
  Set<HTimingPoint::RiseRequiredAncestor>(arg, value);
  Set<HTimingPoint::FallRequiredAncestor>(arg, value);
}

SETTER(HTimingPoints, HTimingPoint, HTimingPoint::ArrivalAncestor)
{
  Set<HTimingPoint::RiseArrivalAncestor>(arg, value);
  Set<HTimingPoint::FallArrivalAncestor>(arg, value);
}

SETTER(HTimingPoints, bool, HTimingPoint::IsRiseArrivalInversed)
{
  if (value)
    m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] |= mc_InversedSignalFlag;
  else
    m_ld->Timing.tpRiseArrivalAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
}

SETTER(HTimingPoints, bool, HTimingPoint::IsFallArrivalInversed)
{
  if (value)
    m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] |= mc_InversedSignalFlag;
  else
    m_ld->Timing.tpFallArrivalAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
}

SETTER(HTimingPoints, bool, HTimingPoint::IsRiseRequiredInversed)
{
  if (value)
    m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] |= mc_InversedSignalFlag;
  else
    m_ld->Timing.tpRiseRequiredAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
}

SETTER(HTimingPoints, bool, HTimingPoint::IsFallRequiredInversed)
{
  if (value)
    m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] |= mc_InversedSignalFlag;
  else
    m_ld->Timing.tpFallRequiredAncestorPoint[ARGID] &= ~mc_InversedSignalFlag;
}

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
