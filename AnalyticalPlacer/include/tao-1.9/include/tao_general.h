/*
   This is the a main TAO include file (for C and C++).  It is included
   in other TAO include files such as "tao_solver.h", so TAO applications and
   solvers almost never have to specifically include it.  It should be
   included when extending the TaoVec, TaoMat, TaoIndexSet, TaoLinearSolver,
   or TaoApplication class.
*/
#if !defined(__TAO_H)
#define __TAO_H


/* ========================================================================== */
/* 
   Current TAO version number and release date
*/
#include "tao_version.h"

/* ========================================================================== */

/* 
   TAO logging/profiling information
*/
/* #include "tao_log.h" */
#include "tao_log.h"
/* ========================================================================== */


/*  
   TAO utilities for basic system-level portability. 
   such as collecting runtime options, printing, registering solvers,
   error handling macros, memory management, strings.
*/
#include "tao_sys.h"
/* Determine this basic functionality at compile time */
/* Use PETSc for this functionality */

#define  TAO_USE_PETSC

#ifdef TAO_NO_PETSC
#undef TAO_USE_PETSC 
#endif

#ifdef TAO_USE_PETSC
#include "tao_pkernal.h"
#endif

/* In a uniprocessor environment, PETSc does not have to be used for this kind of support */
#ifndef TAO_USE_PETSC
#include "src/sys/tao_unigeneral.h"
#endif

#endif
