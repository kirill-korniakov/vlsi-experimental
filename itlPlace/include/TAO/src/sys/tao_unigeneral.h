/*
   This is the main TAO include file (for C and C++).  It is included
   by all other TAO include files, so it almost never has to be 
   specifically included.
*/
#if !defined(__TAO_UNI_SYSTEMS_H)
#define __TAO_UNI_SYSTEMS_H

/* ========================================================================== */

#include <float.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>

#include "tao_sys.h"

/* Basic MPI redefinitions */
typedef int MPI_Comm;
#ifndef MPI_COMM_WORLD
#define MPI_COMM_WORLD 0
#endif
#ifndef MPI_COMM_SELF
#define MPI_COMM_SELF 0
#endif

/* Basic Error handling macros -- Minimal compared to the PETSc macros */
#ifndef SETERRQ1
#define SETERRQ1(a,b,c)               {if (a){ printf(b,c); return(a); }}
#endif
#ifndef SETERRQ
#define SETERRQ(a,b)                  {if (a){ printf(b); return(a); }}
#endif
#ifndef CHKERRQ
#define CHKERRQ(n)                      {if (n) {printf("Error: %s\n",__FUNCT__); return n;}}
#endif
#ifndef PetscLogObjectMemory
#define PetscLogObjectMemory(a,b)       {if (0==1){}}
#endif

#undef  TaoValidIntPointer
#define TaoValidIntPointer(h,arg)                      \
  {if (!h) {SETERRQ(1,"Null Int Pointer: Parameter # %d",arg);}      \
  if ((unsigned long)h & (unsigned long)3){        \
    SETERRQ(1,"Invalid Pointer to Int: Parameter # %d",arg);         \
  }}

#undef  TaoValidScalarPointer
#define TaoValidScalarPointer(h,arg)                    \
  {if (!h) {SETERRQ(1,"Null Pointer: Parameter # %d",arg);}           \
  if ((unsigned long)h & (unsigned long)3) {        \
    SETERRQ(1,"Invalid Pointer to PetscScalar: # %d",arg);  \
  }}

/* Basic Memory Management Macros */
// #define TaoMalloc(A,b)        0;(*((void**)b))=malloc(A)
// #define TaoNew(A,b)        TaoMalloc(sizeof(A),(b))
// #define TaoFree(a)         0; free (a) ;

/* Given the customized header, there must be routines for creating the solver, destroying it, etc. */
extern int TaoObjectCreate( TAO_SOLVER *, MPI_Comm);

#if defined(__cplusplus)
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN 
#define EXTERN_C_END 
#endif

#endif
