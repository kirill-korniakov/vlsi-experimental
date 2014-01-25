/*  
* SlidingWindow.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2008, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/
#include "SlidingWindow.h"
#include "stdlib.h"
#include <algorithm>
//#include <conio.h>
#include <math.h>

void QuickSortRowElement(WindowElement* a, long N)
{
  long i = 0, j = N;
  WindowElement temp, p;
  int rand0 = N / 2;
  p.XCoord = a[rand0].XCoord;
  p.cell = a[rand0].cell;

  do {
    while ( a[i].XCoord < p.XCoord ) i++;
    while ( a[j].XCoord > p.XCoord ) j--;

    if (i <= j)
    {
      temp.XCoord   = a[i].XCoord;
      a[i].XCoord   = a[j].XCoord;
      a[j].XCoord   = temp.XCoord;

      temp.cell = a[i].cell;
      a[i].cell = a[j].cell;
      a[j].cell = temp.cell;

      i++; j--;
    }
  } while ( i<=j );

  if ( j > 0 ) QuickSortRowElement(a, j);
  if ( N > i ) QuickSortRowElement(a+i, N-i);
}

//-----------------BasicWindow-----------------------------------------------------------------
BasicWindow::BasicWindow(int _nRows, int _first, double x_array[], int _num, HDPGrid &_grid): m_grid(_grid)
{
  nRows = _nRows;
  firstRow = _first;
  nCells = _num;
  windowBounds = new double [nRows * 2];

  for (int i = 0; i < nRows * 2; i++)
    windowBounds[i] = x_array[i];
}

//-------------------RectangularWindow---------------------------------------------------------
RectangularWindow::RectangularWindow(int _nRows, int _first, double x_array[], int _num, HCell capturedCells[],
                                     HDPGrid &_grid): BasicWindow(_nRows, _first, x_array, _num, _grid)
{
  cells = new RectangularWindowElement [nCells];

  for (int i = 0; i < nCells; i++)
  {
    HCell currCell = capturedCells[i];    
    cells[i].cell = currCell;    
    cells[i].XCoord = m_grid.Design().GetDouble<HCell::X>(currCell);
    cells[i].YCoord = m_grid.Design().GetDouble<HCell::Y>(currCell);
    cells[i].width  = m_grid.Design().GetDouble<HCell::Width>(currCell);
  }

  std::vector<HNet> nets;
  nets.reserve(nCells * 2);
  nNets = 0;

  for (int i = 0; i < nCells; i++)
  {
    for (HCell::PinsEnumerator pe = _grid.Design().Get<HCell::Pins, HCell::PinsEnumerator>(cells[i].cell); pe.MoveNext(); )
    {
      HNet current_net = _grid.Design().Get<HPin::Net, HNet>(pe);
      if (!::IsNull(current_net))
        if (find(nets.begin(), nets.end(), current_net) == nets.end())
        {
          nets.push_back(current_net);
          nNets++;
        }
    }
  }

  windowNets = new HNet [nNets];

  for (int i = 0; i < nNets; i++)
    windowNets[i] = nets[i];

  nets.clear();
}

void RectangularWindow::PlaceCells()
{
  for (int i = 0; i < nCells; i++)
  {
    HCell currCell = cells[i].cell;    
    m_grid.Design().Set<HCell::X>(currCell, cells[i].XCoord);
    m_grid.Design().Set<HCell::Y>(currCell, cells[i].YCoord);
  }
}

double RectangularWindow::g1WindowBounds()
{
  double penalty = 0;

  for (int i = 0; i < nCells; i++)
  {
    HCell currCell = cells[i].cell;
    double _XCoord = cells[i].XCoord;
    double width   = cells[i].width;
    double _Ycoord = cells[i].YCoord;

    if (_Ycoord < m_grid.RowY(firstRow))
      penalty += m_grid.RowY(firstRow) - _Ycoord;

    if (_Ycoord > m_grid.RowY(firstRow + nRows - 1))
      penalty += _Ycoord - m_grid.RowY(firstRow + nRows - 1);

    for (int j = 0; j < nRows; j++)
    {
      if (fabs(cells[i].GetY() - m_grid.RowY(firstRow + j)) < sw_eps)
      {
        if (_XCoord < windowBounds[j * 2])
          penalty += windowBounds[j * 2] - _XCoord;

        if ((_XCoord + width) > windowBounds[j * 2 + 1])
          penalty += _XCoord + width - windowBounds[j * 2 + 1];

        break;
      }
    }        
  }

  return penalty;
}

double RectangularWindow::g2Overlaps()
{
  double penalty = 0;

  for (int i = 0; i < nCells; i++)
  {
    for (int j = i + 1; j < nCells; j++)
    {
      //if (cells[i].GetY() != cells[j].GetY())
      if (!(fabs(cells[i].GetY() - cells[j].GetY()) < sw_eps))
        continue;

      int first_index  = i;
      int second_index = j;

      if (cells[i].XCoord > cells[j].XCoord)
      {
        first_index  = j;
        second_index = i;
      }

      double width    = cells[first_index].width;
      double first_x  = cells[first_index].XCoord;
      double second_x = cells[second_index].XCoord;

      if (first_x + width > second_x)
        penalty += (first_x - second_x + width);
    }
  }

  return penalty;
}

double RectangularWindow::CalcWL()
{  
  PlaceCells();
  double WL = Utils::CalculateHPWL(m_grid.Design(), true);
  return WL;
}

double RectangularWindow::RecalcWL(double currWL)
{
  PlaceCells();
  /*HCell *capturedCells = new HCell [nCells];

  for (int i = 0; i < nCells; i++)
  capturedCells[i] = cells[i].cell;

  double WLDiff = Utils::CalculateHPWLDiff(m_grid.Design(), capturedCells, nCells, true);
  delete [] capturedCells;*/
  double WLDiff = Utils::CalculateHPWLDiff(m_grid.Design(), windowNets, nNets, true);
  return currWL + WLDiff;
}

