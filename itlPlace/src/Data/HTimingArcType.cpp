#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HTimingArcTypes)

//properties specializations
PROPERTYA(HTimingArcTypes, TimingType, HTimingArcType::Type, m_ld->Tech->arcType)
PROPERTYA(HTimingArcTypes, TimingSense, HTimingArcType::TimingSense, m_ld->Tech->arcSense)
PROPERTYA(HTimingArcTypes, double, HTimingArcType::ResistanceRise, m_ld->Tech->arcResistanceRise)
PROPERTYA(HTimingArcTypes, double, HTimingArcType::ResistanceFall, m_ld->Tech->arcResistanceFall)
PROPERTYA(HTimingArcTypes, double, HTimingArcType::TIntrinsicRise, m_ld->Tech->arcTIntrinsicRise)
PROPERTYA(HTimingArcTypes, double, HTimingArcType::TIntrinsicFall, m_ld->Tech->arcTIntrinsicFall)

void HTimingArcTypes::PinTypesGrowEventHandler(int aFrom, int aTo)
{
  ::Grow(&m_ld->Tech->pinArcStartIdx, aFrom, aTo);
  ::Grow(&m_ld->Tech->pinArcEndIdx, aFrom, aTo);
}

void HTimingArcTypes::PinTypeAddedEventHandler(HPinType type)
{
  m_ld->Tech->pinArcStartIdx[::ToID(type)] = 0;
  m_ld->Tech->pinArcEndIdx[::ToID(type)] = 0;
}

void HTimingArcTypes::Initialize(int arcsLimit)
{
  CHECKIFINITIALIZED();

  Grow(arcsLimit);
  m_ld->Tech->arcSense[0] = TimingSense_Default;
  m_ld->Tech->arcStartPinOffset[0] = 0;
  m_ld->Tech->arcTIntrinsicRise[0] = 0.0;
  m_ld->Tech->arcTIntrinsicFall[0] = 0.0;
  m_ld->Tech->arcResistanceRise[0] = 0.0;
  m_ld->Tech->arcResistanceFall[0] = 0.0;
  m_ld->Tech->arcType[0] = TimingType_Default;
  m_ld->Tech->nArcs = 1;

  PinTypesGrowEventHandler(0, m_ld->Tech->nPinsLimit);

  m_hd->PinTypes.OnGrow += Event<2, int, int>::EventDelegate(this, &HTimingArcTypes::PinTypesGrowEventHandler);
  m_hd->PinTypes.OnPinTypeAllocation += Event<1, HPinType>::EventDelegate(this, &HTimingArcTypes::PinTypeAddedEventHandler);

  OnInitialize(arcsLimit);
}

void HTimingArcTypes::Grow(int arcsLimit)
{
  int old_size = m_ld->Tech->nArcsLimit;
  ::Grow(&m_ld->Tech->arcSense, old_size, arcsLimit);
  ::Grow(&m_ld->Tech->arcStartPinOffset, old_size, arcsLimit);
  ::Grow(&m_ld->Tech->arcResistanceRise, old_size, arcsLimit);
  ::Grow(&m_ld->Tech->arcResistanceFall, old_size, arcsLimit);
  ::Grow(&m_ld->Tech->arcTIntrinsicRise, old_size, arcsLimit);
  ::Grow(&m_ld->Tech->arcTIntrinsicFall, old_size, arcsLimit);
  ::Grow(&m_ld->Tech->arcType, old_size, arcsLimit);
  m_ld->Tech->nArcsLimit = arcsLimit;

  OnGrow(old_size, arcsLimit);
}

HTimingArcType HTimingArcTypes::AddNextArcToPinType(HPinType startType, HPinType endType)
{
  while (m_ld->Tech->nArcs >= m_ld->Tech->nArcsLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->Tech->nArcsLimit, "data.timingArcsTypesLimitGrow", 1, 0, 1.25));

  HTimingArcType res = ::__ConstructPtr<HTimingArcType>(m_ld->Tech->nArcs);
  m_ld->Tech->nArcs++;

  m_ld->Tech->arcStartPinOffset[::ToID(res)] = ::ToID(startType) - ::ToID(endType);

  if(m_ld->Tech->pinArcStartIdx[::ToID(endType)] == 0)
    m_ld->Tech->pinArcStartIdx[::ToID(endType)] = ::ToID(res);
  m_ld->Tech->pinArcEndIdx[::ToID(endType)] = ::ToID(res) + 1;

  OnTimingArcTypeAllocation(res);

  return res;
}
