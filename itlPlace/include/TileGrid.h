/*  
* TileGrid.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef __TILE_GRID_H__
#define __TILE_GRID_H__

#include "Tile.h"
#include "HDPGrid.h"
#include "Line.h"
#include "HPlotter.h"
#include "OpenCV/cv.h"
#include "OpenCV/highgui.h"

struct TileIndexes
{
  int horInd;
  int vertInd;

  TileIndexes(int _h, int _v): horInd(_h), vertInd(_v) {}
};

class TileGrid
{
private:
  int nHorTiles;
  int nVertTiles;
  double tileWidth;
  double tileHeight;
  double xMin;
  double xMax;
  double yMin;
  double yMax;
  Tile **tiles;
  std::list<class Line> lines;
  std::list<class Line> criticalLines;

public:
  TileGrid(int nHor, int nVert, HDPGrid& _grid);
  ~TileGrid();
  TileIndexes FindTileByXY(double _x, double _y);
  void GetLinesFromTree(HSteinerPoint& _point, HDesign& _design);
  void GetLinesFromNet(HNet& net, HDesign& _design);
  void CalcLinesInTiles(HDesign& _design);
  void CalcCriticalWires(HDesign& _design);
  void Print();
  void DrawCongestionMap(HDesign& hd, int nMaxLines);
  void DrawPinDensity(HDesign& hd, int nMaxPins);
  void DrawCriticalCongestionMap(HDesign& hd, int nMaxLines);
};

#endif