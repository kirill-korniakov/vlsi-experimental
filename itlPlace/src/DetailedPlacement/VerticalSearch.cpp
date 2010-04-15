#include "DetailedPlacement.h"
#include "DetailedPlacementSwappers.h"
#include "Utils.h"
#include "Auxiliary.h"
#include <stdlib.h>
#include <conio.h>

VerticalSwapper::VerticalSwapper(HDPGrid& g, int searchZoneSize, int nRowsToSearch,
  ConstraintsController* _checker): m_Grid(g), m_searchZoneSize(searchZoneSize),
  m_nRowsToSearch(nRowsToSearch), checker(_checker)
{
  int maxCellWidthInSites = 0;
  int currWidth;
  HCells::PlaceableCellsEnumeratorW pCell = g.Design().Cells.GetPlaceableCellsEnumeratorW();
  for (; pCell.MoveNext(); )
  {
    currWidth = (int)g.CellSitesNum(pCell);

    if (maxCellWidthInSites < currWidth)
      maxCellWidthInSites = currWidth;
  }

  m_ConsideredGroupOfCells = new HCell[1 + maxCellWidthInSites * m_searchZoneSize];
  BestPositions = new int[1 + maxCellWidthInSites * m_searchZoneSize];
  CurrentPositions = new int[1 + maxCellWidthInSites * m_searchZoneSize];
}

VerticalSwapper::~VerticalSwapper()
{
  delete[] m_ConsideredGroupOfCells;
  delete[] BestPositions;
  delete[] CurrentPositions;
}

void VerticalSwapper::NullConsideredGroupOfCells()
{
  int i = 0;
  for (; i < 1 + SearchZoneWidthInSites(); i++)
  {
    m_ConsideredGroupOfCells[i] = m_Grid.Design().Cells.Null();
  }
  m_GroupSize = 0;
}

bool VerticalSwapper::IsAlreadyInOptimalRow()
{
  return m_Grid.CellRow(m_Cell) == m_OptimalRowIdx;
}

void VerticalSwapper::RecheckBestResult(int siteIdx, int currFreeSpace, int &maxFreeSpace, int &maxFreeSpaceOrigin)
{
  if (currFreeSpace > maxFreeSpace)
  {
    maxFreeSpace = currFreeSpace;
    maxFreeSpaceOrigin = siteIdx - currFreeSpace;
  }
}

int VerticalSwapper::SearchZoneWidthInSites()
{
  return m_searchZoneSize * m_CellWidthInSites;
}

bool VerticalSwapper::DoesColBelongToSearchZone(int colIdx)
{
  //int halfWidth = (m_searchZoneSize - 1) / 2 * m_CellWidthInSites;

  return colIdx >= m_SearchZoneLeftSiteIdx && colIdx < m_SearchZoneLeftSiteIdx + SearchZoneWidthInSites()
    && colIdx >= 0 && colIdx < m_Grid.NumCols();
}

void VerticalSwapper::AnalyzeFreeSpace(int rowIdx, int& rowFreeSpace, int& maxFreeSpace, int& maxFreeSpaceOrigin)
{
  int currFreeSpace = 0;
  rowFreeSpace = 0;
  maxFreeSpace = 0;
  maxFreeSpaceOrigin = -1;

  //2.1.1 calculate free space in optimal region
  int siteIdx = m_SearchZoneLeftSiteIdx;
  for (; DoesColBelongToSearchZone(siteIdx); siteIdx++)
  {
    if (::IsNull(m_Grid(rowIdx, siteIdx)))
    {
      rowFreeSpace++;
      currFreeSpace++;
    }
    else
    {
      RecheckBestResult(siteIdx, currFreeSpace, maxFreeSpace, maxFreeSpaceOrigin);
      currFreeSpace = 0;
    }
  }
  // the following call is needed if the last site in current row is free
  RecheckBestResult(siteIdx, currFreeSpace, maxFreeSpace, maxFreeSpaceOrigin);
}

