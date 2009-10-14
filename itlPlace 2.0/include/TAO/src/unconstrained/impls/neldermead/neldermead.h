#ifndef __TAO_NELDERMEAD_H
#define __TAO_NELDERMEAD_H
#include "src/tao_impl.h"

int TaoSetUp_NelderMead(TAO_SOLVER, void *);
int TaoSetDown_NelderMead(TAO_SOLVER, void *);
int TaoSetOptions_NelderMead(TAO_SOLVER, void *);
int TaoView_NelderMead(TAO_SOLVER, void *);

typedef struct {

  double mu_ic;      
  double mu_oc;
  double mu_r;
  double mu_e;

  double lamda;  // starting point delta for finding starting simplex

  int N;
  double oneOverN;
  TaoVec *Xbar,*Xmuc,*Xmur,*Xmue;
  TaoVec *G;
  TaoVec **simplex;

  double *f_values;
  int *indices;
  
  
} TAO_NelderMead;

#endif
