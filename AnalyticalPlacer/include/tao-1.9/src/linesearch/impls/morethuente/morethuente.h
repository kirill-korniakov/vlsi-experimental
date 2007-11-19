/*
    Context for a line search method for unconstrained minimization
 */

#ifndef __TAO_LINESEARCH_H
#define __TAO_LINESEARCH_H
#include "src/tao_impl.h"

typedef struct {

/* --------------- Parameters used by line search method ----------------- */
  double maxstep;	     /* maximum step size */
  double rtol;		     /* relative tol for acceptable step (rtol>0) */
  double ftol;		     /* tol for sufficient decr. condition (ftol>0) */
  double gtol;		     /* tol for curvature condition (gtol>0)*/
  double stepmin;	     /* lower bound for step */
  double stepmax;	     /* upper bound for step */
  int    maxfev;	     /* maximum funct evals per line search call */
  int    nfev;		     /* number of funct evals per line search call */
  int    bracket;
  int    infoc;
} TAO_LINESEARCH;

#endif
