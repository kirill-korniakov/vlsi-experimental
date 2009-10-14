#include "DetailedPlacement.h"
#include "DetailedPlacementSwappers.h"
#include "Utils.h"
#include "Auxiliary.h"

HorizontalSwapper::HorizontalSwapper(HDPGrid& g, unsigned int quant_size)
  : m_Grid(g)
{
  m_GroupSize = quant_size;
  nextColumnIdx = -1;
  m_ConsideredGroupOfCells = new HCell[quant_size];
  Permutation = new unsigned int[quant_size];
  BestPositions = new int[quant_size];
  CurrentPositions = new int[quant_size];
  for (unsigned int i = 0; i < quant_size; ++i)
    Permutation[i] = i;
  nTotalPermutations = Aux::Factorial(quant_size);
}
HorizontalSwapper::~HorizontalSwapper()
{
  delete[] m_ConsideredGroupOfCells;
  delete[] Permutation;
  delete[] BestPositions;
  delete[] CurrentPositions;
}

void HorizontalSwapper::MakeNextPermutation() { Aux::NextPermutation(Permutation, m_GroupSize); }

void HorizontalSwapper::SaveCellsPositionAsBest()
{
  for (unsigned int i = 0; i < m_GroupSize; ++i)
    BestPositions[i] = m_Grid.CellColumn(m_ConsideredGroupOfCells[i]);
}

void HorizontalSwapper::SaveCurrentPositionAsBest()
{
  ::Swap(&BestPositions, &CurrentPositions);
}

bool HorizontalSwapper::GetNextGroup()
{
  if (nextColumnIdx == -1)
  {//reset
    if (Direction == HorizontalSearchDirection_LEFTTORIGHT)
      nextColumnIdx = 0;
    else
      nextColumnIdx = m_Grid.NumCols() - 1;
  }
  else
  {
    //skip empty space
    while (::IsNull(m_Grid(RowIdx, nextColumnIdx))) 
      if (!m_Grid.IsColumnIdxInRow(GoToNextHorizontalSite(nextColumnIdx)))
        return false;

    //skip first cell - it was used on previous iteration
    HCell first_cell = m_Grid(RowIdx, nextColumnIdx);
    while (m_Grid(RowIdx, nextColumnIdx) == first_cell) //skip cell
      if (!m_Grid.IsColumnIdxInRow(GoToNextHorizontalSite(nextColumnIdx)))
        return false;
    // do not change columnIdx after this moment
    // this value will be used on next iteration
  }

  if (!m_Grid.IsColumnIdxInRow(nextColumnIdx))
    return false;

  //collect group of cells for permutations
  int siteIdx = nextColumnIdx;
  HCell lastCellInGroup = m_Grid.Design().Cells.Null();
  unsigned int nCellsFound = 0;
  nEmptySites = 0;
  int cellsWidthInSites = 0;
  while (nCellsFound < m_GroupSize)
  {
    HCell currentCell = m_Grid(RowIdx, siteIdx);
    if(::IsNull(currentCell))
      nEmptySites++;
    else
    {
      if (currentCell != lastCellInGroup)
      {//add to the group
        lastCellInGroup = currentCell;
        m_ConsideredGroupOfCells[nCellsFound++] = currentCell;
        cellsWidthInSites += m_Grid.CellSitesNum(currentCell);
      }
    }
    if (!m_Grid.IsColumnIdxInRow(GoToNextHorizontalSite(siteIdx)))
      return false;
  }

  //go through sites to the border of cell
  while (m_Grid.IsColumnIdxInRow(siteIdx) && m_Grid(RowIdx, siteIdx) == lastCellInGroup)
    GoToNextHorizontalSite(siteIdx);

  //calc empty space
  while (m_Grid.IsColumnIdxInRow(siteIdx) && ::IsNull(m_Grid(RowIdx, siteIdx)))
  {
    nEmptySites++;
    GoToNextHorizontalSite(siteIdx);
  }

  nSites = abs(siteIdx - nextColumnIdx);
  ASSERT(nSites == nEmptySites + cellsWidthInSites);
  leftSiteIdx = Direction == HorizontalSearchDirection_LEFTTORIGHT
    ? nextColumnIdx
    : siteIdx + 1;

  return true;
}

void HorizontalSwapper::ImplementEquidistantPermutation()
{
  double free_sites_per_cell = ((double)nEmptySites) / (m_GroupSize - 1);
  int free_sites_added = 0;
  double free_sites_to_add = 0.0;
  int current_pos = leftSiteIdx;

  for (unsigned int i = 0; i < m_GroupSize; ++i)
  {
    //leave free space
    current_pos += (int)free_sites_to_add - free_sites_added;
    free_sites_added = (int)free_sites_to_add;
    free_sites_to_add += free_sites_per_cell;
    //place cell
    m_Grid.PutCellIntoSiteFast(m_ConsideredGroupOfCells[Permutation[i]], CurrentPositions[Permutation[i]] = current_pos);
    current_pos += m_Grid.CellSitesNum(m_ConsideredGroupOfCells[Permutation[i]]);
  }
  ASSERT(current_pos == leftSiteIdx + nSites);
}

