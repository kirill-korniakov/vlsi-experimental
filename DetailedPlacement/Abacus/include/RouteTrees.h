#ifndef _ROUTETREES_H_
#define _ROUTETREES_H_

#include "data_structures.h"
using namespace DataStructures;

//creating and deleting routing points
StNodeEx* CreateNodeEx();
void ReleaseNodeEx(StNodeEx* node);
void ReleaseNodesTree(StNodeEx* node);

//yet another HPWL calculator
double HPWL(Circuit& c, Net& net);

//only for routed circuit
inline double PreCalculatedWL(Net& net) { return net.tree->wl; }
double TotalWL(Circuit& c);

//Routing methods
void AdaptiveRoute(Circuit &c, Net& net);
void HPWLRoute(Circuit &c, Net& net);
void FLUTERoute(Circuit &c, Net& net);

//Route all nets (or reroute if nets already routed)
void AdapriveRouteCircuit(Circuit &c);

//Debug prints
void PrintTree(RoutingTree* tree, FILE* output);

#endif// _ROUTETREES_H_