/*$Id$*/

/*
    Context for conjugate gradient method (unconstrained minimization)
 */

#ifndef __TAO_BCG_H
#define __TAO_BCG_H
#include "src/tao_impl.h"

typedef struct {

  TAO_CGTYPES type;
  double beta;
  double eta;
  int restarts;

  TaoVec* DX;
  TaoVec* GP;
  TaoVec* Gprev;
  TaoVec* Work;

} TAO_BCG;

EXTERN_C_BEGIN
int TaoBCGSetRestartTol_TaoBCG(TAO_SOLVER,double);
EXTERN_C_END

#endif


