/*  
 * overlap_removing.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, ITLab, Kornyakov, Zhivoderov
 * email: kirillkornyakov@yandex.ru
 * email: zhivoderov.a@gmail.com
 */

#include "..\include\overlap_removing.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

char** arrOfSites;
double* availableRowLengths;  // available sum length of cells in a row

struct cellIdxAndX
{
  int idx;
  double xOpt;
};

// Return whether xOpt of first element is less than the second
bool predicate( cellIdxAndX elem1, cellIdxAndX elem2 )
{
  return elem1.xOpt < elem2.xOpt;
}

void FillArrOfSites(Circuit& circuit)
{
  double siteWidth = circuit.rows[0].siteWidth;
  double siteHeight = circuit.rows[0].height;
  int numSites = circuit.rows[circuit.nRows/2].numSites;
  
  arrOfSites = new char*[circuit.nRows];
  for (int j = 0; j < circuit.nRows; ++j)
  {
    arrOfSites[j] = new char[(int)(1.1*numSites)];
  }
  //Initial filling
  for (int i = 0; i < circuit.nRows; ++i)
  {
    for (int j = 0; j < numSites; ++j)
    {
      arrOfSites[i][j] = 0;   // site is not blocked
    }
  }
  int currRow  = 0;
  int currSite = 0;
  for (int i = 0; i < circuit.nTerminals; ++i)
  {
    //if (circuit.terminals[i].height / siteHeight > 1)
    {
      currRow  = static_cast<int>((circuit.placement[i + circuit.nNodes].yCoord - 
                   0.5 * circuit.terminals[i].height) / siteHeight);
      currSite = static_cast<int>((circuit.placement[i + circuit.nNodes].xCoord - 
                   0.5 * circuit.terminals[i].width) / siteWidth);
                   
      for (int j = currRow; j < currRow + circuit.terminals[i].height / siteHeight; ++j)
      {
        if (j < 0 || j > circuit.nRows) continue;
        for (int siteIdx = currSite; siteIdx < min(currSite + circuit.terminals[i].width / siteWidth, numSites);
              siteIdx++)
        {
          if (siteIdx < 0 || siteIdx > numSites)
            continue;
          arrOfSites[j][siteIdx] = 1;    // site is marked as blocked
        }
      }
    }
  }
}

/*!
 *  this function takes placement in which all cells are placed in the centers of circuit.arrOfBins
 *  (number of bin rows has to be equal number of rows on the die)
 *  its aim is to create the placement, in which cells are placed within rows without overlaps
 */
MULTIPLACER_ERROR OverlapRemoving(Circuit& circuit)
{   
  MULTIPLACER_ERROR errorCode = OK;

  // check legality of Bins
  errorCode = CheckXEquality(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckYEquality(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckOrder(circuit);
  if (errorCode != OK) return errorCode;
  
  FillArrOfSites(circuit);
  
  int rowIdx;
  double binHeight = circuit.rows[0].height;
  double entireNodesWidth = 0.0;
  
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    entireNodesWidth += circuit.nodes[i].width;
  }
  
  availableRowLengths = new double[circuit.nBinRows];
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    availableRowLengths[rowIdx] = entireNodesWidth / circuit.nBinRows;
  }
  
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    availableRowLengths[rowIdx] = circuit.rows[rowIdx].siteWidth * circuit.rows[rowIdx].numSites;
  }
 
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    if (circuit.nodes[i].height / binHeight > 1)
    {
      rowIdx = static_cast<int>((circuit.placement[i].yCoord - 
                 0.5 * circuit.nodes[i].height) / binHeight);
      
      if (rowIdx < 0 || rowIdx >= circuit.nBinRows) continue;
      
      for (int j = 0; j < circuit.nodes[i].height / binHeight; ++j)
        availableRowLengths[rowIdx + j] -= circuit.nodes[i].width;
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
        availableRowLengths[j] -= circuit.terminals[i].width;
    }
  }
  
  for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    LegalizeRow(circuit, rowIdx);
    /*errorCode = CheckBorders(circuit);
    if (errorCode != OK) cout << "rowIdx " << rowIdx << endl;*/
  }
  
  for (int j = 0; j < circuit.nRows; ++j)
  {
    delete []arrOfSites[j];
  }
  delete []arrOfSites;
  delete []availableRowLengths;
  
  // check legality of circuit.arrOfBins
  //errorCode = IsNotEmpty(circuit);
  //if (errorCode != OK) return errorCode;
  errorCode = CheckYEquality(circuit);
  if (errorCode != OK) return errorCode;
  /*errorCode = CheckOrder(circuit);
  if (errorCode != OK) return errorCode;*/
  
  return errorCode;
}

