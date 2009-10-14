#ifndef __TAO_KT_H
#define __TAO_KT_H
#include "tao_solver.h"

class TaoKTApplication : public TaoApplication {
 public:
  TaoKTApplication(TAO_SOLVER,TAO_SOLVER);
  TAO_SOLVER orig;      // original solver structure
  TAO_SOLVER csolver;      // original solver structure
  double func;          // function value

  virtual int EvaluateConstraints(TaoVec *xx, TaoVec *RR);
  virtual int EvaluateJacobian(TaoVec *xx, TaoMat *JJ);
  virtual int InitializeVariables(TaoVec *x);

  virtual int GetVariableVector(TaoVec **xx);
  virtual int EvaluateVariableBounds(TaoVec *xxll, TaoVec *xxuu);
  virtual int GetJacobianMatrix(TaoMat **JJ);
  virtual int GetLinearSolver(TaoMat *, int, TaoLinearSolver **);
  int SetItUp1();
  int SetItUp2();
};


typedef struct {

  TaoKTApplication* ktapp;

  TAO_SOLVER csolver;    // complementarity solver structure
  char comp_method[256]; // complementarity solver

  TaoVec *G;
  int setupcalled;
} TAO_KT;

#endif
