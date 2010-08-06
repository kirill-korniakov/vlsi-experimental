/*  
* TileGrid.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include <stdio.h>

#include "TileGrid.h"
#include "HCriticalPath.h"


TileGrid::TileGrid(int nHor, int nVert, HDPGrid& grid): nHorTiles(nHor), nVertTiles(nVert)
{
  tiles = new Tile* [nHorTiles];

  for (int i = 0; i < nHorTiles; i++)
  {
    tiles[i] = new Tile [nVertTiles];
  }
    
  xMin = grid.ColumnX(grid.NumCols());
  yMin = grid.RowY(grid.NumRows());
  xMax = grid.ColumnX(0);
  yMax = grid.RowY(0);
  
  for (HNets::ActiveNetsEnumeratorW curNet = grid.Design().Nets.GetActiveNetsEnumeratorW(); curNet.MoveNext();)
  {
        for (HNetWrapper::PinsEnumeratorW currPin = curNet.GetPinsEnumeratorW(); currPin.MoveNext();)
        {
            if (xMin > currPin.X())
                xMin = currPin.X();

            if (xMax < currPin.X())
                xMax = currPin.X();

            if (yMin > currPin.Y())
                yMin = currPin.Y();

            if (yMax < currPin.Y())
                yMax = currPin.Y();
        }
  }

  xMax += grid.SiteWidth();  //in order to place pins with xNax (or yMax)
  yMax += grid.SiteHeight(); //to the last tiles

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
    LOGERROR("error: _x < xMin!\n");
    _x = xMin;
  }

  if (_x > xMax)
  {
    LOGERROR("error: _x > xMax!\n");
    _x = xMax - tileWidth / 8;
  }

  if (_y < yMin)
  {
    LOGERROR("error: _y < yMin!\n");
    _y = yMin;
  }

  if (_y > yMax)
  {
    LOGERROR("error: _y > yMax!\n");
    _y = yMax - tileWidth / 8;
  }

  int horIdx  = min((int)((_x - xMin) / tileWidth), (nHorTiles - 1));
  int vertIdx = min((int)((_y - yMin) / tileHeight), (nVertTiles - 1));
  TileIndexes tileIdx(horIdx, vertIdx);
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

    if (newLine.GetLineType() > -1)
      lines.push_back(newLine);
    GetLinesFromTree(leftPoint, _design);
  }

  if (_design.GetBool<HSteinerPoint::HasRight>(_point))
  {
    HSteinerPoint rightPoint = _design.Get<HSteinerPoint::Left, HSteinerPoint>(_point);
    double x2 = _design.GetDouble<HSteinerPoint::X>(rightPoint);
    double y2 = _design.GetDouble<HSteinerPoint::X>(rightPoint);
    Line newLine(x1, y1, x2, y2);

    if (newLine.GetLineType() > -1)
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
  for(HNets::ActiveNetsEnumeratorW niter = _design.Nets.GetActiveNetsEnumeratorW(); niter.MoveNext(); )
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
    tiles[secondTileIdxs.horInd][secondTileIdxs.vertInd].IncNWires();

    while (!((firstTileIdxs.horInd == secondTileIdxs.horInd)
      && (firstTileIdxs.vertInd == secondTileIdxs.vertInd)))
    {
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
        LOGERROR("error in working with tiles!\n");
      }
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
        }
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
        LOGERROR("error in working with tiles?!\n");
      }
    } //while()

    currLine++;
  } //while(currLine)
}

void TileGrid::Print()
{
  WRITE("minX = %f, minY = %f\n", xMin, yMin);
  for (int i = 0; i < nHorTiles; i++)
  {
    for (int j = 0; j < nVertTiles; j++)
    {
      WRITE("tile[%d][%d] x: %f, y: %f ", i, j, tiles[i][j].GetX(), tiles[i][j].GetY());
    }

    WRITE("\n");
  }

  std::list<Line>::iterator currLine = lines.begin();

  while (currLine != lines.end())
  {
    currLine->Print();
    currLine++;
  }
}