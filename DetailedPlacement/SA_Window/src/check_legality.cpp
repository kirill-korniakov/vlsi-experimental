#include "..\include\check_legality.h"
#include "math.h"
#include <iostream>

const int MAX_REPORTED_ERRORS = 3;

// checks the legality of the current placement
MULTIPLACER_ERROR CheckLegalityOfPlacement(const Circuit& circuit)
{   
  MULTIPLACER_ERROR errorCode = OK;

  // check if cell lay onto site
  errorCode = CheckSiteBelonging(circuit);
  //CheckCode(errorCode);
  
  // check if cell lay into row
  errorCode = CheckRowBelonging(circuit);
  //CheckCode(errorCode);
  
  // check if cell lay into circuit area
  errorCode = CheckBorders(circuit);
  //CheckCode(errorCode);

  // check absence of the overlaps
  errorCode = CheckOverlaps(circuit);
  //CheckCode(errorCode);

  if (errorCode == OK)
  {
    cout << "placement is LEGAL\n";
    return OK;
  }
  else
  {
    cout << "\nplacement is NOT LEGAL\n";
    return NOT_LEGAL;
  }
}

MULTIPLACER_ERROR CheckSiteBelonging(const Circuit& circuit)
{
  int errorCounter = 0;

  int siteWidth = 0;
  siteWidth = circuit.rows[0].siteWidth;

  for (int i = 0; i < circuit.nNodes; ++i)
  {
    if ( fmod(circuit.placement[i].xCoord - 0.5*circuit.nodes[i].width, siteWidth) != 0.0)
    {
      cout << "cell " << circuit.tableOfNames[i].name << " is not onto site" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";
        return NOT_ONTO_SITE;
      }
    }
  }
  if (errorCounter == 0) 
    return OK;
  else 
    return NOT_ONTO_SITE;
}

MULTIPLACER_ERROR CheckRowBelonging(const Circuit& circuit)
{
  int errorCounter = 0;

  int rowHeight = circuit.rows[0].height;

  for (int i = 0; i < circuit.nNodes; ++i)
  {
    if ( fmod(circuit.placement[i].yCoord - 0.5*circuit.nodes[i].height, rowHeight) != 0.0)
    {
      cout << "cell " << circuit.tableOfNames[i].name << " is not into row" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";
        return NOT_INTO_ROW;
      }
    }
  }
  if (errorCounter == 0) 
    return OK;
  else 
    return NOT_INTO_ROW;
}

MULTIPLACER_ERROR CheckBorders(const Circuit& circuit)
{
  int errorCounter = 0;

  int rowOrigin       = 0;
  int rowLength       = 0;
  int coordOfLowerRow = 0;
  int coordOfUpperRow = 0;  

  rowOrigin = static_cast<int>(circuit.rows[0].subrowOrigin);
  rowLength = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;

  coordOfLowerRow = circuit.rows[0].coordinate;
  coordOfUpperRow = circuit.rows[circuit.nRows-1].coordinate;

  // 2. CHECK LEGALITY OF MOVABLE CELLS  
  int cellWidthInSites = 0;
  int rowIdx  = 0;
  int siteIdx = 0;

  //2.2 FOR each cell
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    //2.2.1 check if cells exceed the left border
    if (circuit.placement[i].xCoord - 0.5*circuit.nodes[i].width < rowOrigin)
    {
      cout << "cell " << circuit.tableOfNames[i].name << " exceeds the left border" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";
        return LEFT_BORDER_EXCEEDED;
      }
    }

    //2.2.2  check if cell exceed the right border
    if (circuit.placement[i].xCoord + 0.5*circuit.nodes[i].width > rowOrigin + rowLength)
    {
      cout << "cell " << circuit.tableOfNames[i].name << " exceeds the right border" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";
        return RIGHT_BORDER_EXCEEDED;
      }
    }
    //2.2.4 check if cell exceed the lower border
    if (circuit.placement[i].yCoord - 0.5*circuit.nodes[i].height < coordOfLowerRow)
    {
      cout << "cell " << circuit.tableOfNames[i].name << " exceeds the lower border" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";
        return LOWER_BORDER_EXCEEDED;
      }
    }

    //2.2.5 check if cell exceed the upper border
    if (circuit.placement[i].yCoord - 0.5*circuit.nodes[i].height > coordOfUpperRow)
    {
      cout << "cell " << circuit.tableOfNames[i].name << " exceeds the upper border" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";
        return UPPER_BORDER_EXCEEDED;
      }
    }
  }
  if (errorCounter == 0) 
    return OK;
  else 
    return BORDER_EXCEEDED;
}

