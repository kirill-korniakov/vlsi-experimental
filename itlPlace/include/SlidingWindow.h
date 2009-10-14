/*  
* SlidingWindow.h
* this is a part of itlAnalyticalPlacer
* Copyright (C) 2008, ITLab, Belyakov
* email: alexander.belyakov@mail.itlab.unn.ru
*/

#ifndef _SLIDING_WINDOW_H_
#define _SLIDING_WINDOW_H_

#include "HDPGrid.h"
#include "Utils.h"

#define sw_eps 0.01
#define numOfCells 5
#define numOfSteps 5
#define numOfRows  3

struct Coordinates
{
  double xCoord;
  double yCoord;

  Coordinates(): xCoord(0), yCoord(0) {}
  Coordinates(double _x, double _y): xCoord(_x), yCoord(_y) {}
};

struct WindowElement
{
  HCell cell;
  double XCoord;
};

struct RectangularWindowElement: public WindowElement
{
  int rowIdx;
  double width;
  double YCoord;

  double GetY() {return YCoord; }
};

void QuickSortRowElement(WindowElement* a, long N);

//class BasicWindow:
//contains basic information for all windows
class BasicWindow
{
protected:
  int nRows;            //number of rows in window
  int firstRow;         //index of first row in the window  
  int nCells;           //num of cells in window
  double *windowBounds; //bounds in original coordinates (x,y)
  HDPGrid &m_grid;    

public:
  int GetNumOfRows() { return nRows; }
  int GetCellsNum() { return nCells; }
  BasicWindow(int _nRows, int _first, double x_array[], int _num, HDPGrid &_grid);
  virtual ~BasicWindow() 
  {
    delete [] windowBounds; 
  }
  virtual void PlaceCells()              = 0;
  virtual double CalcWL()                = 0;
  virtual double RecalcWL(double currWL) = 0;
  //virtual void ChangeCoords(const double newCoords[]) = 0;
};

//---------------------Rectangular window----------------------------------------------------

class RectangularWindow: public BasicWindow
{
protected:
  RectangularWindowElement *cells;
  HNet *windowNets;
  int nNets;

public:
  RectangularWindow(int _nRows, int _first, double x_array[], int _num,
    HCell capturedCells[], HDPGrid &_grid);
  ~RectangularWindow();
  void PlaceCells();  
  double g1WindowBounds();
  double g2Overlaps();
  double CalcWL();
  double RecalcWL(double currWL);
};

//------------------Window with not ordered cells-------------------------------------------

class WindowWithNotOrderedCells: public BasicWindow
{  
protected:
  double *boundX;          //array of bounds
  WindowElement *cells;    //cells captured by this window

public:
  WindowWithNotOrderedCells(int rows_num, int first, double x_array[], int cells_num,
    HCell capturedCells[], HDPGrid &_grid);
  double ConvertXY_To_X(HCell cell);
  Coordinates ConvertX_To_XY(double xExtended);
  double g1WindowBounds();
  double g2Overlaps();
  double CalcWL();
  double RecalcWL(double currWL);
  void PlaceCells();
  void ChangeCoords(const double newCoords[]);
  double GetMaxCoord() { return boundX[nRows]; }
  double GetCellCoord(int ind) { return cells[ind].XCoord; }
  virtual ~WindowWithNotOrderedCells() 
  { 
    delete [] boundX; 
    delete [] cells; 
  }
};

//-------Window with ordered cells----------------------------------------------------------

class WindowWithOrderedCells: public WindowWithNotOrderedCells
{
protected:    
  double *spaces; //spaces between cells

public:
  WindowWithOrderedCells(int rows_num, int first, double x_array[], int cells_num,
    HCell capturedCells[], HDPGrid &_grid);
  void GetCoords();        //get extended coordinates of cells from spaces
  double g1WindowBounds();
  double CalcWL();
  double CalcFreeSpace();
  void ChangeCoords(const double newCoords[]);
  void PlaceCells();
  ~WindowWithOrderedCells() { delete [] spaces; }
};

//------functions for configuring sliding windows----------------------------------------------

//void GetStartPoint(int *&siteIndexes, int nRows, const HDPGrid &_grid);
void GetStartPoint(int siteIndexes[], int nRows, const HDPGrid &_grid);
/*bool GetWindowParams(int firstRowIdx, int nRows, int &nCells, HCell *&cells, int siteIndexes[],
double *&bounds, HDPGrid &_grid);*/
bool GetWindowParams(int firstRowIdx, int nRows, int &nCells, HCell cells[], int siteIndexes[],
                     double bounds[], HDPGrid &_grid);
void MakeWindows(int nSteps, int nCells, int nRows, HDPGrid &_grid);

#endif