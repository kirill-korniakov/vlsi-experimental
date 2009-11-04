#ifndef __LOGSUMEXP__
#define __LOGSUMEXP__

#include "ObjectivesConstraints.h"

double CalcNetLSE(AppCtx* context, PetscScalar* coordinates, int netIdx);
double CalcNetLSEGradient(AppCtx* context, int netIdx, int idxInSolutionVector);

double CalcDistance2Sink(AppCtx* context, PetscScalar* coordinates, int netIdx, int sinkIdx);
double CalcDistance2SinkGradient(AppCtx* context, int netIdx, int idxInSolutionVector, int sinkIdx);

void LSE_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double*& f);

void PrecalcExponents(AppCtx* context, PetscScalar* coordinates);

#endif