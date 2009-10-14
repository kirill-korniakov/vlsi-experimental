#include "CellUtils.h"

using namespace Utils;

static int BinarySearch(const LNetList& nl, const string& name, int first, int last)
{
  while (first <= last)
  {
    int mid = (first + last) / 2;
    if (name > nl.cellName[mid])
      first = mid + 1;
    else if (name < nl.cellName[mid]) 
      last = mid - 1;
    else
      return mid;
  }
  return 0;
}

HCell Utils::FindCellByName(HDesign& design, const string& name)
{
  for(int i = 0; i < MacroType_Last + 1; i++)
  {
    int idx = BinarySearch(design._Design.NetList, name, design._Design.NetList.cellTypeBounds[i].startCellID, design._Design.NetList.cellTypeBounds[i + 1].startCellID - 1);
    if(idx != 0)
      return ::__ConstructPtr<HCell>(idx);
  }
  return design.Cells.Null();
}

//operation less
static inline bool CompareCells(LDesign& m_ld, int first_cell_idx, int second_cell_idx)
{
  if(m_ld.Tech->macroType[m_ld.NetList.cellType[first_cell_idx]] == m_ld.Tech->macroType[m_ld.NetList.cellType[second_cell_idx]])
    return m_ld.NetList.cellName[first_cell_idx] < m_ld.NetList.cellName[second_cell_idx];
  else
    return m_ld.Tech->macroType[m_ld.NetList.cellType[first_cell_idx]] < m_ld.Tech->macroType[m_ld.NetList.cellType[second_cell_idx]];
}

void Utils::NormalizeCellsOrder(HDesign& design)
{
  if (design._Design.NetList.nCellsEnd <= 1)
    return;

  //quick sort
  int Index, Index1, ContrIndex, ContrIndex1;
  long int Stack[32][2];
  for (int i = 1; i < 32; i++) Stack[i][0] = Stack[i][1] = -1;
  Stack[0][0] = 1; Stack[0][1] = design._Design.NetList.nCellsEnd - 1;
  int N = 0;
  while (N >= 0)
  {
    Index1 = Index = Stack[N][0];
    ContrIndex1 = ContrIndex = Stack[N][1];
    while(Index != ContrIndex
      && CompareCells(design._Design, Index, Index + 1)
      && CompareCells(design._Design, Index, ContrIndex)) Index++;
    while(Index != ContrIndex
      && CompareCells(design._Design, ContrIndex - 1, ContrIndex)
      && CompareCells(design._Design, Index, ContrIndex)) ContrIndex--;
    if(Index == ContrIndex)
    {
      N--;
      continue;
    }
    HCell Y = ::__ConstructPtr<HCell>(Index);
    while (Index != ContrIndex)
    {
      if (CompareCells(design._Design, ContrIndex, Index))
      {
        design.Cells.Swap(::__ConstructPtr<HCell>(ContrIndex), Y);
        goto Sort7L1;
      }
Sort7L2:    ContrIndex--;
    }
    while (Index != ContrIndex)
    {
      if (CompareCells(design._Design, ContrIndex, Index))
      {
        design.Cells.Swap(::__ConstructPtr<HCell>(Index), Y);
        goto Sort7L2;
      }
Sort7L1:    Index++;
    }
    if ((2 * Index - Index1 - ContrIndex1 - 2) > 0)
    {
      if (Index1 < Index - 1)
      {
        Stack[N][1] = Index - 1;
        N++;
      }
      if (Index + 1 < ContrIndex1)
      {
        Stack[N][0] = Index + 1;
        Stack[N][1] = ContrIndex1;
        N++;
      }
    }
    else
    {
      if (Index + 1 < ContrIndex1)
      {
        Stack[N][0] = Index + 1;
        Stack[N][1] = ContrIndex1;
        N++;
      }
      if (Index1 < Index - 1)
      {
        Stack[N][0] = Index1;
        Stack[N][1] = Index - 1;
        N++;
      }
    }
    N--;
  }// quick sort finished


  // set cell diapasons
  int idx = 1;
  int terminals = 0;
  int fictives = 0;
  design._Design.NetList.cellTypeBounds[0].startCellID = idx;
  design._Design.NetList.cellTypeBounds[0].nTerminals = 0;
  design._Design.NetList.cellTypeBounds[0].nFictive = 0;
  for(int i = 1; i < MacroType_Last + 2; i++)
  {
    for(; design._Design.Tech->macroType[design._Design.NetList.cellType[idx]] < i 
      && idx < design._Design.NetList.nCellsEnd; idx++)
    {
      switch(design._Design.NetList.cellPlStatus[idx])
      {
      case PlacementStatus_Fixed:
        terminals++;
        break;
      case PlacementStatus_Fictive:
        fictives++;
        break;
      default:
        break;
      };
    }
    design._Design.NetList.cellTypeBounds[i].startCellID = idx;
    design._Design.NetList.cellTypeBounds[i].nTerminals = terminals;
    design._Design.NetList.cellTypeBounds[i].nFictive = fictives;
  }
}

CellOrientation Utils::GetCellOrient(const char* orient)
{
  switch (orient[0])
  {
  case 'N': return Orientation_N;
  case 'W': return Orientation_W;
  case 'S': return Orientation_S;
  case 'E': return Orientation_E;
  case 'F':
    switch (orient[1])
    {
    case 'N': return Orientation_FN;
    case 'W': return Orientation_FW;
    case 'S': return Orientation_FS;
    case 'E': return Orientation_FE;
    default: return Orientation_N;
    };
  default: return Orientation_N;
  };
}

const char* Utils::GetCellOrientStr(CellOrientation orient)
{
  switch (orient) {
      case Orientation_N: return ("N");
      case Orientation_W: return ("W");
      case Orientation_S: return ("S");
      case Orientation_E: return ("E");
      case Orientation_FN: return ("FN");
      case Orientation_FW: return ("FW");
      case Orientation_FS: return ("FS");
      case Orientation_FE: return ("FE");
  };
  return ("BOGUS");
}