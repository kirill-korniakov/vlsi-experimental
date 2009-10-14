#ifndef TAOPETSCHEADER_H
#define TAOPETSCHEADER_H

#include "petscksp.h"

// #include "taoapp.h"
typedef struct _p_TAOAPPLICATION* TAO_APPLICATION;
typedef struct _p_TAO_SOLVER* TAO_SOLVER;


/* Actually wrappers around the TAO_SOLVER object so users can get access to PETSC structures */
extern int TaoSetInequalityConstraints(TAO_APPLICATION, Vec, Mat, Vec);
extern int TaoAppGetKSP(TAO_APPLICATION, KSP *);
extern int TaoAppGetTaoSolver(TAO_APPLICATION, TAO_SOLVER *);


extern int TaoSetPetscXMonitor(TAO_SOLVER);
extern int TaoGetKSP(TAO_SOLVER, KSP*);
extern int TaoCopyDualsOfVariableBounds(TAO_SOLVER, Vec, Vec);
extern int TaoGetVariableBoundVecs(TAO_SOLVER, Vec *, Vec *);
extern int TaoAppGetGradientVec(TAO_SOLVER, Vec*);
extern int TaoAppGetHessianMat(TAO_SOLVER, Mat*, Mat*);


/* Interface between TAO_SOLVER and TAO_APPLICATION */

extern int TaoSolveApplication(TAO_APPLICATION, TAO_SOLVER);
extern int TaoSetOptions(TAO_APPLICATION, TAO_SOLVER);
extern int TaoSetupApplicationSolver(TAO_APPLICATION, TAO_SOLVER);


/* May not be supported in future releases */
#define TaoPetscApplicationCreate(a,b)     TaoApplicationCreate(a,b)
#define TaoSolvePetscApplication(a)        TaoSolve(a)
#define TaoSetPetscInitialVector(a,b)      TaoAppSetInitialSolutionVec(a,b)
#define TaoSetPetscVariableBounds(a,b,c)   TaoAppSetVariableBounds(a,b,c)
#define TAO_COMPLEMENTARITY 0
extern int TaoSetApplication(TAO_SOLVER, TAO_APPLICATION);
extern int TaoSetFromOptions(TAO_SOLVER);
extern int TaoApplicationDestroy(TAO_APPLICATION);
extern int TaoSetPetscFunction(TAO_APPLICATION, Vec, int (*)(TAO_SOLVER,Vec,double*,void*),void *);
extern int TaoSetPetscFunctionGradient(TAO_APPLICATION, Vec, Vec, int (*)(TAO_SOLVER,Vec,double*,Vec,void*),void *);
extern int TaoSetPetscGradient(TAO_APPLICATION, Vec, int (*)(TAO_SOLVER,Vec,Vec,void*),void *);
extern int TaoSetPetscHessian(TAO_APPLICATION, Mat, Mat, int (*)(TAO_SOLVER,Vec,Mat*,Mat*,MatStructure*,void*),void *);
extern int TaoSetPetscConstraintsFunction(TAO_APPLICATION, Vec, int (*)(TAO_SOLVER,Vec,Vec,void*),void *);
extern int TaoSetPetscJacobian(TAO_APPLICATION,Mat,int (*)(TAO_SOLVER,Vec,Mat*,void*),void *);

class TaoVecPetsc;
class TaoMatPetsc;
class TaoIndexSetPetsc;
class TaoLinearSolverPetsc;
#include "taovec.h"
#include "taomat.h"
#include "taois.h"
#include "taolinearsolver.h"


extern int TaoWrapPetscVec( Vec, TaoVecPetsc **);
extern int TaoVecGetPetscVec( TaoVec *, Vec *);

extern int TaoWrapPetscMat( Mat, TaoMatPetsc **);
extern int TaoMatGetPetscMat( TaoMat *, Mat *);


// extern int TaoWrapPetscIS( IS, int, TaoIndexSetPetsc **);
extern int TaoWrapPetscIS( IS, int, TaoIndexSetPetsc **);
extern int TaoIndexSetGetPetscIS( TaoIndexSet *, IS *);


typedef enum { /* Method used to identify variables */
  TaoRedistributeSubset=0,
  TaoNoRedistributeSubset=2,
  TaoSingleProcessor=5,
  TaoMaskFullSpace=3,
  TaoMatrixFree=8} TaoPetscISType;

extern int TaoSelectSubset( TaoPetscISType );

extern int TaoWrapKSP( KSP, TaoLinearSolverPetsc **);
extern int TaoLinearSolverGetKSP( TaoLinearSolver *, KSP *);

extern int PetscMapSame(PetscMap, PetscMap, PetscTruth *);
extern int VecMedian(Vec, Vec, Vec, Vec);
extern int VecPointwiseMax(Vec, Vec, Vec);
extern int VecPointwiseMin(Vec, Vec, Vec);
extern int VecFischer(Vec, Vec, Vec, Vec, Vec);
extern int VecBoundProjection(Vec, Vec, Vec, Vec, Vec);
extern int VecCreateSubVec(Vec, IS, Vec *);
extern int VecISAXPY(Vec, PetscScalar, Vec, IS);
extern int VecStepMax(Vec, Vec, PetscReal*);
extern int VecStepMax2(Vec, Vec,Vec, Vec, PetscReal*);
extern int VecCompare(Vec, Vec, PetscTruth *);

// #include "petscdaapp.h"

#endif
