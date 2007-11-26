#if 1
/* 
* main.cpp
* this is a part of itlDragon
* Copyright (C) 2005-2006, ITLab, Kornyakov, Kurina, Zhivoderov
* email: kirillkornyakov@yandex.ru
* email: zhivoderov.a@gmail.com
* email: nina.kurina@gmail.com
*/

#include <iostream>
#include <conio.h>
#include <algorithm>
#include "..\include\parser.h"
#include "..\include\global_placement.h"
#include "..\include\overlap_removing.h"
#include "..\include\detailed_placement.h"
#include "..\include\output.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\check_legality.h"
#include "..\include\errors.h"
#include "..\include\global.h"

using namespace DetailedPlacementNS;

static  char help[] = "This example demonstrates use of the TAO package to \n\
                      solve an unconstrained minimization problem on a single processor.  We \n\
                      minimize the extended Rosenbrock function: \n\
                      sum_{i=0}^{n/2-1} ( alpha*(x_{2i+1}-x_{2i}^2)^2 + (1-x_{2i})^2 ) \n";

int main(int argc, char* argv[])
{  
  Circuit    circuit;
  Statistics statistics;

  MULTIPLACER_ERROR errorCode = OK;

  clock_t startTime  = 0;
  clock_t finishTime = 0;
  clock_t start  = 0;
  clock_t finish = 0;
  double oldWL = 0.0;
  double newWL = 0.0;

  CMDParse(argc, argv);

  // initialize all data structures, parse benchmark and so on
  errorCode = Initialization(circuit, statistics);
  CheckCode(errorCode);

  PetscInitialize(&argc,&argv,(char *)0,help);
  TaoInitialize(&argc,&argv,(char *)0,help);

  int info;       // used to check for functions returning nonzeros
  int size,rank;  // number of processes running
  info = MPI_Comm_size(PETSC_COMM_WORLD,&size); CHKERRQ(info);
  info = MPI_Comm_rank(PETSC_COMM_WORLD,&rank); CHKERRQ(info);

  if (size >1) {
    if (rank == 0)
      PetscPrintf(PETSC_COMM_SELF,"This application is intended for single processor use!\n");
    SETERRQ(1,"Incorrect number of processors");
  }

  // we shift point of origin to the bottom left corner of placement area
  ShiftCoords(circuit);
  statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);
  
  PrintCircuitInfo(circuit);

  if (gOptions.doCheckLegality)
  {// if itlDragon is run with -check key
    cout << "Pin-to-Pin HPWL: " << statistics.currentWL << "\n\n";  
    errorCode = CheckLegalityOfPlacement(circuit);
    CheckCode(errorCode);
    Exit();
  }

  if (gOptions.doConvertToRouter == true)
  {
    cout << "Save nets to router format..." << endl;
    PrintNetsToRouterFormat(circuit);
    cout << "File " << gOptions.GRFileName << " written successful" << endl;
    Exit();
  }

  PrintToTmpPL(circuit, statistics);

  startTime = clock();

  //*************** G L O B A L   P L A C E M E N T ********************//
  if (!gOptions.doGlobalPlacement)  
  { 
    if (gOptions.doOverlapRemoving) // otherwise executed with key -dp
    {
      //errorCode = CreateBinGrid(circuit);
      CheckCode(errorCode);
    }
  }
  else
  {
    cout << "itlDragon started   G L O B A L   P L A C E M E N T\n";
    GlobalPlacement(circuit);
    cout << "G L O B A L   P L A C E M E N T   S U C C E E D\n";
    PrintToTmpPL(circuit, statistics);
  }

  // dump GP if needed
  if (gOptions.doDumpGP)
  {
    statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);

    finishTime = clock();
    statistics.totalWT = (double)(finishTime - startTime) / CLOCKS_PER_SEC;

    char ts[256];
    strcpy(ts, gOptions.benchmarkName);
    PrintToPL(strcat(ts,"_GP.pl"), circuit, statistics);
    if (gOptions.onlyGP)
    {
      CheckCode(EXIT_AFTER_RB_NEEDED);
    }
  }
    
  // update nets' lengths
  statistics.currentWL = cf_recalc_all(UPDATE_NETS_WLS, circuit.nNets, circuit.nets, circuit.placement);

  //*************** D E T A I L E D   P L A C E M E N T **************//
  if (gOptions.doDetailedPlacement/* = false*/)
  {
    cout<<"\nitlDragon started   D E T A I L E D   P L A C E M E N T\n";

    Legalize(circuit);
    errorCode = CheckLegalityOfPlacement(circuit);
    if (errorCode != OK)
    {
      for (int i = 0; i < circuit.nNodes; ++i)
      {
        circuit.placement[i].xCoord = circuit.placement[i].xCoord - fmod(circuit.placement[i].xCoord, circuit.rows[0].siteWidth);
      }
      Legalize(circuit);
    }
    PrintToTmpPL(circuit);
    //statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);
    //cout << statistics.currentWL;
    //CheckOverlaps(circuit);
    errorCode = CheckLegalityOfPlacement(circuit);
    
    CheckCode(errorCode);
    
    char ts[256];
    strcpy(ts, gOptions.benchmarkName);
    statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);
    if (gOptions.doOverlapRemoving) // otherwise executed with key -dp
    {
      PrintToPL(strcat(ts, "_BEFORE_DP.pl"), circuit, statistics);
    }

    errorCode =  DetailedPlacement( circuit, statistics);
    CheckCode(errorCode);
    cout << "D E T A I L E D   P L A C E M E N T   S U C C E E D\n";
  }

  statistics.currentWL = cf_recalc_all(0, circuit.nNets, circuit.nets, circuit.placement);

  finishTime = clock();
  statistics.totalWT = (double)(finishTime - startTime) / CLOCKS_PER_SEC;

  // checking the legality of resulting placement
  errorCode = CheckLegalityOfPlacement(circuit);
  CheckCode(errorCode);

  //*************************** O U T P U T ************************//
  cout << "\nWork Time:\t " << statistics.totalWT << "\n";
  cout << "\nWire length:\t " << statistics.currentWL << "\n";

  PrintToPL(strcat(gOptions.benchmarkName,".pl"), circuit, statistics);
  //PrintPLT (strcat(gOptions.benchmarkName,"t"), circuit, statistics);
  gOptions.benchmarkName[strlen(gOptions.benchmarkName) - 3] = '\0';

  PrintResultString(argc, argv, statistics, errorCode);

  if (gOptions.doTest)
  {
    CreateHTMLReport(statistics.currentWL, statistics.totalWT, errorCode);
  }

  Exit();

  return 0;
}
#endif