bool VerticalSwapper::TryToMoveWithoutShifting(int rowIdx, int siteIdx)
{
  //place the cell into optimal region and look at the benefit
  m_Grid.PutCellFast(m_Cell, rowIdx, siteIdx);
  NullConsideredGroupOfCells();
  m_ConsideredGroupOfCells[0] = m_Cell;
  m_GroupSize = 1;

  if (checker->CheckCriteria(m_ConsideredGroupOfCells, m_GroupSize))
  {//accept move
    m_Grid.PutCell(m_Cell, rowIdx, siteIdx);
    Utils::CalculateHPWLDiff(m_Grid.Design(), m_ConsideredGroupOfCells, m_GroupSize, true);
    return true;
  }
  else
  {//rollback
    m_Grid.PutCellFast(m_Cell, m_Grid.CellRow(m_Cell), m_Grid.CellColumn(m_Cell));
    return false;
  }
}

void VerticalSwapper::ReleaseSpaceForCell(int rowIdx, int freeSiteIdx, unsigned int nCandidatesForShifting)
{
  HCell currentCell = m_Grid.Design().Cells.Null();
  HCell nextCell = m_Grid.Design().Cells.Null();

  //WARNING: do not place increment of m_GroupSize into FOR statement
  //there is tricky dependence of lines
  for (m_GroupSize = 0; m_GroupSize < nCandidatesForShifting; )
  {
    //shift one more cell
    //NOTE: first cell is our current cell, we simply place it
    currentCell = m_ConsideredGroupOfCells[m_GroupSize];
    m_Grid.PutCellFast(currentCell, rowIdx, freeSiteIdx);
    BestPositions[m_GroupSize] = freeSiteIdx;
    m_GroupSize++;

    //check if there's enough space
    int cellWidth = (int)m_Grid.CellSitesNum(currentCell);
    freeSiteIdx += cellWidth;
    int forbiddenSiteIdx;
    nextCell = m_ConsideredGroupOfCells[m_GroupSize];
    if (!::IsNull(nextCell))
    {
      forbiddenSiteIdx = m_Grid.CellColumn(nextCell);
      if (freeSiteIdx <= forbiddenSiteIdx)
        break; //escape because space is released
    }
    else
    {
      forbiddenSiteIdx = m_SearchZoneLeftSiteIdx + SearchZoneWidthInSites(); 
      ASSERT(freeSiteIdx <= forbiddenSiteIdx);
      break;
    }
  }
}

bool VerticalSwapper::TryToMoveWithShifting(int rowIdx)
{
  //find the first free site from the left,
  //we will place cell there and shift needed number of following cells
  int firstFreeSiteIdx = m_SearchZoneLeftSiteIdx;
  while (!::IsNull(m_Grid(rowIdx, firstFreeSiteIdx)))
    ++firstFreeSiteIdx;

  //calculate length of found free region and find first occupied site
  int currFreeSpace = 0;
  int firstOccupiedSiteIdx = firstFreeSiteIdx;
  for (; DoesColBelongToSearchZone(firstOccupiedSiteIdx); ++firstOccupiedSiteIdx)
  {
    if (::IsNull(m_Grid(rowIdx, firstOccupiedSiteIdx)))
    {
      currFreeSpace++;
    }
    else
      break;
  }

  //fill m_ConsideredGroupOfCells
  NullConsideredGroupOfCells();
  unsigned int nCandidatesForShifting = 0;
  //put current cell
  HCell currentCell = m_Cell;
  m_ConsideredGroupOfCells[nCandidatesForShifting] = currentCell;
  CurrentPositions[nCandidatesForShifting] = m_Grid.CellColumn(currentCell); 
  //TODO: we actually don't need coordinates above (CurrentPositions), because we do PutCellFast,
  //so we can get information from grid. Think about this moment.
  nCandidatesForShifting++;
  //put cells from optimal region
  HCell lastCellInGroup = m_Grid.Design().Cells.Null();
  int i = firstOccupiedSiteIdx;
  for (; DoesColBelongToSearchZone(i); ++i)
  {
    HCell currentCell = m_Grid(rowIdx, i);
    if (!::IsNull(currentCell))
    {
      if (currentCell != lastCellInGroup)
      {//add new cell to the group
        lastCellInGroup = currentCell;
        m_ConsideredGroupOfCells[nCandidatesForShifting] = currentCell;
        CurrentPositions[nCandidatesForShifting] = m_Grid.CellColumn(currentCell);
        nCandidatesForShifting++;
      }
    }
  }

  ReleaseSpaceForCell(rowIdx, firstFreeSiteIdx, nCandidatesForShifting);

  if (checker->CheckCriteria(m_ConsideredGroupOfCells, m_GroupSize))
  {//put cells to the better position found (probably initial position)
    for (unsigned int j = 0; j < m_GroupSize; ++j)
      m_Grid.PutCell(m_ConsideredGroupOfCells[j], rowIdx, BestPositions[j]);
    Utils::CalculateHPWLDiff(m_Grid.Design(), m_ConsideredGroupOfCells, m_GroupSize, true);
    return true;
  }
  else
  {//rollback
    for (unsigned int j = 0; j < m_GroupSize; ++j)
      m_Grid.PutCellFast(m_ConsideredGroupOfCells[j], m_Grid.CellRow(m_ConsideredGroupOfCells[j]), CurrentPositions[j]);
    return false;
  }
  return false;
}

