#ifndef __LOW_TECH_H__
#define __LOW_TECH_H__

#include "stdTypes.h"

/// The preferred routing direction types enumeration
enum LayerDirection
{
  LayerDirection_Horizontal = 0, /// Routing parallel to the x axis is preferred.
  LayerDirection_Vertical   = 1, /// Routing parallel to the y axis is preferred.
  LayerDirection_Diag45     = 2, /// Routing along a 45-degree angle is preferred.
  LayerDirection_Diag135    = 3, /// Routing along a 135-degree angle is preferred.
  LayerDirection_Default    = 0
//Info: Angles are measured counterclockwise from the positive x axis.
};

/// Pin direction types enumeration
enum PinDirection 
{
  PinDirection_INPUT   = 0,  /// means that pin is macro input
  PinDirection_OUTPUT  = 1,  /// means that pin is macro output
  PinDirection_INOUT   = 2,  /// indicate that pin is bidirectional
  PinDirection_Default = 0
};

/// Defines special macro function
enum MacroType
{
  MacroType_First                = 0,
  MacroType_FirstSpecial         = 0,
  MacroType_UnknownSpecial       = 0,
  MacroType_CLKBUF               = 1,  //buffer for clock nets
  MacroType_CLKOther             = 2,
  MacroType_FILL                 = 3,
  MacroType_LastSpecial          = 3,

  MacroType_FirstSequential      = 4,
  MacroType_UnknownSequential    = 4,
  MacroType_ANTENNA              = 5, //? special element
  MacroType_LOGIC0               = 6,
  MacroType_LOGIC1               = 7,
  MacroType_FEFF                 = 8, //falling edge triggered flip-flop
  MacroType_REFF                 = 9, //rising  edge triggered flip-flop
  MacroType_HLSL                 = 10, //high level sensitive latch
  MacroType_LLSL                 = 11, //low  level sensitive latch
  MacroType_LastSequential       = 11,

  MacroType_FirstCombinational   = 12,
  MacroType_UnknownCombinational = 12,
  MacroType_AND                  = 13,
  MacroType_AOI                  = 14, // x-Input AND into y-Input NOR
  MacroType_OAI                  = 15, // x-Input OR into y-Input NAND
  MacroType_NAND                 = 16,
  MacroType_NOR                  = 17,
  MacroType_OR                   = 18,
  MacroType_XNOR                 = 19, //EXCLUSIVE NOR
  MacroType_XOR                  = 20,
  MacroType_TBUF                 = 21, //three-state buffer
  MacroType_TINV                 = 22, //three-state inverter
  MacroType_INV                  = 23,
  MacroType_BUF                  = 24,
  MacroType_LastCombinational    = 24,

  MacroType_Last                 = MacroType_LastCombinational,
  MacroType_Default              = MacroType_UnknownCombinational
};


/// Defines special pin function
enum PinFunction
{
  PinFunction_Clock   = 0,
  PinFunction_Ground  = 1,
  PinFunction_Power   = 2,
  PinFunction_Analog  = 3,
  PinFunction_Signal  = 4,// no special function (default)
  PinFunction_Clear   = 5,
  PinFunction_Preset  = 6,
  PinFunction_3State  = 7,
  PinFunction_Default = 4
};

/// Defines timing arc type
enum TimingType
{
  TimingType_Default         = 0,
  TimingType_Combinational   = 0,
  TimingType_HoldRising      = 1,
  TimingType_SetupRising     = 2,
  TimingType_HoldFalling     = 3,
  TimingType_SetupFalling    = 4,
  TimingType_FallingEdge     = 5,
  TimingType_RisingEdge      = 6,
  TimingType_Preset          = 7,
  TimingType_Clear           = 8,
  TimingType_RecoveryRising  = 9,
  TimingType_RecoveryFalling = 10,
  TimingType_RemovalRising   = 11,
  TimingType_RemovalFalling  = 12,
  TimingType_3StateEnable    = 13,
  TimingType_3StateDisable   = 14,
  TimingType_NochangeHigh    = 15,
  TimingType_NochangeLow     = 16
};

enum TimingSense
{
  TimingSense_Default       = 0,
  TimingSense_Undefined     = 0,
  TimingSense_PositiveUnate = 1,
  TimingSense_NegativeUnate = 2,
  TimingSense_NonUnate      = 3
};

