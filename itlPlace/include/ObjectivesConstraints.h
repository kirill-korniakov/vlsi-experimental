#ifndef _DETAILED_PLACEMENT_H_
#define _DETAILED_PLACEMENT_H_

#include "OptimizationContext.h"

#include "tao.h"

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp, Vec X, double* f, Vec G, void* ptr);
void CalcMuInitial(PetscScalar* x, AppCtx* context);

#endif