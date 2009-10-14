#ifndef __TAO_BQPIP_H
#define __TAO_BQPIP_H

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

  /* Variable Vectors */
  TaoVec* XY;
  TaoVec* DXY;
  TaoVec* G;
  TaoVec* DG;
  TaoVec* T;
  TaoVec* DT;
  TaoVec* Z;
  TaoVec* DZ;
  TaoVec* S;
  TaoVec* DS;
  TaoVec* GZwork;
  TaoVec* TSwork;

  /* Work Vectors */
  TaoVec* R12;
  TaoVec* R3;
  TaoVec* R5;
  TaoVec* HDiag;
  TaoVec* Work;

  TaoVec* DiagAxpy;
  TaoVec* RHS;
  TaoVec* RHS2;

  TaoMat *ProjXL, *ProjXU;

  /* Data */
  TaoMat* H;
  TaoVec* B;
  TaoVec* C0;
  double c;
  TaoVec* XU;
  TaoVec* XL;

}TAO_BQPIP;

static int QPIPSetInitialPoint(TAO_SOLVER,TAO_BQPIP *);
static int QPComputeStepDirection(TAO_BQPIP *);
static int QPIPComputeResidual(TAO_BQPIP *);
static int QPStepLength(TAO_BQPIP *);

EXTERN_C_BEGIN
int TaoGetDualVariables_BQPIP(TAO_SOLVER,TaoVec*,TaoVec*);
EXTERN_C_END
int TAOComputeNormFromCentralPath_BQPIP(TAO_SOLVER,double *);

#endif












