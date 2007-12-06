/* data_structures.h
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Aleksey Bader, Kirill Kornyakov
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
 */
 
#ifndef _DATA_STRUCTURES_H_
#define _DATA_STRUCTURES_H_

#include <vector>
#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;

//#undef cout

// stores cell's information 
struct Node
{
  int width;      // width of cell
  int height;     // height of cell
};

// stores pin's information
struct Pin
{
  int  cellIdx;    // index of cell
  double xOffset;    // horizontal pin's offset by the center of a cell
  double yOffset;    // vertical pin's offset by the center of a cell
};

// stores net information
struct Net
{
  Pin*   arrPins;     // array of pins
  int  numOfPins;     // number of pins in net
  double currWL;      // current half-perimeter of bounding rectangular
};

// stores information about cell's location
struct Place        
{
  double xCoord;     // x coordinate of a cell
  double yCoord;     // y coordinate of a cell
  char   orient[3];  // orientation of a cell
};

// stores row's information
struct Row          
{ 
  int  coordinate;  // y coordinate of a row
  int  height;      // height of a row
  int  siteWidth;   // defaults value of siteSpacing
  int  siteSpacing; // the distance between the beginnings 
                    // of neighboring sites in subrow
  char   siteorient[2];// 
  char   sitesymm[2];  // 
  double subrowOrigin; // 
  int    numSites;     // number of sites in a row
};

struct Weight
{
  double wts;       // weight of a cell
};

// stores bin's data (size, cluster of cells)
struct Bin        
{
  vector<int> nodes;    // indexes of nodes in this bin
  vector<int> nets;     // indexes of nets  in this bin
  vector<int> cwts;     // array of nodes' weights (widths)
  vector<int> hedges;   // array of nodes in hyperedges
  vector<int> indexes;  // positions in hedges from which hyperedges starts
  double sumLength;     // summary length of nodes in this bin
  double xCoord;        // x and y coordinates of the bin
  double yCoord;        
  bool isOnMacroBlock;
  
  Bin()
  {
    sumLength      = 0;
    isOnMacroBlock = false;
  }
  
  void Clear()
  {
    nodes.clear();
    nets.clear();
    cwts.clear();
    hedges.clear();
    indexes.clear();
    sumLength = 0;
  }
};

typedef Bin* pBin;

struct str
{
  char name[20];      // name of cell
};

struct Circuit
{
  int     nNodes;
  int     nTerminals;
  int     nNets;
  int     nRows;
  int     nPins;

  double  height;
  double  width;
  int     shiftX;
  int     shiftY;

  Row*    rows;
  Weight* weights;
  Node*   nodes;
  Node*   terminals;
  Net*    nets;

  pBin**  arrOfBins;
  int     nBinRows;
  int     nBinCols;
  double  binsHeight;
  double  binsWidth;

  Place*  placement;

  str*    tableOfNames;
  vector<int>* tableOfConnections;
  vector<double>* rowTerminalBorders;
};

// bin's position in the array of bins
struct BinPosition
{
  int binRowIdx;  // index of the bin's row in array of bins
  int binColIdx;  // index of the bin's column in array of bins

  // default constructor, initialize fields with dummy value
  BinPosition()
  {
    binRowIdx = -1;
    binColIdx = -1;
  }
};

#endif