#if 0
/*$Id$*/

/* Program usage: mpiexec -np 1 itlAnalyticalPlacer [-help] [all TAO options] */

/*  Include "tao.h" so we can use TAO solvers.  */

#include "tao.h"

static  char help[] = "This example demonstrates use of the TAO package to \n\
                      solve an unconstrained minimization problem on a single processor.  We \n\
                      minimize the extended Rosenbrock function: \n\
                      sum_{i=0}^{n/2-1} ( alpha*(x_{2i+1}-x_{2i}^2)^2 + (1-x_{2i})^2 ) \n";

/*T 
Concepts: TAO - Solving an unconstrained minimization problem
Routines: TaoInitialize(); TaoFinalize();
Routines: TaoApplicationCreate(); TaoAppDestroy();
Routines: TaoCreate(); TaoDestroy(); 
Routines: TaoAppSetObjectiveAndGradientRoutine();
Routines: TaoAppSetHessianMat(); TaoAppSetHessianRoutine();
Routines: TaoSetOptions();
Routines: TaoAppSetInitialSolutionVec();
Routines: TaoSolveApplication();
Routines: TaoGetTerminationReason();
Processors: 1
T*/ 


/* 
User-defined application context - contains data needed by the 
application-provided call-back routines that evaluate the function,
gradient, and hessian.
*/
typedef struct {
  int n;          /* dimension */
  PetscReal alpha;   /* condition parameter */
} AppCtx;

