/*  
* SA_Window.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#include "SAWindow.h"
#include <math.h>
#include <conio.h>
#include <time.h>

#define nItersOnSortedCells 1000

void BubbleSortWindowElement(RectangularWindowElement* a, int N)
{
  for (int i = 0; i < N - 1; i++)
    for (int j = i + 1; j < N; j++)
      if ((a[i].GetY() < a[j].GetY()) ||
        ((a[i].GetY() == a[j].GetY()) && (a[i].XCoord > a[j].XCoord)))
      {
        RectangularWindowElement temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
}

SAWindow::SAWindow(int _nRows, int _first, double x_array[], int _num, HCell capturedCells[], double WL,
                   HDPGrid &_grid): RectangularWindow(_nRows, _first, x_array, _num, capturedCells, _grid),
                   currWL(WL), T(MAX_T), currentMin(WL)
{
  /*for (int i = 0; i < nCells; i++)
  {
  cells[i].width = m_grid.Design().GetDouble(HCell::Width)(capturedCells[i]);
  }*/

  /*for (int i = 0; i < nCells; i++)
  {
  ALERT("index: %d, name: %s\n", i, m_grid.Design().GetString<HCell::Name>(cells[i].cell).c_str())_;
  }*/

  optimalCoords = new RectangularWindowElement [nCells];

  for (int i = 0; i < nCells; i++)
    optimalCoords[i] = cells[i];
}

void SAWindow::UpdateCellOptimalCoords()
{
  for (int i = 0; i < nCells; i++)
    optimalCoords[i] = cells[i];
}

double SAWindow::GetFuncValue()
{
  PlaceCells();
  currWL = RecalcWL(currWL);
  return currWL;
}

void SAWindow::SearchOnSortedCells(bool trial)
{
  BubbleSortWindowElement(cells, nCells);
  int nIters = nItersOnSortedCells;
  T = MAX_T;

  if (trial == true)
  {
    nIters = nCells * 3;
    //T = 50;

    for (int iter = 0; iter < nIters; iter++)
    {
      int randCellInd = GetRandomCell();
      ShiftCell(randCellInd);
      ReduceTemperature();
    }

    /*while (T >= MIN_T)
    {
    int randCellInd = GetRandomCell();
    ShiftCell(randCellInd);
    ReduceTemperature();
    }*/
  }

  else
  {
    while (T >= MIN_T)
    {
      int randCellInd = GetRandomCell();
      ShiftCell(randCellInd);
      ReduceTemperature();
    }

    for (int i = 0; i < nCells; i++)
      cells[i] = optimalCoords[i];
  }    
}

int SAWindow::GetRandomCell()
{
  int index = rand() % nCells;
  return index;
}

