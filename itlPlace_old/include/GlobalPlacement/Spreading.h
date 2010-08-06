#ifndef __SPREADING__
#define __SPREADING__

#include "ObjectivesConstraints.h"

void QS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution);

void LRS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution);
void UpdateLRSpreadingMu(HDesign& hd, AppCtx& context, int iterate);

double SpreadingPenalty(AppCtx* user, PetscScalar* x);

int CalcMaxAffectedArea(double potentialSize, double binSize);
void ConstructBinGrid(HDesign& hd, AppCtx& context, int aDesiredNumberOfClustersAtEveryBin);

//HACK: needed only for hack at LRSpreading
void AddSpreadingPenaltyGradient(AppCtx* context, PetscScalar* x, PetscScalar* grad);

#endif