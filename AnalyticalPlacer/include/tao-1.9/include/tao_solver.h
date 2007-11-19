
/*
    User interface for the TAO unconstrained minimization solvers
*/
#if !defined(__TAO_SOLVER_H) 
#define __TAO_SOLVER_H
#include "tao_general.h"
#include "taovec.h"
#include "taois.h"
#include "taomat.h"
#include "taolinearsolver.h"
#include "taoappobject.h"
#include "src/appobject/abcapp/taoabcapp.h"
#include "src/lsolver/taomatselfsolver.h"


/*  Convergence flags.
    Be sure to check that these match the flags in 
    $TAO_DIR/include/finclude/tao_solver.h  
*/
typedef enum {/* converged */
              TAO_CONVERGED_ATOL          =  2, /* F < F_minabs */
              TAO_CONVERGED_RTOL          =  3, /* F < F_mintol*F_initial */
              TAO_CONVERGED_TRTOL         =  4, /* step size small */
              TAO_CONVERGED_MINF          =  5, /* grad F < grad F_min */
	      TAO_CONVERGED_USER          =  6, /* User defined */
              /* diverged */
              TAO_DIVERGED_MAXITS         = -2,  
              TAO_DIVERGED_NAN            = -4, 
              TAO_DIVERGED_MAXFCN         = -5,
              TAO_DIVERGED_LS_FAILURE     = -6,
              TAO_DIVERGED_TR_REDUCTION   = -7,
	      TAO_DIVERGED_USER           = -8, /* User defined */
              TAO_CONTINUE_ITERATING      =  0} TaoTerminateReason;

/* Initialization of TAO */
extern int TaoInitialize(int*,char ***,char[],const char[]);
extern int TaoInitializeFortran(void);
extern int TaoFinalize(void);

/* TAO Solvers */
// extern int TaoCreate(TaoMethod,TAO_SOLVER*);
extern int TaoCreate(MPI_Comm,TaoMethod,TAO_SOLVER*);
extern int TaoCreateFull(TaoMethod,const char*,MPI_Comm,TAO_SOLVER*);
//extern int TaoCreateSolver(TaoMethod,char*,MPI_Comm,TAO_SOLVER*);
extern int TaoResetSolver(TAO_SOLVER);
extern int TaoSetMethod(TAO_SOLVER,TaoMethod);
extern int TaoSetUp(TAO_SOLVER);
extern int TaoSolve(TAO_SOLVER);
extern int TaoSetDown(TAO_SOLVER);
extern int TaoDestroy(TAO_SOLVER);

// extern int TaoSetInitialVector(TAO_SOLVER,TaoVec*);

extern int TaoMonitor(TAO_SOLVER,int,double,double,double,double,TaoTerminateReason*);
extern int TaoSetMonitor(TAO_SOLVER,int(*)(TAO_SOLVER,void*),void *);
extern int TaoClearMonitor(TAO_SOLVER);
extern int TaoSetConvergenceHistory(TAO_SOLVER,double*,int *,int,TaoTruth);
extern int TaoSetDefaultParameters(TAO_SOLVER);
extern int TaoSetDefaultStatistics(TAO_SOLVER);
extern int TaoSetDefaultMonitors(TAO_SOLVER);
extern int TaoGetConvergenceHistory(TAO_SOLVER,double**,int **,int *);
extern int TaoSetDestroyRoutine(TAO_SOLVER,int (*)(void*),void *);

extern int TaoGetArgs(int *,char ***);
extern int TaoStandardRegisterAll();
extern int TaoRegisterAll(const char*);

extern int TaoGetLinearSolver(TAO_SOLVER,TaoLinearSolver **);
extern int TaoDestroyLinearSolver(TAO_SOLVER);
extern int TaoGetSolution(TAO_SOLVER,TaoVec**);
extern int TaoGetStepDirectionVector(TAO_SOLVER,TaoVec**);
extern int TaoSetStepDirectionVector(TAO_SOLVER,TaoVec*);
extern int TaoGetFunction(TAO_SOLVER,TaoVec**,void**);
extern int TaoView(TAO_SOLVER);

extern int TaoSetFromOptions(TAO_SOLVER);
extern int TaoSetMethodFromOptions(TAO_SOLVER);

