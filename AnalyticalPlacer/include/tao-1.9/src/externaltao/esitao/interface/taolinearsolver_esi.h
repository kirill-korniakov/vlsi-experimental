#ifndef TAOESISOLVER_H
#define TAOESISOLVER_H


#include "esi/ESI.h"
#include "taolinearsolver.h"
#include "tao_solver.h"
#include "taomat.h"
#include "taolinearsolver_esi.h"
#include "taovec_esi.h"


class TaoLinearSolverESI : public TaoLinearSolver {

 protected:
  
 public:

  esi::Solver<double,int>* ksp;
  int its;

  TaoLinearSolverESI(esi::Solver<double,int>*);
  /*
  ~TaoLinearSolverPetsc(){int info; if (destroyit){} };
  */
  ~TaoLinearSolverESI(){
    ksp->deleteReference();
  };

  virtual int PreSolve(TaoMat*);
  virtual int Solve(TaoVec*,TaoVec*);
  virtual int GetNumberIterations(int *);
  virtual int SetOptions();
  virtual int View();
  virtual int SetTolerances(double, double, double, int);
};

int TaoWrapESIKSP( esi::Solver<double,int>*, TaoLinearSolverESI **);
int TaoLinearSolverGetESIKSP( TaoLinearSolver *,   esi::Solver<double,int> **);

#endif