MULTIPLACER_ERROR CheckXBorders(const Circuit& circuit, int rowIdx)
{
  int errorCounter = 0;

  int rowOrigin       = 0;
  int rowLength       = 0;
  int coordOfLowerRow = 0;
  int coordOfUpperRow = 0;  

  rowOrigin = static_cast<int>(circuit.rows[0].subrowOrigin);
  rowLength = circuit.rows[0].siteWidth * circuit.rows[circuit.nRows/2].numSites;

  coordOfLowerRow = circuit.rows[0].coordinate;
  coordOfUpperRow = circuit.rows[circuit.nRows-1].coordinate;

  // 2. CHECK LEGALITY OF MOVABLE CELLS  
  int cellWidthInSites = 0;
  int siteIdx = 0;
  int siteHeight = circuit.rows[0].height;

  //2.2 FOR each cell
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    if (floor(circuit.placement[i].yCoord / siteHeight) != rowIdx) continue;
    //2.2.1 check if cells exceed the left border
    if (circuit.placement[i].xCoord - 0.5*circuit.nodes[i].width < rowOrigin)
    {
      //cout << "cell " << circuit.tableOfNames[i].name << " exceeds the left border" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        /*cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";*/
        return LEFT_BORDER_EXCEEDED;
      }
    }

    //2.2.2  check if cell exceed the right border
    if (circuit.placement[i].xCoord + 0.5*circuit.nodes[i].width > rowOrigin + rowLength)
    {
      //cout << "cell " << circuit.tableOfNames[i].name << " exceeds the right border" << "\n";
      errorCounter++;
      if (errorCounter == MAX_REPORTED_ERRORS)
      {
        /*cout << "\nplacement is NOT LEGAL\n";
        cout << "Too many errors, checking is stopped\n";*/
        return RIGHT_BORDER_EXCEEDED;
      }
    }
  }
  if (errorCounter == 0) 
    return OK;
  else 
    return BORDER_EXCEEDED;
}

MULTIPLACER_ERROR CheckOverlaps(const Circuit& circuit)
{ 
  int errorCounter = 0;

  int cellWidthInSites;
  int rowIdx;
  int siteIdx;
  int** arrOfSites;
  bool isAlreadyReported;
  int rowHeight;
  int siteWidth;
  int numSites;
  double rowOrigin; 

  rowOrigin = circuit.rows[0].subrowOrigin;
  rowHeight = circuit.rows[0].height;
  siteWidth = circuit.rows[0].siteWidth;
  numSites  = circuit.rows[circuit.nRows/2].numSites;
  
  // initialize arrOfSites
  arrOfSites = new int*[circuit.nRows];
  for (int i = 0; i < circuit.nRows; ++i)
  {
    arrOfSites[i] = new int[numSites];
    for (int j = 0; j < numSites; ++j)
      arrOfSites[i][j] = -1;
  }

  for (int i = 0; i < circuit.nNodes; ++i)
  {
    cellWidthInSites = circuit.nodes[i].width/siteWidth;
    rowIdx  = static_cast<int>(floor(circuit.placement[i].yCoord / rowHeight));
    siteIdx = static_cast<int>((circuit.placement[i].xCoord - 0.5*circuit.nodes[i].width - rowOrigin) / siteWidth);
    if ((rowIdx < 0)||(siteIdx < 0)||(rowIdx >= circuit.nRows)||(siteIdx > numSites - cellWidthInSites))
    {
      continue;
    }
    isAlreadyReported = false;
    for (int j = 0; j < cellWidthInSites; ++j)
    {
      if (arrOfSites[rowIdx][siteIdx+j] != -1)
      {
        if (!isAlreadyReported)
        {
          isAlreadyReported = true;
          errorCounter++;
          if (errorCounter == MAX_REPORTED_ERRORS)
          {
            cout << "...\nToo many errors, checking is stopped\n";
            return OVERLAP;
          }
          //cout << arrOfSites[rowIdx][siteIdx+j] << "\t";
          cout << "cell " << circuit.tableOfNames[arrOfSites[rowIdx][siteIdx+j]].name 
            << " overlaps cell " << circuit.tableOfNames[i].name << "\n";
        }  
      }
      arrOfSites[rowIdx][siteIdx+j] = i;
    }
  }

  for (int i = 0; i < circuit.nRows; ++i)
    delete[] arrOfSites[i];
  delete[] arrOfSites;

  if (errorCounter == 0) return OK;
  else return OVERLAP;
}