MULTIPLACER_ERROR LegalizeRow(Circuit& circuit, int rowIdx)
{
  double siteWidth = circuit.rows[0].siteWidth;
  double siteHeight = circuit.rows[0].height;
  int    numSites = circuit.rows[circuit.nRows/2].numSites;
  int    numCellsInRow = 0;
  double leftBorder  = 0.0;
  double rightBorder = 0.0;
  
  int    cellIdx = 0;
  double cellWidth = 0.0;
  double binCenterXCoord;
  double currWidth = 0.0;
  int    currRow  = 0;
  int    currSite = 0;
  int    colIdx;

  // (1) we calculate total width of cells in this row
  for (colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
    for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); i++)
    {
      currWidth += circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width;
      numCellsInRow++;
    }

  int numOfCellsInBin;
  vector<cellIdxAndX> sortedNodes;
  cellIdxAndX         newIdxAndX;

  for (colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
  {    
    if (circuit.arrOfBins[rowIdx][colIdx]->isOnMacroBlock == true) continue;
    numOfCellsInBin = static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size());

    // determine Xopt coordinate for each cell
    sortedNodes.clear();
    for (int i = 0; i < numOfCellsInBin; ++i)
    {
      newIdxAndX.idx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
      newIdxAndX.xOpt = GetOptimalX( &newIdxAndX.idx, 1, circuit.placement,
                                     circuit.nNets, circuit.nets, circuit.tableOfConnections,
                                     NULL, -1);
      sortedNodes.push_back(newIdxAndX);
    }

    // sort cell in order of their Xopt coordinate
    sort(sortedNodes.begin(), sortedNodes.end(), predicate);
    
    binCenterXCoord = circuit.arrOfBins[rowIdx][colIdx]->xCoord;
    
    leftBorder = max(0, floor(binCenterXCoord - 0.5*circuit.arrOfBins[rowIdx][colIdx]->sumLength));
    for (int i = 0; i < numOfCellsInBin; ++i)
    {
      cellIdx = sortedNodes[i].idx;
      cellWidth = circuit.nodes[cellIdx].width;
      currSite = static_cast<int>(leftBorder / siteWidth);
      
      // looking for continous section of sites to put current cell there
      for (int siteIdx = currSite; siteIdx < currSite + cellWidth / siteWidth; siteIdx++)
        // if blocked
        if (arrOfSites[rowIdx][siteIdx] != 0)
        {
          currSite = siteIdx + 1;
          //siteIdx = currSite;
          continue;
        }
        
      leftBorder = currSite * siteWidth;
      circuit.placement[cellIdx].xCoord = leftBorder + cellWidth * 0.5;
      
      //currRow = static_cast<int>(circuit.placement[cellIdx].yCoord / siteHeight);
      //currSite = static_cast<int>(leftBorder / siteWidth);
      
      for (int t = currSite; t < currSite + (cellWidth/ siteWidth); ++t)
      {
        if (t >= 1.1*numSites || t < 0)
          continue;
        arrOfSites[rowIdx][t] = 2;
      }
      
      leftBorder += cellWidth;
    }
    if (leftBorder >= circuit.width)
    {
      break;
    }
  }
  int leftCellIdx = cellIdx;
  rightBorder = circuit.width - 1.0;
  if (colIdx < circuit.nBinCols)
  {
    for (colIdx = circuit.nBinCols - 1; colIdx >= 0; --colIdx)
    {    
      if (circuit.arrOfBins[rowIdx][colIdx]->isOnMacroBlock == true) continue;
      numOfCellsInBin = static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size());

      // determine Xopt coordinate for each cell
      sortedNodes.clear();
      for (int i = 0; i < numOfCellsInBin; ++i)
      {
        newIdxAndX.idx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
        newIdxAndX.xOpt = GetOptimalX( &newIdxAndX.idx, 1, circuit.placement,
                                       circuit.nNets, circuit.nets, circuit.tableOfConnections,
                                       NULL, -1);
        sortedNodes.push_back(newIdxAndX);
      }

      // sort cell in order of their Xopt coordinate
      sort(sortedNodes.begin(), sortedNodes.end(), predicate);
      
      /*currSite = 
      for (int siteIdx = currSite; siteIdx > currSite - cellWidth / siteWidth; --siteIdx)
        arrOfSites[rowIdx][siteIdx] = 0;*/
      
      /*binCenterXCoord = circuit.arrOfBins[rowIdx][colIdx]->xCoord;
      
      rightBorder = min(circuit.width - 1.0, floor(binCenterXCoord + 
                            0.5*circuit.arrOfBins[rowIdx][colIdx]->sumLength));*/
      for (int i = 0; i < numOfCellsInBin; ++i)
      {
        cellIdx = sortedNodes[numOfCellsInBin - i - 1].idx;
        cellWidth = circuit.nodes[cellIdx].width;
        currSite = static_cast<int>(rightBorder / siteWidth);
        
        // looking for continuous section of sites to put current cell there
        for (int siteIdx = currSite; siteIdx > currSite - cellWidth / siteWidth; --siteIdx)
          // if blocked
          if (arrOfSites[rowIdx][siteIdx] == 1)
          {
            currSite = siteIdx - 1;
            //siteIdx = currSite;
            continue;
          }
          
        rightBorder = currSite * siteWidth;
        circuit.placement[cellIdx].xCoord = rightBorder - cellWidth * 0.5 + 1.0;
        
        /*for (int t = currSite; t > currSite - (cellWidth / siteWidth); --t)
        {
          if (t >= 1.1*numSites || t < 0)
            continue;
          arrOfSites[rowIdx][t] = 2;
        }*/
        
        rightBorder -= cellWidth;
      }
      if (!CheckOverlaps(circuit, rowIdx) && !CheckXBorders(circuit, rowIdx))
        break;
    }
  }
  
  //int numOfFreeSitesPerCell;
  //int numOfSurplusFreeSites;
  ////int cellsCounter;
  //
  //numOfFreeSitesPerCell = 
  //  static_cast<int>(((availableRowLengths[rowIdx] - currWidth) / siteWidth) / (numCellsInRow - 1));
  //numOfSurplusFreeSites = 
  //  static_cast<int>((availableRowLengths[rowIdx] - currWidth) / siteWidth) % (numCellsInRow - 1);
  //leftBorder = 0.0;
  ////cellsCounter = 0;
  //// we will place cells in row in order of their x-optimal coordinates
  //for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
  //{
  //  if (circuit.arrOfBins[rowIdx][colIdx]->isOnMacroBlock == true) continue;
  //  
  //  sortedNodes.clear();
  //  for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); ++i)
  //  {
  //    newIdxAndX.idx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
  //    newIdxAndX.xOpt = GetOptimalX( &newIdxAndX.idx, 1, circuit.placement,
  //                                   circuit.nNets, circuit.nets, circuit.tableOfConnections,
  //                                   NULL, -1);
  //    sortedNodes.push_back(newIdxAndX);
  //  }
  //  sort(sortedNodes.begin(), sortedNodes.end(), predicate);
  //  for (int i = 0; i < static_cast<int>(sortedNodes.size()); ++i)
  //  {
  //    cellIdx = sortedNodes[i].idx;
  //    cellWidth = circuit.nodes[cellIdx].width;
  //    currSite = static_cast<int>(leftBorder / siteWidth);
  //    
  //    // looking for continous section of sites to put current cell there
  //    for (int siteIdx = currSite; siteIdx < currSite + cellWidth / siteWidth; siteIdx++)
  //      // if blocked
  //      if (arrOfSites[rowIdx][siteIdx] == true)
  //      {
  //        currSite = siteIdx + 1;
  //        //siteIdx = currSite;
  //        continue;
  //      }
  //      
  //    leftBorder = currSite * siteWidth;
  //    /*if (cellsCounter < numOfSurplusFreeSites)
  //    {
  //      circuit.placement[cellIdx].xCoord = leftBorder + cellWidth * 0.5;
  //      leftBorder += cellWidth + (numOfFreeSitesPerCell + 1) * siteWidth;
  //    }
  //    else*/
  //    {
  //      circuit.placement[cellIdx].xCoord = leftBorder + cellWidth * 0.5;
  //      leftBorder += cellWidth + numOfFreeSitesPerCell * siteWidth;
  //      if (leftBorder >= circuit.width)
  //      {
  //        colIdx = -1;
  //        if (numOfFreeSitesPerCell != 0) numOfFreeSitesPerCell--;
  //        leftBorder = 0.0;
  //        break;
  //      }
  //    }
  //    
  //    //cellsCounter++;
  //  }
  //}
  
  return OK;
}

