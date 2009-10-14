/*  
* SAWindow.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef _SA_WINDOW_H_
#define _SA_WINDOW_H_

#include "SlidingWindow.h"
#include "Legalization.h"

#define MAX_T 70000
#define MIN_T 0.001
#define KOEF  0.99

class SAWindow: public RectangularWindow
{
protected:
  double T;
  double currWL;
  RectangularWindowElement *optimalCoords;
  double currentMin;

public:
  SAWindow(int _nRows, int _first, double x_array[], int _num, HCell capturedCells[], double WL, HDPGrid &_grid);
  ~SAWindow() {delete [] optimalCoords; }
  void ReduceTemperature() { T *= KOEF; }
  int GetRandomCell();
  void ShiftCell(const int cellInd);
  void InsertCell();
  void ExchangeTwoCells();
  void ChangeCellsOrder();
  void SearchOnSortedCells(bool trial);
  void GeneralSearch();
  double GetFuncValue();
  void UpdateCellOptimalCoords();
  bool LegalizeCells();
};

void MakeSAWindows(int nSteps, int nCells, int nRows, HDPGrid &_grid);

#endif