//File taoapp_ga.h

/**************************************************************

Author: Limin Zhang, Ph.D.
        Mathematics Department
        Columbia Basin College
        Pasco, WA 99301
        Limin.Zhang@cbc2.org

Mentor: Jarek Naplocha, Ph.D.
        Environmental Molecular Science Laboratory
        Pacific Northwest National Laboratory
        Richland, WA 99352

Date: 7/22/2002

Purpose:
      to design and implement an applicaton interface
between TAO and global arrays.

Revise histroy:

8/9/02
	To get rid of Petsc calls and marcos.
 
**************************************************************/

#ifndef TAOGAAPP_H
#define TAOGAAPP_H

#include "taoappobject.h"
#include "taovec_ga.h"
#include "taomat_ga.h"


#ifndef MAX_TAO_MONITORS
#define MAX_TAO_MONITORS 5
#endif

class TaoGAApplication: public TaoApplication{

 protected:

 public:
  TaoGAApplication(MPI_Comm comm);
  ~TaoGAApplication();

  MPI_Comm comm;

  /* Function Evaluation */
  TaoVecGa *taox;    /* The Variable Vector */
  GAVec V;
  void *usrfctx;
  virtual int GetVariableVector(TaoVec **);
  int  (*computeumfunction)(TaoGAApplication*,GAVec,double*,void*);
  virtual int EvaluateObjectiveFunction(TaoVec *xx, double *ff);

  /* Gradient Evaluation */
  void *usrgctx;
  int  (*computegradient)(TaoGAApplication*,GAVec,GAVec,void*);
  virtual int EvaluateGradient(TaoVec *xx, TaoVec *gg);

  /* Function and Gradient */
  void *usrfgctx;
  int  (*computefunctiongradient)(TaoGAApplication*,GAVec,double*,GAVec,void*); 
  int EvaluateObjectiveAndGradient(TaoVec *xx, double *ff, TaoVec *gg);

  /* Hessian Evaluation */
  GAMat  HesMat;
  TaoMatGa *taoh;    /* The Hessian Matrix */
  void *usrhctx;
  virtual int GetHessianMatrix(TaoMat **HH);
  int  (*computehessian)(TaoGAApplication*,GAVec,GAMat,void*);
  virtual int EvaluateHessian(TaoVec *xx, TaoMat *HH);

  /* Set Variable Bounds */
  TaoVecGa *taoxl;   /* The Lower Bounds on the Variable Vector */
  TaoVecGa *taoxu;   /* The Upper Bounds on the Variable Vector */

  virtual int EvaluateVariableBounds(TaoVec *xxll, TaoVec *xxuu);
  void *usrvbctx;
  int (*computebounds)(TaoGAApplication*, GAVec, GAVec, void*);

  virtual int InitializeVariables(TaoVec *x);

  /* Monitoring */
  int numbermonitors;                                        // # of monitors
  int (*monitor[MAX_TAO_MONITORS])(TaoGAApplication*, void*);// monitor routines
  void *monitorcontext[MAX_TAO_MONITORS];                    // monitor contexts


};


// C stubs 
typedef TaoGAApplication* TAO_GA_APPLICATION;
int TaoGAApplicationCreate(MPI_Comm comm, TAO_GA_APPLICATION* newapp);
int TaoGAAppDestroy(TAO_GA_APPLICATION gaapp);
int TaoGAAppSetObjectiveRoutine(TAO_GA_APPLICATION gaapp, GAVec X, 
				int (*func)(TAO_GA_APPLICATION,GAVec,double*,void*),void *ctx);
int TaoGAAppSetGradientRoutine(TAO_GA_APPLICATION gaapp, 
			       int (*grad)(TAO_GA_APPLICATION,GAVec,GAVec,void*),void *ctx);
int TaoGAAppSetObjectiveAndGradientRoutine(TAO_GA_APPLICATION gaapp, 
					   int (*funcgrad)(TAO_GA_APPLICATION,GAVec,double*,GAVec, void*),
					   void *ctx);
int TaoGAAppSetHessianRoutine(TAO_GA_APPLICATION gaapp, 
			      int (*hess)(TAO_GA_APPLICATION, GAVec, GAMat,void*),void *ctx);
int TaoGAAppSetVariableVec(TAO_GA_APPLICATION gaapp, GAVec X);
int TaoGAAppSetInitialSolutionVec(TAO_GA_APPLICATION gaapp, GAVec X0);
int TaoGAAppSetVariableBoundsRoutine(TAO_GA_APPLICATION gaapp, 
				     int (*func)(TAO_GA_APPLICATION, GAVec, GAVec, void*), void *ctx);
int TaoSolveGAApplication(TAO_GA_APPLICATION gaapp, TAO_SOLVER tao);
int TaoSetupGAApplicationSolver(TAO_GA_APPLICATION gaapp, TAO_SOLVER tao);
int TaoGAAppSetMonitor(TAO_GA_APPLICATION gaapp, int (*mymonitor)(TAO_GA_APPLICATION, void*), void *mctx);
int TaoGAAppMonitor(TAO_GA_APPLICATION gaapp);
#endif