/* -------------- User-defined routines ---------- */
int FormFunctionGradient(TAO_APPLICATION,Vec,double*,Vec,void*);
int FormHessian(TAO_APPLICATION,Vec,Mat*,Mat*,MatStructure*,void*);

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **argv)
{
  int        info;                  /* used to check for functions returning nonzeros */
  PetscScalar zero=0.0;
  Vec        x;                     /* solution vector */
  Mat        H;                     /* Hessian matrix */
  TAO_SOLVER tao;                   /* TAO_SOLVER solver context */
  TAO_APPLICATION taoapp;           /* TAO application context */
  PetscTruth  flg;
  int        size,rank;                  /* number of processes running */
  TaoTerminateReason reason;
  AppCtx     user;                  /* user-defined application context */

  /* Initialize TAO and PETSc */
  PetscInitialize(&argc,&argv,(char *)0,help);
  TaoInitialize(&argc,&argv,(char *)0,help);

  info = MPI_Comm_size(PETSC_COMM_WORLD,&size); CHKERRQ(info);
  info = MPI_Comm_rank(PETSC_COMM_WORLD,&rank); CHKERRQ(info);

  if (size >1) {
    if (rank == 0)
      PetscPrintf(PETSC_COMM_SELF,"This example is intended for single processor use!\n");
    SETERRQ(1,"Incorrect number of processors");
  }


  /* Initialize problem parameters */
  user.n = 2; user.alpha = 99.0;

  /* Check for command line arguments to override defaults */
  info = PetscOptionsGetInt(PETSC_NULL,"-n",&user.n,&flg); CHKERRQ(info);
  info = PetscOptionsGetReal(PETSC_NULL,"-alpha",&user.alpha,&flg); CHKERRQ(info);

  /* Allocate vectors for the solution and gradient */
  info = VecCreateSeq(PETSC_COMM_SELF,user.n,&x); CHKERRQ(info);

  /* 
  Allocate storage space for Hessian matrix; 
  Hessian information is optional -- unless a Newton method is selected
  */
  info = MatCreateSeqBDiag(PETSC_COMM_SELF,user.n,user.n,0,2,0,0,&H);CHKERRQ(info);
  info = MatSetOption(H,MAT_SYMMETRIC); CHKERRQ(info);

  /* The TAO code begins here */

  /* Create TAO solver with desired solution method */
  info = TaoCreate(PETSC_COMM_SELF,"tao_nm",&tao); CHKERRQ(info);
  info = TaoApplicationCreate(PETSC_COMM_SELF,&taoapp); CHKERRQ(info);

  /* Set solution vec and an initial guess */
  info = VecSet(x, zero); CHKERRQ(info);
  info = TaoAppSetInitialSolutionVec(taoapp,x); CHKERRQ(info); 

  /* Set routines for function, gradient, hessian evaluation */
  info = TaoAppSetObjectiveAndGradientRoutine(taoapp,FormFunctionGradient,(void *)&user); 
  CHKERRQ(info);
  info = TaoAppSetHessianMat(taoapp,H,H); CHKERRQ(info);
  info = TaoAppSetHessianRoutine(taoapp,FormHessian,(void *)&user); CHKERRQ(info);

  /* Check for TAO command line options */
  info = TaoSetOptions(taoapp,tao); CHKERRQ(info);

  /* SOLVE THE APPLICATION */
  info = TaoSolveApplication(taoapp,tao); CHKERRQ(info);

  PetscInt ix[2] = {0, 1};
  PetscScalar solution[2];
  VecGetValues(x, PetscInt(2), ix, solution);
  printf("hello from here!\n");
  printf("%f\t%f\n", solution[0], solution[1]);

  /* Get termination information */
  info = TaoGetTerminationReason(tao,&reason); CHKERRQ(info);
  if (reason <= 0)
    PetscPrintf(MPI_COMM_WORLD,"Try a different TAO method, adjust some parameters, or check the function evaluation routines\n");


  /* Free TAO data structures */
  info = TaoDestroy(tao); CHKERRQ(info);
  info = TaoAppDestroy(taoapp); CHKERRQ(info);

  /* Free PETSc data structures */
  info = VecDestroy(x); CHKERRQ(info);
  info = MatDestroy(H); CHKERRQ(info);

  /* Finalize TAO */
  TaoFinalize();
  PetscFinalize();

  return 0;
}