bool VerticalSwapper::TryToMoveCellTowardsOptimalRegion()
{
  string cellName = m_Grid.Design().GetString<HCell::Name>(m_Cell).c_str();
  int rowIdx;
  int destination = 0;

  if (m_OptimalRowIdx < m_CellRowIdx)
  {
    destination = -1;
  } 
  else
  {
    destination = 1;
  }

  //try to place into every row starting from optimal row
  int rowLevelCounter = 1;
  for (; rowLevelCounter <= m_nRowsToSearch; ++rowLevelCounter)
  {
    rowIdx = m_CellRowIdx + destination * rowLevelCounter;
    if (rowIdx < 0 || rowIdx == m_Grid.NumRows()) break;
    //2.1 try to find free space for the cell
    int availableFreeSpace = 0; //entire free space in current row within the search zone
    int maxFreeSpace       = 0; //length of maximum continual free space region
    int maxFreeSpaceOrigin = 0; //index of site from which maximum continual free space region starts
    AnalyzeFreeSpace(rowIdx, availableFreeSpace, maxFreeSpace, maxFreeSpaceOrigin);

    //2.2.2 if there is enough space, place the cell there and BREAK
    if (availableFreeSpace >= m_CellWidthInSites)
    {
      if (maxFreeSpace >= m_CellWidthInSites) 
      {//if there is enough space in a continual free space region
        if (TryToMoveWithoutShifting(rowIdx, maxFreeSpaceOrigin))
        {
          //m_Grid.Design().Plotter.PlotCell(m_Cell);
          //ALERT("- %s\t", cellName.c_str()));
          return true;
        }
      }
      else
      {//if there is still enough space but shifting of other cells is needed
        if (TryToMoveWithShifting(rowIdx))
        {
          //m_Grid.Design().Plotter.PlotCell(m_Cell);
          //ALERT("+ %s\t", cellName.c_str()));
          return true;
        }
      }
    }
  }
  return false;
}

