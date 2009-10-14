// 
// File:          PetscLA_Factory_Impl.cxx
// Symbol:        PetscLA.Factory-v0.0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for PetscLA.Factory
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 
#include "PetscLA_Factory_Impl.hxx"

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
// DO-NOT-DELETE splicer.begin(PetscLA.Factory._includes)
#include <iostream>
#include "petsc.h"
#include "petscvec.h"
#include "petscmat.h"
#include "tao.h"

// DO-NOT-DELETE splicer.end(PetscLA.Factory._includes)

// speical constructor, used for data wrapping(required).  Do not put code here unless you really know what you're doing!
PetscLA::Factory_impl::Factory_impl() : StubBase(reinterpret_cast< 
  void*>(::PetscLA::Factory::_wrapObj(reinterpret_cast< void*>(this))),false) ,
  _wrapped(true){ 
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory._ctor2)
  // Insert-Code-Here {PetscLA.Factory._ctor2} (ctor2)
  // DO-NOT-DELETE splicer.end(PetscLA.Factory._ctor2)
}

// user defined constructor
void PetscLA::Factory_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory._ctor)
  // add construction details here
  // DO-NOT-DELETE splicer.end(PetscLA.Factory._ctor)
}

// user defined destructor
void PetscLA::Factory_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory._dtor)
  // add destruction details here
  // DO-NOT-DELETE splicer.end(PetscLA.Factory._dtor)
}

// static class initializer
void PetscLA::Factory_impl::_load() {
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory._load)
  // Insert-Code-Here {PetscLA.Factory._load} (class initialization)
  // DO-NOT-DELETE splicer.end(PetscLA.Factory._load)
}

// user defined static methods: (none)

// user defined non-static methods:
/**
 * Method:  initialize[]
 */
int32_t
PetscLA::Factory_impl::initialize_impl () 

{
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory.initialize)
  return 0;
  // DO-NOT-DELETE splicer.end(PetscLA.Factory.initialize)
}

/**
 * Method:  finalize[]
 */
int32_t
PetscLA::Factory_impl::finalize_impl () 

{
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory.finalize)
  return 0;
  // DO-NOT-DELETE splicer.end(PetscLA.Factory.finalize)
}

/**
 * createVec returns a pointer to a TaoVec 
 */
void*
PetscLA::Factory_impl::createVec_impl (
  /* in */int32_t n ) 
{
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory.createVec)
  Vec v;
  TaoVecPetsc *tvp;
  int info;
  info = VecCreateSeq(PETSC_COMM_SELF, n, &v);
  if (info) std::cerr << "Unable to create Petsc Vector" << std::endl;
  info = TaoWrapPetscVec(v,&tvp); 
  return (void*)tvp;
  // DO-NOT-DELETE splicer.end(PetscLA.Factory.createVec)
}

/**
 * createMat returns a pointer to a TaoMat
 */
void*
PetscLA::Factory_impl::createMat_impl (
  /* in */int32_t m,
  /* in */int32_t n ) 
{
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory.createMat)
  Mat mat;
  TaoMatPetsc *tmp;
  int info;
  info = MatCreateSeqDense(PETSC_COMM_SELF, m, n, PETSC_NULL, &mat);
  if (info) std::cerr << "Unable to create Petsc Matrix" << std::endl;
  info = TaoWrapPetscMat(mat,&tmp);
  return (void*)tmp;
  // DO-NOT-DELETE splicer.end(PetscLA.Factory.createMat)
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
PetscLA::Factory_impl::setServices_impl (
  /* in */::gov::cca::Services services ) 
// throws:
//     ::gov::cca::CCAException
//     ::sidl::RuntimeException
{
  // DO-NOT-DELETE splicer.begin(PetscLA.Factory.setServices)
  if (services._is_nil()) {
    return;
  }
  gov::cca::Port port = sidl::babel_cast<gov::cca::Port>(*this);
  if (port._is_nil()) {
    std::cerr << "unable to cast self as port (OptimizationPorts::Factory_impl) in PetscLA::Factory_impl::setServices" << std::endl;
  }

  this->myServices = services;
  try {
    this->myServices.addProvidesPort(port,std::string("LAFactory"), std::string("OptimizationPorts.LAFactoryPort"),0);
  }
  catch (gov::cca::CCAException e) {
    std::cerr << "Error adding providesPort in LAFactory: " << e.getNote() << std::endl;
  }
  // DO-NOT-DELETE splicer.end(PetscLA.Factory.setServices)
}


// DO-NOT-DELETE splicer.begin(PetscLA.Factory._misc)
// Put miscellaneous code here
// DO-NOT-DELETE splicer.end(PetscLA.Factory._misc)