extern int TaoGetMethod(TAO_SOLVER,TaoMethod*);
extern int TaoDefaultMonitor(TAO_SOLVER,void*);
extern int TaoVecViewMonitor(TAO_SOLVER,void*);
extern int TaoVecViewMonitorUpdate(TAO_SOLVER,void *);
extern int TaoDefaultSMonitor(TAO_SOLVER,void *);
extern int TaoSetTolerances(TAO_SOLVER,double,double,double,double);
extern int TaoGetTolerances(TAO_SOLVER,double*,double*,double*,double*);

extern int TaoSetTrustRegionTolerance(TAO_SOLVER,double);
extern int TaoSetFunctionLowerBound(TAO_SOLVER,double);
extern int TaoSetMaximumIterates(TAO_SOLVER,int);
extern int TaoSetMaximumFunctionEvaluations(TAO_SOLVER,int);
extern int TaoIncrementGradientsCounter(TAO_SOLVER,int);

extern int TaoGetGradientTolerances(TAO_SOLVER,double*,double*,double*);
extern int TaoSetGradientTolerances(TAO_SOLVER,double,double,double);
extern int TaoGetInitialTrustRegionRadius(TAO_SOLVER,double*);
extern int TaoGetTrustRegionRadius(TAO_SOLVER,double*);
extern int TaoSetTrustRegionRadius(TAO_SOLVER,double);
extern int TaoSetVariableBounds(TAO_SOLVER,TaoVec*,TaoVec*);
extern int TaoGetVariableBounds(TAO_SOLVER,TaoVec**,TaoVec**);
extern int TaoEvaluateVariableBounds(TAO_SOLVER,TaoVec*,TaoVec*);
extern int TaoGetDualVariables(TAO_SOLVER,TaoVec*,TaoVec*);
extern int TaoCGSetRestartTol(TAO_SOLVER,double);
extern int TaoBCGSetRestartTol(TAO_SOLVER,double);
extern int TaoTRONSetMaxPGIts(TAO_SOLVER,int);
extern int TaoGPCGSetMaxPGIts(TAO_SOLVER,int);
extern int TaoInitializeLMVMmatrix(TAO_SOLVER, TaoVec*);
extern int TaoLMVMSetH0(TAO_SOLVER, TaoTruth);
extern int TaoBLMVMSetH0(TAO_SOLVER, TaoTruth);

extern int TaoSetConvergenceTest(TAO_SOLVER,int (*)(TAO_SOLVER,void*),void*);
extern int TaoCheckConvergence(TAO_SOLVER, TaoTerminateReason*);
extern int TaoGetTerminationReason(TAO_SOLVER,TaoTerminateReason*);
extern int TaoSetTerminationReason(TAO_SOLVER,TaoTerminateReason);
extern int TaoGetSolutionStatus(TAO_SOLVER,int*,double*,double*,double*,double*,TaoTerminateReason *);
#define    TaoGetIterationData(a,b,c,d,e,f,g)      TaoGetSolutionStatus(a,b,c,d,e,f,g)


extern int TaoGetHessian(TAO_SOLVER,TaoMat**);
extern int TaoGetGradient(TAO_SOLVER,TaoVec**);
extern int TaoSetLagrangianGradientVector(TAO_SOLVER,TaoVec*);
extern int TaoComputeFunction(TAO_SOLVER,TaoVec*,double*);
extern int TaoComputeGradient(TAO_SOLVER,TaoVec*,TaoVec*);
extern int TaoComputeFunctionGradient(TAO_SOLVER,TaoVec*,double*,TaoVec*);
extern int TaoLineSearchSetDampingParameter(TAO_SOLVER,double*);

extern int TaoSetDefaultMeritFunction(TAO_SOLVER);
extern int TaoComputeMeritFunction(TAO_SOLVER,TaoVec*,double *);
extern int TaoComputeMeritGradient(TAO_SOLVER,TaoVec*,TaoVec*);
extern int TaoComputeMeritFunctionGradient(TAO_SOLVER,TaoVec*,double *,TaoVec*);
extern int TaoSetMeritFunction(TAO_SOLVER, int (*)(TAO_SOLVER,TaoVec*,double*,void*),int (*)(TAO_SOLVER,TaoVec*,double*,TaoVec*,void*),int (*)(TAO_SOLVER,TaoVec*,TaoVec*,void*),int (*)(TAO_SOLVER,void*),void *);
extern int TaoMeritFunctionDestroy(TAO_SOLVER);


extern int TaoConverged_MaxIts(TAO_SOLVER,void*);
extern int TaoConverged_Default(TAO_SOLVER,void*);
extern int TaoCheckFG(TAO_SOLVER);
extern int TaoCheckFGH(TAO_SOLVER);
extern int TaoCheckConstraints(TAO_SOLVER);
extern int TaoCheckConstraintsJ(TAO_SOLVER);
extern int TaoCheckBounds(TAO_SOLVER);
extern int TaoLMVMGetX0(TAO_SOLVER,TaoVec*);

