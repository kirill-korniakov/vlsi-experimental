#ifndef TAOLINEARSOLVER_GA_H
#define TAOLINEARSOLVER_GA_H

#include "taolinearsolver.h"
#include "ga.h" 

class TaoLinearSolverGa: public  TaoLinearSolver {

 protected:
  GAMat mm;
  
 public:
  int linear_iters;
  
  TaoLinearSolverGa();
  virtual ~TaoLinearSolverGa(void){};
  
  virtual int SetOperator(TaoMat*);
  
  virtual int Solve(TaoVec*,TaoVec*);
  
  virtual int GetNumberIterations(int *);
  virtual int SetOptions();
  
  virtual int SetTrustRadius(double);
  
  virtual int Duplicate(TaoLinearSolver**);
  virtual int View();
  
  virtual int SetTolerances(double,double,double,int);
  
};

#endif /* TAOLINEARSOLVER_GA_H */
