#ifndef _DETAILED_PLACEMENT_H_
#define _DETAILED_PLACEMENT_H_

#include "OptimizationContext.h"

#include "tao.h"

#define iCHKERRQ for(int __u##__LINE__=-1;__u##__LINE__++!=0;)if(__u##__LINE__--){CHKERRQ(__u##__LINE__)}else __u##__LINE__=

int AnalyticalObjectiveAndGradient(TAO_APPLICATION taoapp, Vec X, double* f, Vec G, void* ptr);

void InitWeights(PetscScalar* x, AppCtx* context);
void ReportWeights(AppCtx& context);
void ApplyWeights(AppCtx* context);
void UpdateWeights(HDesign& hd, AppCtx& context, int iterate);

void CalculateCostAndGradients(AppCtx* context, double* solution);
void CalcGradientsNorms(AppCtx* context);
void PrintClusterCoordinates(AppCtx* context);

#endif