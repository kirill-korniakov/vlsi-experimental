#ifndef TAOESIPROBLEMOBJECT_H
#define TAOESIPROBLEMOBJECT_H

#include "taoappobject.h"

/** An abstract class representing the implementation of a TAO Vector. 
 *
 */

class TaoVec;
class TaoMat;
class TaoLinearSolver;
// class esi::ESIApplication;

class TaoESIApplication: public TaoApplication{

  esi::ESIApplication* esiapp;

 public:
  
  TaoESIApplication(esi::ESIApplication* eapp);
  ~TaoESIApplication();

  virtual int EvaluateObjectiveFunction(TaoVec *xx, double *ff);
  virtual int EvaluateObjectiveAndGradient(TaoVec *xx, double *ff, TaoVec *gg);

  virtual int InitializeVariables(TaoVec *x);

  virtual int GetVariableVector(TaoVec **xx);
  virtual int EvaluateVariableBounds(TaoVec *xxll, TaoVec *xxuu);

  virtual int GetHessianMatrix(TaoMat **HH);
  virtual int EvaluateHessian(TaoVec *xx, TaoMat *HH);

  virtual int Monitor();

  virtual int GetLinearSolver(TaoMat *, int, TaoLinearSolver **tksp);

};

#endif



