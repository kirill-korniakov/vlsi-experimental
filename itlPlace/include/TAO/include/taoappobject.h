#ifndef TAOPROBLEMOBJECT_H
#define TAOPROBLEMOBJECT_H

/** 
    An abstract class representing an optimization application.  The
    methods in this class will be called by the optimization solver. 
**/

#include "tao_basictypes.h"

class TaoVec;
class TaoIndexSet;
class TaoMat;
class TaoLinearSolver;

class TaoApplication {
  
 public:
  
  virtual ~TaoApplication(){};
  
  /* This method is really important and used frequently */
  virtual int EvaluateObjectiveAndGradient(TaoVec *, double *, TaoVec *);

  /* This method not often used, except by direct search methods  */
  virtual int EvaluateObjectiveFunction(TaoVec *, double *);

  /* This method not often used  */
  virtual int EvaluateGradient(TaoVec *, TaoVec *);

  /* This method is used only by some solvers */
  virtual int EvaluateHessian(TaoVec *, TaoMat *);

  /* This method is used only by some solvers */
  virtual int EvaluateVariableBounds(TaoVec *, TaoVec *);

  /* This method is optional, but it can improve performance */
  virtual int InitializeVariables(TaoVec *);

  /* These methods are important, the first one is mandatory */
  virtual int GetVariableVector(TaoVec **);
  virtual int GetHessianMatrix(TaoMat **);

  /* This method may be used in LMVM methods  */
  virtual int HessianSolve(TaoVec *, TaoVec*,TaoTruth *);

  /* This method is also important for some solvers */
  virtual int GetLinearSolver(TaoMat *, int, TaoLinearSolver **);

  /* Used currently for complementarity, and least squares solvers */
  virtual int EvaluateConstraints(TaoVec *, TaoVec *);
  virtual int EvaluateJacobian(TaoVec *, TaoMat *);
  virtual int GetJacobianMatrix(TaoMat **);
  virtual int GetConstraintVector(TaoVec**);

  /* This method is called only by OOQP */
  virtual int GetInequalityConstraints(TaoVec**, TaoMat **, TaoVec **);

  /* This method is called at each iteration of a TAO solver. Optional, but useful */
  virtual int Monitor();
  virtual int Monitor2(TaoVec*,TaoVec*,TaoVec*,TaoTruth*);
};

#endif



