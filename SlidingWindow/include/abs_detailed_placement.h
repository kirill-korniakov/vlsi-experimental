/*  
* abs_detailed_placement.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2008, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef _ABS_DETAILED_PLACEMENT_H_
#define _ABS_DETAILED_PLACEMENT_H_

#include "..\include\data_structures.h"
#include "..\include\cost_function.h"

struct Coordinates
{
    double xCoord;
    double yCoord;

    Coordinates(): xCoord(0), yCoord(0) {}
};

struct WindowElement
{
    int cellIndex;
    double XCoord;
};

struct RectangularWindowElement: public WindowElement
{
  int rowIdx;
  double YCoord;

  double GetY() {return YCoord;}
};

void QuickSortRowElement(WindowElement* a, long N);

struct BasicWindow
{
    int nRows;            //number of rows in window
    int firstRow;         //index of first row in the window    
    int elementsNum;      //num of elements in window
    double *windowBounds; //bounds in original coordinates (x,y)

    BasicWindow(int _nRows, int _first, double x_array[], int _num);
    ~BasicWindow() { delete [] windowBounds; }    
};

struct RectangularWindow: public BasicWindow
{
  RectangularWindowElement *elements;

  RectangularWindow(int _nRows, int _first, double x_array[], int _num, const Circuit &circuit, int indexes[]);
  ~RectangularWindow() { delete [] elements; }

  void PlaceCells(Circuit &circuit);
  double g1WindowBounds(const Circuit &circuit);
  double g2Overlaps(const Circuit &circuit);
  double CalcWL(const Circuit &circuit);
  double RecalcWl(Circuit &circuit, double currWL, const int changedNets[],
                    int changedNetsCount);
};

struct GeneralWindow: public BasicWindow
{    
    double *boundX;          //array of bounds
    WindowElement *elements; //elements in window

    GeneralWindow(int rows_num, int first, double x_array[], int elements_num, const Circuit &circuit, int indexes[]);
    double ConvertXY_To_X(double xCoord, double yCoord, const Circuit &circuit);
    Coordinates ConvertX_To_XY(double xExtended, const Circuit &circuit, double elementWidth);
    double g1WindowBounds(const Circuit &circuit);
    double g2Overlaps(const Circuit &circuit);
    double CalcWL(const Circuit &circuit);
    double RecalcWl(Circuit &circuit, double currWL, const int changedNets[],
                    int changedNetsCount);

    void PlaceCells(Circuit &circuit);
    ~GeneralWindow() {delete [] boundX; delete [] elements; }

};

struct WindowWithOrderedCells: public GeneralWindow
{
    double *spaces; //spaces between cells
    
    WindowWithOrderedCells(int rows_num, int first, double x_array[], int elements_num, 
                                                const Circuit &circuit, int indexes[]);
    void GetCoords(const Circuit &circuit); //get extended coords of cells from spaces
    double CalcBoundsPenalty(const Circuit &circuit); //similar to g1WindowBounds
    double CalcWireLenght(const Circuit &circuit); //similar to CalcWL
    double CalcFreeSpace(const Circuit &circuit);
    ~WindowWithOrderedCells() { delete [] spaces; }
};

void GetStartPoint(int *&siteIndexes, int nRows, const Circuit &circuit);
bool GetWindowParams(int firstRowIdx, int nRows, int &nCells, int siteIndexes[],
                     int *&cellIndexes, double *&bounds, const Circuit &circuit);
void MakeWindows(Circuit &circuit, int nSteps, int nCells, int nRows);

#endif