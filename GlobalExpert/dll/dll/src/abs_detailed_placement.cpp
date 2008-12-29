/*  
* detailed_placement.cpp
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2005, ITLab, Zhivoderov
* email: zhivoderov.a@gmail.com
*/

#include "..\include\abs_detailed_placement.h"
#include "stdlib.h"
#include <algorithm>
#include <conio.h>

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
  //double lowCoord = firstRow * siteheight;

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
    //if( _X < boundX[4] ) // _x < x3
    //  penalty += boundX[4] - _X;

    //if( ((boundX[5] - width) < _X) && (_X < boundX[5]) ) { // x3' - width < _x < x3' + width
    //  if( (_X + width - boundX[5]) > halfWidth)
    //    penalty += boundX[5] - _X;

    //  else
    //    penalty += _X + width - boundX[5];
    //}

    //double bound = boundX[5] + boundX[3] - boundX[2]; // x3' + x2' - x2
    //
    //if( ((bound - width) < _X) && (_X < bound) ) {
    //  if((_X + width - bound) > halfWidth)
    //    penalty += bound - _X;

    //  else
    //    penalty += _X + width - bound;
    //}

    //bound += boundX[1] - boundX[0]; // x3' + x2' - x2 + x1' - x1

    //if( (bound - width) < _X  )
    //  penalty += _X + width - bound;    
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
      double second_x = elements[second_index].XCoord; // - second_width * 2

      if(first_x + width > second_x)
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