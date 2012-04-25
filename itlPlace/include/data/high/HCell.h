#ifndef __HIGH_CELL_H__
#define __HIGH_CELL_H__

#include "HBaseDesignItem.h"
#include "HMacroType.h"
#include "HPin.h"
#include "Index.h"
#include "HCluster.h"

BEGINITEM(HCell, HCellWrapper)

  enum Properties
  {
    Name,
    X,
    Y,
    Width,
    Height,
    Orientation,
    MacroType,
    Pins,
    PinsCount,
    PlacementStatus,
    IsTerminal,
    IsSpecial,
    IsSequential,
    IsCombinational,
    Cluster
  };

  typedef HEnumerator<HPin> PinsEnumerator;
  typedef HEnumeratorW<HPinWrapper, HPins> PinsEnumeratorW;
  
ENDITEM(HCell)

BEGINHCOLLECTION(HCells, HCell)

protected:
  HIndex m_terminals;
  MacroType m_PlacementStartType;

public:
  int CellsCount() const
    { return m_ld->NetList.nCellsEnd - 1; }
  int CellsCount(MacroType cellType) const
    { return m_ld->NetList.cellTypeBounds[cellType + 1].startCellID
      - m_ld->NetList.cellTypeBounds[cellType].startCellID; }
  int CellsCount(MacroType rangeStart, MacroType rangeEnd) const
    { return m_ld->NetList.cellTypeBounds[rangeEnd + 1].startCellID
      - m_ld->NetList.cellTypeBounds[rangeStart].startCellID; }

  int TerminalsCount() const
    { return m_terminals.Size(); }
  int TerminalsCount(MacroType cellType) const
    { return m_ld->NetList.cellTypeBounds[cellType + 1].nTerminals
      - m_ld->NetList.cellTypeBounds[cellType].nTerminals; }
  int TerminalsCount(MacroType rangeStart, MacroType rangeEnd) const
    { return m_ld->NetList.cellTypeBounds[rangeEnd + 1].nTerminals
      - m_ld->NetList.cellTypeBounds[rangeStart].nTerminals; }

  int FictivesCount() const
    { return FictivesCount(MacroType_First, MacroType_Last); }
  int FictivesCount(MacroType cellType) const
    { return m_ld->NetList.cellTypeBounds[cellType + 1].nFictive
      - m_ld->NetList.cellTypeBounds[cellType].nFictive; }
  int FictivesCount(MacroType rangeStart, MacroType rangeEnd) const
    { return m_ld->NetList.cellTypeBounds[rangeEnd + 1].nFictive
      - m_ld->NetList.cellTypeBounds[rangeStart].nFictive; }

  int MovableCellsCount() const
    { return CellsCount() - TerminalsCount() - FictivesCount(); }
  int PlaceableCellsCount() const
    { return CellsCount(m_PlacementStartType, MacroType_Last)
           - TerminalsCount(m_PlacementStartType, MacroType_Last)
           - FictivesCount(m_PlacementStartType, MacroType_Last); }

  //Enumerators
  typedef HEnumerator<HCell> CellsEnumerator;
  typedef HEnumeratorW<HCellWrapper, HCells> CellsEnumeratorW;
  typedef HIndexEnumerator<HCell> TerminalsEnumerator;
  typedef HIndexEnumeratorW<HCellWrapper, HCells> TerminalsEnumeratorW;

  class MovableCellsEnumeratorW;
  typedef MovableCellsEnumeratorW PlaceableCellsEnumeratorW;

  CellsEnumerator GetEnumerator();
  CellsEnumerator GetEnumerator(MacroType cellType);
  CellsEnumerator GetEnumerator(MacroType firstType, MacroType lastType);
  CellsEnumeratorW GetEnumeratorW();
  CellsEnumeratorW GetEnumeratorW(MacroType cellType);
  CellsEnumeratorW GetEnumeratorW(MacroType firstType, MacroType lastType);
  TerminalsEnumerator GetTerminalsEnumerator();
  TerminalsEnumeratorW GetTerminalsEnumeratorW();
  MovableCellsEnumeratorW GetMovableCellsEnumeratorW();
  MovableCellsEnumeratorW GetMovableCellsEnumeratorW(MacroType cellType);
  PlaceableCellsEnumeratorW GetPlaceableCellsEnumeratorW();

  ///events
  Event<2, int, int> OnGrow; ///int nCellsLimit_before, int nCellsLimit_after
  Event<2, HCell, HCell> BeforeSwap; ///HCell first, HCell second
  Event<1, HCell> OnCellAllocation;

  ///operations
  void Initialize(int cellsLimit);
  void Grow(int cellsLimit);
  void Swap(HCell& first, HCell& second);
  HCell AllocateCell();
  void BuildTerminalsIndex();

  //Getters & Setters
  GETTERS_SETTERS_DEFINITION()

  //properties specializations
  PROPERTYA(string, HCell::Name, m_ld->NetList.cellName)
  PROPERTYA(double, HCell::X, m_ld->NetList.cellX)
  PROPERTYA(double, HCell::Y, m_ld->NetList.cellY)
  PROPERTYA(double, HCell::Width, m_ld->NetList.cellWidth)
  PROPERTYA(double, HCell::Height, m_ld->NetList.cellHeight)
  PROPERTYA(CellOrientation, HCell::Orientation, m_ld->NetList.cellOrient)
  PROPERTYA2(HMacroType, HCell::MacroType, m_ld->NetList.cellType)

  //getters specializations
  GETTERA(PlacementStatus, HCell::PlacementStatus, m_ld->NetList.cellPlStatus)
  GETTER(bool, HCell::IsTerminal)
    { return Get<HCell::PlacementStatus, PlacementStatus>(arg) != PlacementStatus_Movable; }
  GETTER(bool, HCell::IsSpecial)
    { return m_ld->Tech->macroType[m_ld->NetList.cellType[ARGID]] <= MacroType_LastSpecial
      && m_ld->Tech->macroType[m_ld->NetList.cellType[ARGID]] >= MacroType_FirstSpecial; }
  GETTER(bool, HCell::IsSequential)
    { return m_ld->Tech->macroType[m_ld->NetList.cellType[ARGID]] <= MacroType_LastSequential
      && m_ld->Tech->macroType[m_ld->NetList.cellType[ARGID]] >= MacroType_FirstSequential; }
  GETTER(bool, HCell::IsCombinational)
    { return m_ld->Tech->macroType[m_ld->NetList.cellType[ARGID]] >= MacroType_FirstCombinational
      && m_ld->Tech->macroType[m_ld->NetList.cellType[ARGID]] <= MacroType_LastCombinational; }
  GETTER(int, HCell::PinsCount)
    { return m_ld->NetList.cellPinEndIdx[ARGID]
           - m_ld->NetList.cellPinStartIdx[ARGID]; }
  GETTER(HCell::PinsEnumerator, HCell::Pins)
    { return HCell::PinsEnumerator(m_ld->NetList.cellPinStartIdx[ARGID],
                                   m_ld->NetList.cellPinEndIdx[ARGID]); }
  GETTER(HCluster, HCell::Cluster)
    {        
        return ::__ConstructPtr<HCluster>(m_ld->NetList.cellCluster[ARGID]);
    }

 
  GETTER(HCell::PinsEnumeratorW, HCell::Pins); //implemented in HExternalMethods.h

  //setters specialization
  SETTER(PlacementStatus, HCell::PlacementStatus)
  {
    IDType mt_id = m_ld->NetList.cellType[ARGID];
    if (mt_id != 0)
    {
      IDType m_type = m_ld->Tech->macroType[mt_id];
      switch (Get<HCell::PlacementStatus, PlacementStatus>(arg))
      {
      case PlacementStatus_Fictive:
        m_ld->NetList.cellTypeBounds[m_type + 1].nFictive -= 1;
        break;
      case PlacementStatus_Fixed:
        m_ld->NetList.cellTypeBounds[m_type + 1].nTerminals -= 1;
        break;
      case PlacementStatus_Movable:
        break;
      default:
        LOGERROR("Unknown placement status detected");
        break;
      }

      switch (value)
      {
      case PlacementStatus_Fictive:
        m_ld->NetList.cellTypeBounds[m_type + 1].nFictive += 1;
        break;
      case PlacementStatus_Fixed:
        m_ld->NetList.cellTypeBounds[m_type + 1].nTerminals += 1;
        break;
      case PlacementStatus_Movable:
        break;
      default:
        LOGERROR("Unknown placement status detected");
        break;
      }
      m_ld->NetList.cellPlStatus[ARGID] = value;
    }//if (mt_id != 0)
    else
      LOGINFO("Unable to set placement status for cell having unknown macrotype");
  }
  SETTER(HCluster, HCell::Cluster)
  {
      m_ld->NetList.cellCluster[ARGID] = ::ToID(value);
  }

