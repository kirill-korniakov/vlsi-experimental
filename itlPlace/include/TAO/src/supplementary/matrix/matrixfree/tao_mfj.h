/*$Id$*/

/*
    This file should be included in NEW routines that compute the
    differencing parameter for finite difference based matrix-free
    methods.  For example, such routines can compute h for use in
    Hessian-vector products of the form

                       g(u+ha) - g(u)
          H(u)a  ~=  ---------------- ,
                            h

     where g(u) denotes the gradient of f(u) and H(u) denotes the
     Hessian of f(u).
*/

#if !defined(__TAO_MFJ_H__)
#define __TAO_MFJ_H__

#include "include/tao_solver.h"         /*I  "tao_solver.h"   I*/

/*
    Table of functions that manage the computation and understanding
    of the parameter for finite difference based matrix-free computations
*/
typedef struct {
  int (*compute)(MatTaoMFCtx,Vec,Vec,Scalar *);
  int (*view)(MatTaoMFCtx,Viewer);
  int (*destroy)(MatTaoMFCtx);
  int (*printhelp)(MatTaoMFCtx);
  int (*setfromoptions)(MatTaoMFCtx);
} TaoMFOps;

struct _p_MatTaoMFCtx {    /* context for default matrix-free TAO_SOLVER */
  TaoMFOps     *ops;                   /* function table */
  MPI_Comm     comm;                   /* communicator */
  TAO_SOLVER   tao;                    /* TAO solver */
  Vec          w;                      /* work vector */
  MatNullSpace  sp;                     /* null space context; not currently used  */
  double       error_rel;              /* square root of relative error in computing function */
  Scalar       currenth;               /* last differencing parameter h used */
  Scalar       *historyh;              /* history of differencing parameter h */
  int          ncurrenth, maxcurrenth; 
  void         *hctx;
  char         type_name[256];
  Mat          mat;                    /* back reference to shell matrix that contains this */
  /*
        The next three are used only if user called MatTaoMFSetFunction()
  */
  int          (*func)(TAO_SOLVER,Vec,Vec,void*);  /* function used for matrix-free */
  void         *funcctx;                /* the context for the function */
  Vec          funcvec;                 /* location to store func(u) */
};

extern FList MatTaoMFList;
extern int   MatTaoMFRegisterAllCalled;

#endif
