#include "Legalization.h"
#include "Timing.h"
#include <algorithm>
#include <float.h>
#include <math.h>

struct AbacusCluster
{
  size_t FirstCellIdx;
  size_t LastCellIdx;
  
  double TotalWeight; //total weight of cells in this cluster
  int TotalWidth;  //total width of cells in this cluster (sites number)
  double Q;
  int X; //column number
};

struct AbacusRow
{
  typedef TemplateTypes<HCell>::vector AbacusRowCells;
  typedef TemplateTypes<AbacusCluster>::list AbacusRowClusters;

  AbacusRowCells CellsInRow;
  AbacusRowClusters ClustersInRow;
  int Width; //sites number
};

//---Weighted Abacus------
struct WAbacusCell
{
  HCell cell;
  double weightMultiplier;

  WAbacusCell(): weightMultiplier(1) {}
};

class AbacusCellsComparator
{
  HCells& m_cells;

public:
  AbacusCellsComparator(HCells& cells): m_cells(cells) {}

  bool operator() (WAbacusCell first, WAbacusCell second)
  {
    return m_cells.GetDouble<HCell::X>(first.cell) < m_cells.GetDouble<HCell::X>(second.cell);
  }
};

class WeightedAbacus
{
  HDesign& m_design;
  HDPGrid& m_grid;

  AbacusRow* m_rows;

  void _InitAbacusRow(int idx, int maxCellsAwaited)
  {
    m_rows[idx].Width = 0;
    m_rows[idx].CellsInRow.reserve(maxCellsAwaited);
  }
  
  AbacusCluster _MakeNewCluster(int rowIdx, WAbacusCell& wCell)
  {
    AbacusCluster res;
    res.LastCellIdx = res.FirstCellIdx = m_rows[rowIdx].CellsInRow.size();
    res.TotalWidth = m_grid.CellSitesNum(wCell.cell);
    res.TotalWeight = m_grid.SiteWidth() * m_grid.SiteWidth() * res.TotalWidth * res.TotalWidth
                                                                               * wCell.weightMultiplier;
    if (wCell.weightMultiplier > 1)
      int qq = 1;//ALERT("Weight: %f", wCell.weightMultiplier));
    double cellX = m_design.GetDouble<HCell::X>(wCell.cell);
    res.Q = cellX * res.TotalWeight;
    res.X = m_grid.FindColumn(cellX);
    if (res.X + res.TotalWidth > m_grid.NumCols() - 1)
      res.X = m_grid.NumCols() - res.TotalWidth;
    return res;
  }

  AbacusCluster _MergeClusters(const AbacusCluster& first, const AbacusCluster& second)
  {
    AbacusCluster res;
    res.FirstCellIdx = first.FirstCellIdx;
    res.LastCellIdx = second.LastCellIdx;
    res.TotalWeight = first.TotalWeight + second.TotalWeight;
    res.TotalWidth = first.TotalWidth + second.TotalWidth;
    res.Q = first.Q + second.Q - (first.TotalWidth * m_grid.SiteWidth()) * second.TotalWeight;
    res.X = m_grid.FindColumn(res.Q / res.TotalWeight);
    if (res.X + res.TotalWidth > m_grid.NumCols() - 1)
      res.X = m_grid.NumCols() - res.TotalWidth;
    return res;
  }

public:
  void PlaceCellIntoRow(int rowIdx, WAbacusCell& wCell)
  {
    //DEPENDENCY: cluster should be created before adding cell to row
    AbacusCluster lastCluster = _MakeNewCluster(rowIdx, wCell);
    m_rows[rowIdx].CellsInRow.push_back(wCell.cell);
    m_rows[rowIdx].Width += lastCluster.TotalWidth;
    while (!m_rows[rowIdx].ClustersInRow.empty()
      && lastCluster.X < m_rows[rowIdx].ClustersInRow.back().X + m_rows[rowIdx].ClustersInRow.back().TotalWidth)
    {
      lastCluster = _MergeClusters(m_rows[rowIdx].ClustersInRow.back(), lastCluster);
      m_rows[rowIdx].ClustersInRow.pop_back();
    }
    m_rows[rowIdx].ClustersInRow.push_back(lastCluster);
  }

