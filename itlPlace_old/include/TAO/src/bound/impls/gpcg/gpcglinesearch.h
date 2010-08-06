/*
    Context for a Newton line search method (unconstrained minimization)
 */

#ifndef __TAO_GPCGLINESEARCH_H
#define __TAO_GPCGLINESEARCH_H

#include "gpcg.h"        /*I "tao_solver.h" I*/

typedef struct {

/* --------------- Parameters used by line search method ----------------- */
  double maxstep;	     /* maximum step size */
  double rtol;		     /* relative tol for acceptable step (rtol>0) */
  double ftol;		     /* tol for sufficient decrease condition (ftol>0) */
  double gtol;		     /* tol for curvature condition (gtol>0)*/
  double stepmin;	     /* lower bound for step */
  double stepmax;	     /* upper bound for step */
  int    maxfev;	     /* maximum funct evals per line search call */
  int    nfev;		     /* number of funct evals per line search call */
  int    bracket;
  int    infoc;

  int setupcalled;
  TaoVec *W2;
  TaoVec *Gold;

} TAO_GPCGLINESEARCH;

int TaoGPCGCreateLineSearch(TAO_SOLVER tao);

#endif
