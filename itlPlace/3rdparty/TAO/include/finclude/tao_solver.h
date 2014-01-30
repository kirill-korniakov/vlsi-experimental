!
!  $Id$;
!
!  Include file for Fortran use of the TAO package

#define TAO_SOLVER         PetscFortranAddr
#define TAO_APPLICATION    PetscFortranAddr
#define TaoApplication     PetscFortranAddr
#define TaoVecF            PetscFortranAddr
#define TaoMatF            PetscFortranAddr
#define TaoIndexSetF       PetscFortranAddr
#define TaoLinearSolverF   PetscFortranAddr
#define TaoMethod          const character*(80)
#define TaoTerminateReason integer

#include "finclude/tao_general.h"

#if !defined (PETSC_AVOID_DECLARATIONS)
!
!  Convergence flags.
!  Be sure to check that these match the flags in $TAO_DIR/include/tao_solver.h
!
      integer TAO_CONVERGED_ATOL        
      integer TAO_CONVERGED_RTOL
      integer TAO_CONVERGED_TRTOL
      integer TAO_CONVERGED_MINF
      integer TAO_DIVERGED_MAXITS
      integer TAO_DIVERGED_NAN
      integer TAO_DIVERGED_MAXFCN
      integer TAO_DIVERGED_LS_FAILURE
      integer TAO_DIVERGED_TR_REDUCTION
      integer TAO_CONTINUE_ITERATING

      parameter (TAO_CONVERGED_ATOL          =  2)
      parameter (TAO_CONVERGED_RTOL          =  3)
      parameter (TAO_CONVERGED_TRTOL         =  4)
      parameter (TAO_CONVERGED_MINF          =  5)
      parameter (TAO_DIVERGED_MAXITS         = -2)
      parameter (TAO_DIVERGED_NAN            = -4)
      parameter (TAO_DIVERGED_MAXFCN         = -5)
      parameter (TAO_DIVERGED_LS_FAILURE     = -6)
      parameter (TAO_DIVERGED_TR_REDUCTION   = -7)
      parameter (TAO_CONTINUE_ITERATING      =  0)
!
!  Some TAO Fortran functions that the user might pass as arguments
!
      external TAODEFAULTCOMPUTEHESSIAN
      external TAODEFAULTCOMPUTEHESSIANCOLOR
!
! not yet defined
!
!      external TAODEFAULTMONITOR
!      external TAOLGMONITOR
!      external TAOVECVIEWMONITOR
!      external TAOVECVIEWUPDATEMONITOR
!      external TAOCONVERGED_DEFAULT
!      external TAOAPPLY_LINESEARCH

!  End of Fortran include file for the TAO package

#endif
