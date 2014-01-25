#include "HDPGrid.h"
#include <Math.h>
#include <algorithm>

bool operator< (double y, const HDPGrid::DPGridRow& row)
{
  return row > y;
}

bool operator< (double x, const HDPGrid::DPGridColumn& col)
{
  return col > x;
}


int HDPGrid::RecalcSitesNum(HCell cell)
{
  double w_exact = m_hd.GetDouble<HCell::Width>(cell) / SiteWidth();
  return cellSizeInSites[::ToID(cell)] = w_exact - floor(w_exact) < SiteWidth() / 1000.0
    ? (int)floor(w_exact)
    : (int)ceil(w_exact);
}

void HDPGrid::CellsGrowEventHandler(int cellsFrom, int cellsTo)
{
  ::Grow(&cellRow, cellsFrom, cellsTo);
  ::Grow(&cellSiteIdx, cellsFrom, cellsTo);
  ::Grow(&cellSizeInSites, cellsFrom, cellsTo);
}

void HDPGrid::CellsSwapEventHandler(HCell first, HCell second)
{
  ::Swap(cellRow + ::ToID(first), cellRow + ::ToID(second));
  ::Swap(cellSiteIdx + ::ToID(first), cellSiteIdx + ::ToID(second));
  ::Swap(cellSizeInSites + ::ToID(first), cellSizeInSites + ::ToID(second));
}

HDPGrid::HDPGrid(HDesign& hd):
  m_hd(hd),
  m_rows(0),
  m_columns(0),
  m_SiteType(hd.Sites.Null()),
  nRows(0),
  nColumns(0),
  cellRow(0),
  cellSiteIdx(0),
  cellSizeInSites(0)
{
  m_hd.Cells.OnGrow += Event<2, int, int>::EventDelegate(this, &HDPGrid::CellsGrowEventHandler);
  m_hd.Cells.BeforeSwap += Event<2, HCell, HCell>::EventDelegate(this, &HDPGrid::CellsSwapEventHandler);

  ::Grow(&cellRow, 0, m_hd._Design.NetList.nCellsLimit);
  ::Grow(&cellSiteIdx, 0, m_hd._Design.NetList.nCellsLimit);
  ::Grow(&cellSizeInSites, 0, m_hd._Design.NetList.nCellsLimit);
  cellSizeInSites[0] = 1;

  ASSERT(m_hd.Sites.Count() >= 1);

  nRows = m_hd.PlacementRows.Count();

  ::Grow(&m_rows, 0, nRows);

  double left_x = m_hd.Circuit.PlacementMaxX();
  int curRow = 0;
  int totalRows = nRows;
  for (HPlacementRows::EnumeratorW rIter = m_hd.PlacementRows.GetEnumeratorW(); rIter.MoveNext(); ++curRow)
  {
    if (!rIter.IsHorizontal())
    {
      --totalRows;
      continue;
    }
    if (::IsNull(m_SiteType))
      m_SiteType = rIter.Site();
    if (nColumns < rIter.HorizSitesCount())
      nColumns = rIter.HorizSitesCount();
    if (left_x > rIter.X())
      left_x = rIter.X();
    m_rows[curRow].row = rIter;
    m_rows[curRow].y = rIter.Y();
  } // for (HPlacementRows::EnumeratorW rIter...

  if(totalRows != nRows)
  {
    ::Grow(&m_rows, nRows, totalRows);
    nRows = totalRows;
  }

  std::sort(m_rows, m_rows + nRows, std::less<DPGridRow>());

  ::Grow(&m_columns, 0, nColumns);
  for (int col = 0; col < nColumns; ++col)
    m_columns[col].x = left_x + col * SiteWidth();

  for (int j = 0; j < nRows; ++j)
    ::Grow(&m_rows[j].sites, 0, nColumns);
}

HDPGrid::~HDPGrid()
{
  m_hd.Cells.OnGrow -= Event<2, int, int>::EventDelegate(this, &HDPGrid::CellsGrowEventHandler);
  m_hd.Cells.BeforeSwap -= Event<2, HCell, HCell>::EventDelegate(this, &HDPGrid::CellsSwapEventHandler);

  for (int j = 0; j < nRows; ++j)
    ::Grow(&m_rows[j].sites, nColumns, 0);

  ::Grow(&m_columns, nColumns, 0);
  ::Grow(&m_rows, nRows, 0);
  ::Grow(&cellRow, m_hd._Design.NetList.nCellsLimit, 0);
  ::Grow(&cellSiteIdx, m_hd._Design.NetList.nCellsLimit, 0);
  ::Grow(&cellSizeInSites, m_hd._Design.NetList.nCellsLimit, 0);

  nRows = 0;
  nColumns = 0;
}

void HDPGrid::FindCellsPositions()
{
  double sw_invert = 1.0 / m_hd.GetDouble<HSite::Width>(m_SiteType);
  for(HCells::PlaceableCellsEnumeratorW cIter = m_hd.Cells.GetPlaceableCellsEnumeratorW(); cIter.MoveNext(); )
  {
    int r = FindRow(cIter.Y());
    int column = FindColumn(cIter.X());
    PutCell(cIter, r, column);
  }
}

int HDPGrid::FindRow(double y)
{
  DPGridRow* row = std::lower_bound(m_rows, m_rows + nRows, y);
  int ret = std::max(0, (int)(row - m_rows - 1));
  if (ret < nRows - 1)
  {
    if (y - m_rows[ret].y > m_rows[ret + 1].y - y)
      return ret + 1;
    else
      return ret;
  }
  else
    return nRows - 1;
}

int HDPGrid::FindColumn(double x)
{
  //DPGridColumn* col = std::lower_bound(m_columns, m_columns + nColumns, x);
  //int ret = col - m_columns - 1;
  int ret = (int)(0.49999999 + (x - m_columns[0].x) / SiteWidth());
  return std::min(std::max(0, ret), nColumns - 1);
}
