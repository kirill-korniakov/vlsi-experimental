// 
// File:          TaoDriver_Component_Impl.cxx
// Symbol:        TaoDriver.Component-v0.0.2
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TaoDriver.Component
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 
#include "TaoDriver_Component_Impl.hxx"

// 
// Includes for all method dependencies.
// 
#ifndef included_gov_cca_CCAException_hxx
#include "gov_cca_CCAException.hxx"
#endif
#ifndef included_gov_cca_Services_hxx
#include "gov_cca_Services.hxx"
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
#ifndef included_sidl_NotImplementedException_hxx
#include "sidl_NotImplementedException.hxx"
#endif
// DO-NOT-DELETE splicer.begin(TaoDriver.Component._includes)
#include <iostream>
#include "Optimize_OptimizationModel.hxx"
#include "OptimizationPorts_SolverPort.hxx"
#include "Solver_OptimizationSolver.hxx"
#include "gov_cca_Port.hxx"
#include "sidl.hxx"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

long
cputime ()
{
  struct rusage rus;

  getrusage (RUSAGE_SELF, &rus);
  return rus.ru_utime.tv_sec + rus.ru_utime.tv_usec / 1000000l;
}
// DO-NOT-DELETE splicer.end(TaoDriver.Component._includes)

// speical constructor, used for data wrapping(required).  Do not put code here unless you really know what you're doing!
TaoDriver::Component_impl::Component_impl() : StubBase(reinterpret_cast< 
  void*>(::TaoDriver::Component::_wrapObj(reinterpret_cast< void*>(this))),
  false) , _wrapped(true){ 
  // DO-NOT-DELETE splicer.begin(TaoDriver.Component._ctor2)
  // Insert-Code-Here {TaoDriver.Component._ctor2} (ctor2)
  // DO-NOT-DELETE splicer.end(TaoDriver.Component._ctor2)
}

// user defined constructor
void TaoDriver::Component_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TaoDriver.Component._ctor)

  // DO-NOT-DELETE splicer.end(TaoDriver.Component._ctor)
}

// user defined destructor
void TaoDriver::Component_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TaoDriver.Component._dtor)

  // DO-NOT-DELETE splicer.end(TaoDriver.Component._dtor)
}

// static class initializer
void TaoDriver::Component_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TaoDriver.Component._load)
  // Insert-Code-Here {TaoDriver.Component._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TaoDriver.Component._load)
}

// user defined static methods: (none)

// user defined non-static methods:
/**
 *  
 * Execute some encapsulated functionality on the component. 
 * Return 0 if ok, -1 if internal error but component may be 
 * used further, and -2 if error so severe that component cannot
 * be further used safely.
 */
int32_t
TaoDriver::Component_impl::go_impl () 

{
  // DO-NOT-DELETE splicer.begin(TaoDriver.Component.go)
  gov::cca::Port p;
  OptimizationPorts::SolverPort solver;
  int info;
  int starttime, time;

  std::cout << "TaoDriver component executing 'go' command" << std::endl;
  starttime = cputime();
  
  try {
    p = this->myServices.getPort(std::string("OptimizationSolver"));
    solver = sidl::babel_cast<OptimizationPorts::SolverPort>(p);
    if (solver._is_nil()) {
      std::cerr << "No OptimizationPorts::SolverPort provided!" << std::endl;
      return -1;
    }
      
  } catch (gov::cca::CCAException e) {
    std::cerr << "Could not get OptimizationSolver port:" << e.getNote() <<std::endl;
  }
  

  info = solver.Initialize(); if (info) return -1;
  info = solver.SetFromOptions(); if (info) return -1;

  info = solver.Solve(); if (info) return -1;
  info = solver.View(); if (info) return -1;
  info = solver.Finalize(); if (info) return -1;

  // Release solver
  this->myServices.releasePort(std::string("OptimizationSolver"));

  time = cputime() - starttime;
  std::cout << "Time to run: "<< time << std::endl;
  return 0;
  
  // DO-NOT-DELETE splicer.end(TaoDriver.Component.go)
}

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
TaoDriver::Component_impl::setServices_impl (
  /* in */::gov::cca::Services services ) 
// throws:
//     ::gov::cca::CCAException
//     ::sidl::RuntimeException
{
  // DO-NOT-DELETE splicer.begin(TaoDriver.Component.setServices)
  if (services._is_nil()) {
    return;
  }
  std::cout << "Setting service in driver" << std::endl;
  this->myServices = services;
  this->myServices.registerUsesPort(std::string("OptimizationSolver"),
				    std::string("OptimizationPorts.SolverPort"),
				    0);
  this->myServices.addProvidesPort(*this,
				   std::string("go"),
				   std::string("gov.cca.ports.GoPort"),
				   0);
				   
  
  // DO-NOT-DELETE splicer.end(TaoDriver.Component.setServices)
}


// DO-NOT-DELETE splicer.begin(TaoDriver.Component._misc)
// Put miscellaneous code here
// DO-NOT-DELETE splicer.end(TaoDriver.Component._misc)

