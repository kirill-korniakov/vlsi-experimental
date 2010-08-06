#ifndef __PlacementGrid_H__
#define __PlacementGrid_H__

#include "HBaseDesignItem.h"
#include "HCell.h"

class HDPGrid;

class PlacementGridNode
{
protected:
  double x, y;
  int row, column;
  HCell cell;

public:
  PlacementGridNode();
  PlacementGridNode(HCell c);

  double GetX();
  double GetY();
  int GetRow();
  int GetColumn();
  HCell GetCell();

  void SetX(double x_cor);
  void SetY(double y_cor);
  void SetRow(int r);
  void SetColumn(int c);
  void SetCell(HCell c);

};

class HPlacementGrid
{
protected:
  HDesign& design;
  HDPGrid* DPGrid;
  
  PlacementGridNode** grid;
  PlacementGridNode*** cellGrid;
  int* siteInRow;
  PlacementGridNode** path;

  int nRows;
  int nColumns;

  double width;
  double height;

  double indent_x;
  double indent_y;

  double sizeBuffer;
  int sizeSiteBuffer;

public:
  HPlacementGrid(HDesign& design);
  HPlacementGrid(HDesign& design, double sizeBuf);
  ~HPlacementGrid();

  double GetWidth();
  double GetHeight();
  double GetIndent_X();
  double GetIndent_Y();

  int GetNRows();
  int GetNColumns();
  
  int GetRow(double y);
  int GetColumn(double x);

  PlacementGridNode* GetNode(int row, int column);
  PlacementGridNode* GetNode(double x, double y);
  PlacementGridNode** NodeCell(HCell cell);

  PlacementGridNode* NodePosition(double x, double y);
  bool IsSiteFree(double x, double y, double width, double height);

  void Initialize();
  void SetSize();
  void SetSizeBuffer(double sizeBuf);
  void ClearPath();
  void Clear();
  void UpdateGrid();
  void SetCell(HCell cell);

  PlacementGridNode** GetPath(int &lengthPath, double x1, double y1, double x2, double y2);


};


#endif //__PlacementGrid_H__
