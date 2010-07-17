#include "PlacementGrid.h"
#include <math.h>
#include "HDesign.h"
#include "Auxiliary.h"
#include "lef/FEF_util.h"

//#define _CRT_DBG_MAP_ALLOC
//#include "crtdbg.h"

using namespace Aux;

//PlacementGridNode
PlacementGridNode::PlacementGridNode()
{
  x = 0;
  y = 0;
  row = 0;
  column = 0;
}

PlacementGridNode::PlacementGridNode(HCell c)
{
  x = 0;
  y = 0;
  row = 0;
  column = 0;
  cell = c;
}

double PlacementGridNode::GetX()
{
  return x;
}

double PlacementGridNode::GetY()
{
  return y;
}
int PlacementGridNode::GetRow()
{
  return row;
}

int PlacementGridNode::GetColumn()
{
  return column;
}

HCell PlacementGridNode::GetCell()
{
  return cell;
}

void PlacementGridNode::SetX(double x_cor)
{
  x = x_cor;
}

void PlacementGridNode::SetY(double y_cor)
{
  y = y_cor;
}

void PlacementGridNode::SetRow(int r)
{
  row = r;
}

void PlacementGridNode::SetColumn(int c)
{
  column = c;
}

void PlacementGridNode::SetCell(HCell c)
{
  cell = c;
}
//PlacementGridNode

//HPlacementGrid

HPlacementGrid::HPlacementGrid(HDesign& hd): design(hd)
{
  grid = NULL;
  cellGrid = NULL;
  siteInRow = NULL;
  path = NULL;
  nColumns = 0;
  nRows = 0;
  sizeSiteBuffer = 0;
  Initialize();
}

HPlacementGrid::HPlacementGrid(HDesign& hd, double sizeBuf): design(hd)
{
  grid = NULL;
  cellGrid = NULL;
  siteInRow = NULL;
  path = NULL;
  nColumns = 0;
  nRows = 0;
  sizeBuffer = sizeBuf;
  sizeSiteBuffer = 0;
  Initialize();
}

HPlacementGrid::~HPlacementGrid()
{
  Clear();
}

double HPlacementGrid::GetWidth()
{
  return width;
}

double HPlacementGrid::GetHeight()
{
  return height;
}

double HPlacementGrid::GetIndent_X()
{
  return indent_x;
}

double HPlacementGrid::GetIndent_Y()
{
  return indent_y;
}

int HPlacementGrid::GetNRows()
{
  return nRows;
}

int HPlacementGrid::GetNColumns()
{
  return nColumns;
}

PlacementGridNode* HPlacementGrid::GetNode(int row, int column)
{
  return &grid[row][column];
}

int HPlacementGrid::GetRow(double y)
{
  return int (floor((y - indent_y) / height));
}

int HPlacementGrid::GetColumn(double x)
{
  return int (floor((x - indent_x) / width));
}

PlacementGridNode* HPlacementGrid::GetNode(double x, double y)
{
  return &grid[GetRow(y)][GetColumn(x)];
}


PlacementGridNode** HPlacementGrid::NodeCell(HCell cell)
{
  return cellGrid[::ToID(cell)];
}

PlacementGridNode* HPlacementGrid::NodePosition(double x, double y)
{
  return &grid[GetRow(y)][GetColumn(x)];
}

bool HPlacementGrid::IsSiteFree(double x, double y, double width, double height)
{

  double x1 = x, y1 = y, 
    x2 = x + width, y2 =  y,
    x3 = x, y3 = y + height,
    x4 = x + width, y4 = y + height;


  int rowBegin = GetRow(y1);//int (floor((y1 - indent_y) / height)) - 1;
  int rowEnd = GetRow(y3);//int (floor((y3 - indent_y) / height)) - 1;
  int columnBegin = GetColumn(x1);//int (floor((x1 - indent_x) / width)) - 1;
  int columnEnd = GetColumn(x4);//int (floor((x4 - indent_x) / width)) - 1;

  if ( (rowBegin > 0) && (rowBegin < nRows) &&
    (rowEnd > 0) && (rowEnd < nRows) &&
    (columnBegin > 0) && (columnBegin < nColumns) &&
    (columnEnd > 0) && (columnEnd < nColumns))
  for (int i = rowBegin; i <= rowEnd; i++)
    for (int j = columnBegin; j <= columnEnd; j++)
    {
      if (grid[i][j].GetCell() != design.Cells.Null())
        return false;
    }
    return true;

}

