/*$Id$*/

/*
    Context for conjugate gradient method (unconstrained minimization)
 */

#ifndef __TAO_CG_H
#define __TAO_CG_H

#include "tao_solver.h"

typedef struct {
  TaoVec *X2;		// Solution vector
  TaoVec *G1;		// Gradient vector
  TaoVec *G2;		// Gradient vector
  TaoVec *D;		// Direction
  TaoVec *W;		// Work vector

  double eta;		// Restart tolerance
  double delta_max;	// Minimum value for scaling
  double delta_min;	// Maximum value for scaling

  // The algorithm restarts when the gradient at the current point g_k, 
  // and the gradient of the previous point, g_{k-1}, satisfy the 
  // following inequality:
  //
  //      abs(inner(g_k, g_{k-1})) > eta * norm(g_k, 2)^2.

  int grad;		// Number of gradient steps
  int reset;		// Number of reset steps

  int cg_type;    	// Formula to use
} TAO_CG;

#endif

