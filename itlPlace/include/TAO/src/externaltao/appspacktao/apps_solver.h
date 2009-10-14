#ifndef APPS_SOLVER_H
#define APPS_SOLVER_H

#include "tao_solver.h"


typedef struct
{
  int ndim;
  int usecache;  // Assume cache is to be use unless "-apps_cache false" used in options
  int argc;
  TaoVec *xl, *xu;
  char **argv;
  double fval;
} TAO_APPS;

EXTERN_C_BEGIN
extern int TaoRegisterAppspack(void);
int getoptions(TAO_APPS *appsPtr);
int TaoCreate_APPS(TAO_SOLVER tao);
int gather(TAO_SOLVER tao, TAO_APPS *appsPtr);
extern int setargs(TAO_SOLVER tao, int argc, char *argv[]);
EXTERN_C_END

#endif
