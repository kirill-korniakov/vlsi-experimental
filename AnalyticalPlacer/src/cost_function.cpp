/*  
 * cost_function.cpp
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, ITLab, Aleksey Bader, Nina Kurina
 * email: bader@newmail.ru
 * email: ninku@mail.ru
 * last modified 07 February 2006
 */

#include "..\include\cost_function.h"
#include <algorithm>
#include "math.h"

//using namespace MultilevelFramework;

// all functions calculate wirelength for current placement  

double cf_recalc_all(bool flag, const int numOfNets, Net* const nets, 
       const Place* const placement) 
{
  int  i, j; // loop counters
  int  idx;  // index of a cell
  // borders of bounding rectangular
  double lborder, rborder, tborder, bborder;
  double temp;
  double totalWL = 0; //total wirelength
  // recalculating half-perimeter of bounding rectangular for each net
  for (i = 0; i < numOfNets; ++i)
  {
    idx = nets[i].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord + nets[i].arrPins[0].xOffset,
    rborder = placement[idx].xCoord + nets[i].arrPins[0].xOffset,
    tborder = placement[idx].yCoord + nets[i].arrPins[0].yOffset,
    bborder = placement[idx].yCoord + nets[i].arrPins[0].yOffset;
    for (j = 1; j < nets[i].numOfPins; ++j)
    {
      idx  = nets[i].arrPins[j].cellIdx;
      temp = placement[idx].xCoord + nets[i].arrPins[j].xOffset;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord + nets[i].arrPins[j].yOffset;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if (flag)// we change nets[i].currWL
    {    
      nets[i].currWL = rborder + tborder - lborder - bborder;
      totalWL += nets[i].currWL;
    }
    else // we do not change nets[i].currWL
    {
      totalWL += rborder + tborder - lborder - bborder;
    }
  } // for (i = 0; i < numOfNets; ++i)
  return totalWL;
}


double cf_recalc_some_nets( bool flag, const int numOfNets, Net* const nets,
         double currentWL, const int* const netsIdx, 
         int numOfChangedNets, const Place* const placement)
{
  int  i, j; // loop counters
  int  idx;  // index of a cell
  // borders of bounding rectangular
  double lborder, rborder, tborder, bborder;
  double temp;
  // recalculating half-perimeter of bounding rectangular for each of changed nets
  for (i = 0; i < numOfChangedNets; ++i)
  {
    idx = nets[netsIdx[i]].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset,
    bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset;
    for (j = 1; j < nets[netsIdx[i]].numOfPins; ++j)
    {
      idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
      temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if ( flag )// we change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      nets[netsIdx[i]].currWL = rborder + tborder - lborder - bborder;
      currentWL += nets[netsIdx[i]].currWL;
    }
    else // we do not change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      currentWL += rborder + tborder - lborder - bborder;

    }
  } // for (i = 0; i < numOfChangedNets; ++i)
  return currentWL;
}

