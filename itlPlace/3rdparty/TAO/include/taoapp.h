#ifndef TAOAPPHEADER_H
#define TAOAPPHEADER_H

/* 
   This header declared the interface to the nonlinear application
   object.  The objective function, constraints, and derivatives
   of these functions are defined in terms of PETSc vectors and
   matrices.  This interface should be independent of solvers 
   such as TAO.
*/

#include "petscksp.h"

typedef struct _p_TAOAPPLICATION* TAO_APPLICATION;
//typedef struct _p_TAO_SOLVER* TAO_SOLVER;

extern int TAO_APP_COOKIE;

extern int TaoAppSetInitialSolutionVec(TAO_APPLICATION, Vec);
extern int TaoAppSetDefaultSolutionVec(TAO_APPLICATION, Vec);
extern int TaoAppGetSolutionVec(TAO_APPLICATION, Vec*);

extern int TaoAppSetObjectiveRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION,Vec,double*,void*),void *);
extern int TaoAppComputeObjective(TAO_APPLICATION, Vec, double *);

extern int TaoAppSetGradientRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION,Vec,Vec,void*),void *);
extern int TaoAppComputeGradient(TAO_APPLICATION taoapp, Vec X, Vec G);

extern int TaoAppSetObjectiveAndGradientRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION,Vec,double*,Vec,void*),void *);
extern int TaoAppComputeObjectiveAndGradient(TAO_APPLICATION, Vec, double *, Vec);

extern int TaoAppSetHessianMat(TAO_APPLICATION, Mat,Mat);
extern int TaoAppGetHessianMat(TAO_APPLICATION, Mat*,Mat*);
extern int TaoAppSetHessianRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION,Vec,Mat*,Mat*,MatStructure*,void*),void *);
extern int TaoAppComputeHessian(TAO_APPLICATION, Vec, Mat *, Mat *, MatStructure *);
extern int TaoAppSetHessianSolveRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION,Vec,Vec,PetscTruth*,void*),void *);
extern int TaoAppHessianSolve(TAO_APPLICATION, Vec, Vec, PetscTruth*);

extern int TaoAppSetVariableBoundsRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION,Vec,Vec,void*),void *);
extern int TaoAppComputeVariableBounds(TAO_APPLICATION, Vec, Vec);

extern int TaoAppComputeFunction(TAO_APPLICATION, Vec, Vec);
extern int TaoAppSetFunctionVec(TAO_APPLICATION, Vec);
extern int TaoAppGetFunctionVec(TAO_APPLICATION, Vec*);
extern int TaoAppSetConstraintRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION,Vec,Vec,void*),void *);


extern int TaoAppSetJacobianMat(TAO_APPLICATION,Mat,Mat);
extern int TaoAppGetJacobianMat(TAO_APPLICATION,Mat*,Mat*);
extern int TaoAppSetJacobianRoutine(TAO_APPLICATION,int (*)(TAO_APPLICATION,Vec,Mat*,Mat*,MatStructure*,void*),void *);
extern int TaoAppComputeJacobian(TAO_APPLICATION, Vec, Mat*, Mat*, MatStructure*);

extern int TaoAppSetMonitor(TAO_APPLICATION, int (*)(TAO_APPLICATION,void*),void *);
extern int TaoAppMonitor(TAO_APPLICATION);
extern int TaoAppResetCounters(TAO_APPLICATION);
extern int TaoAppCounters(TAO_APPLICATION,int[4]);

extern int TaoAppAddObject(TAO_APPLICATION, char *, void *, int *);
extern int TaoAppQueryForObject(TAO_APPLICATION, char *, void **);
extern int TaoAppQueryRemoveObject(TAO_APPLICATION, char *);
extern int TaoAppQueryReplaceObject(TAO_APPLICATION, char *,void**);

extern int TaoApplicationCreate(MPI_Comm, TAO_APPLICATION*);
extern int TaoAppSetFromOptions(TAO_APPLICATION);

extern int TaoAppSetDestroyRoutine(TAO_APPLICATION,int (*)(void*),void *);
extern int TaoAppDestroy(TAO_APPLICATION);

extern int TaoAppSetOptionsRoutine(TAO_APPLICATION, int (*)(TAO_APPLICATION) );
extern int TaoAppCheckConvergence(TAO_APPLICATION,Vec,PetscTruth*);
extern int TaoAppSetRelativeTolerance(TAO_APPLICATION,double);

/* An extension to the TAO_APPLICATION object, but not part of it */
extern int TaoAppSetColoring(TAO_APPLICATION, ISColoring);
extern int TaoAppGetColoring(TAO_APPLICATION, ISColoring*);
EXTERN_C_BEGIN
extern int TaoAppDefaultComputeGradient(TAO_APPLICATION,Vec,Vec,void*);
extern int TaoAppDefaultComputeHessian(TAO_APPLICATION,Vec,Mat *,Mat *,MatStructure *,void *);
extern int TaoAppDefaultComputeHessianColor(TAO_APPLICATION,Vec,Mat *,Mat *,MatStructure *,void *);
EXTERN_C_END
extern int TaoAppSetFiniteDifferencesOptions(TAO_APPLICATION);

/* Supporting utilities for TAO_APPLICATION,  an extension to the TAO_APPLICATION object, but not part of it  */
extern int TaoAppSetVariableBounds(TAO_APPLICATION, Vec, Vec);
extern int TaoApplicationFreeMemory(void*);

#endif
