/*  
* detailed_placement.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2005, ITLab, Zhivoderov
* email: zhivoderov.a@gmail.com
*/

#ifndef _ABS_DETAILED_PLACEMENT_H_
#define _ABS_DETAILED_PLACEMENT_H_

#include <iostream>
#include "..\include\data_structures.h"
#include "..\include\cost_function.h"

struct Coordinates {
  double xCoord;
  double yCoord;

  Coordinates(): xCoord(0), yCoord(0) {}
};

using namespace DataStructures;

struct WindowElement {
  int cellIndex;
  double XCoord;
};

struct Window {
  int nRows; //number of rows in window
  int firstRow; //index of first row in the window
  double *boundX; //array of bounds
  int elementsNum; //num of elements in window
  WindowElement *elements; //elements in winodow
  

  Window(int rows_num, int first, double x_array[], int elements_num, const Circuit &circuit, int indexes[]);
  double ConvertXY_To_X(double xCoord, double yCoord, double x_array[], const Circuit &circuit);
  Coordinates ConvertX_To_XY(double xExtended, const Circuit &circuit, double x_array[], double elementWidth);
  double g1WindowBounds(const Circuit &circuit);
  double g2Overlaps(const Circuit &circuit);
  double CalcWL(const Circuit &circuit, double x_array[]);
  ~Window() {delete [] boundX; delete [] elements; }
};

#endif