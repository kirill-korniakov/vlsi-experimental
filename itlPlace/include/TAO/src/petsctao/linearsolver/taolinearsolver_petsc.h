#ifndef TAOPETSCSOLVER_H
#define TAOPETSCSOLVER_H

#define TAO_USE_PETSC

#include "taolinearsolver.h"

#include "petscksp.h"
#include "petscpc.h"


class TaoLinearSolverPetsc : public TaoLinearSolver {

 protected:
  
    KSP ksp;
    PetscViewer pkspviewer;
    int linear_its;

 public:

    TaoLinearSolverPetsc(KSP);    
    ~TaoLinearSolverPetsc();
    
    inline KSP GetKSP() {return ksp;}
    int SetKSP(KSP);
    int GetKSP(KSP*);

    /*
      The following methods are implementations of 
      the virtual methods in the base class 
    */

    /* These ones are necessary */
    int PreSolve(TaoMat*);
    int Solve(TaoVec*,TaoVec*, TaoTruth*);

    /* Some methods need this */
    int SolveTrustRegion(TaoVec*,TaoVec*,double, TaoTruth*);
    int SetTolerances(double rtol, double atol, double dtol, int maxits);

    /* These ones are nice to have, but not necessary */
    int GetNumberIterations(int *);
    int View();
    int SetOptions();

    /* These ones are extra, not virtual in base class */
    int SetTrustRadius(double);
    int GetNormDirection(double *);
    int GetObjFcn(double *);
    int GetMinEig(double *);
    int GetLambda(double *);
};

#endif
