#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HPlacementRows)

//properties specializations
PROPERTYA(HPlacementRows, string, HPlacementRow::Name, m_ld->DetailedPlacement.rowName)
PROPERTYA(HPlacementRows, double, HPlacementRow::SiteWidth, m_ld->DetailedPlacement.rowStepX)
PROPERTYA(HPlacementRows, double, HPlacementRow::SiteHeight, m_ld->DetailedPlacement.rowStepY)
PROPERTYA(HPlacementRows, double, HPlacementRow::X, m_ld->DetailedPlacement.rowX)
PROPERTYA(HPlacementRows, double, HPlacementRow::Y, m_ld->DetailedPlacement.rowY)
PROPERTYA(HPlacementRows, int, HPlacementRow::HorizSitesCount, m_ld->DetailedPlacement.rowXSitesNum)
PROPERTYA(HPlacementRows, int, HPlacementRow::VertSitesCount, m_ld->DetailedPlacement.rowYSitesNum)
PROPERTYA(HPlacementRows, CellOrientation, HPlacementRow::Orientation, m_ld->DetailedPlacement.rowSiteOrient)
PROPERTYA2(HPlacementRows, HSite, HPlacementRow::Site, m_ld->DetailedPlacement.rowSite)
PROPERTYA2(HPlacementRows, HPlacementRow, HPlacementRow::NextRow, m_ld->DetailedPlacement.rowNextRow)
PROPERTYA2(HPlacementRows, HPlacementRow, HPlacementRow::PreviousRow, m_ld->DetailedPlacement.rowPreviousRow)

//getters specializations
GETTER(HPlacementRows, bool, HPlacementRow::IsHorizontal)
{ return Get<HPlacementRow::VertSitesCount, int>(arg) == 1; }

GETTER(HPlacementRows, bool, HPlacementRow::IsVertical)
{ return Get<HPlacementRow::HorizSitesCount, int>(arg) == 1; }

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
