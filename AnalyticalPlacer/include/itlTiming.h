#ifndef _ITLTIMING_H_
#define _ITLTIMING_H_

#include "data_structures.h"
using namespace DataStructures;
//Elmore delays calculating
void CalculateDalays(Circuit& circuit, Net& Net);
void HPWLTiming(Circuit& circuit, Net& net);
void ATreeTiming(Circuit& circuit, Net& Net);

//initialization
void MakeTimingLists(Circuit& c);

//signal propagation
void PropagateArrivalTime(Circuit& circuit, bool reanalize, bool reroute);
void PropagateRequiredTime(Circuit& circuit, bool reanalize, bool reroute);

//debug printing
void PrintCircuitArrivals(Circuit& circuit);
void PrintCircuitRequireds(Circuit& circuit);
void PrintTimingReport(Circuit& circuit, char* filename);
void PrintNets(Circuit& circuit, char* filename);

void PrintPrev(Circuit& c, int nodeID);
void PrintArrivalOrder(Circuit& circuit);
void PrintRequiredOrder(Circuit& circuit);

void ComputeNetWeights(Circuit& circuit);

#endif // _ITLTIMING_H_