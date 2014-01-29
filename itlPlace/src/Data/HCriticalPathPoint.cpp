#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HCriticalPathPoints)

//properties specializations
PROPERTYA(HCriticalPathPoints, ::KSignalDirection, HCriticalPathPoint::SignalDirection, m_ld->Timing.cppSigDirection)
PROPERTYA2(HCriticalPathPoints, HTimingPoint, HCriticalPathPoint::TimingPoint, m_ld->Timing.cppTimingPointID)

void HCriticalPathPoints::Initialize(int pointsLimit)
{
  m_initialized = true;
  if(m_ld->Timing.ncppLimit < pointsLimit)
    Grow(pointsLimit);
  m_ld->Timing.ncppsEnd = 1;
  m_ld->Timing.cppSigDirection[0] = SignalDirection_Default;
  m_ld->Timing.cppTimingPointID[0] = 0;

  m_ld->Tech->nPins = 1;
  OnInitialize(pointsLimit);
}

void HCriticalPathPoints::Grow(int pointsLimit)
{
  int old_size = m_ld->Timing.ncppLimit;
  ::Grow(&m_ld->Timing.cppSigDirection, old_size, pointsLimit);
  ::Grow(&m_ld->Timing.cppTimingPointID, old_size, pointsLimit);
  m_ld->Timing.ncppLimit = pointsLimit;
  OnGrow(old_size, pointsLimit);
}

HCriticalPathPoint HCriticalPathPoints::AllocatePoint()
{
  while(m_ld->Timing.ncppsEnd >= m_ld->Timing.ncppLimit)
    Grow(::GetNewSize(m_hd->cfg, m_ld->Timing.ncppLimit, "data.criticalPathPintsLimitGrow", 1, 0, 1.25));

  HCriticalPathPoint res = ::__ConstructPtr<HCriticalPathPoint>(m_ld->Timing.ncppsEnd);
  m_ld->Timing.ncppsEnd++;

  OnPointAllocation(res);

  return res;
}

void HCriticalPathPoints::Swap(HCriticalPathPoint& first, HCriticalPathPoint& second)
{
  BeforeSwap(first, second);

  ::Swap(m_ld->Timing.cppSigDirection + ::ToID(first), m_ld->Timing.cppSigDirection + ::ToID(second));
  ::Swap(m_ld->Timing.cppTimingPointID + ::ToID(first), m_ld->Timing.cppTimingPointID + ::ToID(second));

  ::Swap(&first, &second);
}

void HCriticalPathPoints::SetPoints(HCriticalPath path, HCriticalPathPoint first, HCriticalPathPoint last, bool inverse)
{
  IDType pt1 = m_ld->Timing.cpFirstPointIdx[::ToID(path)] = ::ToID(first);
  IDType pt2 = m_ld->Timing.cpAfterLastPointIdx[::ToID(path)] = ::IsNull(last) ? ::ToID(first) : ::ToID(last) + 1;

  if(inverse)
  {
    --pt2;
    while(pt1 < pt2)
    {
      //KNOTE: not sure if this is correct without reference
      HCriticalPathPoint first = ::__ConstructPtr<HCriticalPathPoint>(pt1++);
      HCriticalPathPoint second = ::__ConstructPtr<HCriticalPathPoint>(pt2--);
      Swap(first, second);
    }
  }
}
