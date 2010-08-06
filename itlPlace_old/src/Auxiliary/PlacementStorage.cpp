#include "PlacementStorage.h"

void PlacementStorage::SavePlacement(HDesign& design)
{
  int cellsCount = design.Cells.CellsCount();
  if (m_allocated_space < cellsCount)
  {
    ::Grow(&m_xrow, m_allocated_space, cellsCount);
    ::Grow(&m_yrow, m_allocated_space, cellsCount);
    m_allocated_space = cellsCount;
  }

  m_data_size = cellsCount;
  int i = 0;
  for (HCells::CellsEnumeratorW cell = design.Cells.GetEnumeratorW(); cell.MoveNext(); ++i)
  {
    m_xrow[i] = cell.X();
    m_yrow[i] = cell.Y();
  }
  ASSERT(i == cellsCount);
}

void PlacementStorage::RestorePlacement(HDesign& design) const
{
  if (design.Cells.CellsCount() != m_data_size)
  {
    LOGINFO("Netlist was probably modified. Placement can be restored incorrectly.");
  }
  int i = 0;
  for (HCells::CellsEnumeratorW cell = design.Cells.GetEnumeratorW(); cell.MoveNext(); ++i)
  {
    if (i >= m_data_size) break;
    cell.SetX(m_xrow[i]);
    cell.SetY(m_yrow[i]);
  }
}

PlacementStorage::~PlacementStorage()
{
  ::Grow(&m_xrow, m_allocated_space, 0);
  ::Grow(&m_yrow, m_allocated_space, 0);
  m_allocated_space = m_data_size = 0;
}