void HPlacementGrid::Initialize()
{
  Clear();
  SetSize();
  UpdateGrid();
}

void HPlacementGrid::SetSize()
{

  int totalRows = design.PlacementRows.Count();
  grid = new PlacementGridNode* [totalRows];
  cellGrid = new PlacementGridNode** [design.Cells.CellsCount()];
  siteInRow = new int [totalRows];
  indent_x = design.Circuit.PlacementMaxX();
  HSite m_SiteType;
  int i = 0;
  for (HPlacementRows::EnumeratorW rIter = design.PlacementRows.GetEnumeratorW(); rIter.MoveNext(); i++)
  {
    if (!rIter.IsHorizontal())
    {
      continue;
    }

    if (::IsNull(m_SiteType))
      m_SiteType = rIter.Site();

    if (nColumns < rIter.HorizSitesCount())
      nColumns = rIter.HorizSitesCount();
    siteInRow[i] = rIter.HorizSitesCount();
    if (indent_x > rIter.X())
      indent_x = rIter.X();
  }

  nRows = i;
  width = design.GetDouble<HSite::Width>(m_SiteType);
  if (sizeBuffer < width)
    sizeSiteBuffer = 1;
  else
  {
    sizeSiteBuffer = int(ceil(sizeBuffer / width));
  }
  if (sizeSiteBuffer <= 0)
    sizeSiteBuffer = 1;

  int j = 0;
  for (HPlacementRows::EnumeratorW rIter = design.PlacementRows.GetEnumeratorW(); rIter.MoveNext(); j++)
  {
    if (!rIter.IsHorizontal())
    {
      continue;
    }

    grid[j] = new PlacementGridNode [nColumns];

    for (int k = 0; k < nColumns; k ++)
    {
      grid[j][k].SetRow(j);
      grid[j][k].SetColumn(k);
      grid[j][k].SetX(indent_x + k * width);
      grid[j][k].SetY(rIter.Y());
      grid[j][k].SetCell(design.Cells.Null());
    }

  }
  indent_y = grid[0][0].GetY();
  height = grid[1][0].GetY() - grid[0][0].GetY();

  path = new PlacementGridNode* [nRows * nColumns + 1];
  ClearPath();
}

void HPlacementGrid::SetSizeBuffer(double sizeBuf)
{
  sizeBuffer = sizeBuf;
}

void HPlacementGrid::ClearPath()
{
  for (int i = 0; i < (nRows * nColumns + 1); i++)
    path[i] = NULL;
}

void HPlacementGrid::Clear()
{
  if (grid != NULL)
  {
    for (int i = 0; i < nRows; i++)
      delete [] grid[i];
    delete [] grid;    
  }

  if (cellGrid != NULL)
  {
    for (int i = 0; i < design.Cells.CellsCount(); i++)
      delete [] cellGrid[i];
    delete [] cellGrid;
  }

  if (siteInRow != NULL)
  {
    delete [] siteInRow;
  }

  if (path != NULL)
  {
    delete [] path;
  }
}

void HPlacementGrid::UpdateGrid()
{
  for (HCells::CellsEnumerator cell = design.Cells.GetEnumerator(); cell.MoveNext();)
  {
    SetCell(cell);
  }
}

