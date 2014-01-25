#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HPinTypes)

//properties specializations
PROPERTYA(HPinTypes, string, HPinType::Name, m_ld->Tech->pinName)
PROPERTYA(HPinTypes, string, HPinType::OriginalName, m_ld->Tech->pinOriginalName)
PROPERTYA(HPinTypes, PinFunction, HPinType::Function, m_ld->Tech->pinFunction)
PROPERTYA(HPinTypes, PinDirection, HPinType::Direction, m_ld->Tech->pinDirection)
PROPERTYA(HPinTypes, double, HPinType::OriginX, m_ld->Tech->pinOriginX)
PROPERTYA(HPinTypes, double, HPinType::OriginY, m_ld->Tech->pinOriginY)
PROPERTYA(HPinTypes, double, HPinType::FallCapacitance, m_ld->Tech->pinFallCapacitance)
PROPERTYA(HPinTypes, double, HPinType::RiseCapacitance, m_ld->Tech->pinRiseCapacitance)
PROPERTYA(HPinTypes, double, HPinType::Capacitance, m_ld->Tech->pinCapacitance)

//getters specializations
GETTER(HPinTypes, int, HPinType::TimingArcsCount)
{ return m_ld->Tech->pinArcEndIdx[ARGID]
       - m_ld->Tech->pinArcStartIdx[ARGID]; }

GETTER(HPinTypes, HPinType::ArcsEnumerator, HPinType::ArcTypesEnumerator)
{ return HPinType::ArcsEnumerator(m_ld->Tech->pinArcStartIdx[ARGID],
                                  m_ld->Tech->pinArcEndIdx[ARGID]); }

GETTER(HPinTypes, HPinType::ArcsEnumeratorW, HPinType::ArcTypesEnumerator); //implemented in HExternalMethods.h


void HPinTypes::Initialize(int pinsLimit)
{
  CHECKIFINITIALIZED();

  Grow(pinsLimit);
  m_ld->Tech->pinDirection[0] = PinDirection_Default;
  m_ld->Tech->pinFallCapacitance[0] = 0;
  m_ld->Tech->pinName[0] = "NULL";
  m_ld->Tech->pinOriginalName[0] = "NULL";
  m_ld->Tech->pinOriginX[0] = 0;
  m_ld->Tech->pinOriginY[0] = 0;
  m_ld->Tech->pinRiseCapacitance[0] = 0;
  m_ld->Tech->pinFunction[0] = PinFunction_Default;
  m_ld->Tech->pinCapacitance[0] = 0;

  m_ld->Tech->nPins = 1;
  OnInitialize(pinsLimit);
}

void HPinTypes::Grow(int pinsLimit)
{
  int old_size = m_ld->Tech->nPinsLimit;
  ::Grow(&m_ld->Tech->pinDirection, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinFallCapacitance, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinName, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinOriginalName, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinOriginX, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinOriginY, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinRiseCapacitance, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinFunction, old_size, pinsLimit);
  ::Grow(&m_ld->Tech->pinCapacitance, old_size, pinsLimit);
  m_ld->Tech->nPinsLimit = pinsLimit;
  OnGrow(old_size, pinsLimit);
}

HPinType HPinTypes::AllocatePinType()
{
  while(m_ld->Tech->nPins >= m_ld->Tech->nPinsLimit)
    Grow(::GetNewSize(m_hd->cfg, m_ld->Tech->nPinsLimit, "data.pinTypesLimitGrow", 1, 0, 1.25));

  HPinType res = ::__ConstructPtr<HPinType>(m_ld->Tech->nPins);
  m_ld->Tech->nPins++;

  OnPinTypeAllocation(res);

  return res;
}

void HPinTypes::SetPinTypes(HMacroType macro, HPinType first, HPinType last)
{
  m_ld->Tech->macroPinStart[::ToID(macro)] = ::ToID(first);
  m_ld->Tech->macroPinEnd[::ToID(macro)] = ::IsNull(last) ? ::ToID(first) : ::ToID(last) + 1;
}
