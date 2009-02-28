/*  
* SA_Window.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2009, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef _SA_WINDOW_H
#define _SA_WINDOW_H

#include "abs_detailed_placement.h"

#define MAX_T 100;
#define MIN_T 1
#define KOEF 0.999;

class SAWindow: public RectangularWindow
{
  protected:
    int T;

  public:
    SAWindow(int _nRows, int _first, double x_array[], int _num, const Circuit &circuit,
             int indexes[]);
    ~SAWindow() {}
    void ReduceTemperature() { T *= KOEF; }
    int GetRandomCell();
    double GetFuncValue(Circuit &circuit);
    void ShiftCell(int cellInd, Circuit &circuit);
    void InsertCell();
    void ExchangeTwoCells();
    void SearchOnSortedCells(bool trial, Circuit &circuit);
    void GeneralSearch();
};

void MakeWindows(Circuit &circuit, int nSteps, int nCells, int nRows);

#endif