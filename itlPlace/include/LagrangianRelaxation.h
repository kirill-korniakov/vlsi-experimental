#ifndef __LAGRANGIAN_RELAXATION__
#define __LAGRANGIAN_RELAXATION__

#include "ObjectivesConstraints.h"

void LR_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double* f);

#endif