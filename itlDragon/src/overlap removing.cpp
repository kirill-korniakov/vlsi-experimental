/*  
 * overlap removing.cpp
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Kornyakov, Zhivoderov
 * email: kirillkornyakov@yandex.ru
 * email: woody-quaker@yandex.ru
 */

#include "..\include\overlap_removing.h"
#include "..\include\optimal_region_determination.h"
#include "..\include\check_legality.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

double* availableRowLength; // available sum length of cells in a row
  
/*!
 *  this function takes placement in which all cells are placed in the centers of bins
 *  (number of bin rows has to be equal to number of rows on the die)
 */
int EqualizeRowLengths(Circuit& circuit)
{   
  ITLDRAGON_ERROR errorCode = CheckBinsOnMacros(circuit);
  if (errorCode != OK) return errorCode;
  
  // we aim to distribute total cell area equally between rows
  // next variable is desired cell width per row
  double desiredRowWidth = 0.0;
  double rowWidth = circuit.rows[0].siteWidth * circuit.rows[0].numSites;
  
  int numOfBins = circuit.nBinRows * circuit.nBinCols;
  int rowIdx;
  // height of bins which equals height of row
  double binHeight = circuit.rows[0].height;
  double entireNodesWidth = 0.0;
  double* desiredRowLength = 0; // desired sum length of cells in a row
  double desiredBinLength  = 0; // desired sum length of cells in a bin
  
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    entireNodesWidth += circuit.nodes[i].width;
  }
  desiredBinLength = entireNodesWidth / numOfBins;
  
  desiredRowLength   = new double[circuit.nBinRows];
  availableRowLength = new double[circuit.nBinRows];
  for (rowIdx = 0; rowIdx < circuit.nBinRows; rowIdx++)
  {
    desiredRowLength[rowIdx] = entireNodesWidth / circuit.nBinRows;
  }
  
  
  for (rowIdx = 0; rowIdx < circuit.nBinRows; rowIdx++)
  {
    desiredRowLength[rowIdx] = circuit.rows[rowIdx].siteWidth * circuit.rows[rowIdx].numSites;
  }
 
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    if (circuit.nodes[i].height / binHeight > 1)
    {
      rowIdx = static_cast<int>((circuit.placement[i].yCoord - 
                 0.5 * circuit.nodes[i].height) / binHeight);
      
      if (rowIdx < 0 || rowIdx >= circuit.nBinRows) continue;
      
      for (int j = 0; j < circuit.nodes[i].height / binHeight; j++)
        desiredRowLength[rowIdx + j] -= circuit.nodes[i].width;
    }
  }
  for (int i = 0; i < circuit.nTerminals; ++i)
  {
    if (circuit.terminals[i].height / binHeight > 1)
    {
      rowIdx = static_cast<int>((circuit.placement[i + circuit.nNodes].yCoord - 
                0.5 * circuit.terminals[i].height) / binHeight);
      
      if (rowIdx < 0 || rowIdx >= circuit.nBinRows) continue;
  
      for (int j = rowIdx; j < min(rowIdx + circuit.terminals[i].height / binHeight, circuit.nBinRows); ++j)
        desiredRowLength[j] -= circuit.terminals[i].width;
    }
  }
  double totalSpaceInRows = 0.0;
  for (rowIdx = 0; rowIdx < circuit.nBinRows; rowIdx++)
  {
    totalSpaceInRows += desiredRowLength[rowIdx];
  }
  for (rowIdx = 0; rowIdx < circuit.nBinRows; rowIdx++)
  {
    availableRowLength[rowIdx] = desiredRowLength[rowIdx];
    desiredRowLength[rowIdx] = (desiredRowLength[rowIdx] / totalSpaceInRows) *
                               (entireNodesWidth) * 1.02;
  }
  
  if (Juggle(circuit, desiredRowLength, 1))
  {
      Juggle(circuit, desiredRowLength, -1);
  }
  
  for (int rowIdx = 0; rowIdx < circuit.nBinRows; rowIdx++ )
  {
    for (int colIdx = 0; colIdx < circuit.nBinCols; colIdx++)
    {
      circuit.arrOfBins[rowIdx][colIdx]->sumLength = 0;
      for (int i = 0; i < static_cast<int>( circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); i++)
      {
        circuit.arrOfBins[rowIdx][colIdx]->sumLength += 
          circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width; 
      }
    }
  }
  
  delete []desiredRowLength; 
  delete []availableRowLength;
  
  return 0;
}

