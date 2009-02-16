/*  
* abs_detailed_placement.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2008, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/
#include "..\include\abs_detailed_placement.h"
#include "stdlib.h"
#include <algorithm>
#include <conio.h>
#include <math.h>

#define eps 0.01

void QuickSortRowElement(WindowElement* a, long N)
{
  long i = 0, j = N;
  WindowElement temp, p;
  int rand0 = N / 2;
  p.XCoord = a[rand0].XCoord;
  p.cellIndex = a[rand0].cellIndex;

  do {
    while ( a[i].XCoord < p.XCoord ) i++;
    while ( a[j].XCoord > p.XCoord ) j--;

    if (i <= j)
    {
      temp.XCoord   = a[i].XCoord;
      a[i].XCoord   = a[j].XCoord;
      a[j].XCoord   = temp.XCoord;

      temp.cellIndex = a[i].cellIndex; 
      a[i].cellIndex = a[j].cellIndex; 
      a[j].cellIndex = temp.cellIndex;

      i++; j--;
    }
  } while ( i<=j );

  if ( j > 0 ) QuickSortRowElement(a, j);
  if ( N > i ) QuickSortRowElement(a+i, N-i);
}

//-----------------BasicWindow-----------------------------------------------------------------

BasicWindow::BasicWindow(int _nRows, int _first, double x_array[], int _num)
{
  nRows = _nRows;
  firstRow = _first;
  elementsNum = _num;
  windowBounds = new double [nRows * 2];

  for (int i = 0; i < nRows * 2; i++)
    windowBounds[i] = x_array[i];
}

//-------------------RectangularWindow---------------------------------------------------------

RectangularWindow::RectangularWindow(int _nRows, int _first, double x_array[], int _num,
                                     const Circuit &circuit, int indexes[]):
                                     BasicWindow(_nRows, _first, x_array, _num)
{
  elements = new RectangularWindowElement [elementsNum];

  for (int i = 0; i < elementsNum; i++)
  {
    int index = indexes[i];
    elements[i].cellIndex = index;    
    elements[i].XCoord = circuit.placement[index].xCoord - 0.5 * circuit.nodes[index].width;
    elements[i].YCoord = circuit.placement[index].yCoord - 0.5 * circuit.rows[0].height;    
  }
}

void RectangularWindow::PlaceCells(DataStructures::Circuit &circuit)
{
  for (int i = 0; i < elementsNum; i++)
  {
    int cellInd = elements[i].cellIndex;
    circuit.placement[cellInd].xCoord = elements[i].XCoord + circuit.nodes[cellInd].width * 0.5;
    circuit.placement[cellInd].yCoord = elements[i].YCoord + circuit.nodes[cellInd].height * 0.5;
  }
}

double RectangularWindow::g1WindowBounds(const Circuit &circuit)
{
  double penalty = 0;
  
  for (int i = 0; i < elementsNum; i++)
  {
    int index = elements[i].cellIndex;
    double _XCoord = elements[i].XCoord;
    double width = circuit.nodes[index].width;    

    for (int j = 0; j < nRows; j++)
    {
      if (fabs(elements[i].GetY() - circuit.rows[firstRow + j].coordinate) < eps)
      {
        if (_XCoord < windowBounds[j * 2])
          penalty += windowBounds[j * 2] - _XCoord;

        if ((_XCoord + width) > windowBounds[j * 2 + 1])
          penalty += windowBounds[j * 2 + 1] - _XCoord + width;
        
        break;
      }
    }        
  }

  return penalty;
}

double RectangularWindow::g2Overlaps(const Circuit &circuit)
{
  double penalty = 0;
  
  for (int i = 0; i < elementsNum; i++)
    for (int j = i + 1; j < elementsNum; j++)
    {
      if (elements[i].GetY() != elements[j].GetY())
        continue;
      
      int first_index = i;
      int second_index = j;

      if (elements[i].XCoord > elements[j].XCoord)
      {
        first_index = j;
        second_index = i;
      }

      double width = circuit.nodes[elements[first_index].cellIndex].width;
      double first_x = elements[first_index].XCoord;
      double second_x = elements[second_index].XCoord;

      if (first_x + width > second_x)
        penalty += (first_x - second_x + width);
    }

  return penalty;
}

double RectangularWindow::CalcWL(const Circuit &circuit)
{
  for (int i = 0; i < elementsNum; i++)
  {
    int index = elements[i].cellIndex;
    double halfWidth = circuit.nodes[index].width * 0.5;        
    double siteheight = circuit.rows[0].height;        
    circuit.placement[index].xCoord = elements[i].XCoord + halfWidth;
    circuit.placement[index].yCoord = elements[i].GetY() + siteheight * 0.5;
  }

  double WL = cf_recalc_all(false, circuit.nNets, circuit.nets, circuit.placement);
  return WL;
}

double RectangularWindow::RecalcWl(Circuit &circuit, double currWL, const int changedNets[],
                               int changedNetsCount)
{
  PlaceCells(circuit);
  double newWL = cf_recalc_some_nets(true, circuit.nNets, circuit.nets, currWL,
                                     changedNets, changedNetsCount, circuit.placement);
  return newWL;
}

//-------------------GeneralWindow-------------------------------------------------------------

GeneralWindow::GeneralWindow(int _nRows, int _first, double x_array[],
                             int _num, const Circuit &circuit,
                             int indexes[]): BasicWindow(_nRows, _first, x_array, _num)
{  
  elements = new WindowElement [_num];
  boundX = new double[nRows + 1];
  boundX[0] = 0;

  for (int i = 1; i < nRows + 1; i++)
  { 
    int curInd = (nRows - i) * 2 + 1;
    boundX[i] = boundX[i - 1] + x_array[curInd] - x_array[curInd - 1];    
  }

  for(int i = 0; i < elementsNum; i++)
  {
    int index = indexes[i];
    elements[i].cellIndex = index;
    double _x = circuit.placement[index].xCoord - 0.5 * circuit.nodes[index].width;		
    double _y = circuit.placement[index].yCoord - 0.5 * circuit.rows[0].height;		
    elements[i].XCoord = ConvertXY_To_X(_x, _y, circuit);
  }
}


double GeneralWindow::ConvertXY_To_X(double xCoord, double yCoord, const Circuit &circuit) {
  double siteheight = circuit.rows[0].height;  

  for (int i = 0; i < nRows; i++)
  {
    int rowInd = nRows - i - 1 + firstRow;

    if (yCoord == rowInd * siteheight) 
      return (xCoord + boundX[i] - windowBounds[(nRows - i - 1) * 2]);    
  }
  
  printf("error in ConvertXY_To_X\n");  
  _getch();
  return -1;  
}

Coordinates GeneralWindow::ConvertX_To_XY(double xExtended, const Circuit &circuit, double cellWidth)
{
  Coordinates coordinates;
  double siteheight = circuit.rows[0].height;
  double halfWidth = cellWidth * 0.5;

  for (int i = 0; i < nRows; i++)
  {
    if ((boundX[i] <= xExtended) && (xExtended < boundX[i + 1])) //cell is in row (nRows - i - 1 + firstRow)
    {
      int rowInd = nRows - i - 1 + firstRow;
      coordinates.xCoord = xExtended - boundX[i] + windowBounds[(nRows - i - 1) * 2];
      coordinates.yCoord = siteheight * rowInd;
    }
  }

  if (xExtended >= boundX[nRows]) //out of the window
  {
    coordinates.yCoord = siteheight * firstRow;
    coordinates.xCoord = xExtended - boundX[nRows - 1] + windowBounds[0];
  }

  if (xExtended < boundX[0])
  {
    coordinates.yCoord = siteheight * (firstRow + nRows - 1);
    coordinates.xCoord = xExtended + windowBounds[(nRows - 1) * 2];
  }   

  return coordinates;
}

double GeneralWindow::g1WindowBounds(const Circuit &circuit)
{
  double penalty = 0;
  
  for (int i = 0; i < elementsNum; i++)
  {
    int index = elements[i].cellIndex;
    double _XCoord = elements[i].XCoord;
    double width = circuit.nodes[index].width;
    double halfWidth = 0.5 * width;
    
    if (_XCoord < boundX[0])
    {
      penalty += boundX[0] - _XCoord;
			//cout<<"windowBounds: index="<<index<<endl;
      continue;
    }

    if ((_XCoord + width) > boundX[nRows])
    {
      penalty += _XCoord + width - boundX[nRows];
			//cout<<"windowBounds: index="<<index<<endl<<circuit.placement[index].xCoord<<endl<<_XCoord<<endl;			
      continue;
    }

    for (int j = 1; j < nRows + 1; j++)
    {
      if (((boundX[j] - width) < _XCoord) && (_XCoord < boundX[j])) {
				if ((_XCoord + width - boundX[j]) > halfWidth) {
          penalty += boundX[j] - _XCoord;					
				}

				else {
          penalty += _XCoord + width - boundX[j];					
				}
				break;
			}
    }     
  }

  return penalty;
}

double GeneralWindow::g2Overlaps(const Circuit &circuit)
{
  double penalty = 0;
  
  for (int i = 0; i < elementsNum; i++)
    for (int j = i + 1; j < elementsNum; j++)
    {
      int first_index = i;
      int second_index = j;

      if (elements[i].XCoord > elements[j].XCoord)
      {
        first_index = j;
        second_index = i;
      }

      double width = circuit.nodes[elements[first_index].cellIndex].width;
      double first_x = elements[first_index].XCoord;
      double second_x = elements[second_index].XCoord;

      if (first_x + width > second_x)
        penalty += (first_x - second_x + width);
    }

  return penalty;
}

double GeneralWindow::CalcWL(const Circuit &circuit)
{
  for (int i = 0; i < elementsNum; i++)
  {
    int index = elements[i].cellIndex;
    double element_width = circuit.nodes[index].width;
    Coordinates coordinates = ConvertX_To_XY(elements[i].XCoord, circuit, element_width);
    double siteheight = circuit.rows[0].height;
    double halfWidth = element_width * 0.5;
    coordinates.xCoord += halfWidth; //coordinates of the element's center
    coordinates.yCoord += siteheight * 0.5;
    circuit.placement[index].xCoord = coordinates.xCoord;
    circuit.placement[index].yCoord = coordinates.yCoord;
  }

  double WL = cf_recalc_all(false, circuit.nNets, circuit.nets, circuit.placement);
  return WL;
}

void GeneralWindow::PlaceCells(Circuit &circuit)
{
    Coordinates coord;    
    for (int i = 0; i < elementsNum; i++)
    {
        int currIdx = elements[i].cellIndex;        
        coord = ConvertX_To_XY(elements[i].XCoord, circuit, circuit.nodes[currIdx].width);
        circuit.placement[currIdx].xCoord = coord.xCoord + 0.5 * circuit.nodes[currIdx].width;
        circuit.placement[currIdx].yCoord = coord.yCoord + 0.5 * circuit.nodes[currIdx].height;
    }    
}

double GeneralWindow::RecalcWl(Circuit &circuit, double currWL, const int changedNets[],
                               int changedNetsCount)
{
  PlaceCells(circuit);
  double newWL = cf_recalc_some_nets(true, circuit.nNets, circuit.nets, currWL,
                                     changedNets, changedNetsCount, circuit.placement);
  return newWL;
}

//------WindowWithSortedCells-------------------------------------------------------------------------------------------

WindowWithOrderedCells::WindowWithOrderedCells(int rows_num, int first, double x_array[],
                                               int elements_num, const Circuit &circuit,
                                               int indexes[]):
                                               GeneralWindow(rows_num, first, x_array, 
                                               elements_num, circuit, indexes)
{
  spaces = new double [elements_num];
  QuickSortRowElement(elements, elements_num - 1);
  spaces[0] = elements[0].XCoord;
  
  for (int i = 1; i < elements_num; i++)
  {
      int index = elements[i - 1].cellIndex;  
      spaces[i] = elements[i].XCoord - circuit.nodes[index].width - elements[i - 1].XCoord;
  }  
}

void WindowWithOrderedCells::GetCoords(const Circuit &circuit)
{
    elements[0].XCoord = spaces[0];
    
    for(int i = 1 ; i < elementsNum; i++) {
        int index = elements[i - 1].cellIndex;
        double cellWidth = circuit.nodes[index].width;
        elements[i].XCoord = elements[i - 1].XCoord + cellWidth + spaces[i]; // = X(i-1) + w(i-1) + site(i)
    }
}

double WindowWithOrderedCells::CalcBoundsPenalty(const Circuit &circuit)
{
    GetCoords(circuit);
    double res = g1WindowBounds(circuit);
    return res;
}

double WindowWithOrderedCells::CalcWireLenght(const Circuit &circuit)
{
    GetCoords(circuit);
    double res = CalcWL(circuit);
    return res;
}

double WindowWithOrderedCells::CalcFreeSpace(const DataStructures::Circuit &circuit)
{
    double totalWidth = 0;

    for(int i = 0; i < elementsNum; i++)
      totalWidth += circuit.nodes[elements[i].cellIndex].width;
    
    return (boundX[nRows] - totalWidth);
}
//----------------------------------------------------------------------------------------------

void GetStartPoint(int *&siteIndexes, int nRows, const Circuit &circuit) { //defines coordinates of the window's left down corner
    siteIndexes = new int[nRows];
    int startIndex = 2; //rand() % circuit.rows[0].numSites;

    for (int i = 0; i < nRows; i++) {      
      siteIndexes[i] = startIndex;
    }
}

//defines parameters of the window (rows'es bounds, number of cells etc.)
bool GetWindowParams(int firstRowIdx, int nRows, int &nCells, int siteIndexes[],
                     int *&cellIndexes, double *&bounds, const Circuit &circuit)
{
    int** arrOfSites;
    int numOfSites = circuit.rows[0].numSites;
    double siteWidth;
    double rowHeight;
    double rowOrigin;
    int siteIdx;
    int cellWidthInSites;
    int lastRowIdx = firstRowIdx + nRows - 1;
    arrOfSites = new int* [nRows];

    for (int i = 0; i < nRows; i++) {
        arrOfSites[i] = new int [numOfSites];

        for (int j = 0; j < numOfSites; j++)
            arrOfSites[i][j] = -1;
    }

    //filling arrOfSites for rows in the window
    for (int i = 0; i < circuit.nNodes; i++) {
        rowHeight = circuit.rows[0].height;
        siteWidth = circuit.rows[0].siteWidth;
        rowOrigin = circuit.rows[0].subrowOrigin;
        int rowIdx = floor(circuit.placement[i].yCoord / rowHeight);

        if ((firstRowIdx <= rowIdx) && (rowIdx <= lastRowIdx)) { //this cell may be included in window      
            cellWidthInSites = circuit.nodes[i].width / siteWidth;
            siteIdx = static_cast<int>((circuit.placement[i].xCoord - 0.5 * circuit.nodes[i].width - rowOrigin) / siteWidth);
            ///static_cast<int>((cell.X() - row.X()) / row.SiteWidth());
            rowIdx -= firstRowIdx;

            for (int j = 0; j < cellWidthInSites; ++j) {
                if (arrOfSites[rowIdx][siteIdx+j] != -1)        
                    //printf("cell %d overlaps cell %d \n", cell._id(), arrOfSites[rowIdx][siteIdx+j]);
                    printf("Overlap!\n");

                arrOfSites[rowIdx][siteIdx+j] = i;
            }
        }

        else
            continue;
    }

    int currCellsNum = 0;
    int endedRows = 0;
    int *rowWidthInSites = new int [nRows];
    bool *endOfRow = new bool [nRows];
    cellIndexes = new int [nCells];

    for (int i = 0; i < nRows; i++) {
        rowWidthInSites[i] = 1;
        endOfRow[i] = false;
        int cellInd = arrOfSites[i][siteIndexes[i]];

        if (cellInd != -1) {
            while ((siteIndexes[i] >= 0) && (arrOfSites[i][siteIndexes[i]] == cellInd))
                siteIndexes[i]--;

            siteIndexes[i]++; //in order to avoid catching previous cell
        }
    }

    for (int i = 0; i < nCells; i++)
        cellIndexes[i] = 0;

    //catching cells in window
    while (currCellsNum < nCells) {    
        for (int i = 0; ((i < nRows) && (currCellsNum < nCells)); i++) { //for all rows
            if (endOfRow[i] == true)
                continue;            

            while ((rowWidthInSites[i] + siteIndexes[i] - 1 < numOfSites) && (arrOfSites[i][rowWidthInSites[i] + siteIndexes[i] - 1] == -1)) 
                rowWidthInSites[i]++;

            if (rowWidthInSites[i] >= numOfSites - siteIndexes[i]) { //the end of the row
                endOfRow[i] = true;
                rowWidthInSites[i]--;
                endedRows++;
                continue;
            }

            else { //cell was found
                int cellIdx = arrOfSites[i][rowWidthInSites[i] + siteIndexes[i] - 1];
                cellWidthInSites = circuit.nodes[cellIdx].width / siteWidth;
                rowWidthInSites[i] += cellWidthInSites;

                cellIndexes[currCellsNum++] = cellIdx;

                while ((rowWidthInSites[i] + siteIndexes[i] - 1 < numOfSites) && (arrOfSites[i][rowWidthInSites[i] + siteIndexes[i] - 1] == -1)) 
                    rowWidthInSites[i]++; //move to the next cell
                
                //rowWidthInSites[i]--;
            }
        }

        if (endedRows == nRows)
        {
            printf("only %d cells were found\n", currCellsNum);
            nCells = currCellsNum;
            break;
        }
    }

    if (nCells == 0)
    {
      return false;
    }

    //initialize bounds
    bounds = new double [nRows * 2];

    for (int i = 0; i < nRows; i++)
    {
        siteWidth = circuit.rows[firstRowIdx + i].siteWidth;
        //circuit.rows[0].numSites;
        bounds[2 * i] = siteIndexes[i] * siteWidth;                         // left bound of this row in the window           
        bounds[2 * i + 1] = siteWidth * rowWidthInSites[i] + bounds[2 * i]; //right bound
        //cout<<"!!!!!!: "<<arrOfSites[0][siteIndexes[0] + rowWidthInSites[0]]<<"\n";
    }

    delete [] rowWidthInSites;
    delete [] endOfRow;
    return true;
}

void MakeWindows(Circuit &circuit, int nSteps, int nCells, int nRows)
{    
    double *x_array = NULL;    
    int *siteIndexes = NULL;
    int *cellIndexes = NULL;    
    int firstRowIdx;    

    for (int i = 0; i < nSteps; i++)
    {
        bool isWindowCreated = false;
        int nFoundCells = nCells;
        firstRowIdx = rand() % circuit.nRows;        
        nRows = min(circuit.nRows - firstRowIdx, nRows);
        GetStartPoint(siteIndexes, nRows, circuit);
        isWindowCreated = GetWindowParams(firstRowIdx, nRows, nFoundCells, siteIndexes, cellIndexes, x_array, circuit);

        if (isWindowCreated == false)
          continue;

        bool *nodeFromThisNet = new bool [circuit.nNets];

        for (int l = 0; l < circuit.nNets; l++)
          nodeFromThisNet[l] = false;

        int changedNetsCount = 0;

        for (int netInd = 0; netInd < circuit.nNets; netInd++)        
          for (int j = 0; j < circuit.nets[netInd].numOfPins; j++)
          {
            for (int k = 0; k < nFoundCells; k++) //improve it
            {
              if (cellIndexes[k] == circuit.nets[netInd].arrPins[j].cellIdx)
              {
                nodeFromThisNet[netInd] = true;                
                break;
              }
            }

            if (nodeFromThisNet[netInd] == true)
            {
              changedNetsCount++;
              break;
            }
          }

        int *changedNetsIdx = new int [changedNetsCount];
        int currSize = 0;

        for (int k = 0; k < circuit.nNets; k++)
          if (nodeFromThisNet[k] == true)
            changedNetsIdx[currSize++] = k;

        double currWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
        
        //GeneralWindow *slidingWindow = new GeneralWindow(nRows, firstRowIdx, x_array, nFoundCells, circuit, cellIndexes);
        RectangularWindow *slidingWindow = new RectangularWindow(nRows, firstRowIdx, x_array, nFoundCells, circuit, cellIndexes);
        slidingWindow->PlaceCells(circuit);        
        printf("bounds penalty: %f\n", slidingWindow->g1WindowBounds(circuit));
        printf("overlaps: %f\n", slidingWindow->g2Overlaps(circuit));
        printf("recalc WL: %f\n", slidingWindow->RecalcWl(circuit, currWL, changedNetsIdx, changedNetsCount));
        delete slidingWindow;
        delete [] x_array;
        delete [] siteIndexes;
        delete [] cellIndexes;
    }
}