RectangularWindow::~RectangularWindow()
{		
  delete [] cells;
  delete [] windowNets;
}

//-------------------WindowWithNotOrderedCells-------------------------------------------------------------

WindowWithNotOrderedCells::WindowWithNotOrderedCells(int _nRows, int _first, double x_array[], int _num,
                                                     HCell capturedCells[], HDPGrid &_grid):
BasicWindow(_nRows, _first, x_array, _num, _grid)
{
  cells = new WindowElement [_num];
  boundX = new double[nRows + 1];
  boundX[0] = 0;

  for (int i = 1; i < nRows + 1; i++)
  {       
    boundX[i] = boundX[i - 1] + x_array[(i - 1) * 2 + 1] - x_array[(i - 1) * 2];
  }

  for(int i = 0; i < nCells; i++)
  {
    HCell currCell = capturedCells[i];
    cells[i].cell = currCell;
    cells[i].XCoord = ConvertXY_To_X(currCell);
  }
}


double WindowWithNotOrderedCells::ConvertXY_To_X(HCell cell)
{
  int rowInd = m_grid.CellRow(cell);    

  if ((rowInd < 0) || (rowInd >= m_grid.NumRows()))
  {
    LOGERROR("error in ConvertXY_To_X!");
    //_getch();
    return -1;
  }

  int rowIndInWindow = rowInd - firstRow;
  double _x = (m_grid.Design()).GetDouble<HCell::X>(cell);
  return (_x + boundX[rowIndInWindow] - windowBounds[rowIndInWindow * 2]);
}

