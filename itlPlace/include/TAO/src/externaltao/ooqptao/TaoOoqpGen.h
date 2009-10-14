#ifndef __TAO_OOQPGEN_H
#define __TAO_OOQPGEN_H

#include "../src/tao_impl.h"
#include "tao.h"

class QpGenTao;
class QpGenData;
class QpGenVars;
class QpGenResiduals;
class GondzioSolver;

//declare an internal structure for the ooqp tao interface
typedef struct {
  /** The current iteration count */
  int iterate; 
  
  TaoVec * xl;
  TaoVec * xu; 
  TaoVec *g;

  TaoVec * y; // Dual variables for equality constraints
  TaoVec * z; // Dual variables for inequality constraints

  TaoVec * phi; // Dual variables for the upper bounds on x
  TaoVec * gamma; // Dual varibles for the lower bounds on x

  TaoMat * Q;

  QpGenTao *qp;     // The Tao-specific problem formulation
  QpGenData *prob;  // The problem data.
  QpGenVars *vars;  // The problem variables.
  QpGenResiduals *resid; // The problem residuals.
  GondzioSolver *ms; // The QP solver.
} TAO_OOQPGEN;

extern "C"
int newTaoQpGen( TaoVec * templateXVec,
		 TaoVec * templateYVec,
		 TaoVec * templateZVec,
		 int nnzQs[], int nnzAs[], int nnzCs[],
		 TaoVec ** c, TaoMat ** Q,
		 TaoVec **  xlow, TaoVec **  xupp, 
		 TaoMat **  A,     TaoVec ** b,
		 TaoMat **  C,
		 TaoVec **  clow, TaoVec **  cupp );

extern "C"
int freeTaoQpGen( TaoVec ** c, TaoMat ** Q,
		  TaoVec **  xlow,  TaoVec **  xupp,
		  TaoMat **  A,     TaoVec ** b,
		  TaoMat **  C, TaoVec **  clow, TaoVec **  cupp );

extern "C"
int TaoRegisterOoqp();

#endif
