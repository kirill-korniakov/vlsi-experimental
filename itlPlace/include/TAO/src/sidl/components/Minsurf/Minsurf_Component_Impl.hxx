// 
// File:          Minsurf_Component_Impl.hxx
// Symbol:        Minsurf.Component-v0.0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for Minsurf.Component
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 

#ifndef included_Minsurf_Component_Impl_hxx
#define included_Minsurf_Component_Impl_hxx

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
#ifndef included_Minsurf_Component_IOR_h
#include "Minsurf_Component_IOR.h"
#endif
#ifndef included_Minsurf_Component_hxx
#include "Minsurf_Component.hxx"
#endif
#ifndef included_OptimizationPorts_ModelPort_hxx
#include "OptimizationPorts_ModelPort.hxx"
#endif
#ifndef included_gov_cca_CCAException_hxx
#include "gov_cca_CCAException.hxx"
#endif
#ifndef included_gov_cca_Component_hxx
#include "gov_cca_Component.hxx"
#endif
#ifndef included_gov_cca_Services_hxx
#include "gov_cca_Services.hxx"
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
#ifndef included_sidl_RuntimeException_hxx
#include "sidl_RuntimeException.hxx"
#endif


// DO-NOT-DELETE splicer.begin(Minsurf.Component._includes)
// Put additional includes or other arbitrary code here...
// DO-NOT-DELETE splicer.end(Minsurf.Component._includes)

namespace Minsurf { 

  /**
   * Symbol "Minsurf.Component" (version 0.0.3)
   */
  class Component_impl : public virtual ::Minsurf::Component 
  // DO-NOT-DELETE splicer.begin(Minsurf.Component._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(Minsurf.Component._inherits)
  {

  // All data marked protected will be accessable by 
  // descendant Impl classes
  protected:

    bool _wrapped;

    // DO-NOT-DELETE splicer.begin(Minsurf.Component._implementation)
        gov::cca::TypeMap myTypeMap;
    gov::cca::Services myServices;

    double *bottom, *top, *left, *right;
    int  mx,my; // global dimension in each direction of the array
    int  bmx, bmy; // bounds
    double bheight;
    int n_iters;
    double fvalue;

    int MSA_BoundaryConditions(); // Calculate boundary conditions
    
    // Put additional implementation details here...
    // DO-NOT-DELETE splicer.end(Minsurf.Component._implementation)

  public:
    // default constructor, used for data wrapping(required)
    Component_impl();
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Component_impl( struct Minsurf_Component__object * s ) : StubBase(s,true),
      _wrapped(false) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Component_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

    // true if this object was created by a user newing the impl
    inline bool _isWrapped() {return _wrapped;}

    // static class initializer
    static void _load();

  public:


    /**
     *  
     * Optional --
     * initialize() will be called from the Optimization Solver 
     * before solving in order to set up any data.
     */
    int32_t
    initialize_impl() ;

    /**
     * Optional --
     * finalize() will be called from the Opimization Solver
     * when it through using the model (ie, when solver is deleted)
     */
    int32_t
    finalize_impl() ;

    /**
     * Required --
     * getNumberVariables()  lets the Optimization Solver know how much 
     * space to allocate for the various vectors and matrices.
     */
    int32_t
    getNumberVariables_impl() ;

    /**
     *  
     * Required --
     * evaluateObjectiveFunction(...) should be straightforward.
     * If you wish to only implement evaluateObjectiveAndGradient(), then
     * this function should allocate a dummy gradient vector and call
     * evaluateObjectiveAndGradient() explicitly.
     */
    void
    evaluateObjectiveFunction_impl (
      /* in array<double> */::sidl::array<double> x,
      /* out */double& f
    )
    ;


    /**
     *  
     * Required --
     * evaluateGradient(...) should be straightforward.
     * If you wish to only implement evaluateObjectiveAndGradient(), then
     * this function should call it explicitly. 
     */
    void
    evaluateGradient_impl (
      /* in array<double> */::sidl::array<double> x,
      /* in array<double> */::sidl::array<double> g
    )
    ;


    /**
     * Required --
     * evaluateObjectiveAndGradient(...) should be straightforward.
     * If you wish to implement the function and gradient evaluation routines
     * separately, then this method should do so explicitly.
     */
    void
    evaluateObjectiveAndGradient_impl (
      /* in array<double> */::sidl::array<double> x,
      /* out */double& f,
      /* in array<double> */::sidl::array<double> g
    )
    ;


    /**
     * Optional --
     * evaluateHessian() is only necessary if using second derivative methods for
     * solving the model. 
     */
    void
    evaluateHessian_impl (
      /* in array<double> */::sidl::array<double> x,
      /* in array<double,2> */::sidl::array<double> H
    )
    ;


    /**
     *  
     * Optional -- Used only by LMVM and BLMVM solvers with the right option is set.
     * hessianSolve(...) solve linear system involved exact or approximate Hessian.
     */
    void
    hessianSolve_impl (
      /* in array<double> */::sidl::array<double> v,
      /* in array<double> */::sidl::array<double> w
    )
    ;

    /**
     * user defined non-static method.
     */
    void
    hessianSolve2_impl (
      /* in array<double> */::sidl::array<double> x,
      /* in array<double> */::sidl::array<double> v,
      /* in array<double> */::sidl::array<double> w
    )
    ;


    /**
     * Optional --
     * getVariableBounds() is only necessary if any of the variable bounds are 
     * not at (-inf, inf).    If a solution method
     * is selected that does not use variable bounds, then this function
     * will not be called.
     */
    void
    getVariableBounds_impl (
      /* in array<double> */::sidl::array<double> xl,
      /* in array<double> */::sidl::array<double> xu
    )
    ;


    /**
     *  
     * Optional --
     * initializeVariables() is called from the Optimization Solver to
     * sets the solution vector to an initial value.
     */
    void
    initializeVariables_impl (
      /* in array<double> */::sidl::array<double> x
    )
    ;


    /**
     *  
     * Optional --
     * monitor() is called from TAO at every iteration.
     * If the application programmer wants to perform some kind of
     * visualization or other output throughout the solve, then this
     * method should be implemented.
     */
    void
    monitor_impl() ;

    /**
     * Optional --
     * checkConvergence() is called from TAO at every iteration.
     * If the application wishes to perform a convergence test, then
     * implement this method to set the flag to nonconverged, 0 for 
     * not converged.
     * The flag is an inout variable to avoid the need for implementing the
     * method if its utilization is not required (ie, 0 is passed in, so if
     * nothing is implemented, then 0 is passed back out).
     */
    void
    checkConvergence_impl (
      /* inout */int32_t& flag
    )
    ;


    /**
     *  Starts up a component presence in the calling framework.
     * @param services the component instance's handle on the framework world.
     * Contracts concerning Svc and setServices:
     * 
     * The component interaction with the CCA framework
     * and Ports begins on the call to setServices by the framework.
     * 
     * This function is called exactly once for each instance created
     * by the framework.
     * 
     * The argument Svc will never be nil/null.
     * 
     * Those uses ports which are automatically connected by the framework
     * (so-called service-ports) may be obtained via getPort during
     * setServices.
     */
    void
    setServices_impl (
      /* in */::gov::cca::Services services
    )
    // throws:
    //     ::gov::cca::CCAException
    //     ::sidl::RuntimeException
    ;

  };  // end class Component_impl

} // end namespace Minsurf

// DO-NOT-DELETE splicer.begin(Minsurf.Component._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(Minsurf.Component._misc)

#endif
