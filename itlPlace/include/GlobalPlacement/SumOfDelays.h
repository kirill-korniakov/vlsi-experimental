#ifndef __SUMOFDELAYS__
#define __SUMOFDELAYS__

#include "ObjectivesConstraints.h"

void SOD_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double*& f);

#endif