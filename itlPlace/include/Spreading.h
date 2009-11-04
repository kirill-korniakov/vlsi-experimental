#ifndef __SPREADING__
#define __SPREADING__

#include "ObjectivesConstraints.h"

void QS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double* &f);
void LRS_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double* &f);
double CalculateDiscrepancy(Vec& x, void* data);
double SpreadingPenalty(AppCtx* user, PetscScalar* x);

#endif