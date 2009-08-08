#ifndef __HIGH_EXTERNALMETHODS_H__
#define __HIGH_EXTERNALMETHODS_H__

#include "HDesign.h"

//Steiner Points
inline void HSteinerPoints::InheritPinCoordinates(HSteinerPoint point)
{
  HPin pin = Get<HSteinerPoint::Pin, HPin>(point);
  Set<HSteinerPoint::X>(point, m_hd->GetDouble<HPin::X>(pin));
  Set<HSteinerPoint::Y>(point, m_hd->GetDouble<HPin::Y>(pin));
}

//Timing arcs
inline HPinType HTimingArcTypes::GetStartPinType(HTimingArcType arcType, HPinType endPinType) const
{ return ::__ConstructPtr<HPinType>(::ToID(endPinType) + m_ld->Tech->arcStartPinOffset[::ToID(arcType)]); }

inline HPin HTimingArcTypes::GetStartPin(HTimingArcType arcType, HPin endPin) const
{ return ::__ConstructPtr<HPin>(::ToID(endPin) + m_ld->Tech->arcStartPinOffset[::ToID(arcType)]); }

inline HPinType HTimingArcTypes::GetEndPinType(HTimingArcType arcType, HPinType startPinType) const
{ return ::__ConstructPtr<HPinType>(::ToID(startPinType) - m_ld->Tech->arcStartPinOffset[::ToID(arcType)]);}

inline HPin HTimingArcTypes::GetEndPin(HTimingArcType arcType, HPin startPin) const
{ return ::__ConstructPtr<HPin>(::ToID(startPin) - m_ld->Tech->arcStartPinOffset[::ToID(arcType)]); }

inline HPinType HTimingArcTypeWrapper::GetStartPinType(HPinType endPinType) const
{ return m_Parent->GetStartPinType(*this, endPinType); }

inline HPin HTimingArcTypeWrapper::GetStartPin(HPin endPin) const
{ return m_Parent->GetStartPin(*this, endPin); }

inline HPinType HTimingArcTypeWrapper::GetEndPinType(HPinType startPinType) const
{ return m_Parent->GetEndPinType(*this, startPinType); }

inline HPin HTimingArcTypeWrapper::GetEndPin(HPin startPin) const
{ return m_Parent->GetEndPin(*this, startPin); }

//Pins
IGETTER(HCell, HPin::Cell, HPins)
{ return ::__ConstructPtr<HCell>(m_ld->NetList.pinCellIdx[ARGID]); }

IGETTER(HNet, HPin::Net, HPins)
{ return ::__ConstructPtr<HNet>(m_ld->NetList.pinNetIdx[ARGID]); }

IGETTER(HNet, HPin::OriginalNet, HPins)
{ return ::__ConstructPtr<HNet>(m_ld->NetList.pinOriginalNetIdx[ARGID]); }

IGETTERWD(HCell, HPinWrapper, Cell)
IGETTERWD(HNet, HPinWrapper, Net)
IGETTERWD(HNet, HPinWrapper, OriginalNet)

ISETTER(HNet, HPin::Net, HPins)
{ m_ld->NetList.pinNetIdx[ARGID] = ::ToID(value); }

ISETTERWD(HNet, HPinWrapper, Net)

//Cells
IGETTER(HCell::PinsEnumeratorW, HCell::Pins, HCells)
{ 
  return HCell::PinsEnumeratorW(
    &m_hd->Pins,
    m_ld->NetList.cellPinStartIdx[ARGID],
    m_ld->NetList.cellPinEndIdx[ARGID]);
}

//nets
IGETTER(HNet::PinsEnumeratorW, HNet::Pins, HNets)
{ 
  return HNet::PinsEnumeratorW(
    &m_hd->Pins,
    m_ld->NetList.netPinStart[ARGID],
    m_ld->NetList.netPinEnd[ARGID]); 
}

IGETTER(HNet::SinksEnumeratorW, HNet::Sinks, HNets)
{ 
  return HNet::SinksEnumeratorW(
    &m_hd->Pins,
    m_ld->NetList.netPinStart[ARGID] + 1,
    m_ld->NetList.netPinEnd[ARGID]); 
}

//Macro Types
IGETTER(HMacroType::PinsEnumeratorW, HMacroType::PinTypesEnumerator, HMacroTypes)
{ 
  return HMacroType::PinsEnumeratorW(
    &m_hd->PinTypes,
    m_ld->Tech->macroPinStart[ARGID],
    m_ld->Tech->macroPinEnd[ARGID]);
}

//Pin Types
IGETTER(HPinType::ArcsEnumeratorW, HPinType::ArcTypesEnumerator, HPinTypes)
{ 
  return HPinType::ArcsEnumeratorW(
    &m_hd->TimingArcTypes,
    m_ld->Tech->pinArcStartIdx[ARGID],
    m_ld->Tech->pinArcEndIdx[ARGID]);
}

//Timing Points
IGETTER(bool, HTimingPoint::IsTimingStartPoint, HTimingPoints)
{
  HPin pin = Get<HTimingPoint::Pin, HPin>(arg);
  if (m_hd->Get<HPin::Direction, PinDirection>(pin) != PinDirection_OUTPUT)
    return false;
  if (m_hd->GetBool<HPin::IsPrimary>(pin))
    return true;
  return m_hd->GetBool<HCell::IsSequential>(m_hd->Get<HPin::Cell, HCell>(pin))
    && !::IsNull(m_hd->Get<HPin::Net, HNet>(pin));
}

IGETTER(bool, HTimingPoint::IsTimingEndPoint, HTimingPoints)
{
  HPin pin = Get<HTimingPoint::Pin, HPin>(arg);
  if (m_hd->Get<HPin::Direction, PinDirection>(pin) != PinDirection_INPUT)
    return false;
  if (m_hd->GetBool<HPin::IsPrimary>(pin))
    return true;
  return m_hd->GetBool<HCell::IsSequential>(m_hd->Get<HPin::Cell, HCell>(pin))
    && !::IsNull(m_hd->Get<HPin::Net, HNet>(pin));
}

//Critical paths
IGETTER(HCriticalPath::PointsEnumeratorW, HCriticalPath::Points, HCriticalPaths)
{
  return HCriticalPath::PointsEnumeratorW(
    &m_hd->CriticalPathPoints,
    m_ld->Timing.cpFirstPointIdx[ARGID],
    m_ld->Timing.cpAfterLastPointIdx[ARGID]);
}

IGETTER(double, HCriticalPath::Criticality, HCriticalPaths)
{
  HCriticalPathPoint cpt = this->Get<HCriticalPath::EndPoint, HCriticalPathPoint>(arg);
  if (m_hd->Get<HCriticalPathPoint::SignalDirection, ::SignalDirection>(cpt) == SignalDirection_Fall)
    return m_hd->GetDouble<HTimingPoint::FallSlack>(m_hd->Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(cpt));
  else
    return m_hd->GetDouble<HTimingPoint::RiseSlack>(m_hd->Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(cpt));
}

#endif //__HIGH_EXTERNALMETHODS_H__