void SAWindow::ShiftCell(int cellInd)
{
  double startFunc = GetFuncValue();
  double optimalFunc = startFunc * 1.1; //optimal coordinates must be changed
  double siteWidth = m_grid.SiteWidth();
  double startX = cells[cellInd].XCoord;
  double startY = cells[cellInd].GetY();
  Coordinates startCoords(startX, startY);
  Coordinates optimalCoords(startX, startY);
  Coordinates currCoords(startX, startY);
  int rowInd;

  for (rowInd = firstRow; rowInd < firstRow + nRows - 1; rowInd++)
    if (fabs(m_grid.RowY(rowInd) - cells[cellInd].GetY()) < sw_eps)
      break;

  rowInd -= firstRow; //index of this row in the window

  //----------shifting to the left-----------------------------------------------------------
  if (cellInd == 0) //the first cell
  {
    currCoords.xCoord -= siteWidth;

    while (currCoords.xCoord >= windowBounds[rowInd * 2])
    {
      cells[0].XCoord = currCoords.xCoord;
      double newFuncValue = GetFuncValue();

      if (optimalFunc > newFuncValue)
      {
        optimalFunc = newFuncValue;
        optimalCoords.xCoord = currCoords.xCoord;
      }

      currCoords.xCoord -= siteWidth;
    }

    currCoords.xCoord += siteWidth;
  }

  else //this cell is not the first
  {
    double leftCellY = cells[cellInd - 1].GetY();
    double leftCellX = cells[cellInd - 1].XCoord + cells[cellInd - 1].width;
    currCoords.xCoord -= siteWidth;

    if (leftCellY == startCoords.yCoord)
    {
      while (currCoords.xCoord >= leftCellX)
      {
        cells[cellInd].XCoord = currCoords.xCoord;
        double newFuncValue = GetFuncValue();

        if (optimalFunc > newFuncValue)
        {
          optimalFunc = newFuncValue;
          optimalCoords.xCoord = currCoords.xCoord;
        }

        currCoords.xCoord -= siteWidth;
      }

      currCoords.xCoord += siteWidth;            
    }

    else
    {
      while (currCoords.xCoord >= windowBounds[rowInd * 2])
      {
        cells[cellInd].XCoord = currCoords.xCoord;
        double newFuncValue = GetFuncValue();

        if (optimalFunc > newFuncValue)
        {
          optimalFunc = newFuncValue;
          optimalCoords.xCoord = currCoords.xCoord;
        }

        currCoords.xCoord -= siteWidth;
      }

      currCoords.xCoord += siteWidth;

      currCoords.xCoord = windowBounds[rowInd * 2 + 3] - cells[cellInd].width;
      currCoords.yCoord = leftCellY; //m_grid.RowY(rowInd + firstRow);
      cells[cellInd].YCoord = currCoords.yCoord;

      while (currCoords.xCoord >= leftCellX)
      {                
        cells[cellInd].XCoord = currCoords.xCoord;
        double newFuncValue = GetFuncValue();

        if (optimalFunc > newFuncValue)
        {
          optimalFunc = newFuncValue;
          optimalCoords.xCoord = currCoords.xCoord;
          optimalCoords.yCoord = currCoords.yCoord;
        }

        currCoords.xCoord -= siteWidth;
      }

      currCoords.xCoord += siteWidth;
    }
  }

  cells[cellInd].XCoord = startCoords.xCoord;
  cells[cellInd].YCoord = startCoords.yCoord;
  currCoords.xCoord = cells[cellInd].XCoord + siteWidth;
  currCoords.yCoord = cells[cellInd].GetY();

  //---------------------shifting to the right---------------------------------------------
  if (cellInd == nCells - 1) //the last cell
  {
    while (currCoords.xCoord <= (windowBounds[rowInd * 2 + 1] - cells[cellInd].width)) ///?
    {
      cells[cellInd].XCoord = currCoords.xCoord;
      double newFuncValue = GetFuncValue();

      if (optimalFunc > newFuncValue) 
      {
        optimalFunc = newFuncValue;
        optimalCoords.xCoord = currCoords.xCoord;
        optimalCoords.yCoord = currCoords.yCoord;
      }

      currCoords.xCoord += siteWidth; 
    }

    currCoords.xCoord -= siteWidth;
  }

  else //this cell is not the last
  {
    double rightCellY = cells[cellInd + 1].GetY();
    double rightCellX = cells[cellInd + 1].XCoord;

    if (rightCellY == startCoords.yCoord)
    {
      while (currCoords.xCoord <= (rightCellX - cells[cellInd].width))
      {
        cells[cellInd].XCoord = currCoords.xCoord;
        double newFuncValue = GetFuncValue();

        if (optimalFunc > newFuncValue)
        {
          optimalFunc = newFuncValue;
          optimalCoords.xCoord = currCoords.xCoord;
          optimalCoords.yCoord = currCoords.yCoord;
        }

        currCoords.xCoord += siteWidth;
      }

      currCoords.xCoord -= siteWidth;
    }

    else
    {
      while (currCoords.xCoord <= (windowBounds[rowInd * 2 + 1] - cells[cellInd].width))
      {
        cells[cellInd].XCoord = currCoords.xCoord;
        double newFuncValue = GetFuncValue();

        if (optimalFunc > newFuncValue)
        {
          optimalFunc = newFuncValue;
          optimalCoords.xCoord = currCoords.xCoord;
          optimalCoords.yCoord = currCoords.yCoord;
        }

        currCoords.xCoord += siteWidth;
      }

      currCoords.xCoord -= siteWidth;

      currCoords.xCoord = windowBounds[(rowInd - 1) * 2];
      currCoords.yCoord = rightCellY; //m_grid.RowY(rowInd - 1 + firstRow - 1);
      cells[cellInd].YCoord = currCoords.yCoord;

      while (currCoords.xCoord <= (rightCellX - cells[cellInd].width))
      {
        cells[cellInd].XCoord = currCoords.xCoord;
        double newFuncValue = GetFuncValue();

        if (optimalFunc > newFuncValue)
        {
          optimalFunc = newFuncValue;
          optimalCoords.xCoord = currCoords.xCoord;
          optimalCoords.yCoord = currCoords.yCoord;
        }

        currCoords.xCoord += siteWidth;
      }

      currCoords.xCoord -= siteWidth;
    }
  }

  double delta = optimalFunc - startFunc;
  double randNumber = rand();
  randNumber /= RAND_MAX;

  if ((delta < 0) || (randNumber < exp(-delta / T)))
    //if (delta < 0)
  {
    cells[cellInd].XCoord = optimalCoords.xCoord;
    cells[cellInd].YCoord = optimalCoords.yCoord;
  }

  else
  {
    cells[cellInd].XCoord = startCoords.xCoord;
    cells[cellInd].YCoord = startCoords.yCoord;
  }

  if (optimalFunc < currentMin)
  {
    currentMin = optimalFunc;
    UpdateCellOptimalCoords();
  }
}