int Juggle(Circuit& circuit, double* desiredRowLength, int direction)
{
  // we are trying to make total cell width in row close to the
  // desired row width and then legalize placement within row
  // free sites distributed between cells equally
  if (direction == 1) // bottom-up direction
  {
    for (int rowIdx = 0; rowIdx < circuit.nBinRows; rowIdx++)
    {
      if (EqualizeRow(circuit, desiredRowLength[rowIdx], rowIdx, 1))
        return 1;
      if (rowIdx % 50) PrintToTmpPL(circuit);
    }
  }
  else // top-down direction
  {
    for (int rowIdx = circuit.nBinRows - 1; rowIdx >= 0; rowIdx--)
    {
      if (EqualizeRow(circuit, 1.1 * desiredRowLength[rowIdx], rowIdx, -1))
        return 1;
      if (rowIdx % 50) PrintToTmpPL(circuit);
    }
  }

  return 0;
}

int EqualizeRow(Circuit& circuit, double desiredRowWidth, int rowIdx, int direction)
{
  double currWidth = 0.0;
  double oldCurrWidth = 0.0;
  double leftBorder = 0.0;
  int numCellsInRow = 0;
  int    cellIdx = 0;
  double cellWidth = 0.0;
  int    colIdx = 0;
  int    nextRowIdx;
  static double rowWidth  = circuit.rows[0].siteWidth * circuit.rows[0].numSites;
  static double rowHeight = circuit.rows[0].height;
  static double siteWidth = circuit.rows[0].siteWidth;
  double Yopt;

  // (1) we calculate total width of cells in this row
  for( colIdx = 0; colIdx < circuit.nBinCols; colIdx++ )
  {
    for( int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); i++ )
    {
      currWidth += circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width;
      numCellsInRow++;
    }
  }
  oldCurrWidth = currWidth;

  int continueCounter = 0;
  bool isAnyMove;

  // (2) then we do total cells width closer to desired row's width by exchange of
  //   cells with lower row
  // (2.1) if total width less than row's width, we'll add cells from 
  // lower bins while total width of cells stayed less than desired row's width
  if ((currWidth < desiredRowWidth) && (rowIdx != circuit.nBinRows - 1))
  {
    if (direction == -1) 
    {
      // time to exit, because direction is -1 and (currWidth < desiredRowWidth)
      // it means that we resolve problem and only have to legalize cells within row
            
      return 0;
    }

    // we will get cells from upper row (only if their optimal y-coordinate 
    // is greater than current y-coordinate) to achieve desired row width
    while(true)
    {
      colIdx = rand() % circuit.nBinCols;
      isAnyMove = false;

      //cout << (int)circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.size() << endl;
      if (circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.size() < 2 ||
          circuit.arrOfBins[rowIdx][colIdx]->isOnMacroBlock == true) 
      {
        continueCounter++;
        if (continueCounter > 50 * circuit.nBinCols) 
        {
          //break;
          continueCounter = 0;
          direction++;
          if (direction >= 15) break;
        }
        continue;
      }
      continueCounter = 0;

      for (int i = 0; i < static_cast<int>( circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.size()); i++ )
      {
        cellIdx = circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[i];
        Yopt = GetOptimalY( &cellIdx, 1, circuit.placement,
                             circuit.nNets, circuit.nets, 
                             circuit.tableOfConnections,
                             NULL, -1);
        if ( ((direction ==  1)&&(circuit.placement[cellIdx].yCoord > Yopt))||
             ((direction == -1)&&(circuit.placement[cellIdx].yCoord < Yopt))  )
        {
          circuit.arrOfBins[rowIdx][colIdx]->nodes.push_back(cellIdx);
          currWidth += circuit.nodes[cellIdx].width;            
          numCellsInRow++;
          isAnyMove = true;
          circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[i] = circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.back();
          circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.pop_back();
          circuit.placement[cellIdx].yCoord = rowHeight * (rowIdx + 0.5);
          break; // we move only one cell from each bin
        }
      }
      if (!isAnyMove)
      {// we will move one cell forcibly
        cellIdx = circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.back();
        circuit.arrOfBins[rowIdx][colIdx]->nodes.push_back(cellIdx);
        currWidth += circuit.nodes[cellIdx].width;            
        numCellsInRow++;
        circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.pop_back();
        circuit.placement[cellIdx].yCoord = rowHeight * (rowIdx + 0.5);
        //circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx][colIdx]->nodes[0]].yCoord;
      }
      if (currWidth >= desiredRowWidth) 
        break;        
    }
  } 
  // (2.2) if total width larger than row's width, we'll move cells to the 
  // lower bins while total width of cells stayed larger than row's width
  else if (currWidth > desiredRowWidth)
  {
    if ((rowIdx == circuit.nBinRows - 1) && (direction == 1))
    {
      //cout << "rowLen\t" << currWidth << endl;
      return 1;
    }

    while(true)
    {
      colIdx = rand() % circuit.nBinCols;
      isAnyMove = false;
      
      if (circuit.arrOfBins[rowIdx][colIdx]->nodes.size() < 2)
        continue;
      
      nextRowIdx = rowIdx + direction;
      while (nextRowIdx < circuit.nRows && nextRowIdx >= 0 && circuit.arrOfBins[nextRowIdx][colIdx]->isOnMacroBlock == true) 
        nextRowIdx += direction;
      
      for (int i = 0; i < static_cast<int>( circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); i++ )
      {
        cellIdx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
        Yopt = GetOptimalY( &cellIdx, 1, circuit.placement,
                             circuit.nNets, circuit.nets, 
                             circuit.tableOfConnections,
                             NULL, -1);
        if ( ((direction ==  1)&&(circuit.placement[cellIdx].yCoord < Yopt))||
             ((direction == -1)&&(circuit.placement[cellIdx].yCoord > Yopt))  )
        {
          circuit.arrOfBins[nextRowIdx][colIdx]->nodes.push_back( cellIdx );
          currWidth -= circuit.nodes[cellIdx].width;            
          numCellsInRow--;
          isAnyMove = true;
          circuit.arrOfBins[rowIdx][colIdx]->nodes[i] = circuit.arrOfBins[rowIdx][colIdx]->nodes.back();
          circuit.arrOfBins[rowIdx][colIdx]->nodes.pop_back();
          circuit.placement[cellIdx].yCoord = rowHeight * (nextRowIdx + 0.5);
          //circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[0]].yCoord;
          break; // we move only one cell from each bin
        }
      }
      if (!isAnyMove)
      {// we will move one cell forcibly
        cellIdx = circuit.arrOfBins[rowIdx][colIdx]->nodes.back();
        circuit.arrOfBins[nextRowIdx][colIdx]->nodes.push_back( cellIdx );
        currWidth -= circuit.nodes[cellIdx].width;            
        numCellsInRow--;
        circuit.arrOfBins[rowIdx][colIdx]->nodes.pop_back();
        circuit.placement[cellIdx].yCoord = rowHeight * (nextRowIdx + 0.5);
        //circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[0]].yCoord;
      }
      if (currWidth < desiredRowWidth)// || ((direction == -1) && (currWidth < availableRowLength[rowIdx])))
        break;
    }// while(true)
  }
  
  cout << "row " << rowIdx << "\told width: " << oldCurrWidth; 
  cout << "\ttotal cell width: " << currWidth;
  cout << "\t" << currWidth/desiredRowWidth*100 << "% of desired, direction " << direction << endl;
  
  ITLDRAGON_ERROR errorCode = CheckOrder(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckYEquality(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckBinsOnMacros(circuit);
  if (errorCode != OK) return errorCode;
  
  return 0;
}