/*
   This is the main TAO include file (for C and C++).  It is included
   by all other TAO include files, so it almost never has to be 
   specifically included.
*/
#if !defined(__TAO_PETSC_SYS_H)
#define __TAO_PETSC_SYS_H

#include "tao_sys.h"

#include "petsc.h"

/* typedef PetscScalar  TaoScalar; */

/* Error handling macros */
#undef  TaoFunctionBegin
#define TaoFunctionBegin             PetscFunctionBegin
#undef  TaoFunctionReturn
#define TaoFunctionReturn            PetscFunctionReturn

#undef  TaoValidIntPointer
#define TaoValidIntPointer(a,arg)        PetscValidIntPointer(a,arg)
#undef  TaoValidScalarPointer
#define TaoValidScalarPointer(a,arg)     PetscValidScalarPointer(a,arg)

/* Memory Management Macros */
#undef  TaoMalloc
#define TaoMalloc                    PetscMalloc
#undef  TaoNew
#define TaoNew                       PetscNew
#undef  TaoFree
#define TaoFree                      PetscFree

/* Make a TAO_SOLVER a PetscObject, so we can use PETSc facilities */
#undef  TAOHEADER
#define TAOHEADER(a)                 PETSCHEADER(a)   
#undef  TaoValidHeaderSpecific
#define TaoValidHeaderSpecific(a,b,arg)  PetscValidHeaderSpecific(a,b,arg)
#undef  TaoValidHeader
#define TaoValidHeader(a,arg)            PetscValidHeader(a,arg)

extern int TaoObjectCreate( TAO_SOLVER *, MPI_Comm);

#endif
