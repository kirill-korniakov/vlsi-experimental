/*$Id$*/

#ifndef __TAO_TRON_H
#define __TAO_TRON_H

#include "tao_solver.h"

typedef struct {

  /* Parameters */
  double pg_ftol;
  double actred;
  double f_new;
 
  double eta1,eta2,eta3,eta4;
  double sigma1,sigma2,sigma3;

  int maxgpits;

  /* Problem variables, vectors and index sets */
  double stepsize;
  double pgstepsize;

  /* Problem statistics */

  int n;   /* Dimension of the Problem */
  double delta;  /* Trust region size */
  double gnorm;
  double f;

  int total_cgits;
  int cg_iterates;
  int total_gp_its;
  int gp_iterates;
  int cgits;

  TaoVec* DXFree;
  TaoVec* R;

  TaoVec* X;
  TaoVec* G;
  TaoVec* PG;

  TaoVec* DX;
  TaoVec* X_New;
  TaoVec* G_New;
  TaoVec* XU;
  TaoVec* XL;
  TaoVec* Work;
  
  TaoMat* Hsub;
  TaoMat* H;

  TaoIndexSet *TT;
  TaoIndexSet *Free_Local;  /* Indices of local variables equal to lower bound */
  TaoIndexSet *Lower_Local;  /* Indices of local variables equal to lower bound */
  TaoIndexSet *Upper_Local;  /* Indices of local variables equal to lower bound */

  int n_free;       /* Number of free variables */
  int n_upper;
  int n_lower;
  int n_bind;       /* Number of binding varibles */

} TAO_TRON;

#endif