ENDHCOLLECTION(HCells)

BEGINWRAPPER(HCellWrapper, HCells)

  //properties
  PROPERTYWD(string, Name)
  PROPERTYWD(double, X)
  PROPERTYWD(double, Y)
  PROPERTYWD(double, Width)
  PROPERTYWD(double, Height)
  PROPERTYWD(CellOrientation, Orientation)
  PROPERTYWD(::PlacementStatus, PlacementStatus)
  PROPERTYWD(HCluster, Cluster)
  PROPERTYW(HMacroType, MacroType, Type)

  //Getters
  GETTERWD(bool, IsTerminal)
  GETTERWD(bool, IsSpecial)
  GETTERWD(bool, IsSequential)
  GETTERWD(bool, IsCombinational)
  GETTERWD(int, PinsCount)
  GETTERW(PinsEnumerator, Pins, GetPinsEnumerator)
  GETTERW(PinsEnumeratorW, Pins, GetPinsEnumeratorW);

ENDWRAPPER(HCellWrapper)

class HCells::MovableCellsEnumeratorW : public HEnumeratorW<HCellWrapper, HCells, 1>
{
  typedef HEnumeratorW<HCellWrapper, HCells, 1> BaseEnumeratorW;
public:
  MovableCellsEnumeratorW(HCells* cl, IDType start, IDType end)
    : BaseEnumeratorW(cl, start, end)
  {
  }

