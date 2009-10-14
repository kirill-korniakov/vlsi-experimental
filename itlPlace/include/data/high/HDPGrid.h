#ifndef __HIGH_DPGRID_H__
#define __HIGH_DPGRID_H__

#include "HDesign.h"

class HDPGrid
{
  struct DPGridRow
  {
    double y;
    HPlacementRow row;
    HCell *sites;

    bool operator < (const DPGridRow& other) const { return y < other.y; }
    bool operator < (double Y) const { return y < Y; }
    bool operator > (double Y) const { return y > Y; }
    DPGridRow(): y(0), row(), sites(0) {}
  };
  friend bool operator< (double y, const DPGridRow& row);

  struct DPGridColumn
  {
    double x;

    bool operator < (const DPGridColumn& other) const { return x < other.x; }
    bool operator < (double X) const { return x < X; }
    bool operator > (double X) const { return x > X; }
  };
  friend bool operator< (double x, const DPGridColumn& col);

  int* cellRow;
  int* cellSiteIdx;
  int* cellSizeInSites;

  HDesign &m_hd;
  DPGridRow* m_rows;
  DPGridColumn* m_columns;
  int nRows;
  int nColumns;
  HSite m_SiteType;

private: void CellsGrowEventHandler(int cellsFrom, int cellsTo);
private: void CellsSwapEventHandler(HCell first, HCell second);
private: void CellsAddEventHandler(HCell cell);

  //denied
  HDPGrid(const HDPGrid&);
  HDPGrid& operator = (const HDPGrid&);

public:
  HDPGrid(HDesign& hd);
  ~HDPGrid();
  int RecalcSitesNum(HCell cell);
  void FindCellsPositions();
  int FindRow(double y);
  int FindColumn(double x);
  double ColumnX(int column) const { return m_columns[column].x; }
  double RowY(int row) const { return m_rows[row].y; }
  double SiteWidth() const { return m_hd.GetDouble<HSite::Width>(m_SiteType); }
  double SiteHeight() const { return m_hd.GetDouble<HSite::Height>(m_SiteType); }
  bool IsColumnIdxInRow(int columnIdx) { return columnIdx >= 0 && columnIdx < NumCols(); }

  HCell operator() (int row_num, int site_num) const
  { 
    ASSERT(row_num >= 0 && site_num >= 0 && row_num < nRows && site_num < nColumns);
    return m_rows[row_num].sites[site_num];
  }

  int NumRows() const { return nRows; }
  int NumCols() const { return nColumns; }
  HDesign& Design() { return m_hd; }

  int CellSitesNum(HCell cell) const { return cellSizeInSites[::ToID(cell)]; }
  int CellColumn(HCell cell) const { return cellSiteIdx[::ToID(cell)]; }
  int CellRow(HCell cell) const { return cellRow[::ToID(cell)]; }
  HPlacementRow CellPlacementRow(HCell cell) { return m_rows[CellRow(cell)].row; }

  void PutCell(HCell cell, int row, int column)
  {
    int row_num = CellRow(cell);
    int site_num = CellColumn(cell);
    int nSites = CellSitesNum(cell);

    ASSERT(row >= 0 && column >= 0 && row < nRows && column + nSites <= nColumns);

    if(nSites == 0)
    {
      nSites = RecalcSitesNum(cell);
      for (int i = 0; i < nSites; ++i)
        m_rows[row].sites[column + i] = cell;
    }
    else
    {
      for (int i = 0; i < nSites; ++i) //clear
        if (m_rows[row_num].sites[site_num + i] == cell)
          m_rows[row_num].sites[site_num + i] = m_hd.Cells.Null();
      for (int i = 0; i < nSites; ++i) //fill
        m_rows[row].sites[column + i] = cell;
    }
    cellRow[::ToID(cell)] = row;
    cellSiteIdx[::ToID(cell)] = column;
    PutCellFast(cell, row, column);
  }

  void PutCellIntoRow(HCell cell, int row)
  {
    PutCell(cell, row, CellColumn(cell));
  }

  void PutCellIntoSite(HCell cell, int column)
  {
    PutCell(cell, CellRow(cell), column);
  }

  void PutCellFast(HCell cell, int row, int column)
  {
    ASSERT(row >= 0 && column >= 0 && row < nRows && column + CellSitesNum(cell) <= nColumns);
    m_hd.Set<HCell::Y>(cell, m_rows[row].y);
    m_hd.Set<HCell::X>(cell, m_columns[column].x);
  }

  void PutCellIntoRowFast(HCell cell, int row)
  {
    ASSERT(row >= 0 && row < nRows);
    m_hd.Set<HCell::Y>(cell, m_rows[row].y);
  }

  void PutCellIntoSiteFast(HCell cell, int column)
  {
    ASSERT(column >= 0 && column + CellSitesNum(cell) <= nColumns);
    m_hd.Set<HCell::X>(cell, m_columns[column].x);
  }

};

#endif //__HIGH_DPGRID_H__