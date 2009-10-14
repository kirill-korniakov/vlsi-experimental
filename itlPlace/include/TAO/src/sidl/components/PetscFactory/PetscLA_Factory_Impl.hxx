// 
// File:          PetscLA_Factory_Impl.hxx
// Symbol:        PetscLA.Factory-v0.0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for PetscLA.Factory
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 

#ifndef included_PetscLA_Factory_Impl_hxx
#define included_PetscLA_Factory_Impl_hxx

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
#ifndef included_PetscLA_Factory_IOR_h
#include "PetscLA_Factory_IOR.h"
#endif
#ifndef included_OptimizationPorts_LAFactoryPort_hxx
#include "OptimizationPorts_LAFactoryPort.hxx"
#endif
#ifndef included_PetscLA_Factory_hxx
#include "PetscLA_Factory.hxx"
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


// DO-NOT-DELETE splicer.begin(PetscLA.Factory._includes)
// Put additional includes or other arbitrary code here...
// DO-NOT-DELETE splicer.end(PetscLA.Factory._includes)

namespace PetscLA { 

  /**
   * Symbol "PetscLA.Factory" (version 0.0.3)
   */
  class Factory_impl : public virtual ::PetscLA::Factory 
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(PetscLA.Factory._inherits)
  {

  // All data marked protected will be accessable by 
  // descendant Impl classes
  protected:

    bool _wrapped;

    // DO-NOT-DELETE splicer.begin(PetscLA.Factory._implementation)
    gov::cca::Services myServices;
    // DO-NOT-DELETE splicer.end(PetscLA.Factory._implementation)

  public:
    // default constructor, used for data wrapping(required)
    Factory_impl();
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Factory_impl( struct PetscLA_Factory__object * s ) : StubBase(s,true),
      _wrapped(false) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Factory_impl() { _dtor(); }

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
    initialize_impl() ;
    /**
     * user defined non-static method.
     */
    int32_t
    finalize_impl() ;

    /**
     * createVec returns a pointer to a TaoVec 
     */
    void*
    createVec_impl (
      /* in */int32_t n
    )
    ;


    /**
     * createMat returns a pointer to a TaoMat
     */
    void*
    createMat_impl (
      /* in */int32_t m,
      /* in */int32_t n
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

  };  // end class Factory_impl

} // end namespace PetscLA

// DO-NOT-DELETE splicer.begin(PetscLA.Factory._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(PetscLA.Factory._misc)

#endif