double rude_cf_recalc_some_nets( bool flag, Net* const nets,
                                 double currentWL, 
                                 const int* const netsIdx1, int numOfChangedNets1,
                                 const int* const netsIdx2, int numOfChangedNets2,
                                 const Place* const placement )
{
  int  i, j; // loop counters
  int  idx;  // index of a cell
  // borders of bounding rectangular
  double lborder, rborder, tborder, bborder;
  double temp;
  int currNetIdx1;
  int currNetIdx2;
  // recalculating half-perimeter of bounding rectangular for each of changed nets
  for (i = 0; i < numOfChangedNets1; ++i)
  {
    currNetIdx1 = netsIdx1[i];
    idx = nets[currNetIdx1].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord;
    rborder = placement[idx].xCoord;
    tborder = placement[idx].yCoord;
    bborder = placement[idx].yCoord;
    for (j = 1; j < nets[currNetIdx1].numOfPins; ++j)
    {
      idx  = nets[currNetIdx1].arrPins[j].cellIdx;
      temp = placement[idx].xCoord;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if ( flag )// we change nets[i].currWL
    {
      currentWL -= nets[currNetIdx1].currWL;
      nets[currNetIdx1].currWL = rborder + tborder - lborder - bborder;
      currentWL += rborder + tborder - lborder - bborder;
    }
    else // we do not change nets[i].currWL
    {
      currentWL -= nets[currNetIdx1].currWL;
      currentWL += rborder + tborder - lborder - bborder;
    }
  } // for (i = 0; i < numOfChangedNets1; ++i)
  
  for (i = 0; i < numOfChangedNets2; ++i)
  {
    currNetIdx2 = netsIdx2[i];
    idx = nets[currNetIdx2].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord;
    rborder = placement[idx].xCoord;
    tborder = placement[idx].yCoord;
    bborder = placement[idx].yCoord;
    for (j = 1; j < nets[currNetIdx2].numOfPins; ++j)
    {
      idx  = nets[currNetIdx2].arrPins[j].cellIdx;
      temp = placement[idx].xCoord;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if ( flag )// we change nets[i].currWL
    {
      currentWL -= nets[currNetIdx2].currWL;
      nets[currNetIdx2].currWL = rborder + tborder - lborder - bborder;
      currentWL += rborder + tborder - lborder - bborder;
    }
    else // we do not change nets[i].currWL
    {
      currentWL -= nets[currNetIdx2].currWL;
      currentWL += rborder + tborder - lborder - bborder;
    }
  } // for (i = 0; i < numOfChangedNets1; ++i)

  return currentWL;
}

double rude_cf_recalc_some_nets( bool flag, Net* const nets,
                                 double currentWL, 
                                 const int* const netsIdx, int numOfChangedNets,
                                 const Place* const placement )
{
  int  i, j; // loop counters
  int  idx;  // index of a cell
  // borders of bounding rectangular
  double lborder, rborder, tborder, bborder;
  double temp;
  int currNetIdx;
  // recalculating half-perimeter of bounding rectangular for each of changed nets
  for (i = 0; i < numOfChangedNets; ++i)
  {
    currNetIdx = netsIdx[i];
    idx = nets[currNetIdx].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord;
    rborder = placement[idx].xCoord;
    tborder = placement[idx].yCoord;
    bborder = placement[idx].yCoord;
    for (j = 1; j < nets[currNetIdx].numOfPins; ++j)
    {
      idx  = nets[currNetIdx].arrPins[j].cellIdx;
      temp = placement[idx].xCoord;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if ( flag )// we change nets[i].currWL
    {
      currentWL -= nets[currNetIdx].currWL;
      nets[currNetIdx].currWL = rborder + tborder - lborder - bborder;
      currentWL += rborder + tborder - lborder - bborder;
    }
    else // we do not change nets[i].currWL
    {
      currentWL -= nets[currNetIdx].currWL;
      currentWL += rborder + tborder - lborder - bborder;
    }
  } // for (i = 0; i < numOfChangedNets1; ++i)

  return currentWL;
}



double cf_recalc_some_nodes( bool flag, const int numOfNets, Net* const nets,
         double currentWL, const int* const nodesIdx, 
         int numOfChangedNodes, vector<int>* tableOfConnections,
         const Place* const placement)

{
  int k,t; // loop counters
  // number of changed nets
  int numOfChangedNets; 
  // indexes of changed nets
  vector<int> netsIdx;
  int netIdx;
  // getting indexes of nets connected with changed nodes using tableOfConnections
  // duplicating allowed
  for (k = 0; k < numOfChangedNodes; ++k)
  { 
    for (t = 0; t < static_cast<int>(tableOfConnections[nodesIdx[k]].size()); ++t)
    {
      netIdx = tableOfConnections[nodesIdx[k]][t];
      if (netsIdx.end() == find(netsIdx.begin(), netsIdx.end(), netIdx) )
        netsIdx.push_back(netIdx);  
    }
  }

  numOfChangedNets = static_cast<int>( netsIdx.size() );
  
  int  i, j; // loop counters
  int  idx;  // index of a cell
  // borders of bounding rectangular
  double lborder, rborder, tborder, bborder;
  double temp;
  // recalculating half-perimeter of bounding rectangular for each of changed nets
  for (i = 0; i < numOfChangedNets; ++i)
  {
    idx = nets[netsIdx[i]].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset,
    bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset;
    for (j = 1; j < nets[netsIdx[i]].numOfPins; ++j)
    {
      idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
      temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if (flag)// we change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      nets[netsIdx[i]].currWL = rborder + tborder - lborder - bborder;
      currentWL += nets[netsIdx[i]].currWL;
    }
    else // we do not change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      currentWL += rborder + tborder - lborder - bborder;

    }
  } // for (i = 0; i < numOfChangedNets; ++i)
  return currentWL;
}

double cf_recalc_some_nodes1( bool flag, const int numOfNets, Net* const nets,
         double currentWL, vector<int>* nodesIdx, 
         int numOfChangedNodes, vector<int>* tableOfConnections,
         const Place* const placement)

{
  int k,t; // loop counters
  // number of changed nets
  int numOfChangedNets; 
  // indexes of changed nets
  vector<int> netsIdx;
  int netIdx;
   // getting indexes of nets connected with changed nodes using tableOfConnections
  // duplicating allowed
  for (k = 0; k < numOfChangedNodes; ++k)
  { 
    for (t = 0; t < static_cast<int>(tableOfConnections[(*nodesIdx)[k]].size()); ++t)
    {
      netIdx = tableOfConnections[(*nodesIdx)[k]][t];
      if (netsIdx.end() == find(netsIdx.begin(), netsIdx.end(), netIdx) )
        netsIdx.push_back(netIdx);  
    }
  }

  numOfChangedNets = static_cast<int>( netsIdx.size() );
  
  int  i, j; // loop counters
  int  idx;  // index of a cell
  // borders of bounding rectangular
  double lborder, rborder, tborder, bborder;
  double temp;
  // recalculating half-perimeter of bounding rectangular for each of changed nets
  for (i = 0; i < numOfChangedNets; ++i)
  {
    idx = nets[netsIdx[i]].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset,
    bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset;
    for (j = 1; j < nets[netsIdx[i]].numOfPins; ++j)
    {
      idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
      temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if ( flag )// we change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      nets[netsIdx[i]].currWL = rborder + tborder - lborder - bborder;
      currentWL += nets[netsIdx[i]].currWL;
    }
    else // we do not change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      currentWL += rborder + tborder - lborder - bborder;
    }
  } // for (i = 0; i < numOfChangedNets; ++i)
  return currentWL;
}
 
double cf_recalc_some_nodes2( bool flag, const int numOfNets, Net* const nets,
         double currentWL, 
         vector<int> nodesIdx1, int numOfChangedNodes1,
         vector<int> nodesIdx2, int numOfChangedNodes2,
         vector<int>* tableOfConnections,
         const Place* const placement)

{
  int k,t; // loop counters
  // number of changed nets
  int numOfChangedNets; 
  // indexes of changed nets
  vector<int> netsIdx;
  int netIdx;
   // getting indexes of nets connected with changed nodes using tableOfConnections
  // duplicating allowed
  for (k = 0; k < numOfChangedNodes1; ++k)
  { 
    for (t = 0; t < static_cast<int>(tableOfConnections[nodesIdx1[k]].size()); ++t)
    {
      netIdx = tableOfConnections[nodesIdx1[k]][t];
      if (netsIdx.end() == find(netsIdx.begin(), netsIdx.end(), netIdx) )
        netsIdx.push_back(netIdx);  
    }
  }
  for (k = 0; k < numOfChangedNodes2; ++k)
  { 
    for (t = 0; t < static_cast<int>(tableOfConnections[nodesIdx2[k]].size()); ++t)
    {
      netIdx = tableOfConnections[nodesIdx2[k]][t];
      if (netsIdx.end() == find(netsIdx.begin(), netsIdx.end(), netIdx) )
        netsIdx.push_back(netIdx);  
    }
  }
  
  numOfChangedNets = static_cast<int>( netsIdx.size() );
  
  int  i, j; // loop counters
  int  idx; // index of a cell
  // borders of half-perimeter
  double lborder, rborder, tborder, bborder;
  double temp;
  // recalculating half-perimeter of bounding window for each of changed nets
  for (i = 0; i < numOfChangedNets; ++i)
  {
    idx = nets[netsIdx[i]].arrPins[0].cellIdx;
    lborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    rborder = placement[idx].xCoord + nets[netsIdx[i]].arrPins[0].xOffset,
    tborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset,
    bborder = placement[idx].yCoord + nets[netsIdx[i]].arrPins[0].yOffset;
    for (j = 1; j < nets[netsIdx[i]].numOfPins; ++j)
    {
      idx  = nets[netsIdx[i]].arrPins[j].cellIdx;
      temp = placement[idx].xCoord + nets[netsIdx[i]].arrPins[j].xOffset;
      (temp < lborder) ? lborder = temp : (temp > rborder)? rborder = temp: true;
      temp = placement[idx].yCoord + nets[netsIdx[i]].arrPins[j].yOffset;
      (temp < bborder) ? bborder = temp : (temp > tborder)? tborder = temp: true;
    }

    if ( flag ) // we change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      nets[netsIdx[i]].currWL = rborder + tborder - lborder - bborder;
      currentWL += nets[netsIdx[i]].currWL;
    }
    else // we do not change nets[i].currWL
    {
      currentWL -= nets[netsIdx[i]].currWL;
      currentWL += rborder + tborder - lborder - bborder;

    }
  } // for (i = 0; i < numOfChangedNets; ++i)
  return currentWL;
}

double LogSumExp(const int& numOfNets, Net* const nets, const Place* const placement, const double& alpha)
{
  int idx;
  double sum = 0.0;
  double logsum1 = 0.0;
  double logsum2 = 0.0;
  double logsum3 = 0.0;
  double logsum4 = 0.0;

  for (int i = 0; i < numOfNets; ++i)
  {
    logsum1 = 0.0;
    logsum2 = 0.0;
    logsum3 = 0.0;
    logsum4 = 0.0;

    for (int j = 0; j < nets[i].numOfPins; ++j)
    {
      idx = nets[i].arrPins[j].cellIdx;

      logsum1 += exp( placement[idx].xCoord / alpha); // FIX ME: use + nets[i].arrPins[0].xOffset
      logsum2 += exp(-placement[idx].xCoord / alpha);
      logsum3 += exp( placement[idx].yCoord / alpha);
      logsum4 += exp(-placement[idx].yCoord / alpha);
    }

    sum += log(logsum1) + log(logsum2) + log(logsum3) + log(logsum4);
  }

  return alpha * sum;
}
