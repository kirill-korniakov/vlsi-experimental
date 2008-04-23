/*  
 * cost_function.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, ITLab, Aleksey Bader, Nina Kurina
 * email: bader@newmail.ru
 * email: ninku@mail.ru
 * last modified 7 February 2006
 */

#ifndef _COST_FUNCTION_H_
#define _COST_FUNCTION_H_

#include "..\include\data_structures.h"

#define UPDATE_NETS_WLS true

using namespace DataStructures;

/// this function calculates wirelength for current placement,
/// it recalculates half-perimeter of bounding rectangular for each net

/// parameter flag in all functions must be set to 0 if we don't want the function 
/// to change nets[i].currWL
/// and to 1 if we want to change it

double cf_recalc_all( bool flag, const int numOfNets, Net* const nets, 
      const Place* const placement );

/// this function calculates wirelength for current placement,
/// it recalculates half-perimeter of bounding rectangular for changed nets

double cf_recalc_some_nets( bool flag, const int numOfNets, Net* const nets,
        double currentWL, 
        const int* const netsIdx,// indexes of changed nets 
        int numOfChangedNets, // number of changed nets
        const Place* const placement );

/*
newWL = rude_cf_recalc_some_nets( 0, nets, currWL, 
                &ArrOfBins[rnd1][rnd2]->nets.front(), static_cast<int>(ArrOfBins[rnd1][rnd2]->nets.size()), 
                &ArrOfBins[rnd3][rnd4]->nets.front(), static_cast<int>(ArrOfBins[rnd3][rnd4]->nets.size()), 
                placement );
*/
double rude_cf_recalc_some_nets( bool flag, Net* const nets,
                                 double currentWL, 
                                 const int* const netsIdx1, int numOfChangedNets1,
                                 const int* const netsIdx2, int numOfChangedNets2,
                                 const Place* const placement );

double rude_cf_recalc_some_nets( bool flag, Net* const nets,
                                 double currentWL, 
                                 const int* const netsIdx, int numOfChangedNets,
                                 const Place* const placement );

/// this function calculates wirelength for current placement,
/// it recalculates half-perimeter of bounding rectangular for nets
/// connected with changed nodes
 
double cf_recalc_some_nodes( bool flag, const int numOfNets, Net* const nets,
         double currentWL,
         const int* const nodesIdx, // indexes of changed nodes 
         int numOfChangedNodes, // number of changed nodes
         vector<int>* tableOfConnections,
         const Place* const placement );

double cf_recalc_some_nodes1( bool flag, const int numOfNets, Net* const nets,
         double currentWL,
         vector<int>* nodesIdx, // indexes of changed nodes 
         int numOfChangedNodes, // number of changed nodes
         vector<int>* tableOfConnections,
         const Place* const placement );


/// this function calculates wirelength for current placement,
/// it recalculates half-perimeter of bounding rectangular for nets
/// connected with changed nodes.
/// the function gets as the input two arrays of changed nodes;

double cf_recalc_some_nodes2( bool flag, const int numOfNets, Net* const nets,
         double currentWL,
         vector<int> nodesIdx1, int numOfChangedNodes1,
         vector<int> nodesIdx2, int numOfChangedNodes2,
         vector<int>* tableOfConnections,
         const Place* const placement );

double LogSumExp(const int& numOfNets, Net* const nets, const Place* const placement, const double& alpha);

#endif