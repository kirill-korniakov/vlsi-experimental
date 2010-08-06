#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HMacroTypes)

void HMacroTypes::Initialize(int macrosLimit)
{
  CHECKIFINITIALIZED();

  Grow(macrosLimit);
  m_ld->Tech->macroOriginX[0] = 0;
  m_ld->Tech->macroOriginY[0] = 0;
  m_ld->Tech->macroName[0] = "NULL";
  m_ld->Tech->macroOriginalName[0] = "NULL";
  m_ld->Tech->macroPinEnd[0] = 0;
  m_ld->Tech->macroPinStart[0] = 0;
  m_ld->Tech->macroSizeX[0] = 0;
  m_ld->Tech->macroSizeY[0] = 0;
  m_ld->Tech->macroType[0] = MacroType_Default;

  m_ld->Tech->nMacro = 1;
  OnInitialize(macrosLimit);
}

void HMacroTypes::Grow(int macrosLimit)
{
  int old_size = m_ld->Tech->nMacroLimit;
  ::Grow(&m_ld->Tech->macroName, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroOriginalName, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroOriginX, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroOriginY, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroPinEnd, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroPinStart, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroSizeX, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroSizeY, old_size, macrosLimit);
  ::Grow(&m_ld->Tech->macroType, old_size, macrosLimit);
  m_ld->Tech->nMacroLimit = macrosLimit;

  OnGrow(old_size, macrosLimit);
}

HMacroType HMacroTypes::AllocateMacroType()
{
  while (m_ld->Tech->nMacro >= m_ld->Tech->nMacroLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->Tech->nMacroLimit, "data.macroTypesLimitGrow", 1, 0, 1.25));

  HMacroType res = ::__ConstructPtr<HMacroType>(m_ld->Tech->nMacro);
  m_ld->Tech->nMacro++;

  OnMacroTypeAllocation(res);

  return res;
}