bool VerticalSwapper::TryToExchangeWithCell()
{
  int nAvailableSitesNearTrialCell;
  int nAvailableSitesNearCurrentCell;

  NullConsideredGroupOfCells();
  m_GroupSize = 0;
  m_ConsideredGroupOfCells[m_GroupSize] = m_Cell;
  m_GroupSize = 1;

  int currCellColIdx = m_Grid.CellColumn(m_Cell);

  //calculate free space from the left side of current cell
  int leftFreeSpace = 0;
  int j = currCellColIdx - 1;
  //CHECK: is it better to use DoesColBelongToOptimalRegion() function
  while (j >= 0 && ::IsNull(m_Grid(m_Grid.CellRow(m_Cell), j)))
  { 
    leftFreeSpace++; 
    j--; 
  }

  //calculate free space from the right side of current cell
  int rightFreeSpace = 0;
  j = currCellColIdx + m_CellWidthInSites;
  while (j < m_Grid.NumCols() && ::IsNull(m_Grid(m_Grid.CellRow(m_Cell), j))) 
  { 
    rightFreeSpace++; 
    j++; 
  }

  //calculate currNumOfAvailableSites - available space after current cell removing
  nAvailableSitesNearCurrentCell = leftFreeSpace + m_CellWidthInSites + rightFreeSpace;

  int rowIdx;
  int destination = 0;

  if (m_OptimalRowIdx < m_CellRowIdx)
  {
    destination = -1;
  } 
  else
  {
    destination = 1;
  }

  //try to exchange with every cell from search zone,
  //escape after first successive try
  int rowLevelCounter = 1;
  for (; rowLevelCounter <= m_nRowsToSearch; ++rowLevelCounter)
  {
    rowIdx = m_CellRowIdx + destination * rowLevelCounter;
    if (rowIdx < 0 || rowIdx == m_Grid.NumRows()) break;

    HCell lastCell = m_Grid.Design().Cells.Null();
    int siteIdx = m_SearchZoneLeftSiteIdx;
    for (; DoesColBelongToSearchZone(siteIdx); siteIdx++)
    {
      int trialCellWidthInSites = 0;
      int leftTrialFreeSpace  = 0;
      int rightTrialFreeSpace = 0;

      HCell trialCell = m_Grid(rowIdx, siteIdx);
      //try to exchange with this cell
      if (!IsNull(trialCell) && trialCell != lastCell)
      {
        lastCell = trialCell;

        m_GroupSize = 1;
        m_ConsideredGroupOfCells[m_GroupSize] = trialCell;
        CurrentPositions[m_GroupSize] = m_Grid.CellColumn(trialCell);
        m_GroupSize = 2;

        trialCellWidthInSites = (int)m_Grid.CellSitesNum(trialCell);
        //siteIdx += trialCellWidthInSites-1; //increase siteIdx
        int trialCellColIdx = m_Grid.CellColumn(trialCell);

        int k = trialCellColIdx - 1;
        while (k >= m_SearchZoneLeftSiteIdx && ::IsNull(m_Grid(rowIdx, k))) 
        { 
          leftTrialFreeSpace++; 
          k--;
        }

        k = trialCellColIdx + trialCellWidthInSites;
        while (DoesColBelongToSearchZone(k) && ::IsNull(m_Grid(rowIdx, k))) 
        { 
          rightTrialFreeSpace++; 
          k++; 
        }

        nAvailableSitesNearTrialCell = leftTrialFreeSpace + trialCellWidthInSites + rightTrialFreeSpace;

        //if enough space available, we perform exchange (we don't make shift)
        int neededSpace4Trial = 0;
        int neededSpace4Current = 0;
        int site4Current = 0;
        int site4Trial = 0;
        if (nAvailableSitesNearTrialCell >= m_CellWidthInSites && nAvailableSitesNearCurrentCell >= trialCellWidthInSites)
        {
          site4Current = trialCellColIdx;
          neededSpace4Current = m_CellWidthInSites - (trialCellWidthInSites + rightTrialFreeSpace);
          if (neededSpace4Current > 0)
          {
            site4Current -= neededSpace4Current;
            site4Current = max(0, site4Current);
          }
          site4Trial = currCellColIdx;
          neededSpace4Trial = trialCellWidthInSites - (m_CellWidthInSites + rightFreeSpace);
          if (neededSpace4Trial > 0)
          {
            site4Trial -= neededSpace4Trial;
            site4Trial = max(0, site4Trial);
          }
          //exchange cells
          m_Grid.PutCellFast(m_Cell, m_Grid.CellRow(trialCell), site4Current);
          m_Grid.PutCellFast(trialCell, m_Grid.CellRow(m_Cell), site4Trial);

          if (checker->CheckCriteria(m_ConsideredGroupOfCells, m_GroupSize))
          {
            //put cells to the better position found
            m_Grid.PutCell(trialCell, m_Grid.CellRow(m_Cell), site4Trial);
            m_Grid.PutCell(m_Cell, rowIdx, site4Current); //WARINING: do not use m_Grid.CellRow(trialCell) here, because it was rewritten
            Utils::CalculateHPWLDiff(m_Grid.Design(), m_ConsideredGroupOfCells, m_GroupSize, true);
            return true;
          }
          else
          {//rollback
            m_Grid.PutCellFast(m_Cell, m_Grid.CellRow(m_Cell), m_Grid.CellColumn(m_Cell));
            m_Grid.PutCellFast(trialCell, m_Grid.CellRow(trialCell), m_Grid.CellColumn(trialCell));
          }
        }
      }
    }
  }
  return false;
}