/*!
 *  this function takes placement in which all cells are placed in the centers of bins
 *  (number of bin rows has to be equal to number of rows on the die)
 */
MULTIPLACER_ERROR EqualizeRowLengths(Circuit& circuit)
{   
  MULTIPLACER_ERROR errorCode = CheckBinsOnMacros(circuit);
  if (errorCode != OK) return errorCode;
  
  // we aim to distribute total cell area equally between rows
  // next variable is desired cell width per row
  double desiredRowWidth = 0.0;
  double rowWidth = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;
  
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
  availableRowLengths = new double[circuit.nBinRows];
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    desiredRowLength[rowIdx] = entireNodesWidth / circuit.nBinRows;
  }
  
  
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
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
      
      for (int j = 0; j < circuit.nodes[i].height / binHeight; ++j)
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
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    totalSpaceInRows += desiredRowLength[rowIdx];
  }
  for (rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx)
  {
    availableRowLengths[rowIdx] = desiredRowLength[rowIdx];
    desiredRowLength[rowIdx] = (desiredRowLength[rowIdx] / totalSpaceInRows) *
                               (entireNodesWidth) * 1.02;
  }
  
  if (Juggle(circuit, desiredRowLength, 1))
  {
      Juggle(circuit, desiredRowLength, -1);
  }
  
  for (int rowIdx = 0; rowIdx < circuit.nBinRows; ++rowIdx )
  {
    for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
    {
      circuit.arrOfBins[rowIdx][colIdx]->sumLength = 0;
      for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); i++)
      {
        circuit.arrOfBins[rowIdx][colIdx]->sumLength += 
          circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width; 
      }
    }
  }
  
  delete []desiredRowLength; 
  delete []availableRowLengths;
  
  errorCode = CheckBins(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckBinsOnMacros(circuit);
  if (errorCode != OK) return errorCode;
  
  return errorCode;
}

