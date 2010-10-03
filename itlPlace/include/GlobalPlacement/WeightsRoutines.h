#ifndef _WEIGHTS_ROUTINES_H_
#define _WEIGHTS_ROUTINES_H_

#include "OptimizationContext.h"
#include "PlacementQualityAnalyzer.h"

void InitWeights(double* x, AppCtx* context);
void UpdateWeights(HDesign& hd, AppCtx& context, PlacementQualityAnalyzer* QA,
                   ClusteringInformation& ci);

void ApplyWeights(AppCtx* context);
void ReportWeights(AppCtx& context);

#endif