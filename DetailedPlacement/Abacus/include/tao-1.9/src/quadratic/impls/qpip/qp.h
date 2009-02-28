#ifndef __TAO_QPIP_H
#define __TAO_QPIP_H
#include "src/tao_impl.h"

typedef struct{

  TaoTruth    predcorr;
  double ksp_tol;

  /* Problem variables, vectors and index sets */
  int n;
  int m;

  /* Problem statistics */
  int iter;         /* Iterations to solve problem */
  double dinfeas;
  double pinfeas;
  double rnorm;
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

  int total_ksp_its;

  /* Variable Vectors */
  Vec XY;
  Vec DXY;

  Vec G,DG,Z,DZ,GZwork,R3;
  Vec W,DW,V,DV,WVwork,R4;
  Vec T,DT,S,DS,TSwork,R5;
  Vec P,DP,Q,DQ,PQwork,R6;

  Vec Ywork;

  /* Work Vectors */
  Vec R12;
  Vec HDiag;
  Vec Work;
  Vec DScaleH1, DScaleH2;
  Vec DiagAxpy;
  Vec DScale;
  Vec RHS,RHS2;


  Mat ProjX, ProjY;
  Mat ProjXL, ProjXU;
  Mat ProjCL, ProjCU, ProjLambda;
  Mat ProjEQ;

  Vec X;
  Vec Y;
  Vec Grad;
  Vec DX;
  Vec C;

  IS  ISx;         /* Rows with variables x */
  IS  ISxl;        /* Inequality constraints with Lower Bounds */
  IS  ISxu;        /* Inequality constraints with Upper Bounds */
  IS  ISd;       
  IS  ISf;
  IS  ISEQ;   
  IS  ISlambda;    /* Any Inequality Constraint */
  IS  ISy;


  /* Data */
  Mat H;
  Vec B;
  Vec C0;
  double c;
  Vec XU;
  Vec XL;
  Vec D;
  Vec F;

  Mat HPC;

}TAO_QPIP;

static int QPIPSetInitialPoint(TAO_SOLVER);
static int QPComputeStepDirection(TAO_QPIP *);
static int QPIPComputeResidual(TAO_QPIP *);
static int QPStepLength(TAO_QPIP *);

static int TaoSetUp_QPIP(TAO_SOLVER);
static int TaoDestroy_QPIP(TAO_SOLVER);
static int TaoSolve_QPIP(TAO_SOLVER, int *);
static int TaoView_QPIP(TAO_SOLVER,Viewer);
static int TaoPrintHelp_QPIP(TAO_SOLVER,char *);
int TaoGetDualVariables_QPIP(TAO_SOLVER,Vec,Vec);
int TAOComputeNormFromCentralPath_QPIP(TAO_SOLVER,double *);

static int TaoSetOptions_QPIP(TAO_SOLVER);
static int TaoDefaultMonitor_QPIP(TAO_SOLVER,void *);
#endif
