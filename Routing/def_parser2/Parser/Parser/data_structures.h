/* data_structures.h
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Aleksey Bader, Kirill Kornyakov
 * email: bader@newmail.ru
 * email: kirillkornyakov@yandex.ru
 */
 
#ifndef _DATA_STRUCTURES_H_
#define _DATA_STRUCTURES_H_

#pragma warning (disable : 4996)

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <list>

using namespace std;

//------- LEF parser structures -------------------------------------
struct PinInfo
{
    enum direction {INPUT = 0, OUTPUT = 1, INOUT = 2};
    std::string Name;
    bool isSpecial;
    direction dir;
    double Resistance;  // OHMS
    double Capacitance; // pF
    double OriginX;     // nm
    double OriginY;     // nm
};

struct MacroInfo
{
    std::string Name;
    double OriginX;   //nm
    double OriginY;   //nm
    double SizeX;     //nm
    double SizeY;     //nm
    std::map<std::string,PinInfo*> Pins;
};

struct RoutingLayerInfo
{
    enum direction {horizontal = 0, vertical = 1, diag45 = 2, diag135=3};

    std::string Name;
    direction dir;
    double RPerDist; // OHMS/nm
    double CPerDist; // pF/nm
    double EdgeCap;  // pF/nm
    double Width;    // nm
    double Pitch;    // nm
};

struct DataBaseUnitsMapping
{
    double Distance;    // nm/DBU
    double Capacitance; // PICOFARADS/DBU
    double Resistance;  // OHMS/DBU

    DataBaseUnitsMapping()
    {
        Distance = (1.0 / 100.0) * 1000.0;
        Capacitance = 1.0 / 1.0e6;
        Resistance = 1.0 / 1.0e4;
    }
};

struct LEFParserData
{
    DataBaseUnitsMapping* gUnits;
    std::list<RoutingLayerInfo*> Layers;
    std::map<std::string,MacroInfo*>* CellTypes;
    MacroInfo* CurrentMacro;
    MacroInfo* SinglePins;
};
//-------End of LEF parser structures -------------------------------

#include <windows.h>
#include <time.h>
// stores cell's information 
struct Node
{
  int width;      // width of cell
  int height;     // height of cell
  MacroInfo *type;
};

struct STAINER_ELMORE_NODE;
// stores pin's information
struct Pin
{
  int      cellIdx;    // index of cell
  double   xOffset;    // horizontal pin's offset by the center of a cell
  double   yOffset;    // vertical pin's offset by the center of a cell
  char     chtype;     // the type of the pin (I, O or B)
  PinInfo* type;
  STAINER_ELMORE_NODE* routeInfo;
};

//------- Tree Node Structure (FBI data struct)----------------------
typedef struct STAINER_TREE_NODE 
{
  //int index;      // index of the node
  double x;         // x coordinate
  double y;         // y coordinate
  int    type;      // 0 source node, 1 internal node, 2 sink node
  double cap;       // node capacitance
  double time;      // required arrival time at the node
  struct STAINER_TREE_NODE *left;
  struct STAINER_TREE_NODE *right;
}StNode;

//FBI node types
#define NODE_TYPE_INPUT    2
#define NODE_TYPE_SINK     2
#define NODE_TYPE_OUTPUT   0
#define NODE_TYPE_SOURCE   0
#define NODE_TYPE_INTERNAL 1

typedef struct STAINER_ELMORE_NODE: public StNode
{
    Pin* origin;        //pin associated with this routing point
    double ExtractedR;  //resistance of input edge
    double ExtractedC;  //capacitance of input edge
    double ObservedC;   //fanout
    double ArrivalTime; //time from source to this routing point
}StNodeEx;

typedef struct STAINER_ROUTING_TREE
{
    double wl;        // wire length
    double srcRes;    // driver resistance
    StNodeEx *nodes;  // A-tree
    int RoutingType;  // tree type
} RoutingTree;

//Routing types (flags)
#define UNROUTED_TREE 0
#define FLUTE_TREE    1
#define HPWL_STUB     2
#define ROUTED_TREE   4

#define A_TREE (FLUTE_TREE | ROUTED_TREE)

//------- Tree Node Structure End -----------------------------------

//special source indexes
#define UKNOWN_INDEX (-1)
#define NO_SOURCE    (-2)

#define HASREALSOURCE(net) (net.sourceIdx >= 0)
// stores net information
struct Net
{
  Pin*   arrPins;     // array of pins
  int  numOfPins;     // number of pins in net
  double currWL;      // current half-perimeter of bounding rectangular
  RoutingTree* tree;  // routing tree (rectilinear A-tree)
  char* name;         // individual net name

  Net* arrivalOrder;
  Net* requiredOrder;
  int sourceIdx;

  Net(): arrPins(0), numOfPins(0),currWL(0),tree(0), name(0), 
      arrivalOrder(0), requiredOrder(0), sourceIdx(UKNOWN_INDEX) {}
};

// stores information about cell's location
struct Place
{
  double xCoord;     // x coordinate of a cell
  double yCoord;     // y coordinate of a cell
  char   orient[3];  // orientation of a cell

  double requiredTime;
  double arrivalTime;
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
  std::vector<int> nodes;    // indexes of nodes in this bin
  std::vector<int> nets;     // indexes of nets  in this bin
  std::vector<int> cwts;     // array of nodes' weights (widths)
  std::vector<int> hedges;   // array of nodes in hyperedges
  std::vector<int> indexes;  // positions in hedges from which hyperedges starts
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
  char* name;      // name of cell

  str() {name = new char[20];name[0]=0;}
  str(const char* _name) {name = new char[strlen(_name)+1]; strcpy(name, _name);}
  ~str(){delete[]name;}
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
  double     shiftX; //!was int
  double     shiftY; //!was int

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

  int nPrimaryInputs;
  int nPrimaryOutputs;
  int* primaryInputs;//indexes of inputs
  int* primaryOutputs;//indexes of outputs
  Net* firstArrival;
  Net* firstRequired;

  str*    tableOfNames;
  std::vector<int>* tableOfConnections;
  std::vector<double>* rowTerminalBorders;

  LEFParserData* tech;
  str designName;
  int defConvertFactor;
  RoutingLayerInfo* hLayer;
  RoutingLayerInfo* vLayer;

  Circuit() {tech = 0; hLayer=vLayer=0;}
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
