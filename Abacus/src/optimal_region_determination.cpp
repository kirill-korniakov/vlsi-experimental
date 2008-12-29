/*  
 * optimal_region_determination.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2006, ITLab, Zhivoderov
 * email: zhivoderov.a@gmail.com
 */
 
#include "..\include\optimal_region_determination.h"
#include <algorithm>
//#include <float.h>

struct Bound
{
  double xCoord;
  int netIdx;
};

int GetOptimalX(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListX,
              int numOfTreatedNets)
{
  vector<double> xSeries;

  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double lborder, rborder;
  double temp;
  int idx;
  int pinIdx = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < numOfNodes; ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[nodesIdx[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[nodesIdx[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      if (nets[netsIdx[i]].numOfPins == 1)
        continue;
      pinIdx = 0;
      if (nets[netsIdx[i]].arrPins[pinIdx].cellIdx == nodesIdx[k])
      {
        pinIdx = 1;
      }
      idx  = nets[netsIdx[i]].arrPins[pinIdx].cellIdx;
      lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset;
      for (j = pinIdx; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (idx == nodesIdx[k]) continue;
        
        temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
        (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      }
      xSeries.push_back( static_cast<int>(lborder) );
      xSeries.push_back( static_cast<int>(rborder) );
    }
  }
  //1.2 Calculate the series' medians
  sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  
  if (borderListX != NULL) *borderListX = xSeries;
  
  return (idx == 0) ? (static_cast<int>( xSeries[idx] )) : static_cast<int>( (xSeries[idx] + xSeries[idx-1]) / 2 );
}

int GetOptimalY(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListY,
              int numOfTreatedNets)
{
  vector<double> ySeries;

  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double tborder, bborder;
  double temp;
  int idx;
  int pinIdx = 0;
  int tocSize;
  int currNetIdx;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < numOfNodes; ++k)
  {
    netsIdx.clear();
    tocSize = static_cast<int>(tableOfConnections[nodesIdx[k]].size());
    for (t = 0; t < tocSize; ++t)
      netsIdx.push_back(tableOfConnections[nodesIdx[k]][t]);

    numOfConnectedNets = static_cast<int>(netsIdx.size());
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      currNetIdx = netsIdx[i];
      if (nets[currNetIdx].numOfPins == 1)
        continue;
      pinIdx = 0;
      if (nets[currNetIdx].arrPins[pinIdx].cellIdx == nodesIdx[k])
      {
        pinIdx = 1;
      }
      idx  = nets[currNetIdx].arrPins[pinIdx].cellIdx;
      tborder = bborder = placement[idx].yCoord + nets[currNetIdx].arrPins[pinIdx].yOffset;
      for (j = pinIdx; j < nets[currNetIdx].numOfPins; ++j)
      {
        idx  = nets[currNetIdx].arrPins[j].cellIdx;
        if (idx == nodesIdx[k]) continue;
        
        temp = placement[idx].yCoord + nets[currNetIdx].arrPins[j].yOffset;
        (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
      }
      ySeries.push_back( static_cast<int>(tborder) );
      ySeries.push_back( static_cast<int>(bborder) );
    }
  }
  //1.2 Calculate the series' medians 
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  
  if (borderListY != NULL) *borderListY = ySeries;
  
  return (idx == 0) ? (static_cast<int>( ySeries[idx] )) : static_cast<int>( (ySeries[idx] + ySeries[idx-1]) / 2 );
}

int GetOptimalYRude(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListY,
              int numOfTreatedNets, int circuitNNodes)
{
  vector<double> ySeries;

  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double tborder, bborder;
  double temp;
  int idx;
  int pinIdx = 0;
  int tocSize;
  int currNetIdx;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < numOfNodes; ++k)
  {
    netsIdx.clear();
    tocSize = static_cast<int>(tableOfConnections[nodesIdx[k]].size());
    for (t = 0; t < tocSize; ++t)
      netsIdx.push_back(tableOfConnections[nodesIdx[k]][t]);

    numOfConnectedNets = static_cast<int>(netsIdx.size());
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      currNetIdx = netsIdx[i];
      if (nets[currNetIdx].numOfPins == 1)
        continue;
      pinIdx = 0;
      if (nets[currNetIdx].arrPins[pinIdx].cellIdx == nodesIdx[k])
      {
        pinIdx = 1;
      }
      idx  = nets[currNetIdx].arrPins[pinIdx].cellIdx;
      if (idx < circuitNNodes)
        tborder = bborder = placement[idx].yCoord;
      else
        tborder = bborder = placement[idx].yCoord + nets[currNetIdx].arrPins[pinIdx].yOffset;
      for (j = pinIdx; j < nets[currNetIdx].numOfPins; ++j)
      {
        idx  = nets[currNetIdx].arrPins[j].cellIdx;
        if (idx == nodesIdx[k]) continue;
        
        if (idx < circuitNNodes)
          temp = placement[idx].yCoord;
        else
          temp = placement[idx].yCoord + nets[currNetIdx].arrPins[j].yOffset;
        (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
      }
      ySeries.push_back( static_cast<int>(tborder) );
      ySeries.push_back( static_cast<int>(bborder) );
    }
  }
  //1.2 Calculate the series' medians 
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  
  if (borderListY != NULL) *borderListY = ySeries;
  
  return (idx == 0) ? (static_cast<int>( ySeries[idx] )) : static_cast<int>( (ySeries[idx] + ySeries[idx-1]) / 2 );
}

void GetOptimalXY(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListX,
              vector<double>* borderListY,
              double* xOpt,
              double* yOpt,
              int numOfTreatedNets)
{
  vector<double> xSeries;
  vector<double> ySeries;
  
  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double lborder, rborder, tborder, bborder;
  double temp;
  int idx;
  int pinIdx = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < numOfNodes; ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[nodesIdx[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[nodesIdx[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      if (nets[netsIdx[i]].numOfPins == 1)
        continue;
      pinIdx = 0;
      if (nets[netsIdx[i]].arrPins[pinIdx].cellIdx == nodesIdx[k])
      {
        pinIdx = 1;
      }
      idx  = nets[netsIdx[i]].arrPins[pinIdx].cellIdx;
      lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset,
      bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset;
      for (j = pinIdx; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (idx == nodesIdx[k]) continue;
        
        temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
        (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
        temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
        (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
      }
      xSeries.push_back( static_cast<int>(lborder) );
      xSeries.push_back( static_cast<int>(rborder) );
      ySeries.push_back( static_cast<int>(tborder) );
      ySeries.push_back( static_cast<int>(bborder) );
    }
  }
  //1.2 Calculate the series' medians
  sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  if (idx == 0)
    *xOpt = xSeries[idx];
  else
    *xOpt = (xSeries[idx] + xSeries[idx-1]) / 2;
  
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  if (idx == 0)
    *yOpt = ySeries[idx];
  else
    *yOpt = (ySeries[idx] + ySeries[idx-1]) / 2;
  
  if (borderListX != NULL) *borderListX = xSeries;
  if (borderListY != NULL) *borderListY = ySeries;
}

void GetOptimalXY41( int nodesIdx, 
                     const int numOfNets, Net* const nets, 
                     Place* placement,
                     vector<int>* tableOfConnections,
                     double& xOpt,
                     double& yOpt )
{
  static vector<double> xSeries;
  static vector<double> ySeries;
  
  static int i, j, t;
  
  //1 Calculate the optimal region for cells
  static double lborder, rborder, tborder, bborder;
  static double temp;
  static int idx;
  static int pinIdx;
  
  // 1.1 Gather series of x and y borders
  xSeries.clear();
  ySeries.clear();
  for (i = 0; i < static_cast<int>(tableOfConnections[nodesIdx].size()); ++i)
  {
    if (nets[tableOfConnections[nodesIdx][i]].numOfPins == 1)
        continue;
    pinIdx = 0;
    if (nets[tableOfConnections[nodesIdx][i]].arrPins[pinIdx].cellIdx == nodesIdx)
    {
      pinIdx = 1;
    }
    idx  = nets[tableOfConnections[nodesIdx][i]].arrPins[pinIdx].cellIdx;
    lborder = placement[idx].xCoord;
    rborder = placement[idx].xCoord;
    tborder = placement[idx].yCoord;
    bborder = placement[idx].yCoord;
    for (j = pinIdx; j < nets[tableOfConnections[nodesIdx][i]].numOfPins; ++j)
    {
      idx = nets[tableOfConnections[nodesIdx][i]].arrPins[j].cellIdx;
      if (idx == nodesIdx) continue;
      
      temp = placement[idx].xCoord;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }
    xSeries.push_back( static_cast<int>(lborder) );
    xSeries.push_back( static_cast<int>(rborder) );
    ySeries.push_back( static_cast<int>(tborder) );
    ySeries.push_back( static_cast<int>(bborder) );
  }

  //1.2 Calculate the series' medians
  sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  if (idx == 0) xOpt = xSeries[0];
  else          xOpt = (xSeries[idx] + xSeries[idx-1]) / 2;
  
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  if (idx == 0) yOpt = ySeries[idx];
  else          yOpt = (ySeries[idx] + ySeries[idx-1]) / 2;
}

int GetOptimalX(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListX,
              int numOfTreatedNets)
{
  vector<double> xSeries;
  
  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double lborder, rborder;
  double temp;
  int idx;
  int pinIdx = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < static_cast<int>(nodesIdx->size()); ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[(*nodesIdx)[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[(*nodesIdx)[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      if (nets[netsIdx[i]].numOfPins == 1)
        continue;
      pinIdx = 0;
      if (nets[netsIdx[i]].arrPins[pinIdx].cellIdx == (*nodesIdx)[k])
      {
        pinIdx = 1;
      }
      idx  = nets[netsIdx[i]].arrPins[pinIdx].cellIdx;
      lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset;
      for (j = pinIdx; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (idx == (*nodesIdx)[k]) continue;
        
        temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
        (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      }
      xSeries.push_back( static_cast<int>(lborder) );
      xSeries.push_back( static_cast<int>(rborder) );
    }
  }
  //1.2 Calculate the series' medians
  sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  
  if (borderListX != NULL) *borderListX = xSeries;
  
  return (idx == 0) ? (static_cast<int>( xSeries[idx] )) : static_cast<int>( (xSeries[idx] + xSeries[idx-1]) / 2 );
}

int GetOptimalY(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListY,
              int numOfTreatedNets)
{
  vector<double> ySeries;

  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double tborder, bborder;
  double temp;
  int idx;
  int pinIdx = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < static_cast<int>(nodesIdx->size()); ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[(*nodesIdx)[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[(*nodesIdx)[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      if (nets[netsIdx[i]].numOfPins == 1)
        continue;
      pinIdx = 0;
      if (nets[netsIdx[i]].arrPins[pinIdx].cellIdx == (*nodesIdx)[k])
      {
        pinIdx = 1;
      }
      idx  = nets[netsIdx[i]].arrPins[pinIdx].cellIdx;
      tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset,
      bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset;
      for (j = pinIdx; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (idx == (*nodesIdx)[k]) continue;
        
        temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
        (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
      }
      ySeries.push_back( static_cast<int>(tborder) );
      ySeries.push_back( static_cast<int>(bborder) );
    }
  }
  //1.2 Calculate the series' medians
  
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  
  if (borderListY != NULL) *borderListY = ySeries;
  
  return (idx == 0) ? (static_cast<int>( ySeries[idx] )) : static_cast<int>( (ySeries[idx] + ySeries[idx-1]) / 2 );
}

void GetOptimalXY(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListX,
              vector<double>* borderListY,
              double* xOpt,
              double* yOpt,
              int numOfTreatedNets)
{
  vector<double> xSeries;
  vector<double> ySeries;

  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double lborder, rborder, tborder, bborder;
  double temp;
  int idx;
  int pinIdx = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < static_cast<int>(nodesIdx->size()); ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[(*nodesIdx)[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[(*nodesIdx)[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      if (nets[netsIdx[i]].numOfPins == 1)
        continue;
      pinIdx = 0;
      if (nets[netsIdx[i]].arrPins[pinIdx].cellIdx == (*nodesIdx)[k])
      {
        pinIdx = 1;
      }
      idx  = nets[netsIdx[i]].arrPins[pinIdx].cellIdx;
      lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset,
      bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset;
      for (j = pinIdx; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (idx == (*nodesIdx)[k]) continue;
        
        temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
        (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
        temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
        (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
      }
      xSeries.push_back( static_cast<int>(lborder) );
      xSeries.push_back( static_cast<int>(rborder) );
      ySeries.push_back( static_cast<int>(tborder) );
      ySeries.push_back( static_cast<int>(bborder) );
    }
  }
  //1.2 Calculate the series' medians
  
  sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  if (idx == 0)
    *xOpt = xSeries[idx];
  else
    *xOpt = (xSeries[idx] + xSeries[idx-1]) / 2;
  
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  if (idx == 0)
    *yOpt = ySeries[idx];
  else
    *yOpt = (ySeries[idx] + ySeries[idx-1]) / 2;
   
  if (borderListX != NULL) *borderListX = xSeries;
  if (borderListY != NULL) *borderListY = ySeries;
}

void GetOptimalInterval(int* nodesIdx, int numOfNodes,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              double* xL,
              double* xR)
{
  vector<double> lSeries;   // for sorted list of borders where the WL function changes the slope
  vector<double> rSeries;   // for sorted list of borders where the WL function changes the slope
  vector<double> boundsList;// list of all x-coords where the WL function changes the slope
  vector<int> fixedCells;
  vector<int> movableCells;
  bool isIntervalFound;
  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double minMovX;
  double maxMovX;
  double minFixX;
  double maxFixX;
  int    leftMovableIdx;
  int    rightMovableIdx;
  int    leftFixedIdx;
  int    rightFixedIdx;
  int idx;
  int pinIdx = 0;
  int numOfVariances = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < numOfNodes; ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[nodesIdx[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[nodesIdx[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      fixedCells.clear();
      movableCells.clear();
      // Gather series of movable and fixed cells
      for (j = 0; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (placement[idx].yCoord == placement[nodesIdx[k]].yCoord)
          movableCells.push_back( idx );
        else
          fixedCells.push_back( idx );
      }
      
      minFixX = 99999999999.0;
      maxFixX = 0.0;
      minMovX = 99999999999.0;
      maxMovX = 0.0;
      leftFixedIdx = -1;
      rightFixedIdx = -1;
      leftMovableIdx = -1;
      rightMovableIdx = -1;
      for (j = 0; j < static_cast<int>(fixedCells.size()); ++j)
      {
        if (minFixX > placement[fixedCells[j]].xCoord)
        {
          leftFixedIdx = fixedCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == leftFixedIdx) break;
          }
          
          minFixX = placement[leftFixedIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
        if (maxFixX < placement[fixedCells[j]].xCoord)
        {
          rightFixedIdx = fixedCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == rightFixedIdx) break;
          }
          
          maxFixX = placement[rightFixedIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
      }
      for (j = 0; j < static_cast<int>(movableCells.size()); ++j)
      {
        if (minMovX > placement[movableCells[j]].xCoord)
        {
          leftMovableIdx = movableCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == leftMovableIdx) break;
          }
          
          minMovX = placement[leftMovableIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
        if (maxMovX < placement[movableCells[j]].xCoord)
        {
          rightMovableIdx = movableCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == rightMovableIdx) break;
          }
          
          maxMovX = placement[rightMovableIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
      }
      if (minFixX != 99999999999.0 && leftMovableIdx == nodesIdx[k])
      {
        lSeries.push_back(minFixX);
      }
      if (maxFixX != 0.0 && rightMovableIdx == nodesIdx[k])
      {
        rSeries.push_back(maxFixX);
      }
    }
  }
  sort(lSeries.begin(), lSeries.end());
  sort(rSeries.begin(), rSeries.end());
  boundsList.resize( lSeries.size() + rSeries.size() );
  merge(lSeries.begin(), lSeries.end(),
        rSeries.begin(), rSeries.end(),
        boundsList.begin());
  // Determine the number of variances for each derived interval
  // supposing that all pins are in one point middleX - in the middle of each interval
  double middleX;
  isIntervalFound = false;
  for (t = 0; t < static_cast<int>(boundsList.size()) - 1; ++t)
  {
    numOfVariances = 0;
    middleX = ( boundsList[t] + boundsList[t+1] ) / 2;
    for (i = 0; i < static_cast<int>( lSeries.size() ); ++i)
    {
      if (lSeries[i] > middleX) numOfVariances++;
      //else break;
    }
    for (i = 0; i < static_cast<int>( rSeries.size() ); ++i)
    {
      if (rSeries[i] < middleX) numOfVariances--;
      //else break;
    }
    if (numOfVariances == 0)
    {
      isIntervalFound = true;
      break;
    }
  }
  if (isIntervalFound == true)
  {
    *xL = boundsList[t];
    *xR = boundsList[t+1];
  }
  else
  {
    if (lSeries.size() > 0) 
      *xL = lSeries[lSeries.size() - 1];
    else *xL = 0;
    
    if (rSeries.size() > 0)
      *xR = rSeries[0];
    //else *xR = 0;
  }
  //1.2 Calculate the series' medians
  /*sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  *xL = (xSeries[idx] + xSeries[idx-1]) / 2;
  
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  *xR = (ySeries[idx] + ySeries[idx-1]) / 2;
  
  if (borderListX != NULL) *borderListX = xSeries;
  if (borderListY != NULL) *borderListY = ySeries;*/ 
}

void GetOptimalInterval(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              double* xL,
              double* xR)
{
  vector<double> lSeries;   //for sorted list of borders where the WL function changes the slope
  vector<double> rSeries;   //for sorted list of borders where the WL function changes the slope
  vector<double> boundsList;
  //vector<double> leftBorderList;   //absolutely all left borders of nets for current set of nodes
  //vector<double> rightBorderList;  //absolutely all right borders of nets for current set of nodes
  vector<int> lNets;
  vector<int> rNets;
  vector<int> fixedCells;
  vector<int> movableCells;
  bool isIntervalFound;
  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  //Bound newBound;
  //double lborder, rborder;
  //double temp;
  double minMovX;
  double maxMovX;
  double minFixX;
  double maxFixX;
  int    leftMovableIdx;
  int    rightMovableIdx;
  int    leftFixedIdx;
  int    rightFixedIdx;
  int idx;
  int pinIdx = 0;
  //int currArrPinsIdx;
  int numOfVariances = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < static_cast<int>(nodesIdx->size()); ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[(*nodesIdx)[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[(*nodesIdx)[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      fixedCells.clear();
      movableCells.clear();
      for (j = 0; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (placement[idx].yCoord == placement[(*nodesIdx)[k]].yCoord)
          movableCells.push_back( idx );
        else
          fixedCells.push_back( idx );
        /*if (idx == (*nodesIdx)[k]) continue;
        
        temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
        (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;*/
      }
    
      minFixX = 99999999999.0;
      maxFixX = 0.0;
      minMovX = 99999999999.0;
      maxMovX = 0.0;
      leftFixedIdx = -1;
      rightFixedIdx = -1;
      leftMovableIdx = -1;
      rightMovableIdx = -1;
      for (j = 0; j < static_cast<int>(fixedCells.size()); ++j)
      {
        if (minFixX > placement[fixedCells[j]].xCoord)
        {
          leftFixedIdx = fixedCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == leftFixedIdx) break;
          }
          
          minFixX = placement[leftFixedIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
        if (maxFixX < placement[fixedCells[j]].xCoord)
        {
          rightFixedIdx = fixedCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == rightFixedIdx) break;
          }
          
          maxFixX = placement[rightFixedIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
      }
      for (j = 0; j < static_cast<int>(movableCells.size()); ++j)
      {
        if (minMovX > placement[movableCells[j]].xCoord)
        {
          leftMovableIdx = movableCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == leftMovableIdx) break;
          }
          
          minMovX = placement[leftMovableIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
        if (maxMovX < placement[movableCells[j]].xCoord)
        {
          rightMovableIdx = movableCells[j];
          
          for (t = 0; t < nets[netsIdx[i]].numOfPins; ++t)
          {
            if (nets[netsIdx[i]].arrPins[t].cellIdx == rightMovableIdx) break;
          }
          
          maxMovX = placement[rightMovableIdx].xCoord + nets[netsIdx[i]].arrPins[t].xOffset;
        }
      }
      if (minFixX != 99999999999.0 && leftMovableIdx == (*nodesIdx)[k])
      {
        /*newBound.xCoord = minFixX;
        newBound.netIdx = netsIdx[i];*/
        lSeries.push_back(minFixX);
        //lNets.push_back(netsIdx[i]);
        //leftBorderList.push_back(minFixX);
        /*if (minFixX > minMovX)
          numOfVariances--;*/
      }
      if (maxFixX != 0.0 && rightMovableIdx == (*nodesIdx)[k])
      {
        /*newBound.xCoord = maxFixX;
        newBound.netIdx = netsIdx[i];*/
        rSeries.push_back(maxFixX);
        //rNets.push_back(netsIdx[i]);
        //rightBorderList.push_back(maxFixX);
        /*if (maxFixX < maxMovX)
          numOfVariances++;*/
      }
      /*if (minFixX == maxFixX)
      {
        rSeries.pop_back();
      }*/
    }
    sort(lSeries.begin(), lSeries.end());
    sort(rSeries.begin(), rSeries.end());
    boundsList.resize( lSeries.size() + rSeries.size() );
    merge(lSeries.begin(), lSeries.end(),
          rSeries.begin(), rSeries.end(),
          boundsList.begin());
  }
  // Determine the number of variances for each derived interval
  // supposing that all pins are in one point middleX - in the middle of each interval
  double middleX;
  isIntervalFound = false;
  for (t = 0; t < static_cast<int>(boundsList.size()) - 1; ++t)
  {
    numOfVariances = 0;
    middleX = ( boundsList[t] + boundsList[t+1] ) / 2;
    for (i = 0; i < static_cast<int>( lSeries.size() ); ++i)
    {
      if (lSeries[i] > middleX) numOfVariances++;
      //else break;
    }
    for (i = 0; i < static_cast<int>( rSeries.size() ); ++i)
    {
      if (rSeries[i] < middleX) numOfVariances--;
      //else break;
    }
    if (numOfVariances == 0)
    {
      isIntervalFound = true;
      break;
    }
  }
  if (isIntervalFound == true)
  {
    *xL = boundsList[t];
    *xR = boundsList[t+1];
  }
  else
  {
    if (lSeries.size() > 0) 
      *xL = lSeries[lSeries.size() - 1];
    else *xL = 0;
    
    if (rSeries.size() > 0)
      *xR = rSeries[0];
    //else *xR = 0;
  }
  //1.2 Calculate the series' medians
  /*sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  *xL = (xSeries[idx] + xSeries[idx-1]) / 2;
  
  sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);
  *xR = (ySeries[idx] + ySeries[idx-1]) / 2;
  
  if (borderListX != NULL) *borderListX = xSeries;
  if (borderListY != NULL) *borderListY = ySeries;*/ 
}

void GetOptimalInterval2(vector<int>* nodesIdx,
              Place* placement,
              const int numOfNets, Net* const nets, 
              vector<int>* tableOfConnections,
              vector<double>* borderListX,
              vector<double>* borderListY,
              double* xL,
              double* xR,
              int numOfTreatedNets)
{
  vector<double> xSeries;
  vector<double> ySeries;

  int i, j, k, t;
  int numOfConnectedNets;  //number of nets which current cell belongs to
  //1 Calculate the optimal region for cells
  vector<int> netsIdx;
  vector<int> tempvect;
  double lborder, rborder, tborder, bborder;
  double temp;
  int idx;
  int pinIdx = 0;
  // getting indexes of nets using which current cell belongs to
  // duplicating indexes allowed
  for (k = 0; k < static_cast<int>(nodesIdx->size()); ++k)
  {
    netsIdx.clear();
    for (t = 0; t < static_cast<int>(tableOfConnections[(*nodesIdx)[k]].size()); ++t)
      netsIdx.push_back(tableOfConnections[(*nodesIdx)[k]][t]);

    numOfConnectedNets = static_cast<int>( netsIdx.size() );
    //1.1 Gather series of x and y borders
    for (i = 0; i < numOfConnectedNets; ++i)
    {
      pinIdx = 0;
      if (nets[netsIdx[i]].arrPins[pinIdx].cellIdx == (*nodesIdx)[k])
      {
        pinIdx = 1;
      }
      idx  = nets[netsIdx[i]].arrPins[pinIdx].cellIdx;
      lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[pinIdx].xOffset,
      tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset,
      bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[pinIdx].yOffset;
      for (j = pinIdx; j < nets[netsIdx[i]].numOfPins; ++j)
      {
        idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
        if (idx == (*nodesIdx)[k]) continue;
        
        temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
        (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
        temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
        (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
      }
      xSeries.push_back( static_cast<int>(lborder) );
      xSeries.push_back( static_cast<int>(rborder) );
      ySeries.push_back( static_cast<int>(tborder) );
      ySeries.push_back( static_cast<int>(bborder) );
    }
  }
  //1.2 Calculate the series' medians
  
  sort(xSeries.begin(), xSeries.end());
  idx = static_cast<int>(xSeries.size() / 2);
  *xL = xSeries[idx];
  *xR = xSeries[idx+1];
  /**xOpt = (xSeries[idx] + xSeries[idx-1]) / 2;*/
  
 /* sort(ySeries.begin(), ySeries.end());
  idx = static_cast<int>(ySeries.size() / 2);*/
  /**yOpt = (ySeries[idx] + ySeries[idx-1]) / 2;*/
   
  if (borderListX != NULL) *borderListX = xSeries;
  if (borderListY != NULL) *borderListY = ySeries;
}