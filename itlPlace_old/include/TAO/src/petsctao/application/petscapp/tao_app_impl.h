#ifndef PETSCAPP_H
#define PETSCAPP_H

#include "petscksp.h"

// typedef struct _p_TAO_SOLVER* TAO_SOLVER;
typedef struct _p_TAOAPPLICATION* TAO_APPLICATION;

#ifndef MAX_TAO_MONITORS
#define MAX_TAO_MONITORS 5
#endif

#ifndef MAX_TAO_USER_DESTROY
#define MAX_TAO_USER_DESTROY 50
#endif

#define MAX_TAO_KEY_LENGTH 100
#define MAX_TAOAPP_ADDON 10

typedef struct{
  int id;
  char key[MAX_TAO_KEY_LENGTH];
  void *ctx;
} TaoAppAddOn;

struct _p_TAOAPPLICATION {

  PETSCHEADER(int);

  int nAddOn;
  TaoAppAddOn  TaoAppCtx[MAX_TAO_KEY_LENGTH];

  /* --------------------------  Monitoring  ------------------------------------*/
  double grtol;
  int   numbermonitors;                            /* number of monitors */
  int   (*monitor[MAX_TAO_MONITORS])(struct _p_TAOAPPLICATION*,void*); /* monitor routine */
  void  *monitorcontext[MAX_TAO_MONITORS];         /* monitor context */

  int   numberoptioncheckers;                            /* number of monitors */
  int   (*checkoptions[MAX_TAO_MONITORS])(struct _p_TAOAPPLICATION*); /* monitor routine */

  /* Vector of Variables */
  Vec V;

  /* Objective Function */
  void *usrfctx;
  int  (*computeumfunction)(TAO_APPLICATION,Vec,double*,void*);
  int nfeval;

  /* Gradient Evaluation */
  void *usrgctx;
  int  (*computegradient)(TAO_APPLICATION,Vec,Vec,void*);
  Vec G;
  int ngeval;

  /* Function and Gradient */
  void *usrfgctx;
  int  (*computefunctiongradient)(TAO_APPLICATION,Vec,double*,Vec,void*); 

  /* Hessian Evaluation */
  void *usrhctx;
  int  (*computehessian)(TAO_APPLICATION,Vec,Mat*,Mat*,MatStructure*,void*);
  Mat H,HP;
  int nheval;

  /* ApproximateHessian */
  void *usrhhhctx;
  int  (*hessiansolve)(TAO_APPLICATION,Vec,Vec,PetscTruth*,void*); 
  int nlsolve;

  /* Set Variable Bounds */
  int  (*computebounds)(TAO_APPLICATION,Vec,Vec,void*);
  void *boundctx;

  /* Routines below this are very preliminary */
  void *usrjctx;
  int  (*computejacobian)(TAO_APPLICATION,Vec,Mat*,Mat*, MatStructure*,void*);
  Mat  J,JP;

  void *usrfvctx;
  int  (*computevfunc)(TAO_APPLICATION,Vec,Vec,void*);
  Vec R;

  /* Routines called when destroying this structure */
  int   numberdestroyers;
  int   (*userdestroy[MAX_TAO_USER_DESTROY])(void*);
  void  *userctxdestroy[MAX_TAO_USER_DESTROY];

  KSP ksp;
};

#endif
