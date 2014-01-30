!
!  $Id$;
!
!  Base include file for Fortran use of the TAO package.
!
#include "petscconf.h"

#include "finclude/tao_def.h"

#if !defined (PETSC_AVOID_DECLARATIONS)
! ------------------------------------------------------------------------
!     Non Common block stuff declared first
!    
!     Flags
!
      integer   TAO_TRUE,TAO_FALSE,TAO_DECIDE
      integer   TAO_DEFAULT_INTEGER,TAO_DETERMINE

      parameter (TAO_TRUE = PETSC_TRUE)
      parameter (TAO_FALSE = PETSC_FALSE)
      parameter (TAO_DECIDE = PETSC_DECIDE)
      parameter (TAO_DETERMINE = PETSC_DETERMINE)
      parameter (TAO_DEFAULT_INTEGER = PETSC_DEFAULT_INTEGER)

! ----------------------------------------------------------------------------
!     BEGIN COMMON-BLOCK VARIABLES

!     Fortran Null
!
      character*(80)     TAO_NULL_CHARACTER
      PetscInt    TAO_NULL_INTEGER
      PetscFortranDouble TAO_NULL_DOUBLE
      PetscScalar        TAO_NULL_SCALAR
!
!     A TAO_NULL_FUNCTION pointer
!
!      external TAO_NULL_FUNCTION
!
!     Common block to store some of the TAO constants,
!     which can be set only at runtime.
!     (A string should be in a different common block.)
!  
      common /taofortran1/ TAO_NULL_CHARACTER
      common /taofortran2/ TAO_NULL_INTEGER
      common /taofortran3/ TAO_NULL_SCALAR
      common /taofortran4/ TAO_NULL_DOUBLE

!     END COMMON-BLOCK VARIABLES
! ----------------------------------------------------------------------------

#endif

