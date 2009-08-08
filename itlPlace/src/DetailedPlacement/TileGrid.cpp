/*  
* TileGrid.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include "TileGrid.h"
#include <stdio.h>
#include "OpenCV/cv.h"
#include "OpenCV/highgui.h"
//#include "HPlotter.h"
#include "HCriticalPath.h"


TileGrid::TileGrid(int nHor, int nVert, HDPGrid& _grid): nHorTiles(nHor), nVertTiles(nVert)
{
  tiles = new Tile* [nHorTiles];

  for (int i = 0; i < nHorTiles; i++)
  {
    tiles[i] = new Tile [nVertTiles];
  }

  //TODO: check it---------------------------------------------
  yMin = _grid.RowY(0);
  yMax = _grid.RowY(_grid.NumRows() - 1) + _grid.SiteHeight();
  xMin = _grid.ColumnX(0);
  xMax = _grid.ColumnX(_grid.NumCols() - 1) + _grid.SiteWidth();
  //------------------------------------------------------------

  tileWidth  = (xMax - xMin) / nHorTiles;
  tileHeight = (yMax - yMin) / nVertTiles;  

  for (int i = 0; i < nHorTiles; i++)
  {
    for (int j = 0; j < nVertTiles; j++)
    {
      double currX = xMin + tileWidth * i;
      double currY = yMin + tileHeight * j;
      tiles[i][j].SetXY(currX, currY);
    }
  }
}

TileGrid::~TileGrid()
{
  for (int j = 0; j < nHorTiles; j++)
  {
    delete [] tiles[j];
  }

  delete [] tiles;
}

TileIndexes TileGrid::FindTileByXY(double _x, double _y)
{
  if (_x < xMin)
  {
    printf("error: _x < xMin!\n");
    _x = xMin;
  }

  if (_x > xMax)
  {
    printf("error: _x > xMax!\n");
    _x = xMax - tileWidth / 8;
  }

  if (_y < yMin)
  {
    printf("error: _y < yMin!\n");
    _y = yMin;
  }

  if (_y > yMax)
  {
    printf("error: _y > yMax!\n");
    _y = yMax - tileWidth / 8;
  }

  int horIdx  = min((int)((_x - xMin) / tileWidth), (nHorTiles - 1));
  int vertIdx = min((int)((_y - yMin) / tileHeight), (nVertTiles - 1));
  TileIndexes tileIdx(horIdx, vertIdx);
  //printf("idx: %d, %d\n", tileIdx.horInd, tileIdx.vertInd);
  return tileIdx;
}

void TileGrid::GetLinesFromTree(HSteinerPoint& _point, HDesign& _design)
{
  double x1 = _design.GetDouble<HSteinerPoint::X>(_point);
  double y1 = _design.GetDouble<HSteinerPoint::Y>(_point);

  if (_design.GetBool<HSteinerPoint::HasLeft>(_point))
  {
    HSteinerPoint leftPoint = _design.Get<HSteinerPoint::Left, HSteinerPoint>(_point);
    double x2 = _design.GetDouble<HSteinerPoint::X>(leftPoint);
    double y2 = _design.GetDouble<HSteinerPoint::X>(leftPoint);
    Line newLine(x1, y1, x2, y2);
    lines.push_back(newLine);
    GetLinesFromTree(leftPoint, _design);
  }

  if (_design.GetBool<HSteinerPoint::HasRight>(_point))
  {
    HSteinerPoint rightPoint = _design.Get<HSteinerPoint::Left, HSteinerPoint>(_point);
    double x2 = _design.GetDouble<HSteinerPoint::X>(rightPoint);
    double y2 = _design.GetDouble<HSteinerPoint::X>(rightPoint);
    Line newLine(x1, y1, x2, y2);
    lines.push_back(newLine);
    GetLinesFromTree(rightPoint, _design);
  }
}

void TileGrid::GetLinesFromNet(HNet& net, HDesign& _design)
{
  HWireWrapper wire = _design[_design.Wires[net]];
  HSteinerPoint root = wire.Root();
  GetLinesFromTree(root, _design);
}

void TileGrid::CalcLinesInTiles(HDesign& _design)
{
  for(HNets::ActiveNetsEnumeratorW niter = _design.Nets.GetNetsEnumeratorW(); niter.MoveNext(); )
    GetLinesFromNet(niter, _design);

  std::list<Line>::iterator currLine = lines.begin();

  while (currLine != lines.end())
  {
    double x1 = currLine->GetX1();
    double y1 = currLine->GetY1();
    double x2 = currLine->GetX2();
    double y2 = currLine->GetY2();
    TileIndexes firstTileIdxs  = FindTileByXY(x1, y1);
    TileIndexes secondTileIdxs = FindTileByXY(x2, y2);
    //printf("horInd: %d, vertInd: %d\n", firstTileIdxs.horInd, firstTileIdxs.vertInd);
    //printf("horInd: %d, vertInd: %d\n", secondTileIdxs.horInd, secondTileIdxs.vertInd);
    tiles[secondTileIdxs.horInd][secondTileIdxs.vertInd].IncNWires();

    while (!((firstTileIdxs.horInd == secondTileIdxs.horInd)
      && (firstTileIdxs.vertInd == secondTileIdxs.vertInd)))
    {
      //printf("horInd: %d, vertInd: %d\n", firstTileIdxs.horInd, firstTileIdxs.vertInd);
      tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].IncNWires();
      //double currTileX = xMin + tileWidth * firstTileIdxs.horInd;

      //8 possible cases:
      //1 - up        
      if ((firstTileIdxs.horInd == secondTileIdxs.horInd) && (firstTileIdxs.vertInd < secondTileIdxs.vertInd))
      {
        firstTileIdxs.vertInd++;
      }

      //2 - up, right
      else if ((firstTileIdxs.horInd < secondTileIdxs.horInd) && (firstTileIdxs.vertInd < secondTileIdxs.vertInd))
      {
        double xr = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX() + tileWidth;
        double yu = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY() + tileHeight;
        double lineY = currLine->GetY(xr);

        if (IsEqual(lineY, yu))
        {
          firstTileIdxs.horInd++;
          firstTileIdxs.vertInd++;
        }

        else if (lineY > yu)
        {
          firstTileIdxs.vertInd++;
        }

        else
        {
          firstTileIdxs.horInd++;
        }
      }

      //3 - right
      else if ((firstTileIdxs.horInd < secondTileIdxs.horInd) && (firstTileIdxs.vertInd == secondTileIdxs.vertInd))
      {
        firstTileIdxs.horInd++;
      }

      //4 - down, right
      else if ((firstTileIdxs.horInd < secondTileIdxs.horInd) && (firstTileIdxs.vertInd > secondTileIdxs.vertInd))
      {
        double xr = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX() + tileWidth;
        double yd = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY();
        double lineY = currLine->GetY(xr);

        if (IsEqual(lineY, yd))
        {
          firstTileIdxs.horInd++;
          firstTileIdxs.vertInd--;
        }

        else if (lineY < yd)
        {
          firstTileIdxs.vertInd--;
        }

        else
        {
          firstTileIdxs.horInd++;
        }
      }

      //5 - down
      else if ((firstTileIdxs.horInd == secondTileIdxs.horInd) && (firstTileIdxs.vertInd > secondTileIdxs.vertInd))
      {
        firstTileIdxs.vertInd--;
      }

      //6 - down, left
      else if ((firstTileIdxs.horInd > secondTileIdxs.horInd) && (firstTileIdxs.vertInd > secondTileIdxs.vertInd))
      {
        double xl = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX();
        double yd = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY();
        double lineY = currLine->GetY(xl);

        if (IsEqual(lineY, yd))
        {
          firstTileIdxs.horInd--;
          firstTileIdxs.vertInd--;
        }

        else if (lineY < yd)
        {
          firstTileIdxs.vertInd--;
        }

        else
        {
          firstTileIdxs.horInd--;
        };
      }

      //7 - left
      else if ((firstTileIdxs.horInd > secondTileIdxs.horInd) && (firstTileIdxs.vertInd == secondTileIdxs.vertInd))
      {
        firstTileIdxs.horInd--;
      }

      //8 - up, left
      else if ((firstTileIdxs.horInd > secondTileIdxs.horInd) && (firstTileIdxs.vertInd < secondTileIdxs.vertInd))
      {
        double xl = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX();
        double yu = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY() + tileHeight;
        double lineY = currLine->GetY(xl);

        if (IsEqual(lineY, yu))
        {
          firstTileIdxs.horInd--;
          firstTileIdxs.vertInd++;
        }

        else if (lineY > yu)
        {
          firstTileIdxs.vertInd++;
        }

        else
        {
          firstTileIdxs.horInd--;
        }
      }

      else //error!
      {
        printf("error: where are the tiles?!\n");
      }

      //printf("finished\n");
    } //while()

    currLine++;
  } //while(currLine)
}

void TileGrid::CalcCriticalWires(HDesign& _design)
{
  std::list<Line>::iterator currLine = criticalLines.begin();

  while (currLine != criticalLines.end())
  {
    double x1 = currLine->GetX1();
    double y1 = currLine->GetY1();
    double x2 = currLine->GetX2();
    double y2 = currLine->GetY2();
    TileIndexes firstTileIdxs  = FindTileByXY(x1, y1);
    TileIndexes secondTileIdxs = FindTileByXY(x2, y2);
    tiles[secondTileIdxs.horInd][secondTileIdxs.vertInd].IncNCriticalWires();

    while (!((firstTileIdxs.horInd == secondTileIdxs.horInd)
      && (firstTileIdxs.vertInd == secondTileIdxs.vertInd)))
    {
      tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].IncNCriticalWires();

      //8 possible cases:
      //1 - up        
      if ((firstTileIdxs.horInd == secondTileIdxs.horInd) && (firstTileIdxs.vertInd < secondTileIdxs.vertInd))
      {
        firstTileIdxs.vertInd++;
      }

      //2 - up, right
      else if ((firstTileIdxs.horInd < secondTileIdxs.horInd) && (firstTileIdxs.vertInd < secondTileIdxs.vertInd))
      {
        double xr = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX() + tileWidth;
        double yu = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY() + tileHeight;
        double lineY = currLine->GetY(xr);

        if (IsEqual(lineY, yu))
        {
          firstTileIdxs.horInd++;
          firstTileIdxs.vertInd++;
        }

        else if (lineY > yu)
        {
          firstTileIdxs.vertInd++;
        }

        else
        {
          firstTileIdxs.horInd++;
        }
      }

      //3 - right
      else if ((firstTileIdxs.horInd < secondTileIdxs.horInd) && (firstTileIdxs.vertInd == secondTileIdxs.vertInd))
      {
        firstTileIdxs.horInd++;
      }

      //4 - down, right
      else if ((firstTileIdxs.horInd < secondTileIdxs.horInd) && (firstTileIdxs.vertInd > secondTileIdxs.vertInd))
      {
        double xr = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX() + tileWidth;
        double yd = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY();
        double lineY = currLine->GetY(xr);

        if (IsEqual(lineY, yd))
        {
          firstTileIdxs.horInd++;
          firstTileIdxs.vertInd--;
        }

        else if (lineY < yd)
        {
          firstTileIdxs.vertInd--;
        }

        else
        {
          firstTileIdxs.horInd++;
        }
      }

      //5 - down
      else if ((firstTileIdxs.horInd == secondTileIdxs.horInd) && (firstTileIdxs.vertInd > secondTileIdxs.vertInd))
      {
        firstTileIdxs.vertInd--;
      }

      //6 - down, left
      else if ((firstTileIdxs.horInd > secondTileIdxs.horInd) && (firstTileIdxs.vertInd > secondTileIdxs.vertInd))
      {
        double xl = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX();
        double yd = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY();
        double lineY = currLine->GetY(xl);

        if (IsEqual(lineY, yd))
        {
          firstTileIdxs.horInd--;
          firstTileIdxs.vertInd--;
        }

        else if (lineY < yd)
        {
          firstTileIdxs.vertInd--;
        }

        else
        {
          firstTileIdxs.horInd--;
        };
      }

      //7 - left
      else if ((firstTileIdxs.horInd > secondTileIdxs.horInd) && (firstTileIdxs.vertInd == secondTileIdxs.vertInd))
      {
        firstTileIdxs.horInd--;
      }

      //8 - up, left
      else if ((firstTileIdxs.horInd > secondTileIdxs.horInd) && (firstTileIdxs.vertInd < secondTileIdxs.vertInd))
      {
        double xl = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetX();
        double yu = tiles[firstTileIdxs.horInd][firstTileIdxs.vertInd].GetY() + tileHeight;
        double lineY = currLine->GetY(xl);

        if (IsEqual(lineY, yu))
        {
          firstTileIdxs.horInd--;
          firstTileIdxs.vertInd++;
        }

        else if (lineY > yu)
        {
          firstTileIdxs.vertInd++;
        }

        else
        {
          firstTileIdxs.horInd--;
        }
      }

      else //error!
      {
        printf("error: where are the tiles?!\n");
      }

      //printf("finished\n");
    } //while()

    currLine++;
  } //while(currLine)
}

void TileGrid::Print()
{
  printf("minX = %f, minY = %f\n", xMin, yMin);
  for (int i = 0; i < nHorTiles; i++)
  {
    for (int j = 0; j < nVertTiles; j++)
    {
      printf("tile[%d][%d] x: %f, y: %f ", i, j, tiles[i][j].GetX(), tiles[i][j].GetY());
    }

    printf("\n");
  }

  std::list<Line>::iterator currLine = lines.begin();

  while (currLine != lines.end())
  {
    currLine->Print();
    currLine++;
  }
}

void TileGrid::DrawCongestionMap(HDesign& hd, int nMaxLines)
{
  CalcLinesInTiles(hd);

  for (int i = 0; i < nHorTiles; i++)
  {
    for (int j = 0; j < nVertTiles; j++)
    {            
      double x1 = tiles[i][j].GetX();
      double y1 = tiles[i][j].GetY();
      double x2 = x1 + tileWidth;
      double y2 = y1 + tileHeight;
      hd.Plotter.DrawTileWires(x1, y1, x2, y2, tiles[i][j].GetNWires(), nMaxLines);
    }
  }
}

void TileGrid::DrawPinDensity(HDesign& hd, int nMaxPins)
{
  //calculate number of pins in each tile
  for (HNets::NetsEnumeratorW curNet = hd.Nets.GetNetsEnumeratorW(); curNet.MoveNext();)
  {
    for (HNetWrapper::PinsEnumeratorW currPin = curNet.GetPinsEnumeratorW(); currPin.MoveNext();)
    {
      TileIndexes currTileIdxs = FindTileByXY(currPin.X(), currPin.Y());
      tiles[currTileIdxs.horInd][currTileIdxs.vertInd].IncNPins();
    }
  }

  //draw pin density
  for (int i = 0; i < nHorTiles; i++)
  {
    for (int j = 0; j < nVertTiles; j++)
    {
      double x1 = tiles[i][j].GetX();
      double y1 = tiles[i][j].GetY();
      double x2 = x1 + tileWidth;
      double y2 = y1 + tileHeight;
      hd.Plotter.DrawTilePins(x1, y1, x2, y2, tiles[i][j].GetNPins(), nMaxPins);
    }
  }
}

void TileGrid::DrawCriticalCongestionMap(HDesign &hd, int nMaxLines)
{
  for (HCriticalPaths::EnumeratorW path = hd.CriticalPaths.GetEnumeratorW(); path.MoveNext();)
  {
    HCriticalPath::PointsEnumeratorW currPoint = path.GetEnumeratorW();
    int nCriticalPoints = 1;
    HPinWrapper currPin = hd[hd.Get<HTimingPoint::Pin, HPin>(currPoint.TimingPoint())];
    double x1 = currPin.X();
    double y1 = currPin.Y();
    currPoint.MoveNext();

    while (currPoint.MoveNext())
    {
      nCriticalPoints++;
      HPinWrapper currPin = hd[hd.Get<HTimingPoint::Pin, HPin>(currPoint.TimingPoint())];
      //TileIndexes currTileIdxs = FindTileByXY(currPin.X(), currPin.Y());
      //tiles[currTileIdxs.horInd][currTileIdxs.vertInd].IncNPins();
      HPinWrapper currentPin = hd[hd.Get<HTimingPoint::Pin, HPin>(currPoint.TimingPoint())];
      double x2 = currentPin.X();
      double y2 = currentPin.Y();
      Line newLine(x1, y1, x2, y2);
      criticalLines.push_back(newLine);
      x1 = x2;
      y1 = y2;
    }
    
    if (nCriticalPoints <= 1)
    {
      printf("not enough points in critical path\n");
    }
  }

  //draw
  CalcCriticalWires(hd);

  for (int i = 0; i < nHorTiles; i++)
  {
    for (int j = 0; j < nVertTiles; j++)
    {            
      double x1 = tiles[i][j].GetX();
      double y1 = tiles[i][j].GetY();
      double x2 = x1 + tileWidth;
      double y2 = y1 + tileHeight;
      hd.Plotter.DrawTileWires(x1, y1, x2, y2, tiles[i][j].GetNCriticalWires(),
                               nMaxLines);
    }
  }
}