Coordinates WindowWithNotOrderedCells::ConvertX_To_XY(double xExtended)
{
  Coordinates coordinates;    

  for (int i = 0; i < nRows; i++)
  {
    if ((boundX[i] <= xExtended) && (xExtended < boundX[i + 1]))
    {
      //int rowInd = nRows - i - 1 + firstRow;
      coordinates.xCoord = xExtended - boundX[i] + windowBounds[i * 2];
      coordinates.yCoord = m_grid.RowY(firstRow + i);
    }
  }

  if (xExtended >= boundX[nRows]) //out of the window
  {
    coordinates.yCoord = m_grid.RowY(firstRow + nRows - 1);
    coordinates.xCoord = xExtended - boundX[nRows - 1] + windowBounds[(nRows - 1) * 2];
  }

  if (xExtended < boundX[0])
  {
    coordinates.yCoord = m_grid.RowY(firstRow);
    coordinates.xCoord = xExtended + windowBounds[0];
  }   

  return coordinates;
}

double WindowWithNotOrderedCells::g1WindowBounds()
{
  double penalty = 0;

  for (int i = 0; i < nCells; i++)
  {
    HCell currCell = cells[i].cell;
    double _XCoord = cells[i].XCoord;    
    double width = m_grid.Design().GetDouble<HCell::Width>(currCell);
    double halfWidth = 0.5 * width;

    if (_XCoord < boundX[0])
    {
      penalty += boundX[0] - _XCoord;
      continue;
    }

    if ((_XCoord + width) > boundX[nRows])
    {
      penalty += _XCoord + width - boundX[nRows];						
      continue;
    }

    for (int j = 1; j < nRows + 1; j++)
    {
      if (((boundX[j] - width) < _XCoord) && (_XCoord < boundX[j]))
      {
        if ((_XCoord + width - boundX[j]) > halfWidth)
          penalty += boundX[j] - _XCoord;

        else        
          penalty += _XCoord + width - boundX[j];

        break;
      }
    }     
  }
  return penalty;
}

double WindowWithNotOrderedCells::g2Overlaps()
{
  double penalty = 0;

  for (int i = 0; i < nCells; i++)
  {
    for (int j = i + 1; j < nCells; j++)
    {
      int first_index = i;
      int second_index = j;

      if (cells[i].XCoord > cells[j].XCoord)
      {
        first_index = j;
        second_index = i;
      }

      double width = m_grid.Design().GetDouble<HCell::Width>(cells[first_index].cell);
      double first_x = cells[first_index].XCoord;
      double second_x = cells[second_index].XCoord;

      if (first_x + width > second_x)
        penalty += (first_x - second_x + width);
    }
  }
  return penalty;
}

double WindowWithNotOrderedCells::CalcWL()
{
  PlaceCells();
  double WL = Utils::CalculateHPWL(m_grid.Design(), true);
  return WL;
}

void WindowWithNotOrderedCells::PlaceCells()
{
  Coordinates coord;
  for (int i = 0; i < nCells; i++)
  {
    HCell currCell = cells[i].cell;
    coord = ConvertX_To_XY(cells[i].XCoord);        
    m_grid.Design().Set<HCell::X>(currCell, coord.xCoord);
    m_grid.Design().Set<HCell::Y>(currCell, coord.yCoord);
  }    
}

void WindowWithNotOrderedCells::ChangeCoords(const double newCoords[])
{
  double siteWidth = m_grid.SiteWidth();

  for (int i = 0; i < nCells; i++)
    cells[i].XCoord = ((int)(newCoords[i]) / siteWidth) * siteWidth;
}

double WindowWithNotOrderedCells::RecalcWL(double currWL)
{
  PlaceCells();
  HCell *capturedCells = new HCell [nCells];

  for (int i = 0; i < nCells; i++)
    capturedCells[i] = cells[i].cell;

  double WLDiff = Utils::CalculateHPWLDiff(m_grid.Design(), capturedCells, nCells, true);
  delete [] capturedCells;
  return currWL + WLDiff;
}

//------WindowWithSortedCells-------------------------------------------------------------------------------------------
WindowWithOrderedCells::WindowWithOrderedCells(int rows_num, int first, double x_array[], int cells_num,
                                               HCell capturedCells[], HDPGrid &_grid):
