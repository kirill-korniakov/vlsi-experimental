#ifndef __TIMING_H__
#define __TIMING_H__

#include "HDesign.h"

void FindTopologicalOrder(HDesign& design);
void PropagateDelays(HDesign& design, double clock_cycle);
void PropagateDelays(HDesign& design);
void FindCriticalPaths(HDesign& design);

void ComputeNetWeights(HDesign& hd);
void ExportNetWeights(HDesign& hd, const char* fileName);
void ImportNetWeights(HDesign& hd, const char* fileName);
void ComputeAndExportWeights(HDesign& hd);
void PrepareNextNetWeightingLoop(HDesign& hd, int& nCyclesCounter);

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
void GetArrivalArc(HDesign& hd,
                         HTimingPoint startPoint,
                         HTimingPoint endPoint,
                         HTimingArcType* arc,
                         double* arcTime,
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
void GetRequiredArc(HDesign& hd,
                          HTimingPoint startPoint,
                          HTimingPoint endPoint,
                          HTimingArcType* arc,
                          double* arcTime,
                          bool* isInversed);


HTimingArcType FindArrivalArc(HDesign& hd,
                         HTimingPoint arcEndPoint,
                         SignalDirection dir,
                         double& arcTime,
                         bool& isInversed);

HTimingArcType FindRequiredArc(HDesign& hd,
                         HTimingPoint arcStartPoint,
                         SignalDirection dir,
                         double& arcTime,
                         bool& isInversed);

inline void InitializeTiming(HDesign& design)
{
  if (!design.CanDoTiming()) return;
  design.TimingPoints.Initialize();
  FindTopologicalOrder(design);
}

#endif //__TIMING_H__