MULTIPLACER_ERROR CheckOverlaps(const Circuit& circuit, int rowIdxToCheck)
{
  int errorCounter = 0;

  int cellWidthInSites;
  int rowIdx;
  int siteIdx;
  int* arrOfSites;
  bool isAlreadyReported;
  int rowHeight;
  int siteWidth;
  int numSites;
  double rowOrigin; 

  rowOrigin = circuit.rows[0].subrowOrigin;
  rowHeight = circuit.rows[0].height;
  siteWidth = circuit.rows[0].siteWidth;
  numSites  = circuit.rows[circuit.nRows/2].numSites;
  
  // initialize arrOfSites
  arrOfSites = new int[numSites];
  for (int i = 0; i < numSites; ++i)
  {
    arrOfSites[i] = -1;
  }

  for (int i = 0; i < circuit.nNodes; ++i)
  {
    cellWidthInSites = circuit.nodes[i].width/siteWidth;
    rowIdx  = static_cast<int>(floor(circuit.placement[i].yCoord / rowHeight));
    siteIdx = static_cast<int>((circuit.placement[i].xCoord - 0.5*circuit.nodes[i].width - rowOrigin) / siteWidth);
    if ((rowIdx < 0)||(siteIdx < 0)||(rowIdx >= circuit.nRows)||(siteIdx > numSites - cellWidthInSites)||(rowIdx != rowIdxToCheck))
    {
      continue;
    }
    isAlreadyReported = false;
    for (int j = 0; j < cellWidthInSites; ++j)
    {
      if (arrOfSites[siteIdx+j] != -1)
      {
        if (!isAlreadyReported)
        {
          isAlreadyReported = true;
          errorCounter++;
          if (errorCounter == MAX_REPORTED_ERRORS)
          {
            //cout << "...\nToo many errors, checking is stopped\n";
            return OVERLAP;
          }
          //cout << arrOfSites[rowIdx][siteIdx+j] << "\t";
          /*cout << "cell " << circuit.tableOfNames[arrOfSites[siteIdx+j]].name 
            << " overlaps cell " << circuit.tableOfNames[i].name << "\n";*/
        }  
      }
      arrOfSites[siteIdx+j] = i;
    }
  }

  delete[] arrOfSites;

  if (errorCounter == 0) return OK;
  else return OVERLAP;
}