extern int TaoComputeHessian(TAO_SOLVER,TaoVec*,TaoMat*);

extern int TaoComputeJacobian(TAO_SOLVER,TaoVec*,TaoMat*);
extern int TaoComputeConstraints(TAO_SOLVER,TaoVec*,TaoVec*);
extern int TaoSetConstraintsBounds(TAO_SOLVER,TaoVec *,TaoMat *, TaoVec *);

extern int TaoSetLineSearch(TAO_SOLVER, int (*)(TAO_SOLVER,void*),int (*)(TAO_SOLVER,void*),int (*)(TAO_SOLVER,TaoVec*,TaoVec*,TaoVec*,TaoVec*,double*,double*,double*,int*,void*),int (*)(TAO_SOLVER,void*),int (*)(TAO_SOLVER,void*),void *);

extern int TaoCreateMoreThuenteBoundLineSearch(TAO_SOLVER, double, double);
extern int TaoCreateMoreThuenteLineSearch(TAO_SOLVER,double,double);
extern int TaoCreateUnitLineSearch(TAO_SOLVER);
extern int TaoCreateProjectedLineSearch(TAO_SOLVER);
extern int TaoCreateNDProjectedArmijoLineSearch(TAO_SOLVER);
extern int TaoCreateNDProjectedLineSearch(TAO_SOLVER);
extern int TaoCreateArmijoLineSearch(TAO_SOLVER);
extern int TaoCreateNDArmijoLineSearch(TAO_SOLVER);
extern int TaoCreateProjectedArmijoLineSearch(TAO_SOLVER);
extern int TaoLineSearchView(TAO_SOLVER);
extern int TaoLineSearchSetUp(TAO_SOLVER);
extern int TaoLineSearchDestroy(TAO_SOLVER);
extern int TaoLineSearchApply(TAO_SOLVER,TaoVec*,TaoVec*,TaoVec*,TaoVec*,double*,double*,double*,int*);
extern int TaoLineSearchSetFromOptions(TAO_SOLVER);

extern int TaoGetConstraints(TAO_SOLVER, TaoVec **);
extern int TaoGetJacobian(TAO_SOLVER, TaoMat **);

extern int TaoSetSolver(TAO_SOLVER,int (*)(TAO_SOLVER,void*),int (*)(TAO_SOLVER,void*),int (*)(TAO_SOLVER,void*),int (*)(TAO_SOLVER,void*),int (*)(TAO_SOLVER,void*),void *);
extern int TaoSetTaoViewRoutine(TAO_SOLVER, int (*)(TAO_SOLVER,void*));
extern int TaoSetTaoOptionsRoutine(TAO_SOLVER, int (*)(TAO_SOLVER,void*));
extern int TaoSetTaoSetUpDownRoutines(TAO_SOLVER, int (*)(TAO_SOLVER,void*), int (*)(TAO_SOLVER,void*));
extern int TaoSetTaoSolveRoutine(TAO_SOLVER, int (*)(TAO_SOLVER,void*), void*);
extern int TaoSetTaoDualVariablesRoutine(TAO_SOLVER, int (*)(TAO_SOLVER,TaoVec*,TaoVec*,void*));
extern int TaoGetSolverContext(TAO_SOLVER, TaoMethod, void **);

extern int TaoCreateLinearSolver(TAO_SOLVER, TaoMat *, int, TaoLinearSolver **);
extern int TaoLinearSolve(TAO_SOLVER, TaoMat *, TaoVec *, TaoVec*, TaoTruth *);
extern int TaoPreLinearSolve(TAO_SOLVER, TaoMat *);
extern int TaoLinearSolveTrustRegion(TAO_SOLVER, TaoMat *, TaoVec *, TaoVec*, double, TaoTruth *);
extern int TaoViewLinearSolver(TAO_SOLVER);
extern int TaoSetLinearSolverOptions(TAO_SOLVER);

extern int TaoSetApplication(TAO_SOLVER, TaoApplication *);
extern int TaoGetApplication(TAO_SOLVER, TaoApplication **);
extern int TaoDestroyApplication(TaoApplication *);

extern int TaoVecDestroy(TaoVec*);
extern int TaoMatDestroy(TaoMat*);
extern int TaoIndexSetDestroy(TaoIndexSet*);

#endif

