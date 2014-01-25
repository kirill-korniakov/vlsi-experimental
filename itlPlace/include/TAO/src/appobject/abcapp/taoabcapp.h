#ifndef TAOABCAPP_H
#define TAOABCAPP_H

#include "taoappobject.h"
//#include "src/vector/tvecdouble.h"
#include "tvecdouble.h"

class TaoABCApplication: public TaoApplication{

 private:

 public:
  TaoABCApplication();
  ~TaoABCApplication();

  TaoVecDoubleArray *taox;   /* The Variable Vector */
  int SetNumberOfVariables(int);
  int GetSolution(double*&, int&);

  /* Function and Gradient */
  int GetVariableVector(TaoVec **);
  int EvaluateObjectiveFunction(TaoVec *, double *);
  int EvaluateGradient(TaoVec *, TaoVec *);
  int EvaluateObjectiveAndGradient(TaoVec *xx, double *ff, TaoVec *gg);

  virtual int  ComputeObjectiveAndGradient(double*,int,double*,double *)=0;

  /* Set Variable Bounds */
  int EvaluateVariableBounds(TaoVec *xxll, TaoVec *xxuu);
  virtual int ComputeVariableBounds(double *xl, double *xu, int n);

  /* Initialize Starting Point */
  int InitializeVariables(TaoVec *x);
  virtual int StartingPoint(double *x, int n);

};

#endif


