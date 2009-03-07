/* data_structures.h
* this file is a part of itlAnaliticalPlacer
* Copyright (C) 2005-2008, ITLab, Aleksey Bader, Kirill Kornyakov, Andrey Kamaev
* email: bader@newmail.ru
* email: kirillkornyakov@yandex.ru
* email: akamit@rambler.ru
*/

#ifndef _DATA_STRUCTURES_H_
#define _DATA_STRUCTURES_H_

#define BUF_COUNT_RESERVE (10000)

#pragma warning (disable : 4996)

#include <windows.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <list>

using namespace std;

namespace DataStructures
{
  const int DEFAULT_NAME_LENGTH = 20;

  /* -------------------------------------------------------------------
  * ------- LEF parser structures -------------------------------------
  * -------------------------------------------------------------------*/

  /**  PinInfo struct.
  * This struct contains detailed pin description.
  * Pin belongs to one of macro type.
  * Used in technology information tree.
  */
  struct PinInfo
  {
    /// Pin direction type enumeration.
    enum direction 
    {
      INPUT = 0,  /// means that pin is macro input
      OUTPUT = 1, /// means that pin is macro output
      INOUT = 2   /// indicate that pin is bidirectional
    };

    std::string Name;   /// symbolic pin name
    bool isSpecial;     /// true if this pin has special use (power, grownd, clock, etc)
    direction dir;      /// pin direction
    double Resistance;  /// driver resistance, for output pins (OHMS)
    double Capacitance; /// pin capacitance (pF)
    double OriginX;     /// pin x offset (from bottom left corner) (nm)
    double OriginY;     /// pin y offset (from bottom left corner) (nm)

    PinInfo()
    {
      isSpecial = false;  dir = INOUT;
      Resistance = 0.0;   Capacitance = 0.0;
      OriginX = 0.0;      OriginY = 0.0;
    }
  };

  /**  MacroInfo struct.
  * This struct contains detailed macro element description.
  * Used in technology information tree.
  */
  struct MacroInfo
  {
    /// Defines special macro function
    enum MacroType
    {
      Schematic, // no special function
      FlipFlop,  // any flip-flop
      Latch,     // any latch
      Buffer,    // any buffer
      Inverter,  // any inverter
      Primary    // Unable to distinct inputs and outputs on this level
    };

    std::string Name; /// symbolic name of macro type
    MacroType   Type; /// special function of these macros'

    // Origin specifies how to find the origin of the macro to align with a placement 
    //point. For example, if the ORIGIN is 0, -1, then macro geometry at 0, 1 are 
    //shifted to 0, 0, and then aligned to the placement point.
    //Provided only for information.
    double OriginX;   /// x origin (nm)
    double OriginY;   /// y origin (nm)

    // Size specifies a placement bounding rectangle for the macro.
    double SizeX;     /// x size component (nm)
    double SizeY;     /// y size component (nm)

    /// Name-value dictionary of macro's pin types
    std::map<std::string, PinInfo*> Pins;

    MacroInfo()
    {
      OriginX = 0.0;  OriginY = 0.0;
      SizeX = 0.0;    SizeY = 0.0;
      Type = Schematic;
    }
  };

  /**  RoutingLayerInfo struct.
  * This struct contains detailed description for one routing layer.
  * Used in technology information.
  */
  struct RoutingLayerInfo
  {
    /// The preferred routing direction types enumeration.
    enum direction 
    {
      horizontal = 0, /// Routing parallel to the x axis is preferred.
      vertical   = 1, /// Routing parallel to the y axis is preferred.
      diag45     = 2, /// Routing along a 45-degree angle is preferred.
      diag135    = 3  /// Routing along a 135-degree angle is preferred.

      //Info: Angles are measured counterclockwise from the positive x axis.
    };

    std::string Name; /// Symbolic layer name.
    direction dir;    /// Preferred routing direction.
    double RPerDist;  /// Specifies the resistance for a unit of wire. (OHMS/nm)
    double CPerDist;  /// Specifies the capacitance for each unit length. (pF/nm)
    double EdgeCap;   /// Specifies value of peripheral capacitance for a unit length. (pF/nm)
    double Width;     /// Specifies the wire width for the layer. (nm)
    double Pitch;     /// Specifies the required routing pitch for the layer. (nm)

    RoutingLayerInfo()
    {
      RPerDist = 0.0;  CPerDist = 0.0;
      EdgeCap = 0.0;   Width =0.0;
      Pitch = 0.0;
      dir = horizontal;
    }
  };

  /**  DataBaseUnitsMapping struct.
  * This struct contains information about LEF database units.
  * For internal use in LEF parser only.
  */
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