void HPlacementGrid::SetCell(HCell cell)
{
  HCellWrapper cellW = design[cell];
  double x1 = cellW.X(), y1 = cellW.Y(), 
    x2 = cellW.X() + cellW.Width(), y2 =  cellW.Y(),
    x3 = cellW.X(), y3 = cellW.Y() + cellW.Height(),
    x4 = cellW.X() + cellW.Width(), y4 = cellW.Y() + cellW.Height();
  double cellWWidth = cellW.Width();
  double cellWHeight = cellW.Height();

  int cellId = ::ToID(cell);

  int rowBegin = GetRow(y1);//int (floor((y1 - indent_y) / height)) - 1;
  int rowEnd = GetRow(y3);//int (floor((y3 - indent_y) / height)) - 1;
  int columnBegin = GetColumn(x1);//int (floor((x1 - indent_x) / width)) - 1;
  int columnEnd = GetColumn(x4);//int (floor((x4 - indent_x) / width)) - 1;

  int sizeCell = (rowEnd - rowBegin + 1) * (columnEnd - columnBegin + 1);
  cellGrid[cellId] = new PlacementGridNode* [sizeCell];
  int k = 0;
  if ( (rowBegin > 0) && (rowBegin < nRows) &&
    (rowEnd > 0) && (rowEnd < nRows) &&
    (columnBegin > 0) && (columnBegin < nColumns) &&
    (columnEnd > 0) && (columnEnd < nColumns))
    for (int i = rowBegin; i <= rowEnd; i++)
      for (int j = columnBegin; j <= columnEnd; j++)
      {
        grid[i][j].SetCell(cell);
        cellGrid[cellId][k] = &grid[i][j];
        k++;
      }
}

//PlacementGridNode** HPlacementGrid::GetPath(int &lengthPath, double x1, double y1, double x2, double y2)
//{
//  /*
//  1. идем от начала
//  2. если приблизились то назад не более чем на м
//  3. сортирум по уровни по удаленности от начала
//  4. следующая точка приближает хоть по одной координате
//  5. из двух одинаковых точек выбираем ту что ближе к диагонали
//  6. из равносильных путей выбираем более короткий
//  */
//
//
//  int rowBegin = GetRow(y1);//int (floor((y1 - indent_y) / height)) - 1;
//  int rowEnd = GetRow(y2);//int (floor((y3 - indent_y) / height)) - 1;
//  int columnBegin = GetColumn(x1);//int (floor((x1 - indent_x) / width)) - 1;
//  int columnEnd = GetColumn(x2);//int (floor((x4 - indent_x) / width)) - 1;
//
//  int rowSize = abs(rowEnd - rowBegin) + 1;
//  int columnSize = abs(columnEnd - columnBegin) + 1;
//
//  int incrementRow = sign(double(rowEnd - rowBegin));
//  int incrementColumn = sign(double(columnEnd - columnBegin));
//
//  int minSize = MIN(rowSize, columnSize);
//  int maxSize = MAX(rowSize, columnSize);
//
//
//  /*PlacementGridNode*** freeNode = new PlacementGridNode** [rowSize + 1];
//
//  for (int i = 0; i < (rowSize + 1); i++)
//  {
//    freeNode[i] = new PlacementGridNode* [columnSize + 1];
//    for (int j = 0; j < (columnSize + 1); j++)
//      freeNode[i][j] = NULL;
//  }*/
//  int totalPoint = 0;
//  //int levelInd = 0;
//  /*for (int t = 1; t < minSize; t++)
//  {
//  levelInd = 0;
//  for (int k = 0; k < t; k++)
//  {
//  if (grid[rowBegin + k * incrementRow][columnBegin + t * incrementColumn].GetCell() == design.Cells.Null())
//  {        
//  totalPoint++;
//  freeNode[t][levelInd] = &grid[rowBegin + k * incrementRow][columnBegin + t * incrementColumn];
//  levelInd++;
//  }
//
//  if (grid[rowBegin + t * incrementRow][columnBegin + k * incrementColumn].GetCell() == design.Cells.Null())
//  {        
//  totalPoint++;
//  freeNode[t][levelInd] = &grid[rowBegin + t * incrementRow][columnBegin + k * incrementColumn];
//  levelInd++;
//  }
//  }
//
//  if (grid[rowBegin + t * incrementRow][columnBegin + t * incrementColumn].GetCell() == design.Cells.Null())
//  {      
//  totalPoint++;
//  freeNode[t][levelInd] = &grid[rowBegin + t * incrementRow][columnBegin + t * incrementColumn];
//  levelInd++;
//  }
//
//  }*/
//
//  //if(rowSize > columnSize)
//  //{
//  //int m = columnSize;
//  totalPoint = 0;
//  PlacementGridNode** path = new PlacementGridNode* [rowSize * columnSize];
//  for (int i = 0; i <= rowSize; i++)
//  {
//    //levelInd = 0;
//    for (int j = 0; j <= columnSize; j++)
//    {
//      if (((rowBegin + i * incrementRow) >= 0) && ((columnBegin + j * incrementColumn) >=0) && ((columnBegin + j * incrementColumn) <= nColumns) && ((rowBegin + i * incrementRow) <= nRows))
//        if (grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn].GetCell() == design.Cells.Null())
//        {
//          path[totalPoint] = &grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn];
//          totalPoint++;
//          //freeNode[i][levelInd] = &grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn];
//          //levelInd++;
//        }
//    }
//  }
//
//  /*if (totalPoint == 64)
//  int jfgj = 0;*/
//
//  /*}
//  else
//  {
//  //columnSize > rowSize
//  int m = rowSize;
//  for (int i = m; i <= columnSize; i++)
//  {
//  levelInd = 0;
//  for (int j = 0; j <= rowSize; j++)
//  {
//  if (grid[rowBegin + j * incrementRow][columnBegin + i * incrementColumn].GetCell() == design.Cells.Null())
//  {          
//  totalPoint++;
//  freeNode[i][levelInd] = &grid[rowBegin + j * incrementRow][columnBegin + i * incrementColumn];
//  levelInd++;
//  }
//  }
//  }
//  }*/
//  lengthPath = totalPoint;
//
//  /*int j = 0;
//  for (int i = 0; i <= rowSize; i++)
//  {
//    levelInd = 0;
//    while ((freeNode[i][levelInd] != NULL))
//    {
//      path[j] = freeNode[i][levelInd];
//      levelInd++;
//      j++;
//    }
//  }*/
//
//  //lengthPath = j;
//
//  //if (lengthPath != totalPoint)
//  //  ALERT("ERROR 5!!! lengthPath = %d\ttotalPoint = %d", lengthPath, totalPoint);
//
// /* for (int i = 0; i < (rowSize + 1); i++)
//    delete [] freeNode[i];
//  delete [] freeNode;*/
//  return path;
//}*/

