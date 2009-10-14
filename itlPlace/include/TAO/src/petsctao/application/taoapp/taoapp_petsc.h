#ifndef TAOPETSCAPP_H
#define TAOPETSCAPP_H


#include "taoappobject.h"
#include "../../vector/taovec_petsc.h"
#include "../../matrix/taomat_petsc.h"
#include "../../linearsolver/taolinearsolver_petsc.h"
#include "petscksp.h"

//#include "taoapp.h"

typedef struct _p_TAO_SOLVER* TAO_SOLVER;
typedef struct _p_TAOAPPLICATION* TAO_APPLICATION;


class TaoPetscApplication: public TaoApplication{

 protected:
  MPI_Comm comm;

 public:
  TaoPetscApplication(MPI_Comm comm);
  TaoPetscApplication(){};
  int Setup();

  ~TaoPetscApplication();
  int TakeDown();

  TAO_SOLVER tao;
  TAO_APPLICATION papp;
  int GetComm(MPI_Comm *comm);
  int Monitor();
  int Monitor2(TaoVec *xx, TaoVec *gl, TaoVec *dx,TaoTruth *flag);

  /* Function Evaluation */
  int InitializeVariables(TaoVec *x);

  int GetVariableVector(TaoVec **xx);
  int EvaluateObjectiveFunction(TaoVec *xx, double *ff);

  /* Gradient Evaluation */
  int EvaluateGradient(TaoVec *xx, TaoVec *gg);

  /* Function and Gradient */
  int EvaluateObjectiveAndGradient(TaoVec *xx, double *ff, TaoVec *gg);


  /* Hessian Evaluation */
  int GetHessianMatrix(TaoMat **HH);
  int EvaluateHessian(TaoVec *xx, TaoMat *HH);

  int HessianSolve(TaoVec *vv, TaoVec *ww, TaoTruth *flag);

  /* Set Variable Bounds */
  int EvaluateVariableBounds(TaoVec *, TaoVec *);

  int GetLinearSolver(TaoMat *, int, TaoLinearSolver **);

  /* Routines below this are very preliminary */
  int GetJacobianMatrix(TaoMat **JJ);
  int EvaluateJacobian(TaoVec *xx, TaoMat *JJ);

  int GetConstraintVector(TaoVec** rr);
  int EvaluateConstraints(TaoVec *xx, TaoVec *RR);
 
  virtual int GetInequalityConstraints(TaoVec**, TaoMat **, TaoVec **);

  // Its really stupid to have these public; fix it later
  TaoVecPetsc *taox;    /* The Variable Vector */
  TaoMatPetsc *taoh;    /* The Hessian Matrix */
  TaoMatPetsc *taoj;    /* The Jacobian Matrix */
  TaoVecPetsc *taofv;
  TaoVecPetsc *taofvll;
  TaoVecPetsc *taofvuu;
  TaoMatPetsc *taoAA;
  TaoLinearSolverPetsc *ksptmp;
};

#endif