  /**  LEFParserData struct.
  * This struct contains technology information from LEF file.
  * This struct is generated by LEF parser.
  */
  struct LEFParserData
  {
    DataBaseUnitsMapping* gUnits;
    std::list<RoutingLayerInfo*> Layers;
    std::map<std::string,MacroInfo*>* CellTypes;
    MacroInfo* CurrentMacro;
    MacroInfo* SinglePins;

    LEFParserData()
    {
      gUnits = 0;
      CellTypes = 0;
      CurrentMacro = 0;
      SinglePins = 0;
    }
  };
  /* ------- End of LEF parser structures ------------------------------*/

  /* -------------------------------------------------------------------
  * ------- Routing data structures (based on FBI data struct) --------
  * -------------------------------------------------------------------*/
  //FBI node types
#define NODE_TYPE_INPUT    2
#define NODE_TYPE_SINK     2
#define NODE_TYPE_OUTPUT   0
#define NODE_TYPE_SOURCE   0
#define NODE_TYPE_INTERNAL 1

  /**  StNode struct.
  * Base FBI tree node structure.
  * This structure represent node of routing A-Tree.
  *
  * Do not modify.
  * Do not use outside of FBI code.
  */
  typedef struct STAINER_TREE_NODE 
  {
    //int index;      /// index of the node
    double x;         /// x coordinate
    double y;         /// y coordinate
    int    type;      /// 0 source node, 1 internal node, 2 sink node
    double cap;       /// Node capacitance
    double time;      /// Required arrival time at the node
    struct STAINER_TREE_NODE *left;
    struct STAINER_TREE_NODE *right;
  }StNode;

  struct Pin;

  /**  StNodeEx struct.
  * This is extension of FBI routing node.
  * This structure represent node of routing A-Tree.
  *
  * FEATURE: use CreateNodeEx and ReleaseNodeEx instead of
  * new/delete operators for this structures.
  */
  typedef struct STAINER_ELMORE_NODE: public StNode
  {
    Pin* origin;        /// Pin associated with this routing point.
    double ExtractedR;  /// Resistance of input edge. (Calculated during RC extraction)
    double ExtractedC;  /// Capacitance of input edge. (Calculated during RC extraction)
    double ObservedC;   /// Fanout (Calculated during delay calculation)
    double NetPathDelay; /// Time from source to this routing point. (Result of delay calculation)
  }StNodeEx;

  //Routing types (flags)
#define UNROUTED_TREE 0
  /// Tree isn't routed yet.
#define FLUTE_TREE    1
  /// A-Tree generated by FLUTE utility.
#define HPWL_STUB     2
  /// Substitute for tree when used HPWL approximation.
#define ROUTED_TREE   4
  /// A-Tree generated by external router tool.
#define A_TREE (FLUTE_TREE | ROUTED_TREE)
  /// Tree correnspond to A-Tree definition.

  /**  RoutingTree struct.
  * This struct is container for routing topology information.
  * Generated by one of routing algorithms.
  */
  typedef struct STAINER_ROUTING_TREE
  {
    double wl;        /// Wire length.
    double srcRes;    /// Driver resistance.
    StNodeEx *nodes;  /// A-tree.
    int RoutingType;  /// Tree type.

    STAINER_ROUTING_TREE()
    {
      wl = 0.0; srcRes = 0.0;
      nodes = 0;
      RoutingType = UNROUTED_TREE;
    }
  } RoutingTree;
  /* ------- End of routing data structures ----------------------------*/

  /**  Node struct.
  * Stores cell's information.
  * Cell means the same as macro in LEF structures.
  */
  struct Node
  {
    // common fields
    int width;       /// Width of cell
    int height;      /// Height of cell

    // fields come from lef/def format
    MacroInfo *type; /// Pointer to technology macro type information.

    Node()
    {
      width = 0; height = 0;
      type = 0;
    }
  };

  /**  Pin struct.
  * Stores pin's information.
  * This structure is linked to net. There are no easy way to obtain pins for cell.
  */
  struct Pin
  {
    // common fields
    int      cellIdx;    /// Index of cell.
    /// WARNING: Index of cell takes values from 0 to (nNets+nTerminals)
    double   xOffset;    /// Horizontal pin's offset by the center of a cell.
    double   yOffset;    /// Vertical pin's offset by the center of a cell.
    char     chtype;     /// The type of the pin (I, O or B).
    /// 'I' == INPUT
    /// 'O' == OUTPUT
    /// 'B' == INOUT

    // fields come from lef/def format
    PinInfo* type;       /// Poiner to technology pin type information.
    StNodeEx* routeInfo; /// Pointer to routing point associated with this pin.
    /// WARNING: This field avaible only if parent net is routed.

    Pin()
    {
      cellIdx = 0;
      xOffset = 0.0; yOffset = 0.0;
      chtype = 'I';
      type = 0;      routeInfo = 0;
    }
  };

