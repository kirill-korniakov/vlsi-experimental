/*  
 * simulated_annealing.cpp
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Nina Kurina
 * email: ninku@mail.ru
 * last modified 15 February, 2006
 */


#ifndef _SIMULATED_ANNEALING_H_
#define _SIMULATED_ANNEALING_H_

#include "..\include\data_structures.h"
#include "..\include\cost_function.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\global_placement.h"
#include "..\include\global.h"

/// this function takes a cell (with index idx) which is located in the bin with 
/// indexes currBinRowIdx and currBinColIdx;
/// the index of the cell in bin.nodes is idxInBin;
/// the function virtually moves the cell to another bin: 
/// it changes cell's coordinates, but doesn't change contents of bins

inline ITLDRAGON_ERROR TryDisplace( const int currCellIdx, const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfpBins, Place* const placement, Node* const nodes );

/// this function takes a cell (with index idx) which is located in the bin with 
/// indexes currBinRowIdx and currBinColIdx;
/// the index of the cell in bin.nodes is idxInBin;
/// the function virtually swops two cells (the current and a new randomly chosen one):  
/// exchanges cells coordinates, but doesn't change contents of bins

inline ITLDRAGON_ERROR TryExchange( const int currCellIdx, const int newCellIdx, 
           const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfBins, Place* const placement, Node* const nodes );

inline ITLDRAGON_ERROR RejectDisplace( const int currCellIdx, const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfpBins, Place* const placement );

inline ITLDRAGON_ERROR RejectExchange( const int currCellIdx, const int newCellIdx,
           const int currBinRowIdx, const int currBinColIdx,
           const int newBinRowIdx, const int newBinColIdx, 
           pBin** const arrOfBins, Place* const placement );

inline int IsBinBalanceBetter( const int binRowIdx, const int binColIdx, const double oldLength, 
           pBin** const arrOfBins, const double binUnbalance );

inline int IsRowBalanceBetter( const int rowIdx, const double oldLength, const double rowUnbalance );

inline ITLDRAGON_ERROR GenerateRandomNeighbour( const int currBinRowIdx, const int currBinColIdx,
                            const int numOfBinRows, const int numOfBinCols, 
                            int* const horOffset, int* const vertOffset,
                            const int idxOfRandNum,
                            int& newBinRowIdx, int& newBinColIdx );

inline ITLDRAGON_ERROR GenerateTopologicalNeighbour( const int currCellIdx, const int numOfBinCols,
                                 const int numOfNets, Net* const nets,
                                 vector<int>* tableOfConnections, pBin** const arrOfBins,
                                 const double binHeight, Place* placement,
                                 int& newBinRowIdx, int& newBinColIdx, const int numOfNodes,
                                 float* const randNet, float* const randPin, const int idxOfRandNum );


inline ITLDRAGON_ERROR GenerateOptTopologicalNeighbour( const int currCellIdx, const int numOfBinCols, const int numOfBinRows,
                                             const int numOfNets, Net* const nets,
                                             vector<int>* tableOfConnections, pBin** const arrOfBins,
                                             const double binHeight, Place* placement,
                                             int& newBinRowIdx, int& newBinColIdx, const int numOfNodes,
                                             float* const randNet, float* const randPin, const int idxOfRandNum );

inline ITLDRAGON_ERROR GenerateOptimalNeighbour( int currCellIdx, const int numOfBinCols, const int numOfBinRows, 
                                 const int numOfNets, Net* const nets,
                                 vector<int>* tableOfConnections, pBin** const arrOfBins,
                                 const double binHeight, Place* placement,
                                 int& newBinRowIdx, int& newBinColIdx );

/// this function takes placement in which all cells are placed in the centers 
/// of bins and moves single cells between bins locally to minimize wirelength;
/// it uses low temperature simulated annealing algorithm;
/// we make an assumption that there are no empty bins in both
/// income and outcome placements;
/// in outcome placement cells are still placed in the centers of bins

ITLDRAGON_ERROR SimulatedAnnealing(Circuit& circuit, Statistics& statistics);

#endif
