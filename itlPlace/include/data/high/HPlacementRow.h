#ifndef __HIGH_PLACEMENTROW_H__
#define __HIGH_PLACEMENTROW_H__

#include "HBaseDesignItem.h"
#include "HSite.h"

BEGINITEM(HPlacementRow, HPlacementRowWrapper)

  enum Properties
  {
    Name,
    SiteWidth,
    SiteHeight,
    HorizSitesCount,
    VertSitesCount,
    X,
    Y,
    Orientation,
    Site,
    IsVertical,
    IsHorizontal,
    NextRow,
    PreviousRow
  };

ENDITEM(HPlacementRow)

BEGINHCOLLECTION(HPlacementRows, HPlacementRow)
  
  int Count() const {return m_ld->DetailedPlacement.nRows - 1;}

  ///Enumerator
  typedef HConstEnumerator<HPlacementRow,1> Enumerator;
  typedef HEnumeratorW<HPlacementRowWrapper, HPlacementRows> EnumeratorW;
  Enumerator GetEnumerator() const {return Enumerator(m_ld->DetailedPlacement.nRows);}
  EnumeratorW GetEnumeratorW();

  ///Events
  Event<2, int, int> OnGrow; ///int nRowsLimit_before, int nRowsLimit_after
  Event<1, int> OnInitialize; ///int nRowsLimit
  Event<1, HPlacementRow> OnPlacementRowAllocation;
  
  ///Operations
  void Initialize(int rowsLimit);
  void Grow(int rowsLimit);
  HPlacementRow AllocateRow();

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(string, HPlacementRow::Name, m_ld->DetailedPlacement.rowName)
  PROPERTYA(double, HPlacementRow::SiteWidth, m_ld->DetailedPlacement.rowStepX)
  PROPERTYA(double, HPlacementRow::SiteHeight, m_ld->DetailedPlacement.rowStepY)
  PROPERTYA(double, HPlacementRow::X, m_ld->DetailedPlacement.rowX)
  PROPERTYA(double, HPlacementRow::Y, m_ld->DetailedPlacement.rowY)
  PROPERTYA(int, HPlacementRow::HorizSitesCount, m_ld->DetailedPlacement.rowXSitesNum)
  PROPERTYA(int, HPlacementRow::VertSitesCount, m_ld->DetailedPlacement.rowYSitesNum)
  PROPERTYA(CellOrientation, HPlacementRow::Orientation, m_ld->DetailedPlacement.rowSiteOrient)
  PROPERTYA2(HSite, HPlacementRow::Site, m_ld->DetailedPlacement.rowSite)
  PROPERTYA2(HPlacementRow, HPlacementRow::NextRow, m_ld->DetailedPlacement.rowNextRow)
  PROPERTYA2(HPlacementRow, HPlacementRow::PreviousRow, m_ld->DetailedPlacement.rowPreviousRow)

  //getters specializations
  GETTER(bool, HPlacementRow::IsHorizontal)
    { return Get<HPlacementRow::VertSitesCount, int>(arg) == 1; }

  GETTER(bool, HPlacementRow::IsVertical)
    { return Get<HPlacementRow::HorizSitesCount, int>(arg) == 1; }

ENDHCOLLECTION(HPlacementRows)

BEGINWRAPPER(HPlacementRowWrapper, HPlacementRows)

  //properties
  PROPERTYWD(string, Name)
  PROPERTYWD(double, SiteWidth)
  PROPERTYWD(double, SiteHeight)
  PROPERTYWD(double, X)
  PROPERTYWD(double, Y)
  PROPERTYWD(int, HorizSitesCount)
  PROPERTYWD(int, VertSitesCount)
  PROPERTYWD(CellOrientation, Orientation)
  PROPERTYWD(HSite, Site)
  PROPERTYWD(HPlacementRow, NextRow)
  PROPERTYWD(HPlacementRow, PreviousRow)

  ///Getters
  GETTERWD(bool, IsHorizontal)
  GETTERWD(bool, IsVertical)

ENDWRAPPER(HPlacementRowWrapper)

inline HPlacementRows::EnumeratorW HPlacementRows::GetEnumeratorW()
  { return EnumeratorW(this, 1, m_ld->DetailedPlacement.nRows); }

#endif //__HIGH_PLACEMENTROW_H__