MULTIPLACER_ERROR CheckBins(const Circuit& circuit)
{
  MULTIPLACER_ERROR errorCode = OK;

  /*errorCode = IsNotEmpty(circuit);
  if (errorCode != OK) return errorCode;*/
  errorCode = CheckXEquality(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckYEquality(circuit);
  if (errorCode != OK) return errorCode;
  errorCode = CheckOrder(circuit);
  if (errorCode != OK) return errorCode;

  return OK;
}

int IsNotEmpty(const Circuit& circuit)
{
  for (int i = 0; i < circuit.nBinRows; ++i)
  {
    for (int j = 0; j < circuit.nBinCols; ++j)
    {
      if (circuit.arrOfBins[i][j]->nodes.size() == 0)
      {
        cout << "Check-up IsNotEmpty FAILED!\n";
        cout << "Bin["<< i << "][" << j << "] is empty!\n";
        return EMPTY_BINS;
      }
    }
  }
  return OK;
}

int CheckXEquality(const Circuit& circuit)
{
  for (int i = 0; i < circuit.nBinRows; ++i)
  {
    for (int j = 0; j < circuit.nBinCols; ++j)
    {
      for (int k = 0; k < static_cast<int>(circuit.arrOfBins[i][j]->nodes.size()); ++k)
      {
        if (fabs(circuit.arrOfBins[i][j]->xCoord - 
            circuit.placement[circuit.arrOfBins[i][j]->nodes[k]].xCoord) >= 1e-3)
        {
           cout << "Check-up CheckXEquality FAILED!\n";
           cout << "X-coordinates of cells in Bin["<< i << "][" << j << "] are different!\n";
           cout << "bin center " << "\t x: "
                << circuit.arrOfBins[i][j]->xCoord << "\n";
           cout << "cell " << circuit.arrOfBins[i][j]->nodes[k] << "\t x: "
                << circuit.placement[circuit.arrOfBins[i][j]->nodes[k]].xCoord << "\n";
           return X_COORDS_UNEQUAL;
        }
      }
    }
  }
  return OK;
}

int CheckYEquality(const Circuit& circuit)
{
  int binNodesSize;
  for (int i = 0; i < circuit.nBinRows; ++i)
  {
    for (int j = 0; j < circuit.nBinCols; ++j)
    {
      binNodesSize = static_cast<int>(circuit.arrOfBins[i][j]->nodes.size());
      for (int k = 0; k < binNodesSize; ++k)
      {
        if (fabs(circuit.arrOfBins[i][j]->yCoord - 
            circuit.placement[circuit.arrOfBins[i][j]->nodes[k]].yCoord) >= 1e-10)
        {
           cout << "Check-up CheckYEquality FAILED!\n";
           cout << "Y-coordinates of cells in Bin["<< i << "][" << j << "] are different!\n";
           cout << "bin center " << "\t y: "
                << circuit.arrOfBins[i][j]->yCoord << "\n";
           cout << "cell " << circuit.arrOfBins[i][j]->nodes[k] << "\t y: "
                << circuit.placement[circuit.arrOfBins[i][j]->nodes[k]].yCoord << "\n";
           return Y_COORDS_UNEQUAL;
        }
      }
    }
  }
  return OK;
}

int CheckOrder(const Circuit& circuit)
{
  for (int i = 1; i < circuit.nBinRows; ++i)
  {
    for (int j = 1; j < circuit.nBinCols; ++j)
    {
      if (circuit.arrOfBins[i][j-1]->xCoord >= 
          circuit.arrOfBins[i][j]->xCoord)
      {
        cout << "X coordinate of Bin["<< i << "][" << j - 1 << "]\tx: "
             << circuit.arrOfBins[i][j-1]->xCoord << "\texceeds\n";
        cout << "X coordinate of Bin["<< i << "][" << j << "]\tx: "
             << circuit.arrOfBins[i][j]->xCoord << "\n";
        return WRONG_BINS_INDEXATION;
      }
      if (circuit.arrOfBins[i-1][j]->yCoord >= 
          circuit.arrOfBins[i][j]->yCoord)
      {
        cout << "Y coordinate of Bin["<< i - 1 << "][" << j << "]\ty: "
             << circuit.arrOfBins[i-1][j]->yCoord << "\texceeds\n";
        cout << "Y coordinate of Bin["<< i << "][" << j << "]\ty: "
             << circuit.arrOfBins[i][j]->yCoord << "\n";
        return WRONG_BINS_INDEXATION;
      }
    }
  }
  return OK;
}

MULTIPLACER_ERROR CheckBinsOnMacros(const Circuit& circuit)
{
  for (int i = 0; i < circuit.nBinRows; ++i)
  {
    for (int j = 0; j < circuit.nBinCols; ++j)
    {
      if (circuit.arrOfBins[i][j]->nodes.size() != 0 && circuit.arrOfBins[i][j]->isOnMacroBlock == true)
      {
        cout << "Bin[" << i << "][" << j << "] is on macro-block and is not empty\n";
        return BINS_ON_MACROS;
      }
    }
  }
  return OK;
}

int CheckArrOfSites(const Circuit& circuit, int** arrOfSites)
{
  int **newArrOfSites;
  int numOfSites = circuit.rows[circuit.nRows/2].numSites;
  int siteHeight = circuit.rows[0].height;
  int siteWidth  = circuit.rows[0].siteWidth;
  newArrOfSites = new int*[circuit.nRows];
  
  for (int j = 0; j < circuit.nRows; ++j)
  {
    newArrOfSites[j] = new int[numOfSites];
  }
  
  for (int i = 0; i < circuit.nRows; ++i)
  {
    for (int j = 0; j < numOfSites; ++j)
    {
      newArrOfSites[i][j] = -1;
    }
  }
  
  int currRow;
  int currSite;
  //Filling sites with cell indexes if they are occupied
  for (int i = 0; i < circuit.nNodes; ++i)
  {
    currRow = static_cast<int>( floor(circuit.placement[i].yCoord / siteHeight) );
    currSite = static_cast<int>((circuit.placement[i].xCoord - 0.5 * circuit.nodes[i].width) / siteWidth);
    for (int t = currSite; t < currSite + (circuit.nodes[i].width / siteWidth); ++t)
    {
      newArrOfSites[currRow][t] = i;
    }
  }
  
  bool isVariance = false;
  for (int i = 0; i < circuit.nRows; ++i)
  {
    for (int j = 0; j < numOfSites; ++j)
    {
      if (newArrOfSites[i][j] != arrOfSites[i][j])
      {
        cout << "arrOfSites is illegal in row\t" << i << "\tsite\t" << j << "\n";
        isVariance = true;
        break;
      }
    }
    if (isVariance) break;
  }
  
  for (int i = 0; i < circuit.nRows; ++i)
    delete[] newArrOfSites[i];
  delete[] newArrOfSites;
  
  if (isVariance) return 1;
  
  return OK;
}

MULTIPLACER_ERROR CheckRowLength(const Circuit& circuit, double availableRowLength, int rowIdx)
{
  double rowLength1 = 0.0;
  double rowLength2 = 0.0;
  for (int colIdx = 0; colIdx < circuit.nBinCols; ++colIdx)
  {
    for (int i = 0; i < static_cast<int>(circuit.arrOfBins[rowIdx][colIdx]->nodes.size()); ++i)
    {
      rowLength1 += circuit.nodes[circuit.arrOfBins[rowIdx][colIdx]->nodes[i]].width;
    }
    rowLength2 += circuit.arrOfBins[rowIdx][colIdx]->sumLength;
  }
  if (rowLength1 != rowLength2)
  {
    cout << "Length error in row " << rowIdx << endl;
    return ROW_LENGTH_EXCESS;
  }
  
  if (rowLength1 > availableRowLength)
  {
    cout << "Excess in row " << rowIdx << endl;
    return ROW_LENGTH_EXCESS;
  }
  
  return OK;
}

int PrintBinsSize(const Circuit& circuit)
{
  for (int i = 0; i < circuit.nBinRows; i++ )
  {
    for (int j = 0; j < circuit.nBinCols; ++j )
      cout << static_cast<int>(circuit.arrOfBins[i][j]->nodes.size()) << "\t";
    cout << "\n";
  }

  return OK;
}

int PrintBinsCoordinates(const Circuit& circuit)
{
  for (int i = 0; i < circuit.nBinRows; i++ )
  {
    for (int j = 0; j < circuit.nBinCols; ++j )
      cout << "bin[" << i <<"]["<< j << "]:("
           << circuit.arrOfBins[i][j]->xCoord
           << ";" << circuit.arrOfBins[i][j]->yCoord
           << ")\t";
    cout << "\n";
  }

  return OK;
}