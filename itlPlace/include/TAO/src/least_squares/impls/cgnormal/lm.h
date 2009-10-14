/*$Id$*/

#ifndef __TAO_LM_H
#define __TAO_LM_H
#include "src/tao_impl.h"
#include "taoappobject.h"

class TaoLeastSquaresApplication: public TaoApplication{

 public:
  
  TaoMat *HH;      // R^{nxn}// J^T * D1 * J + D2 -- contains pointers to Petsc objects J,D1, and D2 
  TaoMat *JJac;      // R^{mxn}    // Jacobian of the constraints function c(x)
  TaoMat *JJ;      // R^{mxn}    // Jacobian of the constraints function c(x)
  TaoVec *DD1,*DD2;// R^m, R^n     // D1 = [1,1,..1]^T, D2 = [0,0,..0]^T  -- diagonal elements of the matrices 
  TaoVec *RR;      // R^m   
  TaoVec *GG;      // R^n
  TAO_SOLVER outertao;

  TaoLeastSquaresApplication();
  ~TaoLeastSquaresApplication();

  virtual int InitializeVariables(TaoVec *);
  virtual int GetVariableVector(TaoVec **);
  virtual int EvaluateObjectiveFunction(TaoVec *, double *);
  virtual int GetGradientVector(TaoVec **);
  virtual int EvaluateGradient(TaoVec *, TaoVec *);
  virtual int EvaluateObjectiveAndGradient(TaoVec *, double *, TaoVec *);
  virtual int GetHessianMatrix(TaoMat **);
  virtual int EvaluateHessian(TaoVec *, TaoMat *);
  virtual int GetVariableBounds(TaoVec **, TaoVec **);

  int SetupCG(TaoVec*, TaoVec *, TaoMat*);
};


typedef struct{

  TAO_SOLVER innertao;  // The trust region solver

  TaoLeastSquaresApplication* lsq;

}TAO_NLSQ;


#endif