  bool TryPlaceCellIntoRow(int rowIdx, WAbacusCell& wCell, double& newX)
  {
    if (m_rows[rowIdx].Width + m_grid.CellSitesNum(wCell.cell) > m_grid.NumCols())
      return false;
    AbacusCluster last_cluster = _MakeNewCluster(rowIdx, wCell);
    int cell_width = last_cluster.TotalWidth;

    if (!m_rows[rowIdx].ClustersInRow.empty())
      for (AbacusRow::AbacusRowClusters::reverse_iterator cl = m_rows[rowIdx].ClustersInRow.rbegin();
        cl != m_rows[rowIdx].ClustersInRow.rend()
        && last_cluster.X < cl->X + cl->TotalWidth; ++cl)
      {
        last_cluster = _MergeClusters(*cl, last_cluster);
      }

    newX = m_grid.ColumnX(last_cluster.X + last_cluster.TotalWidth - cell_width);
    return true;
  }

  bool ConsiderRow(WAbacusCell& wCell, int rowIdx, int& bestRow, double& bestDistance)
  {
    if (rowIdx >= 0 && rowIdx < m_grid.NumRows())
    {
      double cellX = m_design.GetDouble<HCell::X>(wCell.cell);
      double cellY = m_design.GetDouble<HCell::Y>(wCell.cell);
      if (Distance(cellX, cellY, cellX, m_grid.RowY(rowIdx)) < bestDistance)
      {
        double newX = DBL_MAX;
        if (TryPlaceCellIntoRow(rowIdx, wCell, newX))
        {
          double dist = Distance(cellX, cellY, newX, m_grid.RowY(rowIdx));
          if (dist < bestDistance)
          {
            bestRow = rowIdx;
            bestDistance = dist;
          }
        }
        return true;
      }
      return false;//placeing cell to this row makes no sense
    }
    return false;//row outside of design
  }

  void UpdateCellsCoordinates()
  {
    for (int i = 0; i < m_grid.NumRows(); ++i)
    {
      for (AbacusRow::AbacusRowClusters::iterator cluster = m_rows[i].ClustersInRow.begin();
        cluster != m_rows[i].ClustersInRow.end(); ++cluster)
      {
        int column = cluster->X;
        for (size_t j = cluster->FirstCellIdx; j <= cluster->LastCellIdx; ++j)
        {
          m_grid.PutCell(m_rows[i].CellsInRow[j], i, column);
          column += m_grid.CellSitesNum(m_rows[i].CellsInRow[j]);
        }
      }
    }
  }

  double Distance(double x1, double y1, double x2, double y2)
  {
    return fabs(x1 - x2) + fabs(y1 - y2);
  }

  WeightedAbacus(HDPGrid& grid): m_grid(grid), m_design(grid.Design())
  {
    m_rows = new AbacusRow[m_grid.NumRows()];

    int cellsPerRow = 3 * m_design.Cells.PlaceableCellsCount() / m_grid.NumRows() / 2;
    for (int i = 0; i < m_grid.NumRows(); ++i)
      _InitAbacusRow(i, cellsPerRow);
  }

  ~WeightedAbacus()
  {
    delete[] m_rows;
  }
};

double GetMultiplier(HCell cell, HDesign& hd)
{
  double multiplier = 1;
  
  for (HCell::PinsEnumeratorW pin = hd.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); pin.MoveNext();)
  {
    HNetWrapper netW = hd[pin.Net()];

    if (multiplier < netW.Weight())
      multiplier = netW.Weight();
  }

  return multiplier;
}

void InitialiseCellsWeights(WAbacusCell* wCells, int nWCells, HDesign& hd, const double multiplier)
{
  FindCriticalPaths(hd);

  for (HCriticalPaths::EnumeratorW cpIter = hd.CriticalPaths.GetEnumeratorW(); cpIter.MoveNext();)
  {
    for (HCriticalPath::PointsEnumeratorW pIter = cpIter.GetEnumeratorW(); pIter.MoveNext();)
    {
      HPinWrapper pin = hd[hd.Get<HTimingPoint::Pin, HPin>(pIter.TimingPoint())];

      if (hd.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT &&
        !::IsNull(pin.Cell()))
      {
        HCell cell = pin.Cell();

        for (int i = 0; i < nWCells; i++)
        {
          if (wCells[i].cell == cell)
          {
            if (wCells[i].weightMultiplier < 1000)
              wCells[i].weightMultiplier *= multiplier;
            //ALERT("Weight: %f", wCells[i].weightMultiplier));
            break;
          }
        }
      }
    }
  }
}

