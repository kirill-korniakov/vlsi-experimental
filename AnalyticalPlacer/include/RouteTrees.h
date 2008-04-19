#ifndef _ROUTETREES_H_
#define _ROUTETREES_H_

#include "data_structures.h"

StNodeEx* CreateNodeEx();
void ReleaseNodeEx(StNodeEx* node);
void ReleaseNodesTree(StNodeEx* node);

void MakeRouteTree(Circuit &c, Net& net);
void PrintTree(RoutingTree* tree, FILE* output);

#endif// _ROUTETREES_H_