void VerticalSwapper::FindOptimalRow()
{
  //find center of optimal region
  m_OptimalRowIdx = GetOptimalRowIdx(m_Grid, m_Cell);
  /*int tmp;
  GetOptimalPosition(m_Grid, m_Cell, m_OptimalRowIdx, tmp);*/

  /*if (m_Grid.Design().cfg.ValueOf("DetailedPlacement.tmp_drawOptimalSites", false))
  {//TODO: delete or accept this plotting
    m_Grid.Design().Plotter.DrawCircle(m_Grid.Design().Circuit.PlacementMinX() + m_OriginColIdx*m_Grid.SiteWidth(), 
      m_Grid.Design().Circuit.PlacementMinY() + m_OptimalRowIdx*m_Grid.SiteHeight(), 4, Color_Green);
  }*/

  //shift from center to the lower left corner
  //ShiftCoordinateToLowerBorder(m_OptimalRowIdx, m_OptimalRegionHalfHeight, 0, m_Grid.NumRows());
  //ShiftCoordinateToLowerBorder(m_OriginColIdx, m_OptimalRegionHalfWidth, 0, m_Grid.NumCols());
}

void VerticalSwapper::ProcessCell(HCell cell)
{
  m_Cell = cell;
  m_CellWidthInSites       = (int)m_Grid.CellSitesNum(m_Cell);
  m_CellRowIdx             = (int)m_Grid.CellRow(m_Cell);
  m_CellLeftSiteIdx        = (int)m_Grid.CellColumn(m_Cell);
  m_SearchZoneLeftSiteIdx  = m_CellLeftSiteIdx - (m_searchZoneSize - 1) / 2 * m_CellWidthInSites;

  string cellName = m_Grid.Design().GetString<HCell::Name>(m_Cell).c_str();

  //if (m_CellWidthInSites > SearchZoneWidthInSites())
  //  return; //we can't place cell at optimal region in any case

  //find the optimal region
  FindOptimalRow();

  //skip cell if it already belongs to its optimal region
  if (IsAlreadyInOptimalRow())
  {
    //ALERT("! %s\t", cellName.c_str()));
    return;
  }

  //try to move cell to its optimal region
  if (TryToMoveCellTowardsOptimalRegion())
    return;

  //try to exchange it with a cell from the optimal region
  if (TryToExchangeWithCell())
  {
    //ALERT("* %s\t", cellName.c_str()));
    return;
  }

  //ALERT("# %s\t", cellName.c_str()));

  return;
}

void VerticalSearch(HDPGrid& grid, ConstraintsController* checker)
{
  ConfigContext ctx = grid.Design().cfg.OpenContext("VerticalSearch");

  VerticalSwapper hswapper(grid, 
    grid.Design().cfg.ValueOf(".searchZoneSize", 7),
    grid.Design().cfg.ValueOf(".nRowsToSearch", 6),
    checker);

  HCells::PlaceableCellsEnumeratorW pCell = grid.Design().Cells.GetPlaceableCellsEnumeratorW();
  for (; pCell.MoveNext();)
  {
    hswapper.ProcessCell(pCell);
    grid.Design().Plotter.AutoShowPlacement();
  }
}