bool SAWindow::LegalizeCells()
{
  //-------placing cells legally, near each other--------------------------------------------------------------
  double rightBound = windowBounds[(nRows - 1) * 2]; //
  double freeSpaceInRow[numOfRows];
  //freeSpaceInRow[0] = windowBounds[1] - rightBound; // - cells[0].width;

  for (int row = 0; row < nRows; row++)
    freeSpaceInRow[row] = windowBounds[row * 2 + 1] - windowBounds[row * 2];

  int currRow = nRows - 1;

  for (int i = 0; i < nCells; i++)
  {        
    double cellWidth = cells[i].width;

    while (currRow >= 0)
    {
      //rightBound = windowBounds[currRow * 2];
      //freeSpaceInRow[currRow] = windowBounds[currRow * 2 + 1] - rightBound;

      if (freeSpaceInRow[currRow] >= cellWidth) //this cell can be inserted in this row
      {
        cells[i].XCoord = rightBound;
        cells[i].YCoord = m_grid.RowY(firstRow + currRow);
        freeSpaceInRow[currRow] -= cellWidth;
        rightBound += cellWidth;
        break; //this cell was placed
      }

      else  //try to insert cell in the next row
      {
        currRow--;

        if (currRow < 0) //there are no free space in the window
        {              
          return false;  //therefore cells can't be placed in window in this order
        }

        rightBound = windowBounds[currRow * 2];
        freeSpaceInRow[currRow] = windowBounds[currRow * 2 + 1] - rightBound;
      }
    }

    //if (currRow < 0) //there are no free space in the window
    //return false;  //therefore cells can't be placed in window in this order
  }

  //---------now distribute free space between cells in each row
  /*int currCellInd = 0;

  for (int j = 0; j < nRows; j++)
  {
  if (freeSpaceInRow[j] == 0)
  {
  ;
  continue;
  }


  }*/

  return true;
}

