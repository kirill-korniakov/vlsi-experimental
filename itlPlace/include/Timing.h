#ifndef __TIMING_H__
#define __TIMING_H__

#include "HDesign.h"

void FindTopologicalOrder(HDesign& design);

void PropagateDelays(HDesign& design, double clock_cycle);
void PropagateDelays(HDesign& design);


void SetStartPointsArrivals(HDesign& design, double clock_cycle);
void SetEndPointsRequires(HDesign& design, double clock_cycle);

void PropagateArrivals(HDesign& design);
void PropagateRequires(HDesign& design);

void FindCriticalPaths(HDesign& design);

void ComputeNetWeights(HDesign& hd);

void ExportNetWeights(HDesign& hd, const char* fileName);

void ImportNetWeights(HDesign& hd, const char* fileName);

void MakeAnotherLoop(HDesign& hd, int argc, char** argv);
void PrepareNextLoop(HDesign& hd, int argc, char** argv, int& nCyclesCounter);

void GetArrivalRisingArc(HDesign& hd,
                         HTimingPoint startPoint,
                         HTimingPoint endPoint,
                         HTimingArcType* risingArc,
                         double* arcRiseTime,
                         bool* isInversed);
void GetArrivalFallingArc(HDesign& hd,
                          HTimingPoint startPoint,
                          HTimingPoint endPoint,
                          HTimingArcType* fallingArc,
                          double* arcFallTime,
                          bool* isInversed);
void GetRequiredFallingArc(HDesign& hd,
                           HTimingPoint startPoint,
                           HTimingPoint endPoint,
                           HTimingArcType* fallingArc,
                           double* arcFallTime,
                           bool* isInversed);
void GetRequiredRisingArc(HDesign& hd,
                          HTimingPoint startPoint,
                          HTimingPoint endPoint,
                          HTimingArcType* risingArc,
                          double* arcRiseTime,
                          bool* isInversed);

inline void InitializeTiming(HDesign& design)
{
  design.SteinerPoints.Initialize();
  design.Wires.Initialize();
  design.TimingPoints.Initialize();
  FindTopologicalOrder(design);
}

#endif //__TIMING_H__
