#ifndef __BORDER_PENALTY__
#define __BORDER_PENALTY__

#include "ObjectivesConstraints.h"

void AddBPwithGrad(AppCtx* context, PetscScalar* solution, double*& f, PetscScalar* gradient);

#endif