void HorizontalSwapper::ImplementTightPermutation(int firstSiteIdx)
{
  int current_pos = firstSiteIdx;
  for (unsigned int i = 0; i < m_GroupSize; ++i)
  {
    m_Grid.PutCellIntoSiteFast(m_ConsideredGroupOfCells[Permutation[i]], CurrentPositions[Permutation[i]] = current_pos);
    current_pos += m_Grid.CellSitesNum(m_ConsideredGroupOfCells[Permutation[i]]);
  }
}

void HorizontalSwapper::PlaceCellsOnTheLeft()
{
  ImplementTightPermutation(leftSiteIdx);
}

void HorizontalSwapper::PlaceCellsOnTheRight()
{
  int totalCellsWidth = 0;
  for (unsigned int i = 0; i < m_GroupSize; ++i)
  {
    totalCellsWidth += m_Grid.CellSitesNum(m_ConsideredGroupOfCells[i]);
  }
  int firstSite4Placement = leftSiteIdx + nSites - totalCellsWidth;
  ImplementTightPermutation(firstSite4Placement);
}

int HorizontalSwapper::GoToNextHorizontalSite(int& columnIdx)
{
  if (Direction == HorizontalSearchDirection_LEFTTORIGHT)
    return ++columnIdx;
  else
    return --columnIdx;
}

void HorizontalSwapper::ProcessRow(int rowIdx)
{
  RowIdx = rowIdx;
  nextColumnIdx = -1;

  if (Direction == HorizontalSearchDirection_RANDOM)
    Direction = ((double)rand()) / RAND_MAX >= 0.5
    ? HorizontalSearchDirection_LEFTTORIGHT
    : HorizontalSearchDirection_RIGHTTOLEFT;

  while (GetNextGroup())
  {
    SaveCellsPositionAsBest();
    double best_diff = 0.0;

    //exhaustive search of best permutation for group
    for (unsigned int i = 0; i < nTotalPermutations; ++i) 
    {
      //TODO: fix problem with terminals      
      PlaceCellsOnTheLeft();
      double diff2 = Utils::CalculateHPWLDiff(m_Grid.Design(), m_ConsideredGroupOfCells, m_GroupSize, false);
      if (diff2 < best_diff)
      {
          best_diff = diff2;
          SaveCurrentPositionAsBest();
      }

      if (nEmptySites > 0)
      {
        ImplementEquidistantPermutation();
        double diff1 = Utils::CalculateHPWLDiff(m_Grid.Design(), m_ConsideredGroupOfCells, m_GroupSize, false);
        if (diff1 < best_diff)
        {
            best_diff = diff1;
            SaveCurrentPositionAsBest();
        }

        PlaceCellsOnTheRight();
        double diff3 = Utils::CalculateHPWLDiff(m_Grid.Design(), m_ConsideredGroupOfCells, m_GroupSize, false);
        if (diff3 < best_diff)
        {
            best_diff = diff3;
            SaveCurrentPositionAsBest();
        }
      }
      MakeNextPermutation();
    }

    //put cells to the best position found (probably initial position)
    for (unsigned int j = 0; j < m_GroupSize; ++j)
      m_Grid.PutCellIntoSite(m_ConsideredGroupOfCells[j], BestPositions[j]);

    if (best_diff < 0.0) //if better permutation was found, update wire length
    {
      Utils::CalculateHPWLDiff(m_Grid.Design(), m_ConsideredGroupOfCells, m_GroupSize, true);
    }
  }
}

void HorizontalSwapper::DetermineDirection()
{
  static int iterationNumber = 0;
  if (iterationNumber % 2 == 0)
  {
    Direction = HorizontalSearchDirection_LEFTTORIGHT;
  }
  else
  {
    Direction = HorizontalSearchDirection_RIGHTTOLEFT;
  }
  iterationNumber++;
}

void HorizontalSearch(HDPGrid& grid)
{
  ConfigContext ctx = grid.Design().cfg.OpenContext("HorizontalSearch");

  HorizontalSwapper hswapper(grid, grid.Design().cfg.ValueOf(".horizontalGroupSize", 4));

  hswapper.DetermineDirection();

  for (int i = 0; i < grid.NumRows(); ++i)
  {
    hswapper.ProcessRow(i);
    grid.Design().Plotter.AutoShowPlacement();
    //ALERTFORMAT(("Row %d replaced.", i));
  }
}