  bool MoveNext()
  {
    while(BaseEnumeratorW::MoveNext())
      if(this->PlacementStatus() == PlacementStatus_Movable)
        return true;
    return false;
  }
};

inline HCells::CellsEnumeratorW HCells::GetEnumeratorW()
  { return CellsEnumeratorW(this, 1, m_ld->NetList.nCellsEnd); }

inline HCells::CellsEnumeratorW HCells::GetEnumeratorW(MacroType cellType)
  { return CellsEnumeratorW(this, m_ld->NetList.cellTypeBounds[cellType].startCellID, m_ld->NetList.cellTypeBounds[cellType + 1].startCellID); }

inline HCells::CellsEnumeratorW HCells::GetEnumeratorW(MacroType firstType, MacroType lastType)
  { return CellsEnumeratorW(this, m_ld->NetList.cellTypeBounds[firstType].startCellID, m_ld->NetList.cellTypeBounds[lastType + 1].startCellID); }

inline HCells::CellsEnumerator HCells::GetEnumerator() 
  { return CellsEnumerator(1, m_ld->NetList.nCellsEnd); }

inline HCells::CellsEnumerator HCells::GetEnumerator(MacroType cellType) 
  { return CellsEnumerator(m_ld->NetList.cellTypeBounds[cellType].startCellID, m_ld->NetList.cellTypeBounds[cellType + 1].startCellID); }

inline HCells::CellsEnumerator HCells::GetEnumerator(MacroType firstType, MacroType lastType)
  { return CellsEnumerator(m_ld->NetList.cellTypeBounds[firstType].startCellID, m_ld->NetList.cellTypeBounds[lastType + 1].startCellID); }

inline HCells::TerminalsEnumerator HCells::GetTerminalsEnumerator()
  { return TerminalsEnumerator(m_terminals);}

inline HCells::TerminalsEnumeratorW HCells::GetTerminalsEnumeratorW()
  { return TerminalsEnumeratorW(this, m_terminals);}

inline HCells::MovableCellsEnumeratorW HCells::GetMovableCellsEnumeratorW()
  { return MovableCellsEnumeratorW(this, 1, m_ld->NetList.nCellsEnd); }

inline HCells::MovableCellsEnumeratorW HCells::GetMovableCellsEnumeratorW(MacroType cellType)
  { return MovableCellsEnumeratorW(this, m_ld->NetList.cellTypeBounds[cellType].startCellID, m_ld->NetList.cellTypeBounds[cellType + 1].startCellID); }

inline HCells::PlaceableCellsEnumeratorW HCells::GetPlaceableCellsEnumeratorW()
  { return PlaceableCellsEnumeratorW(this, m_ld->NetList.cellTypeBounds[m_PlacementStartType].startCellID, m_ld->NetList.nCellsEnd); }

#endif //__HIGH_CELL_H__