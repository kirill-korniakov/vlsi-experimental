// 
// File:          TAO_Solver_Impl.hxx
// Symbol:        TAO.Solver-v0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TAO.Solver
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 

#ifndef included_TAO_Solver_Impl_hxx
#define included_TAO_Solver_Impl_hxx

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
#ifndef included_TAO_Solver_IOR_h
#include "TAO_Solver_IOR.h"
#endif
#ifndef included_LinearAlgebra_Factory_hxx
#include "LinearAlgebra_Factory.hxx"
#endif
#ifndef included_Optimize_OptimizationModel_hxx
#include "Optimize_OptimizationModel.hxx"
#endif
#ifndef included_Solver_OptimizationSolver_hxx
#include "Solver_OptimizationSolver.hxx"
#endif
#ifndef included_Solver_TerminateReason_hxx
#include "Solver_TerminateReason.hxx"
#endif
#ifndef included_TAO_Solver_hxx
#include "TAO_Solver.hxx"
#endif
#ifndef included_sidl_BaseClass_hxx
#include "sidl_BaseClass.hxx"
#endif
#ifndef included_sidl_BaseInterface_hxx
#include "sidl_BaseInterface.hxx"
#endif
#ifndef included_sidl_ClassInfo_hxx
#include "sidl_ClassInfo.hxx"
#endif


// DO-NOT-DELETE splicer.begin(TAO.Solver._includes)
#include "tao.h"
#include "LinearAlgebra_Factory.hxx"
// DO-NOT-DELETE splicer.end(TAO.Solver._includes)

namespace TAO { 

  /**
   * Symbol "TAO.Solver" (version 0.3)
   */
  class Solver_impl : public virtual ::TAO::Solver 
  // DO-NOT-DELETE splicer.begin(TAO.Solver._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(TAO.Solver._inherits)
  {

  // All data marked protected will be accessable by 
  // descendant Impl classes
  protected:

    bool _wrapped;

    // DO-NOT-DELETE splicer.begin(TAO.Solver._implementation)
    TAO_SOLVER tao;
    LinearAlgebra::Factory lafactory;
    double diag; // Diagonal element for lmvm init hessian
    // DO-NOT-DELETE splicer.end(TAO.Solver._implementation)

  public:
    // default constructor, used for data wrapping(required)
    Solver_impl();
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Solver_impl( struct TAO_Solver__object * s ) : StubBase(s,true),
      _wrapped(false) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Solver_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

    // true if this object was created by a user newing the impl
    inline bool _isWrapped() {return _wrapped;}

    // static class initializer
    static void _load();

  public:

    /**
     * user defined non-static method.
     */
    int32_t
    Create_impl (
      /* in */const ::std::string& method
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    Destroy_impl() ;
    /**
     * user defined non-static method.
     */
    int32_t
    SetTolerances_impl (
      /* in */double fatol,
      /* in */double frtol,
      /* in */double catol,
      /* in */double crtol
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SolveApplication_impl (
      /* in */::Optimize::OptimizationModel app
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    View_impl() ;
    /**
     * user defined non-static method.
     */
    int32_t
    GetIterationData_impl (
      /* out */int32_t& iterate,
      /* out */double& f,
      /* out */double& gnorm,
      /* out */double& cnorm,
      /* out */double& xdiff,
      /* out */::Solver::TerminateReason& reason
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    GetMethod_impl (
      /* out */::std::string& type
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SetFunctionLowerBound_impl (
      /* in */double fmin
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    GetGradientTolerances_impl (
      /* out */double& gatol,
      /* out */double& grtol,
      /* out */double& gttol
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SetGradientTolerances_impl (
      /* in */double gatol,
      /* in */double grtol,
      /* in */double gttol
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SetMaximumFunctionEvaluations_impl (
      /* in */int32_t nfcn
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SetMaximumIterates_impl (
      /* in */int32_t maxits
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SetMethod_impl (
      /* in */const ::std::string& type
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SetLAFactory_impl (
      /* in */::LinearAlgebra::Factory factory
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    SetFromOptions_impl() ;
    /**
     * user defined non-static method.
     */
    int32_t
    GetSolverHandle_impl (
      /* out */void*& handle
    )
    ;

  };  // end class Solver_impl

} // end namespace TAO

// DO-NOT-DELETE splicer.begin(TAO.Solver._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(TAO.Solver._misc)

#endif
