#ifndef TAOLINEARSOLVER_H
#define TAOLINEARSOLVER_H

#include "tao_basictypes.h"

class TaoIndexSet;
class TaoVec;
class TaoMat;

/** 
    An abstract class representing the implementation of a Linear Solver
*/

class TaoLinearSolver {

protected:
  
public:
  TaoLinearSolver() { };
  virtual ~TaoLinearSolver() { };

  /* These two methods are very important */
  virtual int PreSolve(TaoMat *);
  virtual int Solve(TaoVec *, TaoVec *, TaoTruth *);

  /* Some solvers need this method */
  virtual int SolveTrustRegion(TaoVec *, TaoVec *, double, TaoTruth *);
  virtual int SetTolerances(double rtol, double atol, double dtol, int maxits);

  /* These methods are nice to have but not necessary */
  virtual int SetOptions();
  virtual int GetNumberIterations(int *);
  virtual int View();
};

#endif