WindowWithNotOrderedCells(rows_num, first, x_array, cells_num,
                          capturedCells, _grid)
{
  spaces = new double [cells_num];
  QuickSortRowElement(cells, cells_num - 1);
  spaces[0] = cells[0].XCoord;

  for (int i = 1; i < cells_num; i++)
  {
    HCell cell = cells[i - 1].cell;
    spaces[i] = cells[i].XCoord - _grid.Design().GetDouble<HCell::Width>(cell) - cells[i - 1].XCoord;
  }  
}

void WindowWithOrderedCells::GetCoords()
{
  cells[0].XCoord  = spaces[0];
  double leftX     = spaces[0];
  double siteWidth = m_grid.SiteWidth();

  for(int i = 1 ; i < nCells; i++)
  {
    HCell cell = cells[i - 1].cell;
    double cellWidth = m_grid.Design().GetDouble<HCell::Width>(cell);
    //cells[i].XCoord = cells[i - 1].XCoord + cellWidth + spaces[i]; // = X(i-1) + w(i-1) + site(i)
    leftX = leftX + cellWidth + spaces[i];
    cells[i].XCoord = ((int)(leftX / siteWidth)) * siteWidth;
  }
}

double WindowWithOrderedCells::g1WindowBounds()
{
  GetCoords();
  double res = WindowWithNotOrderedCells::g1WindowBounds();
  return res;
}

double WindowWithOrderedCells::CalcWL()
{
  GetCoords();
  double res = WindowWithNotOrderedCells::CalcWL();
  return res;
}

double WindowWithOrderedCells::CalcFreeSpace()
{
  double totalWidth = 0;

  for(int i = 0; i < nCells; i++)
    totalWidth += m_grid.Design().GetDouble<HCell::Width>(cells[i].cell);

  return (boundX[nRows] - totalWidth);
}

void WindowWithOrderedCells::PlaceCells()
{
  GetCoords();
  WindowWithNotOrderedCells::PlaceCells();
}

void WindowWithOrderedCells::ChangeCoords(const double newCoords[])
{
  for (int i = 0; i < nCells; i++)
    spaces[i] = newCoords[i];
}
//----------------------------------------------------------------------------------------------

//void GetStartPoint(int *&siteIndexes, int nRows, const HDPGrid &_grid) //defines coordinates of the window's left down corner
void GetStartPoint(int siteIndexes[], int nRows, const HDPGrid &_grid)
{
  //siteIndexes = new int[nRows];
  int startIndex = rand() % _grid.NumCols();

  for (int i = 0; i < nRows; i++)        
    siteIndexes[i] = startIndex;
}

