/*  
 * detailed_placement.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, ITLab, Zhivoderov
 * email: zhivoderov.a@gmail.com
 */

#ifndef _DETAILED_PLACEMENT_H_
#define _DETAILED_PLACEMENT_H_

#include <iostream>
#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\check_legality.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\output.h"
#include "..\include\global.h"
#include <time.h>    //for timing
#include "math.h"
//#include "mkl.h"
//#include <windows.h>

//#define _OPT_REG_INFO_
//#define FULL_WL_NOTATION


struct RowElement
{
  double xCoord;
  int  cellIdx;
};

struct Coordinates {
	double x;
	double y;
};

struct CellsInCluster {
	int curnum;
	RowElement *cells_i_cl; //cells in this cluster
	double weight; // == total weight of all cells in this cluster
	double width; // == total width
	double q;
	double x;

	CellsInCluster(): weight(0), width(0), q(0), curnum(0), x(0) { cells_i_cl = NULL; }
	void create_array(int _num) {cells_i_cl = new RowElement [_num]; }
	void free_cluster();
	~CellsInCluster();
	void AddCellToCluster(const Circuit &circuit, const RowElement &_element, bool recalc);
	//void CopyCell(RowElement &_element);
	//void AddCluster(CellsInCluster &cluster);
};
//------------------------------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------------
struct CellsInRow {
	int cellsnum;
	//int curnum;
	std::list<RowElement> cells_ir; // cells in row
	CellsInCluster *clusters; //array of clusters - needed in PlaceRow()
	int clustersnum;

	CellsInRow() { clustersnum = 0; cellsnum = 0; }
	~CellsInRow() { cells_ir.clear(); cellsnum = 0; }
	void AddCellToRow(Circuit &circuit, int cellind);
	void AddCellToRow(const Circuit &circuit, const RowElement &_element);

	void AddCluster(const Circuit &circuit, int first, int second);
	void RemoveLastCluster();
	void Collapse(const Circuit &circuit, int ClInd);
	bool PlaceRow(const Circuit &circuit, Coordinates coordinates[]);
};

//--------------------------------------------------------------------------------------------------------------------------------------------

//-+----------------------------------------------------------------------------------------------------------------------------------------

namespace DetailedPlacementNS
{
  struct Cluster
  {
    int leftX;
    double width;
    vector<int> cellIdxs;
    vector<double> boundsList;
  };
  typedef Cluster* pCluster;
}
enum HorSearchDir{ LEFT, RIGHT, RANDOM };

MULTIPLACER_ERROR DetailedPlacement(Circuit& circuit, Statistics& statistics);

MULTIPLACER_ERROR Legalize(Circuit& circuit);
MULTIPLACER_ERROR Abacus_Legalize(Circuit& circuit);

double HorizontalSearch(HorSearchDir direction, RowElement** greedy_array, int* numOfCellsInRow, int row, 
        int number, Circuit& circuit, Statistics& statistics, int** arrOfSites);

double VerticalSearch(int currCellIdx, int* numOfCellsInRow, int** arrOfSites, Circuit& circuit, Statistics& statistics);

double GlobalSwap(int currCellIdx, int* numOfCellsInRow, int** arrOfSites,
        Circuit& circuit, Statistics& statistics);

void QuickSortRowElement(RowElement* a, long N);

void InsertSortRowElement(RowElement* a, long DataCount);

void NextPermutation(unsigned int x[], unsigned int len);

int  Factorial(unsigned int a);

#endif