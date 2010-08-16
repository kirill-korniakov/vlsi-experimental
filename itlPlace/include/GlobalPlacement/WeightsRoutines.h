#ifndef _WEIGHTS_ROUTINES_H_
#define _WEIGHTS_ROUTINES_H_

#include "OptimizationContext.h"

void InitWeights(double* x, AppCtx* context);
void UpdateWeights(HDesign& hd, AppCtx& context);
void ApplyWeights(AppCtx* context);
void ReportWeights(AppCtx& context);

#endif