//defines parameters of the window (rows'es bounds, number of cells etc.)
bool GetWindowParams(int firstRowIdx, int nRows, int &nCells, HCell cells[], int siteIndexes[],
                     //                     double *&bounds, HDPGrid &_grid)
                     double bounds[], HDPGrid &_grid)
{    
  int numOfSites = _grid.NumCols();
  double siteWidth = _grid.SiteWidth();   
  int cellWidthInSites;
  bool isWindowCreated = true;
  int currCellsNum = 0;
  int endedRows = 0;
  int *rowWidthInSites = new int [nRows];
  bool *endOfRow = new bool [nRows];
  //cells = new HCell [nCells];

  for (int i = 0; i < nRows; i++)
  {
    int currRow = firstRowIdx + i;
    rowWidthInSites[i] = 1;
    endOfRow[i] = false;
    HCell currCell = _grid(currRow, siteIndexes[i]); //arrOfSites[i][siteIndexes[i]];

    if (!(::IsNull(currCell)))
    {
      while ((siteIndexes[i] >= 0) && (_grid(currRow,siteIndexes[i]) == currCell))
        siteIndexes[i]--;

      siteIndexes[i]++; //in order to avoid catching previous cell
    }
  }

  for (int i = 0; i < nCells; i++)
    cells[i] = _grid.Design().Cells.Null();

  HCell *lastCellInRow = new HCell [nRows];

  for (int i = 0; i < nRows; i++)
    lastCellInRow[i] = _grid.Design().Cells.Null();

  //catching cells in window
  while (currCellsNum < nCells)
  {    
    for (int i = 0; ((i < nRows) && (currCellsNum < nCells)); i++) //for all rows
    {
      if (endOfRow[i] == true)
        continue;

      int currRow = firstRowIdx + i;
      //(arrOfSites[i][rowWidthInSites[i] + siteIndexes[i] - 1] == -1)) 

      while ((rowWidthInSites[i] + siteIndexes[i] - 1 < numOfSites)
        && ((::IsNull(_grid(currRow, rowWidthInSites[i] + siteIndexes[i] - 1)))
        || (_grid(currRow, rowWidthInSites[i] + siteIndexes[i] - 1) == lastCellInRow[i])))
      {
        rowWidthInSites[i]++;
      }

      if ((rowWidthInSites[i] + siteIndexes[i] - 1) >= numOfSites) //the end of the row
      {
        endOfRow[i] = true;
        rowWidthInSites[i]--;
        endedRows++;
        continue;
      }

      else //cell was found
      {
        HCell capturedCell = _grid(currRow, rowWidthInSites[i] + siteIndexes[i] - 1);
        cellWidthInSites = static_cast<int>(_grid.Design().GetDouble<HCell::Width>(capturedCell) / siteWidth);
        cells[currCellsNum++] = capturedCell;
        lastCellInRow[i] = capturedCell;
        rowWidthInSites[i] += cellWidthInSites - 1;
      }
    }

    if (endedRows == nRows)
    {
      WRITE("only %d cells were found\n", currCellsNum);
      nCells = currCellsNum;
      //isWindowCreated = false;
      break;
    }
  }

  delete [] lastCellInRow;

  if (nCells <= 1)
  {
    isWindowCreated = false;
  }

  //initialize bounds
  for (int i = 0; i < nRows; i++)
  {        
    bounds[2 * i] = _grid.ColumnX(siteIndexes[i]); // left bound of this row in the window           
    bounds[2 * i + 1] = siteWidth * rowWidthInSites[i] + bounds[2 * i]; //right bound

    double sw_x1 = bounds[2 * i];
    double sw_x2 = bounds[2 * i + 1];
    double sw_y1 = _grid.RowY(i + firstRowIdx);
    double sw_y2 = sw_y1 + _grid.SiteHeight();
    _grid.Design().Plotter->DrawRectangle(sw_x1, sw_y1, sw_x2, sw_y2, Color_Black);

    if (siteIndexes[i] + rowWidthInSites[i] > _grid.NumCols())
    {
      LOGERROR("error while counting sites\n");
      //_getch();
    }
  }

  delete [] rowWidthInSites;
  delete [] endOfRow;
  return isWindowCreated;
}

void MakeWindows(int nSteps, int nCells, int _nRows, HDPGrid &_grid)
{    
  //double *x_array      = NULL;
  //int *siteIndexes     = NULL;    
  //HCell *capturedCells = NULL;
  double x_array[numOfRows * 2];
  int siteIndexes[numOfRows];
  HCell capturedCells[numOfCells];
  int firstRowIdx;

  for (int i = 0; i < nSteps; i++)
  {
    bool isWindowCreated = false;
    int nFoundCells = nCells;
    firstRowIdx = rand() % (_grid.NumRows() - _nRows); //circuit.nRows;        
    int nRows = std::min(_grid.NumRows() - firstRowIdx, _nRows);
    GetStartPoint(siteIndexes, nRows, _grid);
    isWindowCreated = GetWindowParams(firstRowIdx, nRows, nFoundCells, capturedCells, siteIndexes,
      x_array, _grid);

    if (isWindowCreated == false)
      continue;

    WindowWithOrderedCells *slidingWindow = new WindowWithOrderedCells(
      nRows, firstRowIdx, x_array,
      nFoundCells, capturedCells, _grid);
    slidingWindow->PlaceCells();        
    WRITE("bounds penalty: %f\n", slidingWindow->g1WindowBounds());
    WRITE("recalc WL: %f\n", slidingWindow->CalcWL());
    delete slidingWindow;
    //_getch();
  }
}
