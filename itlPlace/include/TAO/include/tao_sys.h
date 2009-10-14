/*
   This is the main TAO include file (for C and C++).  It is included
   by all other TAO include files, so it almost never has to be 
   specifically included.
*/
#if !defined(__TAO_TSYS_H)
#define __TAO_TSYS_H

/* ========================================================================== */

#include <float.h>
#include <math.h>

#include "tao_basictypes.h"

typedef struct _p_TAO_SOLVER* TAO_SOLVER;
typedef const char* TaoMethod;

extern int TAO_COOKIE;
extern int TaoInitializeCalled;

#define TaoMin(a,b)      (((a)<(b)) ? (a) : (b))
#define TaoMax(a,b)      (((a)<(b)) ? (b) : (a))
#define TaoMid(a,b,c)    (((a) < (b)) ?                    \
                           (((b) < (c)) ? (b) :            \
                             (((a) < (c)) ? (c) : (a))) :  \
                           (((a) < (c)) ? (a) :            \
                             (((b) < (c)) ? (c) : (b))))
#define TaoAbsInt(a)     (((a)<0)   ? -(a) : (a))
#define TaoAbsDouble(a)  (((a)<0)   ? -(a) : (a))
#define TaoAbsScalar(a)     (((a)<0.0)   ? -(a) : (a))

// This test works because Inf - Inf = NaN, NaN - NaN = NaN,
// and NaN is not equal to anything else.
// #define TaoNaN(a)        ((a) != (a))
// #define TaoInf(a)       (((a) == (a)) && ((a) - (a)) != 0.0)
// #define TaoInfOrNaN(a)                  (((a) - (a)) != 0.0)

#if defined(_MSC_VER)

//Special Microsoft builtins
#define TaoNaN(a)        _isnan(a)
#define TaoInf(a)        (!_finite(a))
#define TaoInfOrNaN(a)	 TaoInf(a)

#else

// These tests are from ISO C99
#define TaoNaN(a)        isnan(a)
#define TaoInf(a)        isinf(a)
#define TaoInfOrNaN(a)   (TaoInf(a) || TaoNaN(a))

#endif

#define TAO_DEFAULT        -13456834
#define TAO_INFINITY        1.0e20
#define TAO_NINFINITY       -1.0e20
#define TAO_NULL           0
#define TAO_EPSILON     DBL_EPSILON

#if !defined(__FUNCT__)
#define __FUNCT__ "unknownfunction"
#endif

/* Basic Error handling macros -- Defined later to provide more support such as printing the message
   #define __FUNCT__                      "Function not known"
   #define SETERRQ1(a,b,c)               {if (a){ return(a); }}
   #define SETERRQ(a,b)                  {if (a){ return(a); }}
   #define CHKERRQ(n)                    {if (n){ return n;  }}
   #define PetscLogObjectMemory(a,b)     {if (0==1){}}
*/

/* Additional PETSc support needs to be defined */
//#ifndef PetscLogInfo
//#define PetscLogInfo(a)                 0;
//#endif

#define TaoFunctionBegin                ;
#define TaoFunctionReturn               return

#define TaoValidIntPointer(h,arg)                      \
  {if (!h) {SETERRQ(1,"Null Int Pointer");}      \
  if ((unsigned long)h & (unsigned long)3){        \
    SETERRQ(1,"Invalid Pointer to Int: Parameter # %d",arg);         \
  }}

#define TaoValidScalarPointer(h,arg)                    \
  {if (!h) {SETERRQ(1,"Null Pointer");}           \
  if ((unsigned long)h & (unsigned long)3) {        \
    SETERRQ(1,"Invalid Pointer to PetscScalar: Parameter # %d",arg);  \
  }}


/* Basic Memory Management Macros */
#define TaoMalloc(A,b)        0;(*((void**)b))=malloc(A)
#define TaoNew(A,b)        TaoMalloc(sizeof(A),(b))
#define TaoFree(a)         0; free (a) ;

/* Printing statements */
extern int TaoPrintStatement(TAO_SOLVER, const char *);
extern int TaoPrintInt(TAO_SOLVER, const char *,int);
extern int TaoPrintDouble(TAO_SOLVER, const char *, double);
extern int TaoPrintString(TAO_SOLVER, const char *, const char*);

/* Collect Runtime Options */
extern int TaoOptionsHead(const char *);
extern int TaoOptionsTail();
extern int TaoOptionInt(const char *,const char *,const char *,int,int *,TaoTruth *);
extern int TaoOptionDouble(const char *,const char *,const char *,double,double *,TaoTruth *);
extern int TaoOptionName(const char *,const char *,const char *,TaoTruth *);
extern int TaoOptionList(const char *opt, const char *ltext, const char *man,
                         const char **list, int nlist, const char *defaultv,
                         int *value, TaoTruth *set);
extern int TaoMethodsList(const char *,const char *,const char *,const char *,char *,int,TaoTruth *);
extern int TaoOptionString(const char *,const char *,const char *,const char *,char *,int,TaoTruth *);

extern int TaoStrcpy(char*,const char *);
extern int TaoStrcmp(const char *,const char *,TaoTruth*);

/* 
   Register the solvers into an TAO_SOLVER archive -- 
   Create the archive, add solvers to it, find solvers, destroy archive 
*/
extern int TaoLogClassRegister(int*,const char *);
extern int TaoFindSolver(TAO_SOLVER, TaoMethod type,  int (**)(TAO_SOLVER)  );
extern int TaoRegisterDestroy(void);
extern int TaoCompareMethod(TAO_SOLVER, TaoMethod,TaoTruth *);


/* The TAO header -- can be customized to include other structures such as a PetscObject */
#define TAOHEADER(a)                    a comm; a refct; int tcookie; char type_name[50];
#define TaoValidHeaderSpecific(a,b,arg) \
    {if (!a) {SETERRQ(1,"Null TAO Object: Parameter # %d",arg);}}
#define TaoValidHeader(a,arg)  \
    {if (!a) {SETERRQ(1,"Null TAO Object: Parameter # %d",arg);}}

/* Given the customized header, there must be routines for creating the solver, destroying it, etc. */
/* extern int TaoObjectCreate( TAO_SOLVER *, MPI_Comm); */
extern int TaoObjectDestroy( TAO_SOLVER );

#if defined(__cplusplus)
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN 
#define EXTERN_C_END 
#endif

EXTERN_C_BEGIN /* // make extern c to allow c funtion pointer as argument */
extern int TaoRegisterDynamic(const char *,const char *,const char *,int(*)(TAO_SOLVER));
EXTERN_C_END


#endif
