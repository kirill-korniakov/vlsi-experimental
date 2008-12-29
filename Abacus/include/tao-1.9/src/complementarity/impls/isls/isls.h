
#ifndef __TAO_ISLS_H
#define __TAO_ISLS_H
#include "src/tao_impl.h"

int Tao_ISLS_SFunction(TAO_SOLVER, TaoVec *, double *, void *);
int Tao_ISLS_SFunctionGradient(TAO_SOLVER, TaoVec *, double *, TaoVec *, void *);

#endif