/* -------------------------------------------------------------------- */
#undef __FUNCT__
#define __FUNCT__ "FormFunctionGradient"
/*  
FormFunctionGradient - Evaluates the function, f(X), and gradient, G(X). 

Input Parameters:
.   taoapp  - the TAO_APPLICATION context
.   X    - input vector
.   ptr  - optional user-defined context, as set by TaoSetFunctionGradient()

Output Parameters:
.   G - vector containing the newly evaluated gradient
.   f - function value

Note:
Some optimization methods ask for the function and the gradient evaluation
at the same time.  Evaluating both at once may be more efficient that
evaluating each separately. 
*/
int FormFunctionGradient(TAO_APPLICATION taoapp,Vec X,double *f, Vec G,void *ptr)
{
  AppCtx *user = (AppCtx *) ptr;  
  int    i,info,nn=user->n/2;
  double ff=0,t1,t2,alpha=user->alpha;
  PetscScalar *x,*g;

  /* Get pointers to vector data */
  info = VecGetArray(X,&x); CHKERRQ(info);
  info = VecGetArray(G,&g); CHKERRQ(info);

  /* Compute G(X) */
  for (i=0; i<nn; i++){
    t1 = x[2*i+1]-x[2*i]*x[2*i]; t2= 1-x[2*i];
    ff += alpha*t1*t1 + t2*t2;
    g[2*i] = -4*alpha*t1*x[2*i]-2.0*t2;
    g[2*i+1] = 2*alpha*t1;
  }

  /* Restore vectors */
  info = VecRestoreArray(X,&x); CHKERRQ(info);
  info = VecRestoreArray(G,&g); CHKERRQ(info);
  *f=ff;

  info = PetscLogFlops(nn*15); CHKERRQ(info);
  return 0;
}

/* ------------------------------------------------------------------- */
#undef __FUNCT__
#define __FUNCT__ "FormHessian"
/*
FormHessian - Evaluates Hessian matrix.

Input Parameters:
.  taoapp   - the TAO_APPLICATION context
.  x     - input vector
.  ptr   - optional user-defined context, as set by TaoSetHessian()

Output Parameters:
.  H     - Hessian matrix

Note:  Providing the Hessian may not be necessary.  Only some solvers
require this matrix.
*/
int FormHessian(TAO_APPLICATION taoapp,Vec X,Mat *HH, Mat *Hpre, MatStructure *flag,void *ptr)
{
  AppCtx  *user = (AppCtx*)ptr;
  int     i, nn=user->n/2, info, ind[2];
  double  alpha=user->alpha;
  PetscScalar  v[2][2],*x;
  Mat H=*HH;
  PetscTruth assembled;

  /* Zero existing matrix entries */
  info = MatAssembled(H,&assembled); CHKERRQ(info);
  if (assembled){info = MatZeroEntries(H);  CHKERRQ(info);}


  /* Get a pointer to vector data */
  info = VecGetArray(X,&x); CHKERRQ(info);

  /* Compute H(X) entries */
  for (i=0; i<user->n/2; i++){
    v[1][1] = 2*alpha;
    v[0][0] = -4*alpha*(x[2*i+1]-3*x[2*i]*x[2*i]) + 2;
    v[1][0] = v[0][1] = -4.0*alpha*x[2*i];
    ind[0]=2*i; ind[1]=2*i+1;
    info = MatSetValues(H,2,ind,2,ind,v[0],INSERT_VALUES); CHKERRQ(info);
  }
  info = VecRestoreArray(X,&x); CHKERRQ(info);

  /* Assemble matrix */
  info = MatAssemblyBegin(H,MAT_FINAL_ASSEMBLY); CHKERRQ(info);
  info = MatAssemblyEnd(H,MAT_FINAL_ASSEMBLY); CHKERRQ(info);
  *flag=SAME_NONZERO_PATTERN;

  info = PetscLogFlops(nn*9); CHKERRQ(info);
  return 0;
}
#endif
