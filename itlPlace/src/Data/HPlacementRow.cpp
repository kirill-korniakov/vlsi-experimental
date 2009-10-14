#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HPlacementRows)

void HPlacementRows::Initialize(int rowsLimit)
{
  CHECKIFINITIALIZED();

  Grow(rowsLimit);
  m_ld->DetailedPlacement.rowName[0] = "NULL";
  m_ld->DetailedPlacement.rowSite[0] = 0;
  m_ld->DetailedPlacement.rowSiteOrient[0] = Orientation_Default;
  m_ld->DetailedPlacement.rowStepX[0] = 0;
  m_ld->DetailedPlacement.rowStepY[0] = 0;
  m_ld->DetailedPlacement.rowX[0] = 0;
  m_ld->DetailedPlacement.rowXSitesNum[0] = 0;
  m_ld->DetailedPlacement.rowY[0] = 0;
  m_ld->DetailedPlacement.rowYSitesNum[0] = 0;
  m_ld->DetailedPlacement.rowNextRow[0] = 0;
  m_ld->DetailedPlacement.rowPreviousRow[0] = 0;

  m_ld->DetailedPlacement.nRows = 1;
  OnInitialize(rowsLimit);
}

void HPlacementRows::Grow(int rowsLimit)
{
  int old_size = m_ld->DetailedPlacement.nRowsLimit;
  ::Grow(&m_ld->DetailedPlacement.rowName, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowSite, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowSiteOrient, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowStepX, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowStepY, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowX, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowXSitesNum, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowY, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowYSitesNum, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowNextRow, old_size, rowsLimit);
  ::Grow(&m_ld->DetailedPlacement.rowPreviousRow, old_size, rowsLimit);
  m_ld->DetailedPlacement.nRowsLimit = rowsLimit;

  OnGrow(old_size, rowsLimit);
}

HPlacementRow HPlacementRows::AllocateRow()
{
  while(m_ld->DetailedPlacement.nRows >= m_ld->DetailedPlacement.nRowsLimit)
    Grow(GetNewSize(m_hd->cfg, m_ld->DetailedPlacement.nRowsLimit, "data.placementRowsLimitGrow", 4, 0, 1.25));

  HPlacementRow res = ::__ConstructPtr<HPlacementRow>(m_ld->DetailedPlacement.nRows);
  m_ld->DetailedPlacement.nRows++;

  OnPlacementRowAllocation(res);

  return res;
}