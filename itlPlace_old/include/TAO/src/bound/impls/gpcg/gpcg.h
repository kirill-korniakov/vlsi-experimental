/*$Id$*/

#ifndef __TAO_GPCG_H
#define __TAO_GPCG_H
#include "tao_solver.h"
#include "src/bound/impls/gpcg/gpcglinesearch.h"

typedef struct{

  /* Parameters */
  double pg_ftol;
  double actred;
  double f_new;
  double minstep;
  double stepsize;
  double gnorm;

  double sigma1,sigma2,sigma3;

  int maxgpits;

  /* Problem variables, vectors and index sets */

  /* Problem statistics */

  int n;   /* Dimension of the Problem */

  int total_cgits;
  int cg_iterates;
  int total_gp_its;
  int gp_iterates;
  int cgits;

  TaoVec * G_New;
  TaoVec * DXFree;
  TaoVec * R;
  TaoVec * DX;
  TaoVec * X;
  TaoVec * X_New;
  TaoVec * G, *PG;
  TaoVec * XU;
  TaoVec * XL;
  TaoVec * Work;

  TaoMat *H;
  TaoVec * B;
  double c;
  
  double f;
  double step;
  TaoMat *Hsub;

  TaoIndexSet * Free_Local;  /* Indices of local variables equal to lower bound */
  TaoIndexSet * TT;  /* Indices of local variables equal to upper bound */

  int n_free;       /* Number of free variables */
  int n_upper;
  int n_lower;
  int n_bind;       /* Number of binding varibles */

}TAO_GPCG;

/* GPCG Routines */
int TaoGPCGComputeFunctionGradient(TAO_SOLVER, TaoVec *, double *, TaoVec *);

#endif