  /// Macro definitions of Net source special indexes.
#define UKNOWN_INDEX    (-1)
#define NO_SOURCE       (-2)
#define MULTIPLE_SOURCE (-3)

  /// Determine wether net has output pin or not.
  /// Returns true if Net has exactly one output pin (=source).
#define HASREALSOURCE(net) (net.sourceIdx >= 0)

  /// Determine wether net has many output pins (sources)
#define HASMULTIPLESOURCE(net) (net.sourceIdx == MULTIPLE_SOURCE)

  /**  Net struct.
  * Stores net's information.
  */
  struct Net
  {
    // common fields
    Pin*   arrPins;     /// Array of pins
    int  numOfPins;     /// Number of pins in net
    double currWL;      /// Current half-perimeter of bounding rectangular

    // fields come from lef/def format
    char* name;         /// Symbolic net name
    int sourceIdx;      /// Index of source pin. (NO_SOURCE value if net contains only inputs)
    Net* arrivalOrder;  /// Pointer to next net in arrival time calculation
    Net* requiredOrder; /// Pointer to next net in required time calculation
    RoutingTree* tree;  /// Routing tree (rectilinear A-tree)

    Net()
    {
      arrPins = 0,      tree =0,        name = 0;
      arrivalOrder = 0, requiredOrder = 0;
      numOfPins = 0,    sourceIdx = UKNOWN_INDEX;
      currWL = 0.0;
    }
  };

  /**  Place struct.
  * Stores information about cell's location.
  */
  struct Place
  {
    // common fields
    double xCoord;       /// x coordinate of a cell
    double yCoord;       /// y coordinate of a cell
    char   orient[3];    /// Orientation of a cell
    /// can be: "N", "W", "S", "E", "FN", "FW", "FS", "FE".

    // fields come from lef/def format
    double requiredTime; /// Required time for node.
    double arrivalTime;  /// Arrival time for node.

    Place()
    {
      xCoord = 0.0;       yCoord = 0.0;
      orient[0] = 'N',    orient[1] = 0,     orient[2] = 0;
      requiredTime = 0.0; arrivalTime = 0.0;
    }
  };

  /**  Row struct.
  * Stores row's information.
  */
  struct Row
  { 
    int  coordinate;     /// y coordinate of a row.
    int  height;         /// Height of a row.
    int  siteWidth;      /// Defaults value of siteSpacing.
    int  siteSpacing;    /// The distance between the beginnings
    ///of neighboring sites in subrow.
    char   siteorient[3];/// 
    char   sitesymm[3];  /// 
    double subrowOrigin; /// 
    int    numSites;     /// Number of sites in a row

    Row()
    {
      coordinate = 0;     height = 0;
      siteWidth = 0;      siteSpacing = 0;
      siteorient[0] = 0;  siteorient[2] = 0;
      sitesymm[0] = 0;    sitesymm[2] = 0;
      numSites = 0;
      subrowOrigin = 0.0;
    }
  };

  /**  Weight struct.
  * Stores information about cell weight.
  */
  struct Weight
  {
    double wts;       /// Weight of a cell.

    Weight()
    {
      wts = 1.0;
    }
  };