void SAWindow::InsertCell()
{
  int firstCellInd  = GetRandomCell();
  int secondCellInd = GetRandomCell();

  while (firstCellInd == secondCellInd)
    secondCellInd = GetRandomCell();

  if (secondCellInd < firstCellInd)
  {
    int tmp       = firstCellInd;
    firstCellInd  = secondCellInd;
    secondCellInd = tmp;
  }

  RectangularWindowElement currCell = cells[firstCellInd];
  cells[firstCellInd] = cells[secondCellInd];

  int rightCell = min(secondCellInd + 1, nCells);

  for (int i = firstCellInd + 1; i < rightCell; i++)
  {
    RectangularWindowElement tmpCell = cells[i];
    cells[i] = currCell;
    currCell = tmpCell;
  }
}

void SAWindow::ExchangeTwoCells()
{
  int firstCellInd  = GetRandomCell();
  int secondCellInd = GetRandomCell();

  while (firstCellInd == secondCellInd)
    secondCellInd = GetRandomCell();

  RectangularWindowElement tmp = cells[firstCellInd];
  cells[firstCellInd]          = cells[secondCellInd];
  cells[secondCellInd]         = tmp;
}

void SAWindow::ChangeCellsOrder()
{
  if ((rand() % 2) == 0)
    ExchangeTwoCells();
  else
    InsertCell();
}

void SAWindow::GeneralSearch()
{
  SearchOnSortedCells(false);    

  for (int i = 0; i < 100; i++)
  {
    ChangeCellsOrder();

    if (LegalizeCells() == true)
    {
      SearchOnSortedCells(true);
    }
  }    

  for (int i = 0; i < nCells; i++)
    cells[i] = optimalCoords[i];

  SearchOnSortedCells(false);
}

//---------------------------------------------------------------------------------------------------------------

void MakeSAWindows(int nSteps, int nCells, int nRows, HDPGrid &_grid)
{    
  //double *x_array      = NULL;
  //int *siteIndexes     = NULL;
  //HCell *capturedCells = NULL;
  double x_array[numOfRows * 2];
  int siteIndexes[numOfRows];
  HCell capturedCells[numOfCells];    
  int firstRowIdx;
  double startWL = Utils::CalculateHPWL(_grid.Design(), true);
  time_t t1 = time(NULL);

  for (int i = 0; i < nSteps; i++)
  {
    ALERT("iter: %d\n", i);
    bool isWindowCreated = false;
    int nFoundCells = nCells;
    firstRowIdx = rand() % (_grid.NumRows() - nRows); //circuit.nRows;        
    //int nRows = min(_grid.NumRows() - firstRowIdx, _nRows);
    GetStartPoint(siteIndexes, nRows, _grid);
    isWindowCreated = GetWindowParams(firstRowIdx, nRows, nFoundCells, capturedCells, siteIndexes,
      x_array, _grid);

    if (isWindowCreated == false)
      continue;

    double currWL = Utils::CalculateHPWL(_grid.Design(), true);
    ALERT("wl before = %f\n", currWL);
    SAWindow *slidingWindow = new SAWindow(nRows, firstRowIdx, x_array, nFoundCells, capturedCells,
      currWL, _grid);						
    //slidingWindow->SearchOnSortedCells(false);
    slidingWindow->GeneralSearch();
    slidingWindow->PlaceCells();
    ALERT("bounds penalty: %f\n", slidingWindow->g1WindowBounds());
    ALERT("overlaps: %f\n", slidingWindow->g2Overlaps());
    _grid.FindCellsPositions();
    delete slidingWindow;
  }

  time_t t2 = time(NULL);
  CheckOverlaps(_grid, 100, true);
  _grid.Design().Plotter->PlotPlacement();
  double currWL = Utils::CalculateHPWL(_grid.Design(), false);
  ALERT("final wl = %f, time: %f\n", currWL, difftime(t2, t1));
  ALERT("start wl = %f, improvement: %f% \n", startWL, ((startWL - currWL) * 100 / startWL));
  _getch();
}