struct LTech
{
  //layers
  int nLayers;
  int nLayersLimit;
  LayerDirection *layerDirection;
  string *layerName;      /// Symbolic layer name.
  double *layerRPerDist;  /// Specifies the resistance for a unit of wire. (kohms/nm)
  double *layerCPerDist;  /// Specifies the capacitance for each unit length. (pF/nm)
  double *layerEdgeCap;   /// Specifies value of peripheral capacitance for a unit length. (pF/nm)
  double *layerWidth;     /// Specifies the wire width for the layer. (nm)
  double *layerPitch;     /// Specifies the required routing pitch for the layer. (nm)

  //macro types
  int nMacro;
  int nMacroLimit;
  MacroType *macroType;      /// special function of these macro
  string *macroName;         /// symbolic name of macro type
  string *macroOriginalName; /// original name from technology file
  double *macroSizeX;        /// x size component (nm)
  double *macroSizeY;        /// y size component (nm)
  IDType *macroPinStart;
  IDType *macroPinEnd;
  //Origin specifies how to find the origin of the macro to align with a placement 
  //point. For example, if the ORIGIN is 0, -1, then macro geometry at 0, 1 are 
  //shifted to 0, 0, and then aligned to the placement point.
  //Provided only for information.
  double *macroOriginX;     /// x origin (nm)
  double *macroOriginY;     /// y origin (nm)

  //pin types
  int nPins;
  int nPinsLimit;
  string *pinName;            /// symbolic pin name
  string *pinOriginalName;    /// original name from technology file
  PinFunction *pinFunction;   /// pin's special use (power, ground, clock, etc)
  PinDirection *pinDirection; /// pin direction
  double *pinOriginX;         /// pin x offset (from bottom left corner) (nm)
  double *pinOriginY;         /// pin y offset (from bottom left corner) (nm)
  double *pinFallCapacitance;
  double *pinRiseCapacitance;
  double *pinCapacitance;

  //sites
  string *siteName;
  double *siteHeight;
  double *siteWidth;
  int nSites;
  int nSitesLimit;

  //timing arcs
  IDType *pinArcStartIdx;
  IDType *pinArcEndIdx;
  int nArcs;
  int nArcsLimit;
  int *arcStartPinOffset; //pin index offset related to first pin
        //i.e. pins[index] - first pin of timing arc
        //pinArcStartIdx[index] - index of first timing arc that starts from this pin
        //index + arcSecondPinOffset[pinArcStartIdx[index]] - index of second pin
  TimingType *arcType;
  TimingSense *arcSense;
  double *arcTIntrinsicRise;
  double *arcTIntrinsicFall;
  double *arcResistanceRise;
  double *arcResistanceFall;

  LTech()
  {
    this->arcResistanceFall = 0;
    this->arcResistanceRise = 0;
    this->arcStartPinOffset = 0;
    this->arcTIntrinsicRise = 0;
    this->arcTIntrinsicFall = 0;
    this->arcType = 0;
    this->arcSense = 0;
    this->layerCPerDist = 0;
    this->layerDirection = 0;
    this->layerEdgeCap = 0;
    this->layerName = 0;
    this->layerPitch = 0;
    this->layerRPerDist = 0;
    this->layerWidth = 0;
    this->macroName = 0;
    this->macroOriginalName = 0;
    this->macroOriginX = 0;
    this->macroOriginY = 0;
    this->macroPinEnd = 0;
    this->macroPinStart = 0;
    this->macroSizeX = 0;
    this->macroSizeY = 0;
    this->macroType = 0;
    this->nArcs = 0;
    this->nArcsLimit = 0;
    this->nLayers = 0;
    this->nPins = 0;
    this->nMacro = 0;
    this->pinArcEndIdx = 0;
    this->pinArcStartIdx = 0;
    this->pinDirection = 0;
    this->pinFallCapacitance = 0;
    this->pinCapacitance = 0;
    this->pinName = 0;
    this->pinOriginalName = 0;
    this->pinOriginX = 0;
    this->pinOriginY = 0;
    this->pinRiseCapacitance = 0;
    this->pinFunction = 0;
    this->nLayersLimit = 0;
    this->nPinsLimit = 0;
    this->nMacroLimit = 0;

    this->siteName = 0;
    this->siteHeight = 0;
    this->siteWidth = 0;
    this->nSites = 0;
    this->nSitesLimit = 0;
  }
};

#endif //__LOW_TECH_H__