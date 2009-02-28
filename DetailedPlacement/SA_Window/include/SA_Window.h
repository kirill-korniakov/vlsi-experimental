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
    double T;
    double currWL;
    int *windowNets;
    int nNets;    

public:
    SAWindow(int _nRows, int _first, double x_array[], int _num, const Circuit &circuit, int indexes[],
        double _wl, int *changedNets, int changedNetsCount);
    ~SAWindow() {}
    void ReduceTemperature() { T *= KOEF; }
    int GetRandomCell();    
    void ShiftCell(int cellInd, Circuit &circuit);
    void InsertCell();
    void ExchangeTwoCells();
    void SearchOnSortedCells(bool trial, Circuit &circuit);
    void GeneralSearch();
    double GetFuncValue(Circuit &circuit);
};

void MakeWindows(Circuit &circuit, int nSteps, int nCells, int nRows);

#endif