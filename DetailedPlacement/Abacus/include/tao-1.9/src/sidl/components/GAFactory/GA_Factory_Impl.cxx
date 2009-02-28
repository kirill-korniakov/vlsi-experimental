// 
// File:          GA_Factory_Impl.cc
// Symbol:        GA.Factory-v0.0.3
// Symbol Type:   class
// Babel Version: 0.8.8
// Description:   Server-side implementation for GA.Factory
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// babel-version = 0.8.8
// 
#include "GA_Factory_Impl.hh"

// DO-NOT-DELETE splicer.begin(GA.Factory._includes)
#include <iostream>
#include "tao.h"
#include "ga.h"
#include "taovec_ga.h"
// DO-NOT-DELETE splicer.end(GA.Factory._includes)

// user defined constructor
void GA::Factory_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(GA.Factory._ctor)
  // add construction details here
  // DO-NOT-DELETE splicer.end(GA.Factory._ctor)
}

// user defined destructor
void GA::Factory_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(GA.Factory._dtor)
  // add destruction details here
  // DO-NOT-DELETE splicer.end(GA.Factory._dtor)
}

// user defined static methods: (none)

// user defined non-static methods:
/**
 * Method:  initialize[]
 */
int32_t
GA::Factory_impl::initialize () 
throw () 

{
  // DO-NOT-DELETE splicer.begin(GA.Factory.initialize)
  GA_Initialize();
  int nproc = GA_Nnodes();
  int myheap = 20000/nproc;
  int mystack = 20000/nproc;
  if (!MA_init (MT_F_DBL, mystack, myheap))
    std::cerr << "MA_init failed" << std::endl;
  // DO-NOT-DELETE splicer.end(GA.Factory.initialize)
}

/**
 * Method:  finalize[]
 */
int32_t
GA::Factory_impl::finalize () 
throw () 

{
  // DO-NOT-DELETE splicer.begin(GA.Factory.finalize)
  GA_Terminate();
  // DO-NOT-DELETE splicer.end(GA.Factory.finalize)
}

/**
 * createVec returns a pointer to a TaoVec 
 */
void*
GA::Factory_impl::createVec (
  /*in*/ int32_t n ) 
throw () 
{
  // DO-NOT-DELETE splicer.begin(GA.Factory.createVec)
  GAVec ga_x;
  TaoVecGa *tvg_x;
  int dims[2];
  dims[0] = n;
  dims[1] = 0;
  ga_x = NGA_Create (C_DBL, 1, dims, "GA_X", NULL);
  TaoWrapGaVec(ga_x, &tvg_x);
  std::cout << "Creating Global Array" << std::endl;
  return (void*)tvg_x;
  // DO-NOT-DELETE splicer.end(GA.Factory.createVec)
}

/**
 * createMat returns a pointer to a TaoMat
 */
void*
GA::Factory_impl::createMat (
  /*in*/ int32_t m,
  /*in*/ int32_t n ) 
throw () 
{
  // DO-NOT-DELETE splicer.begin(GA.Factory.createMat)
  return (void*)0;
  // DO-NOT-DELETE splicer.end(GA.Factory.createMat)
}

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
GA::Factory_impl::setServices (
  /*in*/ ::gov::cca::Services services ) 
throw ( 
  ::gov::cca::CCAException
){
  // DO-NOT-DELETE splicer.begin(GA.Factory.setServices)

  int err = 0;
  gov::cca::Port port = sidl::babel_cast<gov::cca::Port>(*this);
  if (port._is_nil()) {
    std::cerr << "unable to cast self as port (GALA::Factory_impl) in GALA::Factory_impl::setServices" << std::endl;
  }

  this->myServices = services;
  try {
    this->myServices.addProvidesPort(port,std::string("LAFactory"), std::string("GALA.Factory"),0);
  }
  catch (gov::cca::CCAException e) {
    std::cerr << "Error adding providesPort in LAFactory: " << e.getNote() << std::endl;
  }

  // DO-NOT-DELETE splicer.end(GA.Factory.setServices)
}


// DO-NOT-DELETE splicer.begin(GA.Factory._misc)
// Put miscellaneous code here
// DO-NOT-DELETE splicer.end(GA.Factory._misc)

