/*
  This file defines basic types in TAO needed for the 
  Vector and Matrix Objects 
*/

#if !defined(__TAO_TYPES_H)
#define __TAO_TYPES_H

typedef enum { TAO_FALSE,TAO_TRUE } TaoTruth;

#ifdef TAO_USE_PETSC

#include "petsc.h"
typedef PetscScalar  TaoScalar;

#else

typedef double TaoScalar;

#endif

#endif
