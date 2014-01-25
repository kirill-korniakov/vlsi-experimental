#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HPins)

//properties specializations
PROPERTYA(HPins, double, HPin::OffsetX, m_ld->NetList.pinOffsetX)
PROPERTYA(HPins, double, HPin::OffsetY, m_ld->NetList.pinOffsetY)
PROPERTYA(HPins, PinDirection, HPin::Direction, m_ld->NetList.pinDirection)
PROPERTYA2(HPins, HPinType, HPin::Type, m_ld->NetList.pinType)

//getters specializations
GETTER(HPins, bool, HPin::IsPrimary)
{ return ARGID < m_ld->NetList.nPrimariesEnd && ARGID > 0; }

GETTER(HPins, bool, HPin::IsPrimaryInput)
{ return ARGID < m_ld->NetList.nPrimariesEnd && ARGID > 0
  && m_ld->NetList.pinDirection[ARGID] == PinDirection_OUTPUT; }

GETTER(HPins, bool, HPin::IsPrimaryOutput)
{ return ARGID < m_ld->NetList.nPrimariesEnd && ARGID > 0
  && m_ld->NetList.pinDirection[ARGID] == PinDirection_INPUT; }

GETTER(HPins, double, HPin::X)
{ return m_ld->NetList.cellX[m_ld->NetList.pinCellIdx[ARGID]] + m_ld->NetList.pinOffsetX[ARGID]; }

GETTER(HPins, double, HPin::Y)
{ return m_ld->NetList.cellY[m_ld->NetList.pinCellIdx[ARGID]] + m_ld->NetList.pinOffsetY[ARGID]; }

GETTER(HPins, string, HPin::Name)
{
  return ARGID < m_ld->NetList.nPinsStart
    ? m_ld->NetList.pinName[ARGID]
    : m_ld->Tech->pinName[m_ld->NetList.pinType[ARGID]];
}

GETTER(HPins, HCell, HPin::Cell);       //implemented in HExternalMethods.h
GETTER(HPins, HNet, HPin::Net);         //implemented in HExternalMethods.h
GETTER(HPins, HNet, HPin::OriginalNet); //implemented in HExternalMethods.h

//setters specialization
SETTER(HPins, HNet, HPin::Net); //implemented in HExternalMethods.h

SETTER(HPins, string, HPin::Name)
{
if (ARGID < m_ld->NetList.nPinsStart)
        m_ld->NetList.pinName[ARGID] = value;
    else
  LOGERROR("Unable to set name for non primary arg.");
}


void HPins::Initialize(int primariesNum, int pinsLimit)
{
  CHECKIFINITIALIZED();

  Grow(pinsLimit);
  ::Grow(&m_ld->NetList.pinName, 0, primariesNum + 1);
  
  m_ld->NetList.pinCellIdx[0] = 0;
  m_ld->NetList.pinNetIdx[0] = 0;
  m_ld->NetList.pinOffsetX[0] = 0;
  m_ld->NetList.pinOffsetY[0] = 0;
  m_ld->NetList.pinOriginalNetIdx[0] = 0;
  m_ld->NetList.pinType[0] = 0;
  m_ld->NetList.pinDirection[0] = PinDirection_Default;
  m_ld->NetList.pinName[0] = "NULL";

  m_ld->NetList.nPinsEnd = m_ld->NetList.nPinsStart = primariesNum + 1;
  m_ld->NetList.nPrimariesEnd = 1;

  OnInitialize(pinsLimit);
}

void HPins::Grow(int pinsLimit)
{
  int old_size = m_ld->NetList.nPinsLimit;
  ::Grow(&m_ld->NetList.pinCellIdx, old_size, pinsLimit);
  ::Grow(&m_ld->NetList.pinNetIdx, old_size, pinsLimit);
  ::Grow(&m_ld->NetList.pinOffsetX, old_size, pinsLimit);
  ::Grow(&m_ld->NetList.pinOffsetY, old_size, pinsLimit);
  ::Grow(&m_ld->NetList.pinOriginalNetIdx, old_size, pinsLimit);
  ::Grow(&m_ld->NetList.pinType, old_size, pinsLimit);
  ::Grow(&m_ld->NetList.pinDirection, old_size, pinsLimit);
  m_ld->NetList.nPinsLimit = pinsLimit;

  OnGrow(old_size, pinsLimit);
}

HPin HPins::AllocatePrimaryPin()
{
  int ind = m_ld->NetList.nPrimariesEnd;

  if (ind >= m_ld->NetList.nPinsStart)
  {
    if (m_ld->NetList.nPinsStart == m_ld->NetList.nPinsEnd)
    {
      int new_size = ind * 3 / 2 + 1;
      ::Grow(&m_ld->NetList.pinName, m_ld->NetList.nPinsStart, new_size);
      if (m_ld->NetList.nPinsLimit < new_size)
        Grow(new_size);
      m_ld->NetList.nPinsStart = m_ld->NetList.nPinsEnd = new_size;
    }
    else
      LOGCRITICAL("Unable to allocate new primary pin. No space is available.");
  }

  m_ld->NetList.nPrimariesEnd++;
  HPin res = ::__ConstructPtr<HPin>(ind);
  m_ld->NetList.pinCellIdx[ind] = 0;
  m_ld->NetList.pinNetIdx[ind] = 0;
  m_ld->NetList.pinOriginalNetIdx[ind] = 0;
  OnPinAllocation(res);
  return res;
}
 
void HPins::AllocatePins(HCell cell)
{
  HMacroType cell_type = m_hd->Get<HCell::MacroType, HMacroType>(cell);
  int pc = m_hd->GetInt<HMacroType::PinsCount>(cell_type);
  
  while(m_ld->NetList.nPinsEnd + pc > m_ld->NetList.nPinsLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->NetList.nPinsLimit, "data.pinsLimitGrow", 1, m_ld->NetList.nPinsEnd + pc, 1.25));

  m_ld->NetList.cellPinStartIdx[::ToID(cell)] = m_ld->NetList.nPinsEnd;
  m_ld->NetList.nPinsEnd += pc;
  m_ld->NetList.cellPinEndIdx[::ToID(cell)] = m_ld->NetList.nPinsEnd;

  HCell::PinsEnumeratorW pi = m_hd->Cells.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell);
  HMacroType::PinsEnumerator mpi = m_hd->Get<HMacroType::PinTypesEnumerator, HMacroType::PinsEnumerator>(cell_type);
  for(; pi.MoveNext(), mpi.MoveNext();)
  {
    pi.SetOffsetX(m_hd->GetDouble<HPinType::OriginX>(mpi));
    pi.SetOffsetY(m_hd->GetDouble<HPinType::OriginY>(mpi));
    pi.SetType(mpi);
    pi.SetDirection(m_hd->Get<HPinType::Direction, PinDirection>(mpi));
    m_ld->NetList.pinCellIdx[::ToID(pi)] = ::ToID(cell);
    OnPinAllocation(pi);
  }
}
