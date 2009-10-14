#include "Legalization.h"
#include <math.h>

bool CheckGridConsistency(HDPGrid& grid, unsigned maxCellsToReport, double sensitivity, bool drawMismatchses)
{
  unsigned errors_count = 0;
  HDesign& hd = grid.Design();
  if (drawMismatchses) 
    hd.Plotter.SetAutoRefreshMinFrequency(200);

  for (HCells::PlaceableCellsEnumeratorW cell = hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
  {
    if (fabs(grid.ColumnX(grid.CellColumn(cell)) - cell.X()) > sensitivity
      || fabs(grid.RowY(grid.CellRow(cell)) - cell.Y()) > sensitivity)
    {
      if (drawMismatchses)
      {
        hd.Plotter.DrawLine(
          cell.X(),
          cell.Y(),
          grid.ColumnX(grid.CellColumn(cell)),
          grid.RowY(grid.CellRow(cell)),
          Color_Red);
      }
      
      if (errors_count++ < maxCellsToReport)
      {
        LOGERRORFORMAT(("Cell %s left its site.", cell.Name().c_str()));
      }
      else
        if (!drawMismatchses)
          return false;
    }
  }
  if (errors_count > 0)
  {
    ALERTFORMAT(("CheckGridConsistency: %d errors found", errors_count));
  }
  if (drawMismatchses) 
    hd.Plotter.ResetAutoRefreshFrequency();
  return errors_count == 0;
}

bool CheckGridBoundary(HDPGrid& grid, unsigned maxCellsToReport, bool drawMismatchses)
{
  unsigned errors_count = 0;
  HDesign& hd = grid.Design();
  if (drawMismatchses) 
    hd.Plotter.SetAutoRefreshMinFrequency(200);

  for (HCells::PlaceableCellsEnumeratorW cell = hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
  {
    if (cell.X() < grid.ColumnX(0)
      || cell.X() + grid.SiteWidth() * grid.CellSitesNum(cell) > grid.SiteWidth() + grid.ColumnX(grid.NumCols() - 1)
      || cell.Y() < grid.RowY(0)
      || cell.Y() > grid.RowY(grid.NumRows() - 1))
    {
      if (drawMismatchses)
      {
        hd.Plotter.DrawBar(
          cell.X(),
          cell.Y(),
          cell.X() + grid.SiteWidth() * grid.CellSitesNum(cell),
          cell.Y() + cell.Height(),
          Color_Pink);
      }

      if (errors_count++ < maxCellsToReport)
      {
        LOGERRORFORMAT(("Cell %s lays outside of placement grid.", cell.Name().c_str()));
      }
      else
        if (!drawMismatchses)
          return false;
    }
  }

  if (errors_count > 0)
  {
    ALERTFORMAT(("CheckGridBoundary: %d errors found", errors_count));
  }
  if (drawMismatchses) 
    hd.Plotter.ResetAutoRefreshFrequency();
  return errors_count == 0;
}

bool CheckOverlaps(HDPGrid& grid, unsigned maxCellsToReport, bool drawOverlaps)
{
  unsigned errors_count = 0;
  HDesign& hd = grid.Design();
  if (drawOverlaps) 
    hd.Plotter.SetAutoRefreshMinFrequency(5000);

  for (HCells::PlaceableCellsEnumeratorW cell = hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
  {
    HCell overlapped = hd.Cells.Null();
    int row = grid.CellRow(cell);
    int column = grid.CellColumn(cell);

    for (int i = 0; i < grid.CellSitesNum(cell); ++i)
    {
      if (grid(row, column + i) != cell 
        && !::IsNull(grid(row, column + i)))//empty is strange, but not critical
      {
        if (overlapped != grid(row, column + i))
        {
          overlapped = grid(row, column + i);
          if (errors_count++ < maxCellsToReport)
          {
            LOGERRORFORMAT(("Cell %s overlaps with %s", cell.Name().c_str(), hd.GetString<HCell::Name>(overlapped).c_str()));
          }
           else
            if (!drawOverlaps)
              return false;
        }
        if (drawOverlaps)
        {
          hd.Plotter.DrawBar(
            grid.ColumnX(column + i),
            grid.RowY(row),
            grid.ColumnX(column + i) + grid.SiteWidth(),
            grid.RowY(row) + grid.SiteHeight(),
            Color_Orange);
        }
      }
    }
  }

  if (errors_count > 0)
  {
    ALERTFORMAT(("CheckOverlaps: %d errors found", errors_count));
  }
  if (drawOverlaps) 
    hd.Plotter.ResetAutoRefreshFrequency();
  return errors_count == 0;
}