int Juggle(Circuit& circuit, double* desiredRowLength, int direction)
{
  MULTIPLACER_ERROR errorCode;
  // we are trying to make total cell width in row close to the
  // desired row width and then legalize placement within row
  // free sites distributed between cells equally
  if (direction == 1) // bottom-up direction
  {
    for (int rowIdx = 801; rowIdx < circuit.nBinRows; ++rowIdx)
    {
      if (errorCode = EqualizeRow(circuit, desiredRowLength[rowIdx], rowIdx, 1))
        return errorCode;
      if (rowIdx % 50 == 0) PrintToTmpPL(circuit);
    }
  }
  else // top-down direction
  {
    for (int rowIdx = circuit.nBinRows - 1; rowIdx >= 0; rowIdx--)
    {
      if (errorCode = EqualizeRow(circuit, 1.1 * desiredRowLength[rowIdx], rowIdx, -1))
        return errorCode;
      if (rowIdx % 50 == 0) PrintToTmpPL(circuit);
    }
  }

  return 0;
}

MULTIPLACER_ERROR EqualizeRow(Circuit& circuit, double desiredRowWidth, int rowIdx, int direction)
{
  double currWidth = 0.0;
  double oldCurrWidth = 0.0;
  double leftBorder = 0.0;
  double currY;
  int numCellsInRow = 0;
  int    cellIdx = 0;
  double cellWidth = 0.0;
  int    colIdx = 0;
  int    nextRowIdx;
  int    binNodesSize;
  static double rowWidth  = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;
  static double rowHeight = circuit.rows[0].height;
  static double siteWidth = circuit.rows[0].siteWidth;
  double Yopt;

  // (1) we calculate total width of cells in this row
  for (colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
  {
    binNodesSize = static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size());
    for (int i = 0; i < binNodesSize; ++i)
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
            
      return OK;
    }

    // we will get cells from upper row (only if their optimal y-coordinate 
    // is greater than current y-coordinate) to achieve desired row width
    while(true)
    {
      colIdx = rand() % circuit.nBinCols;
      isAnyMove = false;
      
      /*if (rowIdx + direction >= 182)
        for (int i = 0; i < circuit.nBinCols; ++i)
        {
          cout << static_cast<int>(circuit.arrOfBins[rowIdx+direction][i]->nodes.size()) << "\t";
        }*/
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
          if (direction >= 15 || rowIdx+direction >= circuit.nRows) break;
        }
        continue;
      }
      continueCounter = 0;

      binNodesSize = static_cast<int>(circuit.arrOfBins[rowIdx+direction][colIdx]->nodes.size());
      for (int i = 0; i < binNodesSize; ++i)
      {
        cellIdx = circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[i];
        Yopt = GetOptimalYRude( &cellIdx, 1, circuit.placement,
                                 circuit.nNets, circuit.nets, 
                                 circuit.tableOfConnections,
                                 NULL, -1, circuit.nNodes);
        currY = circuit.placement[cellIdx].yCoord;
        if ( ((direction > 0)&&(currY > Yopt))||
             ((direction < 0)&&(currY < Yopt))  )
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
      return 1;
    }
    if (rowIdx == 0 && direction < 0)
    {
      if (currWidth <= availableRowLengths[rowIdx])
        return OK;
      else
        return EQUALIZE_ROW_LEN_ERR;
    }

    while(true)
    {
      colIdx = rand() % circuit.nBinCols;
      isAnyMove = false;
      
      if (circuit.arrOfBins[rowIdx][colIdx]->nodes.size() < 2)
        continue;
      
      nextRowIdx = rowIdx + direction;
      while (nextRowIdx < circuit.nRows - 1 && nextRowIdx >= 0 && circuit.arrOfBins[nextRowIdx][colIdx]->isOnMacroBlock == true) 
        nextRowIdx += direction;
      
      if (nextRowIdx >= circuit.nRows - 1 || nextRowIdx < 0)
      {
        continueCounter++;
        if (continueCounter > 500 * circuit.nBinCols) 
        {
          break;
        }
        continue;
      }
      continueCounter = 0;
      
      binNodesSize = static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size());
      for (int i = 0; i < binNodesSize; ++i)
      {
        cellIdx = circuit.arrOfBins[rowIdx][colIdx]->nodes[i];
        Yopt = GetOptimalYRude( &cellIdx, 1, circuit.placement,
                                 circuit.nNets, circuit.nets, 
                                 circuit.tableOfConnections,
                                 NULL, -1, circuit.nNodes);
        currY = circuit.placement[cellIdx].yCoord;
        if ( ((direction > 0)&&(currY < Yopt))||
             ((direction < 0)&&(currY > Yopt))  )
        {
          circuit.arrOfBins[nextRowIdx][colIdx]->nodes.push_back(cellIdx);
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
        circuit.arrOfBins[nextRowIdx][colIdx]->nodes.push_back(cellIdx);
        currWidth -= circuit.nodes[cellIdx].width;            
        numCellsInRow--;
        circuit.arrOfBins[rowIdx][colIdx]->nodes.pop_back();
        circuit.placement[cellIdx].yCoord = rowHeight * (nextRowIdx + 0.5);
        //circuit.placement[cellIdx].yCoord = circuit.placement[circuit.arrOfBins[rowIdx+direction][colIdx]->nodes[0]].yCoord;
      }
      if (currWidth < desiredRowWidth)// || ((direction == -1) && (currWidth < availableRowLengths[rowIdx])))
        break;
    }// while(true)
  }
  
  cout << "row " << rowIdx << "\told width: " << oldCurrWidth; 
  cout << "\ttotal cell width: " << currWidth;
  cout << "\t" << currWidth/desiredRowWidth*100 << "% of desired, direction " << direction << endl;
  
  /*MULTIPLACER_ERROR errorCode = CheckOrder(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckYEquality(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckBinsOnMacros(circuit);
  if (errorCode != OK) return errorCode;*/
  
  return OK;
}