  /**  Bin struct.
  * Stores bin's data (size, cluster of cells).
  */
  typedef struct Bin
  {
    std::vector<int> nodes;    /// Indexes of nodes in this bin.
    std::vector<int> nets;     /// Indexes of nets  in this bin.
    std::vector<int> cwts;     /// Array of nodes' weights (widths).
    std::vector<int> hedges;   /// Array of nodes in hyperedges.
    std::vector<int> indexes;  /// Positions in hedges from which hyperedges starts.
    double sumLength;          /// Summary length of nodes in this bin.
    double xCoord;             /// x coordinates of the bin.
    double yCoord;             /// y coordinates of the bin.
    bool isOnMacroBlock;       ///
    double sumPotential;

    Bin()
    {
      sumLength      = 0;
      isOnMacroBlock = false;
      xCoord = 0.0; yCoord = 0.0;
      sumPotential = 0.0;
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
  } *pBin;

  /**  str struct.
  * Store name value. Used for cell names.
  * WARNING: default string length is 20 characters.
  */
  struct str
  {
    char* name;      /// Name of cell.

    str() {name = new char[DEFAULT_NAME_LENGTH];name[0]=0;}
    str(const char* _name) {name = new char[strlen(_name)+1]; strcpy(name, _name);}
    ~str(){delete[]name;}

    void operator=(const char* right)
    {
      if (strlen(right) < DEFAULT_NAME_LENGTH)
      {
        strcpy(name, right);
      }
      else
      {
        delete[]name;
        name = new char[strlen(right)+1];
        strcpy(name, right);
      }
    }

    void operator=(const str& right)
    {
      *this = right.name;
    }

    /// TRICKY: if you need write to this structure string more 
    ///than 20 characters long, you may use following code:
    /// str* already_created_str;
    /// already_created_str->~str();
    /// new ((void*)(already_created_str)) str(new_str_value);
  };

  /**  Circuit struct.
  * Store all information about design.
  */
  struct Circuit
  {
    // common fields
    int     nNodes;          /// Number of nodes in design.
    int     nTerminals;      /// Number of terminals (i.e. unmovable nodes)
    int     nNets;           /// Number of nets in netlist.
    int     nRows;           /// Number of placement rows.
    int     nPins;           /// Total number of pins (sum for all nets)

    double  height;          /// Height of placement area.
    double  width;           /// Width of placement area.
    double  shiftX;          /// x coordinate bottom left corner of design area.
    double  shiftY;          /// y coordinate bottom left corner of design area.

    Row*    rows;            /// Array of placement rows. Contains nRows elements.
    Weight* weights;         /// Array of cell weight. Contains (nNodes + nTerminals) elements.
    Node*   nodes;           /// Array of nodes. Contains nNodes elements.
    /// WARNING: DEF parser generate "nodes" and "terminals" arrays on the same
    ///memory chunk. In any case "cellIdx" of pins fall through upper nodes bound.
    ///Thus if "cellIdx > nNodes" than this node is terminal "terminals[pin.cellIdx-nNodes]".
    Node*   terminals;       /// Array of terminal nodes. Contains nTerminals elements.
    Net*    nets;            /// Array of nets. Contains nNets elements.

    pBin**  arrOfBins;       /// Array of bins.
    int     nBinRows;        ///
    int     nBinCols;        ///          Bin grid parameters.
    double  binsHeight;      ///
    double  binsWidth;       ///

    Place*  placement;       /// Array of node placement information 
    /// Contains (nNodes + nTerminals) elements.

    str*    tableOfNames;    /// Array of node names. Contains (nNodes + nTerminals) elements.
    std::vector<int>* tableOfConnections; /// Array of cell's incident nets.
    /// Each vector contains indexes of nets incident to current cell.
    /// Contains (nNodes + nTerminals) elements.

    std::vector<double>* rowTerminalBorders;/// ?

    // fields come from lef/def format
    LEFParserData* tech;     /// Complete technology information from LEF file.
    str  designName;         /// Name of this design.
    int  defConvertFactor;   /// For DEF parser/exporter internal use only.
    RoutingLayerInfo* hLayer;/// Horizontal layer information. (for 2 layers routing and timing)
    RoutingLayerInfo* vLayer;/// Vertical layer information. (for 2 layers routing and timing)

    int    nBuffers;         /// Number of buffers.
    int    nPrimaryInputs;   /// Number of primary inputs.
    int    nPrimaryOutputs;  /// Number of primary outputs.
    int*   primaryInputs;    /// Indexes of primary inputs. WARNING: indexes calculated from beginning of nodes array.
    int*   primaryOutputs;   /// Indexes of primary outputs. WARNING: indexes calculated from beginning of nodes array.
    Net*   firstArrival;     /// Entry point for arrival time calculation.
    Net*   firstRequired;    /// Entry point for required time calculation.
    int    Shift_;

    double*   netWeights;

    Circuit() 
    {
      nNodes = 0,              nTerminals = 0,        nNets = 0,
      nPins = 0,               nRows = 0,             nBinRows = 0,
      nBinCols = 0,            nPrimaryInputs = 0,    nPrimaryOutputs = 0,
      defConvertFactor = 0;

      height = 0.0,            width = 0.0,           shiftX = 0.0,
      shiftY = 0.0,            binsHeight = 0.0,      binsWidth = 0.0;

      tech = 0,                hLayer = 0,            vLayer=0,
      rows = 0,                weights = 0,           nodes = 0,
      terminals = 0,           nets = 0,              arrOfBins = 0,
      placement = 0,           primaryInputs = 0,     primaryOutputs = 0,
      firstArrival = 0,        firstRequired = 0,     tableOfNames = 0,
      tableOfConnections = 0,  rowTerminalBorders = 0;

      nBuffers = 0;            Shift_ = 0;

      netWeights = NULL;
    }
  };

  /**  BinPosition struct.
  * Store bin's position in the array of bins.
  */
  struct BinPosition
  {
    int binRowIdx;  /// Index of the bin's row in array of bins.
    int binColIdx;  /// Index of the bin's column in array of bins.

    /// Default constructor, initialize fields with dummy value.
    BinPosition()
    {
      binRowIdx = -1;
      binColIdx = -1;
    }
  };
}
#endif //_DATA_STRUCTURES_H_