// 
// File:          TAO_Environment_Impl.hxx
// Symbol:        TAO.Environment-v0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TAO.Environment
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 

#ifndef included_TAO_Environment_Impl_hxx
#define included_TAO_Environment_Impl_hxx

#ifndef included_sidl_cxx_hxx
#include "sidl_cxx.hxx"
#endif
#ifndef included_TAO_Environment_IOR_h
#include "TAO_Environment_IOR.h"
#endif
#ifndef included_Solver_ProjectState_hxx
#include "Solver_ProjectState.hxx"
#endif
#ifndef included_TAO_Environment_hxx
#include "TAO_Environment.hxx"
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


// DO-NOT-DELETE splicer.begin(TAO.Environment._includes)
// Put additional includes or other arbitrary code here...
// DO-NOT-DELETE splicer.end(TAO.Environment._includes)

namespace TAO { 

  /**
   * Symbol "TAO.Environment" (version 0.3)
   */
  class Environment_impl : public virtual ::TAO::Environment 
  // DO-NOT-DELETE splicer.begin(TAO.Environment._inherits)
  // Put additional inheritance here...
  // DO-NOT-DELETE splicer.end(TAO.Environment._inherits)
  {

  // All data marked protected will be accessable by 
  // descendant Impl classes
  protected:

    bool _wrapped;

    // DO-NOT-DELETE splicer.begin(TAO.Environment._implementation)
    
    // DO-NOT-DELETE splicer.end(TAO.Environment._implementation)

  public:
    // default constructor, used for data wrapping(required)
    Environment_impl();
    // sidl constructor (required)
    // Note: alternate Skel constructor doesn't call addref()
    // (fixes bug #275)
    Environment_impl( struct TAO_Environment__object * s ) : StubBase(s,true),
      _wrapped(false) { _ctor(); }

    // user defined construction
    void _ctor();

    // virtual destructor (required)
    virtual ~Environment_impl() { _dtor(); }

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
    Initialize_impl (
      /* in array<string> */::sidl::array< ::std::string> argv
    )
    ;

    /**
     * user defined non-static method.
     */
    int32_t
    InitializeNoArgs_impl() ;
    /**
     * user defined non-static method.
     */
    int32_t
    Finalize_impl() ;
  };  // end class Environment_impl

} // end namespace TAO

// DO-NOT-DELETE splicer.begin(TAO.Environment._misc)
// Put miscellaneous things here...
// DO-NOT-DELETE splicer.end(TAO.Environment._misc)

#endif
