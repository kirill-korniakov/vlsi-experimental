#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HCells)

void HCells::Initialize(int cellsLimit)
{
  CHECKIFINITIALIZED();

  this->Grow(cellsLimit);
  m_ld->NetList.cellHeight[0] = 0;
  m_ld->NetList.cellName[0] = "NULL";
  m_ld->NetList.cellOrient[0] = Orientation_Default;
  m_ld->NetList.cellPinEndIdx[0] = 0;
  m_ld->NetList.cellPinStartIdx[0] = 0;
  m_ld->NetList.cellType[0] = 0;
  m_ld->NetList.cellWidth[0] = 0;
  m_ld->NetList.cellX[0] = 0;
  m_ld->NetList.cellY[0] = 0;
  m_ld->NetList.cellPlStatus[0] = PlacementStatus_Default;
  m_ld->NetList.cellCluster[0] = 0;

  for(int i = 0; i < MacroType_Last + 2; i++)
  {
    m_ld->NetList.cellTypeBounds[i].startCellID = 1;
    m_ld->NetList.cellTypeBounds[i].nTerminals = 0;
    m_ld->NetList.cellTypeBounds[i].nFictive = 0;
  }

  m_ld->NetList.nCellsEnd = 1;

  m_PlacementStartType = m_hd->cfg.ValueOf("data.skipSpecialCells", true)
    ? MacroType_FirstSequential
    : MacroType_FirstSpecial;
}

void HCells::Grow(int cellsLimit)
{
  int old_size = m_ld->NetList.nCellsLimit;
  ::Grow(&m_ld->NetList.cellHeight, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellName, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellOrient, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellPinEndIdx, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellPinStartIdx, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellType, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellWidth, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellX, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellY, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellPlStatus, old_size, cellsLimit);
  ::Grow(&m_ld->NetList.cellCluster, old_size, cellsLimit);
  m_ld->NetList.nCellsLimit = cellsLimit;
  OnGrow(old_size, cellsLimit);
}

void HCells::Swap(HCell& first, HCell& second)
{
  BeforeSwap(first, second);

  ::Swap(m_ld->NetList.cellHeight + ::ToID(first), m_ld->NetList.cellHeight + ::ToID(second));
  ::Swap(m_ld->NetList.cellName + ::ToID(first), m_ld->NetList.cellName + ::ToID(second));
  ::Swap(m_ld->NetList.cellOrient + ::ToID(first), m_ld->NetList.cellOrient + ::ToID(second));
  ::Swap(m_ld->NetList.cellPinEndIdx + ::ToID(first), m_ld->NetList.cellPinEndIdx + ::ToID(second));
  ::Swap(m_ld->NetList.cellPinStartIdx + ::ToID(first), m_ld->NetList.cellPinStartIdx + ::ToID(second));
  ::Swap(m_ld->NetList.cellType + ::ToID(first), m_ld->NetList.cellType + ::ToID(second));
  ::Swap(m_ld->NetList.cellWidth + ::ToID(first), m_ld->NetList.cellWidth + ::ToID(second));
  ::Swap(m_ld->NetList.cellPlStatus + ::ToID(first), m_ld->NetList.cellPlStatus + ::ToID(second));
  ::Swap(m_ld->NetList.cellX + ::ToID(first), m_ld->NetList.cellX + ::ToID(second));
  ::Swap(m_ld->NetList.cellY + ::ToID(first), m_ld->NetList.cellY + ::ToID(second));
  ::Swap(m_ld->NetList.cellCluster + ::ToID(first), m_ld->NetList.cellCluster + ::ToID(second));

  ::Swap(&first, &second);
}

HCell HCells::AllocateCell()
{
  int ind = m_ld->NetList.nCellsEnd;
  while (ind >= m_ld->NetList.nCellsLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->NetList.nCellsLimit, "data.cellsLimitGrow", 128, 0, 1.25));
  
  m_ld->NetList.nCellsEnd++;
  HCell res = ::__ConstructPtr<HCell>(ind);

  OnCellAllocation(res);

  return res;
}

void HCells::BuildTerminalsIndex()
{
  m_terminals.Clear();
  for (CellsEnumerator i = GetEnumerator(); i.MoveNext(); )
    if (GetBool<HCell::IsTerminal>(i) && Get<HCell::PlacementStatus, PlacementStatus>(i) != PlacementStatus_Fictive)
    {
      m_terminals.Add(::ToID(i));
    }
}