PlacementGridNode** HPlacementGrid::GetPath(int &lengthPath, double x1, double y1, double x2, double y2)
{

  //ClearPath();

  int rowBegin = GetRow(y1);//int (floor((y1 - indent_y) / height)) - 1;
  int rowEnd = GetRow(y2);//int (floor((y3 - indent_y) / height)) - 1;
  int columnBegin = GetColumn(x1);//int (floor((x1 - indent_x) / width)) - 1;
  int columnEnd = GetColumn(x2);//int (floor((x4 - indent_x) / width)) - 1;

  int rowSize = abs(rowEnd - rowBegin) + 1;
  int columnSize = abs(columnEnd - columnBegin) + 1;

  int incrementRow = sign(double(rowEnd - rowBegin));
  int incrementColumn = sign(double(columnEnd - columnBegin));

  int totalPoint = 0;

  
  for (int i = 0; i <= rowSize; i++)
  {
    //levelInd = 0;
    for (int j = 0; j <= columnSize; j++)
    {
      if (((rowBegin + i * incrementRow) >= 0) && ((columnBegin + j * incrementColumn) >=0) && ((columnBegin + j * incrementColumn) < nColumns) && ((rowBegin + i * incrementRow) < nRows))
        if(IsSiteFree(grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn].GetX(), grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn].GetY(), sizeBuffer, height))
        //if (grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn].GetCell() == design.Cells.Null())
        {
          path[totalPoint] = &grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn];
          totalPoint++;
          //freeNode[i][levelInd] = &grid[rowBegin + i * incrementRow][columnBegin + j * incrementColumn];
          //levelInd++;
        }
    }
  }
  lengthPath = totalPoint;
  return path;
}

//HPlacementGrid