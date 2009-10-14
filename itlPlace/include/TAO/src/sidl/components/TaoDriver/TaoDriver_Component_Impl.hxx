// 
// File:          TaoDriver_Component_Impl.hxx
// Symbol:        TaoDriver.Component-v0.0.2
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TaoDriver.Component
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 

#ifndef included_TaoDriver_Component_Impl_hxx
#define included_TaoDriver_Component_Impl_hxx

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
#ifndef included_TaoDriver_Component_IOR_h
#include "TaoDriver_Component_IOR.h"
#endif
#ifndef included_TaoDriver_Component_hxx
#include "TaoDriver_Component.hxx"
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
#ifndef included_gov_cca_ports_GoPort_hxx
#include "gov_cca_ports_GoPort.hxx"
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


// DO-NOT-DELETE splicer.begin(TaoDriver.Component._includes)
// Put additional includes or other arbitrary code here...
// DO-NOT-DELETE splicer.end(TaoDriver.Component._includes)

namespace TaoDriver { 

  /**
   * Symbol "TaoDriver.Component" (version 0.0.2)
   */
  class Component_impl : public virtual ::TaoDriver::Component 
  // DO-NOT-DELETE splicer.begin(TaoDriver.Component._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(TaoDriver.Component._inherits)
  {

  // All data marked protected will be accessable by 
  // descendant Impl classes
  protected:

    bool _wrapped;

    // DO-NOT-DELETE splicer.begin(TaoDriver.Component._implementation)
    gov::cca::Services myServices;
    // DO-NOT-DELETE splicer.end(TaoDriver.Component._implementation)

  public:
    // default constructor, used for data wrapping(required)
    Component_impl();
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Component_impl( struct TaoDriver_Component__object * s ) : StubBase(s,true),
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
     * Execute some encapsulated functionality on the component. 
     * Return 0 if ok, -1 if internal error but component may be 
     * used further, and -2 if error so severe that component cannot
     * be further used safely.
     */
    int32_t
    go_impl() ;

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

} // end namespace TaoDriver

// DO-NOT-DELETE splicer.begin(TaoDriver.Component._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(TaoDriver.Component._misc)

#endif
