#ifndef TAOABCFLOATAPP_H
#define TAOABCFLOATAPP_H

#include "taoappobject.h"
#include "src/vector/tvecsingle.h"

class TaoABCFloatApplication: public TaoApplication{

 private:

 public:
  TaoABCFloatApplication();
  ~TaoABCFloatApplication();

  TaoVecFloatArray *taox;   /* The Variable Vector */
  int SetNumberOfVariables(int);
  int GetSolutionAndGradient(float*&, float*&, int&);

  /* Function and Gradient */
  int GetVariableVector(TaoVec **);

  int EvaluateObjectiveFunction(TaoVec *, double *);
  int EvaluateGradient(TaoVec *, TaoVec *);
  int EvaluateObjectiveAndGradient(TaoVec *xx, double *ff, TaoVec *gg);

  virtual int  ComputeObjectiveAndGradient(float*,int,float*,float *)=0;

  /* Set Variable Bounds */
  int EvaluateVariableBounds(TaoVec *xxll, TaoVec *xxuu);
  virtual int ComputeVariableBounds(float *xl, float *xu, int n);

  /* Initialize Starting Point */
  int InitializeVariables(TaoVec *x);
  virtual int StartingPoint(float *x, int n);

};

#endif