//void InitialiseCellsWeights(WAbacusCell* wCells, int nWCells, HDesign& hd, const double multiplier)
//{
//  FindCriticalPaths(hd);
//  for (HCriticalPaths::EnumeratorW cpIter = hd.CriticalPaths.GetEnumeratorW(); cpIter.MoveNext();)
//  {
//    for (HCriticalPath::PointsEnumeratorW pIter = cpIter.GetEnumeratorW(); pIter.MoveNext();)
//    {
//      HPinWrapper pin  = hd[hd.Get<HTimingPoint::Pin, HPin>(pIter.TimingPoint())];
//      HNetWrapper netW = hd[pin.Net()];
//      //HCell cell = pin.Cell();
//      for (HNetWrapper::PinsEnumeratorW currPin = netW.GetPinsEnumeratorW(); currPin.MoveNext(); )
//      {
//        HCell cell = currPin.Cell();
//
//        for (int i = 0; i < nWCells; i++)
//        {
//          if (wCells[i].cell == cell)
//          {
//            if (wCells[i].weightMultiplier < 1000)
//              wCells[i].weightMultiplier *= multiplier;
//            //ALERT("Weight: %f", wCells[i].weightMultiplier));
//            break;
//          }
//        }
//      }      
//      pIter.MoveNext();
//    }
//  }
//}

void AbacusLegalization(HDPGrid& grid)
{
  HDesign& design = grid.Design();
  ConfigContext ctx = design.cfg.OpenContext("Abacus");

  WAbacusCell* wCells = new WAbacusCell[design.Cells.PlaceableCellsCount()];

  unsigned maxDeviationInRows = design.cfg.ValueOf(".maxDeviationInRows", 3u);

  bool usePlotter = design.cfg.ValueOf(".usePlotter", false);
  bool drawSites = design.cfg.ValueOf(".drawSites", false);
  int plotterStep = design.cfg.ValueOf(".plotterStep", 500);
  HPlotter::WaitTime plotterSpeed = (HPlotter::WaitTime)design.cfg.ValueOf(".plotterSpeed", 1);

  //order cells by X coordinate
  int pos = 0;
  for (HCells::PlaceableCellsEnumeratorW pCell = design.Cells.GetPlaceableCellsEnumeratorW(); pCell.MoveNext(); )
  {
    wCells[pos].cell = pCell;
    //wCells[pos].weightMultiplier = GetMultiplier(pCell, design);
    pos++;
    grid.RecalcSitesNum(pCell);
  }
  ASSERT(pos == design.Cells.PlaceableCellsCount());
  double multiplier = design.cfg.ValueOf(".weightMultiplier", 1);
  //ALERT("weight multiplier: %f", multiplier);

  if (design.cfg.ValueOf(".useWeights", false))
    InitialiseCellsWeights(wCells, pos, design, multiplier);

  std::sort(wCells, wCells + pos, AbacusCellsComparator(design.Cells));
  WeightedAbacus abacus(grid);

  //major loop of legalization
  for (int i = 0; i < pos; i++)
  {
    double bestDistance = DBL_MAX;
    int nearestRow = grid.FindRow(design.GetDouble<HCell::Y>(wCells[i].cell));
    int bestRow = -1;

    //try to place cell into nearest row
    abacus.ConsiderRow(wCells[i], nearestRow, bestRow, bestDistance);

    //try to improve current result using adjacent rows
    for (unsigned d = 1; d <= maxDeviationInRows; d++)
    {
      // this variable MUST be defined inside cycle
      int attempts = 2; //NOTE: optimization, we not going too far if it does not make sense
      
      //look below nearest row
      if (!abacus.ConsiderRow(wCells[i], nearestRow - d, bestRow, bestDistance))
        attempts--;

      //look above nearest row
      if (!abacus.ConsiderRow(wCells[i], nearestRow + d, bestRow, bestDistance))
        attempts--;

      if (attempts == 0)
        break;
    }

    if (bestRow < 0)
    {
      LOGERROR("Abacus unable to legalize design");
      return;
    }
    else
      abacus.PlaceCellIntoRow(bestRow, wCells[i]);

    if (usePlotter)
      if ((i + 1) % plotterStep == 0)
      {
        abacus.UpdateCellsCoordinates();
        design.Plotter->ShowLegalizationState(plotterSpeed, drawSites);
      }
  }//for (int i = 0; i < nCells; i++)

  abacus.UpdateCellsCoordinates();

  if (usePlotter)
    design.Plotter->ShowLegalizationState(plotterSpeed, drawSites);
  
  delete[] wCells;
}