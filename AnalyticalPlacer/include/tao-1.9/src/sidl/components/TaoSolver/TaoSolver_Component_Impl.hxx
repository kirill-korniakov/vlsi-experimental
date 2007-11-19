// 
// File:          TaoSolver_Component_Impl.hxx
// Symbol:        TaoSolver.Component-v0.0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TaoSolver.Component
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 

#ifndef included_TaoSolver_Component_Impl_hxx
#define included_TaoSolver_Component_Impl_hxx

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
#ifndef included_TaoSolver_Component_IOR_h
#include "TaoSolver_Component_IOR.h"
#endif
#ifndef included_LinearAlgebra_Factory_hxx
#include "LinearAlgebra_Factory.hxx"
#endif
#ifndef included_OptimizationPorts_SolverPort_hxx
#include "OptimizationPorts_SolverPort.hxx"
#endif
#ifndef included_Optimize_OptimizationModel_hxx
#include "Optimize_OptimizationModel.hxx"
#endif
#ifndef included_Solver_TerminateReason_hxx
#include "Solver_TerminateReason.hxx"
#endif
#ifndef included_TaoSolver_Component_hxx
#include "TaoSolver_Component.hxx"
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


// DO-NOT-DELETE splicer.begin(TaoSolver.Component._includes)
#include "Solver_ProjectState.hxx"
#include "Solver_OptimizationSolver.hxx"
#include "OptimizationPorts_ModelPort.hxx"
#include "OptimizationPorts_LAFactoryPort.hxx"

#include "gov_cca_ports_ParameterPortFactory.hxx"
#include "gov_cca_ports_ParameterPort.hxx"
// DO-NOT-DELETE splicer.end(TaoSolver.Component._includes)

namespace TaoSolver { 

  /**
   * Symbol "TaoSolver.Component" (version 0.0.3)
   */
  class Component_impl : public virtual ::TaoSolver::Component 
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component._inherits)
  //    : public virtual ParameterGetListener
  // DO-NOT-DELETE splicer.end(TaoSolver.Component._inherits)
  {

  // All data marked protected will be accessable by 
  // descendant Impl classes
  protected:

    bool _wrapped;

    // DO-NOT-DELETE splicer.begin(TaoSolver.Component._implementation)
    void setupParameters(gov::cca::ports::ParameterPortFactory&);
    
    gov::cca::Port myPort;
    gov::cca::Services myServices;
    Solver::ProjectState tao_environment;
    Solver::OptimizationSolver tao;
    OptimizationPorts::ModelPort model;
    OptimizationPorts::LAFactoryPort factory;

    gov::cca::ports::ParameterPortFactory ppf;
    gov::cca::ports::ParameterPort pp;
    gov::cca::TypeMap tm;

    // Actual parameter choices
    /*
    StringParameter *method;
    StringParameter *options;
    IntParameter *max_its;
    IntParameter *max_funcs;
    IntParameter *lmvm_lm;
    DoubleParameter *fatol;
    DoubleParameter *frtol;
    DoubleParameter *gatol;
    DoubleParameter *grtol;
    DoubleParameter *gttol;
    DoubleParameter *steptol;
    DoubleParameter *lmvm_initH;
    BoolParameter *taomonitor;
    BoolParameter *taoview;
    */

  public:
    //    bool updateParameterPort(ConfigurableParameterPort *opp);



    // DO-NOT-DELETE splicer.end(TaoSolver.Component._implementation)

  public:
    // default constructor, used for data wrapping(required)
    Component_impl();
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Component_impl( struct TaoSolver_Component__object * s ) : StubBase(s,true),
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

    /**
     * user defined non-static method.
     */
    int32_t
    Initialize_impl() ;
    /**
     * user defined non-static method.
     */
    int32_t
    Solve_impl() ;
    /**
     * user defined non-static method.
     */
    int32_t
    Finalize_impl() ;
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

  };  // end class Component_impl

} // end namespace TaoSolver

// DO-NOT-DELETE splicer.begin(TaoSolver.Component._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(TaoSolver.Component._misc)

#endif
