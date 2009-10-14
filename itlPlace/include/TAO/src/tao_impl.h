#ifndef __TAO_IMPL_H
#define __TAO_IMPL_H

#include "tao_solver.h"

/*
   TAO solver context
 */
#define MAX_TAO_MONITORS 5

#ifndef MAX_TAO_USER_DESTROY
#define MAX_TAO_DESTROY 50
#endif

struct _p_TAO_SOLVER {

  TAOHEADER(int);

  /*  ------------------------ User-provided Info -------------------------------*/

  /*  --- Routines and data that are unique to each particular solver ----------- */
  void         *data;                              /* algorithm implementation-specific data */

  int          (*setup)(TAO_SOLVER,void*);               /* routine to set up the nonlinear solver */
  int          (*solve)(TAO_SOLVER,void*);          /* a nonlinear optimization solver */
  int          (*setdown)(TAO_SOLVER,void*);             /* destroys solver */

  int          (*setfromoptions)(TAO_SOLVER,void*);      /* sets options from database */
  int          (*view)(TAO_SOLVER,void*);         /* views solver info */

  TaoTruth        setupcalled;        /* true if setup has been called */
  TaoTruth        set_method_called;  /* flag indicating set_method has been called */

  /* ------------------------- Function Evaluation ------------------------*/
  TaoApplication* taoappl;

  TaoVec*          vec_sol;            /* pointer to solution */
  TaoVec*          vec_sol_update;              /* pointer to solution update */
  double       fc;                                 /* function value */
  int          nfuncs;             /* number of function evaluations */
  int          max_funcs;          /* max number of function evals */

  /* ------------------------- Gradient Evaluation ------------------------*/
  TaoVec*          vec_grad;          /* pointer to gradient */
  int          ngrads;             /* number of gradient evaluations */
  int          nfgrads;             /* number of function/gradient evaluations */
  TaoTruth     viewgradient;

  /* ------------------------- Hessian Evaluation --------------------------*/
  TaoMat    * hessian;                            /* Hessian matrix */
  int          nhesss;          /* number of Hessian evaluations */
  TaoTruth     viewhessian;
  TaoLinearSolver*  ksp;      /* linear solver context */
        
  /* Bound Information */
  TaoVec*          XL;                                 /* lower bound */
  TaoVec*          XU;                                 /* upper bound */
  double       cnorm;
  double       cnorm0;
  int    (*CopyDuals)(TAO_SOLVER,TaoVec*,TaoVec*,void*);


  /* Constraint information */
  TaoMat *jacobian;
  int njac;
  TaoTruth     viewjacobian;

  TaoVec* vfunc;
  TaoVec *RXL, *RXU;
  TaoMat *CA;
  int nvfunc;
  TaoTruth     viewvfunc;


  /* ------------------------ Line Search Context -------------------------*/
  /* Line Search termination code and function pointers */
  void         *linectx;

  int lsflag;  /* Line search termination code (set line=1 on success) */

  int    (*LineSearchSetUp)(TAO_SOLVER,void*);
  int    (*LineSearchSetFromOptions)(TAO_SOLVER,void*);
  int    (*LineSearchApply)(TAO_SOLVER,TaoVec*,TaoVec*,TaoVec*,TaoVec*,double*,double*,double*,int*,void*);
  int    (*LineSearchView)(TAO_SOLVER,void*);
  int    (*LineSearchDestroy)(TAO_SOLVER,void*);

  /* Support for a merit function */
  int    (*MeritFunctionApply)(TAO_SOLVER,TaoVec *, double *, void*);
  int    (*MeritFunctionGradientApply)(TAO_SOLVER,TaoVec *, double *, TaoVec *, void*);
  int    (*MeritGradientApply)(TAO_SOLVER,TaoVec *, TaoVec *, void*);
  int    (*MeritFunctionDestroy)(TAO_SOLVER,void*);
  void *meritctx;
  
  /* Trust Region information */
  double     trtol;    /* Miminum Trust region radius */
  double     trust0;   /* Initial Trust Region Radius */
  double     step;    /* last step length or trust region radius */

  /* --------------------------  Monitoring  ------------------------------------*/

  int   numbermonitors;                            /* number of monitors */
  int   (*defaultmonitor)(TAO_SOLVER,void*);       /* default monitor routine */
  int   (*monitor[MAX_TAO_MONITORS])(TAO_SOLVER,void*); /* monitor routine */
  void  *monitorcontext[MAX_TAO_MONITORS];         /* monitor context */
  int   (*converged)(TAO_SOLVER,void*);  /* convergence routine */

  /* --------------------------  Parameters -------------------------------------- */

  double     fatol;              /* Absolute tolerance for objective value */
  double     frtol;              /* Relative tolerance for objective value */
  double     catol;              /* Absolute tolerance for constraints */
  double     crtol;              /* Relative tolerance for constraints */
  double     gatol;              /* Absolute tolerance for gradient */
  double     grtol;              /* Relative tolerance for gradient */
  double     gttol;              /* Relative tolerance for gradient */
  double     xtol;               /* relative tolerance in solution */
  double     fmin;               /* minimum tolerance for function value */

  /* -------------------------- Statistics -------------------------------------- */
  int        max_its;            /* max number of iterations */
  int        iter;               /* global iteration number */
  TaoTerminateReason reason;

  double     norm;               /* KKT residual norm of current iterate */
  double     norm0;              /* KKT residual norm of residual iterate */
  int        linear_its;         /* total number of linear solver iterations */

  /* ------------------------- Convergence History ------------------------ */

  void       *cnvP;	         /* convergence context */
  double     *conv_hist;         /* If !0, stores function norm (or
				    gradient norm) at each iteration */
  int        *conv_hist_its;     /* linear iterations for each Newton step */
  int        conv_hist_len;      /* size of convergence history array */
  int        conv_hist_max;      /* actual amount of data in conv_history */
  TaoTruth conv_hist_reset;    /* reset counter for each new TAO_SOLVER solve */

  TaoTruth     viewtao;
  TaoTruth     viewksptao;

  /* Routines called when destroying this structure */
  int   numberdestroyers;
  int   (*userdestroy[MAX_TAO_DESTROY])(void*);
  void  *userctxdestroy[MAX_TAO_DESTROY];


  /* ------------------------ Default Work-area Management ---------------------- */

  TaoVec*        WorkX1;
};

#define TaoLogConvHistory(tao_um,res,its) \
  { if (tao_um->conv_hist && tao_um->conv_hist_max > tao_um->conv_hist_len) \
    { tao_um->conv_hist[tao_um->conv_hist_len]       = res; \
      tao_um->conv_hist_its[tao_um->conv_hist_len++] = its; \
    }}


#endif
