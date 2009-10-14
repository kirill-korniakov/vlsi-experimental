#ifndef TAOMATSELFSOLVER_H
#define TAOMATSELFSOLVER_H

#include "taolinearsolver.h"

/** An abstract class representing the implementation of a Linear Solver
 *
 */
class TaoMatSelfSolver: public TaoLinearSolver {

protected:

  TaoMat *tmoperator;
  double radius;

public:

  TaoMatSelfSolver(void){tmoperator=0;};
  virtual ~TaoMatSelfSolver(void){};

  int PreSolve(TaoMat*);
  int Solve(TaoVec*,TaoVec*,TaoTruth*);

  int SetOptions();

  int GetNumberIterations(int *);

  int SetTolerances(double,double,double,int);

  int View();

};

#endif
