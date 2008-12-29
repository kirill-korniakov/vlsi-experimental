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

void QuickSortRowElement(WindowElement* a, long N)
{
  long i = 0, j = N;
  WindowElement temp, p;
  int rand0 = N / 2;
  //GetIntegerRandomNumbers( &rand0, 1, 0, N);

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

Window::Window(int rows_num, int first, double x_array[], int elements_num, const Circuit &circuit, int indexes[])
{
  nRows = rows_num;
  firstRow = first;
  elementsNum = elements_num;
  elements = new WindowElement [elementsNum];
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
		//cout<<"width: "<<circuit.nodes[index].width<<endl;
    double _y = circuit.placement[index].yCoord - 0.5 * circuit.rows[0].height;
		//cout<<"height: "<<circuit.rows[0].height<<endl;
    elements[i].XCoord = ConvertXY_To_X(_x, _y, x_array, circuit);
  }
}


double Window::ConvertXY_To_X(double xCoord, double yCoord, double x_array[], const Circuit &circuit) {
  double siteheight = circuit.rows[0].height;  

  for (int i = 0; i < nRows; i++)
  {
    int rowInd = nRows - i - 1 + firstRow;

    if (yCoord == rowInd * siteheight) 
      return (xCoord + boundX[i] - x_array[(nRows - i - 1) * 2]);    
  }

  cout<<"error in ConvertXY_To_X!!! yCoord = "<<yCoord<<endl;
  system("PAYSE");
  return -1;  
}

Coordinates Window::ConvertX_To_XY(double xExtended, const Circuit &circuit, double x_array[], double cellWidth)
{
  Coordinates coordinates;
  double siteheight = circuit.rows[0].height;
  double halfWidth = cellWidth * 0.5;

  for (int i = 0; i < nRows; i++)
  {
    if ((boundX[i] <= xExtended) && (xExtended < boundX[i + 1])) //cell is in row (nRows - i - 1 + firstRow)
    {
      int rowInd = nRows - i - 1 + firstRow;
      coordinates.xCoord = xExtended - boundX[i] + x_array[(nRows - i - 1) * 2];
      coordinates.yCoord = siteheight * rowInd;
    }
  }

  if (xExtended >= boundX[nRows]) //out of the window
  {
    coordinates.yCoord = siteheight * firstRow;
    coordinates.xCoord = xExtended - boundX[nRows - 1] + x_array[0];
  }

  if (xExtended < boundX[0])
  {
    coordinates.yCoord = siteheight * (firstRow + nRows - 1);
    coordinates.xCoord = xExtended + x_array[(nRows - 1) * 2];
  }   

  return coordinates;
}

double Window::g1WindowBounds(const Circuit &circuit)
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
			cout<<"windowBounds: index="<<index<<endl<<circuit.placement[index].xCoord<<endl<<_XCoord<<endl;
			//cout<<" "<<boundX[4]<<endl;
			cout<<elements[9].XCoord<<endl;
      continue;
    }

    for (int j = 1; j < nRows + 1; j++)
    {
      if (((boundX[j] - width) < _XCoord) && (_XCoord < boundX[j])) {
				if ((_XCoord + width - boundX[j]) > halfWidth) {
          penalty += boundX[j] - _XCoord;
					//cout<<"windowBounds: index="<<index<<endl;
				}

				else {
          penalty += _XCoord + width - boundX[j];
					//cout<<"windowBounds: index="<<index<<endl;
				}
				break;
			}
    }     
  }

  return penalty;
}

double Window::g2Overlaps(const Circuit &circuit)
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

double Window::CalcWL(const Circuit &circuit, double x_array[])
{
  for (int i = 0; i < elementsNum; i++)
  {
    int index = elements[i].cellIndex;
    double element_width = circuit.nodes[index].width;
    Coordinates coordinates = ConvertX_To_XY(elements[i].XCoord, circuit, x_array, element_width);
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
//----------------------------------------------------------------------------------------------------------

WindowWithOrderedCells::WindowWithOrderedCells(int rows_num, int first, double x_array[],
                                               int elements_num, const Circuit &circuit,
                                               int indexes[]):
                                               Window(rows_num, first, x_array, 
                                               elements_num, circuit, indexes)
{
  spaces = new double [elements_num];
  QuickSortRowElement(elements, elements_num - 1);

  /*for(int i = 0; i < elementsNum; i++)
        cout<<"X["<<i<<"]= "<<elements[i].XCoord<<"; ";*/

  spaces[0] = elements[0].XCoord;
  
  for (int i = 1; i < elements_num; i++)
  {
      int index = elements[i - 1].cellIndex;  
      spaces[i] = elements[i].XCoord - circuit.nodes[index].width - elements[i - 1].XCoord;
  }

  /*cout<<endl;
  for(int i = 0; i < elementsNum; i++)
        cout<<"X["<<i<<"]= "<<spaces[i]<<"; ";*/
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

double WindowWithOrderedCells::CalcWireLenght(const Circuit &circuit, double x_array[])
{
    GetCoords(circuit);
    double res = CalcWL(circuit, x_array);
    return res;
}

double WindowWithOrderedCells::CalcFreeSpace(const DataStructures::Circuit &circuit)
{
    double totalWidth = 0;

    for(int i = 0; i < elementsNum; i++)
      totalWidth += circuit.nodes[elements[i].cellIndex].width;

    //cout<<"totalwidth="<<totalWidth<<endl<<boundX[nRows] - totalWidth<<endl;
    return (boundX[nRows] - totalWidth);
}