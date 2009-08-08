// 
// File:          GA_Factory_Impl.hh
// Symbol:        GA.Factory-v0.0.3
// Symbol Type:   class
// Babel Version: 0.8.8
// Description:   Server-side implementation for GA.Factory
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.8.8
// 

#ifndef included_GA_Factory_Impl_hh
#define included_GA_Factory_Impl_hh

#ifndef included_SIDL_cxx_hh
#include "SIDL_cxx.hh"
#endif
#ifndef included_GA_Factory_IOR_h
#include "GA_Factory_IOR.h"
#endif
// 
// Includes for all method dependencies.
// 
#ifndef included_GA_Factory_hh
#include "GA_Factory.hh"
#endif
#ifndef included_SIDL_BaseInterface_hh
#include "SIDL_BaseInterface.hh"
#endif
#ifndef included_SIDL_ClassInfo_hh
#include "SIDL_ClassInfo.hh"
#endif
#ifndef included_gov_cca_CCAException_hh
#include "gov_cca_CCAException.hh"
#endif
#ifndef included_gov_cca_Services_hh
#include "gov_cca_Services.hh"
#endif


// DO-NOT-DELETE splicer.begin(GA.Factory._includes)
// Put additional includes or other arbitrary code here...
// DO-NOT-DELETE splicer.end(GA.Factory._includes)

namespace GA { 

  /**
   * Symbol "GA.Factory" (version 0.0.3)
   */
  class Factory_impl
  // DO-NOT-DELETE splicer.begin(GA.Factory._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(GA.Factory._inherits)
  {

  private:
    // Pointer back to IOR.
    // Use this to dispatch back through IOR vtable.
    Factory self;

    // DO-NOT-DELETE splicer.begin(GA.Factory._implementation)
    gov::cca::Services myServices;
    // DO-NOT-DELETE splicer.end(GA.Factory._implementation)

  private:
    // private default constructor (required)
    Factory_impl() {} 

  public:
    // SIDL constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Factory_impl( struct GA_Factory__object * s ) : self(s,true) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Factory_impl() { _dtor(); }

    // user defined destruction
    void _dtor();

  public:

    /**
     * user defined non-static method.
     */
    int32_t
    initialize() throw () 
    ;
    /**
     * user defined non-static method.
     */
    int32_t
    finalize() throw () 
    ;

    /**
     * createVec returns a pointer to a TaoVec 
     */
    void*
    createVec (
      /*in*/ int32_t n
    )
    throw () 
    ;


    /**
     * createMat returns a pointer to a TaoMat
     */
    void*
    createMat (
      /*in*/ int32_t m,
      /*in*/ int32_t n
    )
    throw () 
    ;


    /**
     * Starts up a component presence in the calling framework.
     * @param Svc the component instance's handle on the framework world.
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
    setServices (
      /*in*/ ::gov::cca::Services services
    )
    throw ( 
      ::gov::cca::CCAException
    );

  };  // end class Factory_impl

} // end namespace GA

// DO-NOT-DELETE splicer.begin(GA.Factory._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(GA.Factory._misc)

#endif
