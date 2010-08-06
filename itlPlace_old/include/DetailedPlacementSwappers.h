#ifndef __DETAILED_PLACEMENT_SWAPPERS_H__
#define __DETAILED_PLACEMENT_SWAPPERS_H__

#include "ConstraintsController.h"

class GlobalSwapper
{
public:
  GlobalSwapper(HDPGrid& g, int optimalRegionHalfWidth, int optimalRegionHalfHeight,
                ConstraintsController* _checker);
  ~GlobalSwapper();
  void ProcessCell(HCell cell);

private:
  HDPGrid& m_Grid;
  ConstraintsController* checker;
  HCell m_Cell;
  unsigned int m_GroupSize;
  HCell* m_ConsideredGroupOfCells;
  int* BestPositions;     //indexes of sites
  int* CurrentPositions;  //indexes of sites
  int m_CellWidthInSites;
  int m_OriginRowIdx;     //origin of optimal region - it is lower left site of the region
  int m_OriginColIdx;
  const int m_OptimalRegionHalfWidth;
  const int m_OptimalRegionHalfHeight;

  bool TryToMoveWithShifting(int rowIdx);
  bool TryToMoveWithoutShifting(int rowIdx, int siteIdx);
  bool TryToMoveCellToOptimalRegion();
  bool TryToExchangeWithCell();

  bool IsAlreadyInOptimalRegion();
  int GetNextRowIdx(int& rowIdx);
  bool IsColWithinBorders(int colIdx);
  bool DoesRowBelongToOptimalRegion(int rowIdx);
  bool DoesColBelongToOptimalRegion(int colIdx);
  void AnalyzeFreeSpace(int rowIdx, int& rowFreeSpace, int& maxFreeSpace, int& maxFreeSpace_siteIdx);

  void RecheckBestResult(int siteIdx, int currFreeSpace, int &maxFreeSpace, int &maxFreeSpaceOrigin);
  void ReleaseSpaceForCell(int rowIdx, int firstFreeSiteIdx, unsigned int nCandidatesForShifting);
  void ShiftCoordinateToLowerBorder(int& coordinate, int distanceToBorder, int minCoordinate, int maxCoordinate);
  void FindOptimalRegion();
  void NullConsideredGroupOfCells();
  int OptimalRegionWidth();
  int OptimalRegionHeight();
  int OptimalRow();
  int OptimalColumn();
};

class HorizontalSwapper
{
public:
  HorizontalSwapper(HDPGrid& g, unsigned int quant_size, ConstraintsController* _checker);
  ~HorizontalSwapper();
  void ProcessRow(int rowIdx);
  void DetermineDirection();

private:
  enum HorizontalSearchDirection 
  {
    HorizontalSearchDirection_LEFTTORIGHT = 0,
    HorizontalSearchDirection_RIGHTTOLEFT = 1,
    HorizontalSearchDirection_RANDOM = 2
  } Direction;

  HDPGrid& m_Grid;
  ConstraintsController* checker;
  int RowIdx;
  unsigned int m_GroupSize;
  HCell* m_ConsideredGroupOfCells;
  int* BestPositions; //indexes of sites
  int* CurrentPositions; //indexes of sites
  unsigned int* Permutation;
  unsigned int nTotalPermutations;
  int nextColumnIdx;
  int nEmptySites;
  int nSites;
  int leftSiteIdx;

  bool GetNextGroup();
  void MakeNextPermutation();
  void ImplementEquidistantPermutation();
  void ImplementTightPermutation(int firstSiteIdx);
  void PlaceCellsOnTheLeft();
  void PlaceCellsOnTheRight();
  void SaveCellsPositionAsBest();
  void SaveCurrentPositionAsBest();
  int GoToNextHorizontalSite(int& columnIdx);
};

class VerticalSwapper
{
public:
  VerticalSwapper(HDPGrid& g, int searchZoneSize, int nRowsToSearch,
                  ConstraintsController* _checker);
  ~VerticalSwapper();
  void ProcessCell(HCell cell);

private:
  HDPGrid& m_Grid;
  ConstraintsController* checker;
  HCell m_Cell;
  unsigned int m_GroupSize;
  HCell* m_ConsideredGroupOfCells;
  int* BestPositions;     //indexes of sites
  int* CurrentPositions;  //indexes of sites
  int m_CellWidthInSites; 
  int m_CellRowIdx;
  int m_CellLeftSiteIdx;
  int m_SearchZoneLeftSiteIdx;
  int m_OptimalRowIdx;
  const int m_searchZoneSize;   // характеризует величину зоны поиска в очередном ряду (кол-во сайтов для поиска есть
  // произведение этой величины на размер селла в сайтах) 
  const int m_nRowsToSearch;

  bool TryToMoveWithShifting(int rowIdx);
  bool TryToMoveWithoutShifting(int rowIdx, int siteIdx);
  bool TryToMoveCellTowardsOptimalRegion();  // try to put cell into the new row without exchange
  bool TryToExchangeWithCell();

  bool IsAlreadyInOptimalRow();
  int  GetNextRowIdx(int& rowIdx);
  bool DoesColBelongToSearchZone(int colIdx);
  void AnalyzeFreeSpace(int rowIdx, int& rowFreeSpace, int& maxFreeSpace, int& maxFreeSpace_siteIdx);

  void RecheckBestResult(int siteIdx, int currFreeSpace, int &maxFreeSpace, int &maxFreeSpaceOrigin);
  void ReleaseSpaceForCell(int rowIdx, int firstFreeSiteIdx, unsigned int nCandidatesForShifting);
  void FindOptimalRow();
  int  SearchZoneWidthInSites();
  void NullConsideredGroupOfCells();
};

#endif