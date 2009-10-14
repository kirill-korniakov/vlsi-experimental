#ifndef __TAO_ICP_H
#define __TAO_ICP_H

#include "tao_solver.h"

typedef struct{

  /* Parameters */
  int    predcorr;
  double ksp_tol;

  /* Problem variables, vectors and index sets */
  int n;   /* Dimension of the Problem */
  int m;  /* Number of constraints */

  /* Problem statistics */
  double dinfeas;
  double pinfeas;
  double pobj;
  double dobj;
  double gap;
  double rgap;
  double mu;
  double sigma;
  double pathnorm;
  double pre_sigma;
  double psteplength;
  double dsteplength;
  double rnorm;
  TaoTruth usedcorrector;

  double sigmamu;
  /* Variable Vectors */
  TaoVec* f;
  TaoVec* x;
  TaoVec* dx;
  TaoVec* g;
  TaoVec* dg;
  TaoVec* t;
  TaoVec* dt;
  TaoVec* z;
  TaoVec* dz;
  TaoVec* s;
  TaoVec* ds;
  TaoVec* gzwork;
  TaoVec* tswork;

  TaoVec *g2;
  /* Work Vectors */
  TaoVec* r12;
  TaoVec* r3;
  TaoVec* r5;
  TaoVec* JDiag;
  TaoVec* work;

  TaoVec* diagaxpy;
  TaoVec* rhs;
  TaoVec* rhs2;

  /* Data */
  TaoMat* J;
  TaoVec* xu;
  TaoVec* xl;

}TAO_ICP;

static int QPIPSetInitialPoint(TAO_SOLVER,TAO_ICP *);
static int QPComputeStepDirection(TAO_ICP *);
static int QPIPComputeResidual(TAO_ICP *);
static int QPStepLength(TAO_ICP *);

int TAOComputeNormFromCentralPath_ICP(TAO_SOLVER,double *);

#endif












