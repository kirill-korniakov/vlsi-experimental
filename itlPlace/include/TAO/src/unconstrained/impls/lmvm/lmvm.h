/*$Id$*/

/*
 Context for limited memory variable metric method for unconstrained 
 optimization.
*/

#ifndef __TAO_LMVM_H
#define __TAO_LMVM_H
#include "tao_solver.h"
#include "src/matrix/lmvmmat.h"

typedef struct {
  TaoLMVMMat *M;

  TaoVec *G;
  TaoVec *D;
  TaoVec *W;

  TaoVec *Xold;
  TaoVec *Gold;

  int bfgs;
  int sgrad;
  int grad;
} TAO_LMVM;

#endif
