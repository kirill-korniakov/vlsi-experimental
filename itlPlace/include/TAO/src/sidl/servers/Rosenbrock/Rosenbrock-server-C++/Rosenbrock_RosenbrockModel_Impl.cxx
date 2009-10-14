// 
// File:          Rosenbrock_RosenbrockModel_Impl.cxx
// Symbol:        Rosenbrock.RosenbrockModel-v0.0.2
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for Rosenbrock.RosenbrockModel
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 
#include "Rosenbrock_RosenbrockModel_Impl.hxx"

// 
// Includes for all method dependencies.
// 
#ifndef included_sidl_BaseInterface_hxx
#include "sidl_BaseInterface.hxx"
#endif
#ifndef included_sidl_ClassInfo_hxx
#include "sidl_ClassInfo.hxx"
#endif
#ifndef included_sidl_NotImplementedException_hxx
#include "sidl_NotImplementedException.hxx"
#endif
// DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._includes)

// DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._includes)

// speical constructor, used for data wrapping(required).  Do not put code here unless you really know what you're doing!
Rosenbrock::RosenbrockModel_impl::RosenbrockModel_impl() : 
  StubBase(reinterpret_cast< 
  void*>(::Rosenbrock::RosenbrockModel::_wrapObj(reinterpret_cast< 
  void*>(this))),false) , _wrapped(true){ 
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._ctor2)
  // Insert-Code-Here {Rosenbrock.RosenbrockModel._ctor2} (ctor2)
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._ctor2)
}

// user defined constructor
void Rosenbrock::RosenbrockModel_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._ctor)
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._ctor)
}

// user defined destructor
void Rosenbrock::RosenbrockModel_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._dtor)
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._dtor)
}

// static class initializer
void Rosenbrock::RosenbrockModel_impl::_load() {
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._load)
  // Insert-Code-Here {Rosenbrock.RosenbrockModel._load} (class initialization)
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._load)
}

// user defined static methods: (none)

// user defined non-static methods:
/**
 * Method:  setNumberVariables[]
 */
void
Rosenbrock::RosenbrockModel_impl::setNumberVariables_impl (
  /* in */int32_t n ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.setNumberVariables)
  this->n = n;
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.setNumberVariables)
}

/**
 * Method:  setAlpha[]
 */
void
Rosenbrock::RosenbrockModel_impl::setAlpha_impl (
  /* in */double alpha ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.setAlpha)
  this->alpha = alpha;
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.setAlpha)
}

/**
 *  
 * Optional --
 * initialize() will be called from the Optimization Solver 
 * before solving in order to set up any data.
 */
int32_t
Rosenbrock::RosenbrockModel_impl::initialize_impl () 

{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.initialize)
  this->alpha = 99.0;
  this->n = 2;
  return 0;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.initialize)
}

/**
 * Optional --
 * finalize() will be called from the Opimization Solver
 * when it through using the model (ie, when solver is deleted)
 */
int32_t
Rosenbrock::RosenbrockModel_impl::finalize_impl () 

{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.finalize)
  return 0;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.finalize)
}

/**
 * Required --
 * getNumberVariables()  lets the Optimization Solver know how much 
 * space to allocate for the various vectors and matrices.
 */
int32_t
Rosenbrock::RosenbrockModel_impl::getNumberVariables_impl () 

{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.getNumberVariables)
  return this->n;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.getNumberVariables)
}

/**
 *  
 * Required --
 * evaluateObjectiveFunction(...) should be straightforward.
 * If you wish to only implement evaluateObjectiveAndGradient(), then
 * this function should allocate a dummy gradient vector and call
 * evaluateObjectiveAndGradient() explicitly.
 */
void
Rosenbrock::RosenbrockModel_impl::evaluateObjectiveFunction_impl (
  /* in array<double> */::sidl::array<double> x,
  /* out */double& f ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.evaluateObjectiveFunction)
  int i;
  double *rawx = x._get_ior()->d_firstElement;
  double t1,t2,alpha =this->alpha;

  f = 0;
  for (i=0;i<this->n/2;i++) {
    t1 = rawx[2*i+1] - rawx[2*i] * rawx[2*i]; 
    t2 = 1-rawx[2*i];
    f += alpha*t1*t1 + t2*t2;
  }
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.evaluateObjectiveFunction)
}

/**
 *  
 * Required --
 * evaluateGradient(...) should be straightforward.
 * If you wish to only implement evaluateObjectiveAndGradient(), then
 * this function should call it explicitly. 
 */
void
Rosenbrock::RosenbrockModel_impl::evaluateGradient_impl (
  /* in array<double> */::sidl::array<double> x,
  /* in array<double> */::sidl::array<double> g ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.evaluateGradient)
  int i;
  double *rawx = x._get_ior()->d_firstElement;
  double *rawg = g._get_ior()->d_firstElement;
  double t1,t2,alpha =this->alpha;

  for (i=0;i<this->n/2;i++) {
    t1 = rawx[2*i+1] - rawx[2*i] * rawx[2*i]; 
    t2 = 1-rawx[2*i];
    rawg[2*i] = -4.0*alpha*t1*rawx[2*i] - 2.0*t2;
    rawg[2*i+1] = 2.0*alpha*t1;
  }
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.evaluateGradient)
}

/**
 * Required --
 * evaluateObjectiveAndGradient(...) should be straightforward.
 * If you wish to implement the function and gradient evaluation routines
 * separately, then this method should do so explicitly.
 */
void
Rosenbrock::RosenbrockModel_impl::evaluateObjectiveAndGradient_impl (
  /* in array<double> */::sidl::array<double> x,
  /* out */double& f,
  /* in array<double> */::sidl::array<double> g ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.evaluateObjectiveAndGradient)
  int i;
  double *rawx = x._get_ior()->d_firstElement;
  double *rawg = g._get_ior()->d_firstElement;
  double t1,t2,alpha =this->alpha;

  f=0;
  for (i=0;i<this->n/2;i++) {
    t1 = rawx[2*i+1] - rawx[2*i] * rawx[2*i]; 
    t2 = 1.0-rawx[2*i];
    f += alpha*t1*t1 + t2*t2;
    rawg[2*i] = -4.0*alpha*t1*rawx[2*i] - 2.0*t2;
    rawg[2*i+1] = 2.0*alpha*t1;
  }

  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.evaluateObjectiveAndGradient)
}

/**
 * Optional --
 * evaluateHessian() is only necessary if using second derivative methods for
 * solving the model. 
 */
void
Rosenbrock::RosenbrockModel_impl::evaluateHessian_impl (
  /* in array<double> */::sidl::array<double> x,
  /* in array<double,2> */::sidl::array<double> H ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.evaluateHessian)
  int i;
  double xeven, xodd;
  // This should be done with an H.set(), but used the C API to allow both
  // babel-0.7.4 and earlier versions to work (0.7.4 changed arg. order)
  struct sidl_double__array *ior_H = H._get_ior();

  for (i=0;i<this->n/2;i++) {
    xeven = x.get(2*i);
    xodd = x.get(2*i+1);

    sidlArrayElem2(ior_H,2*i+1,2*i+1) = 2*this->alpha;
    sidlArrayElem2(ior_H,2*i, 2*i) = 2.0 - 4.0 * this->alpha * (xodd - 3.0*xeven*xeven);
    sidlArrayElem2(ior_H,2*i+1,2*i) = sidlArrayElem2(ior_H,2*i,2*i+1) = 
      -4.0 * this->alpha * xeven;

  }
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.evaluateHessian)
}

/**
 *  
 * Optional -- Used only by LMVM and BLMVM solvers with the right option is set.
 * hessianSolve(...) solve linear system involved exact or approximate Hessian.
 */
void
Rosenbrock::RosenbrockModel_impl::hessianSolve_impl (
  /* in array<double> */::sidl::array<double> v,
  /* in array<double> */::sidl::array<double> w ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.hessianSolve)
  // insert implementation here
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.hessianSolve)
}

/**
 * Method:  hessianSolve2[]
 */
void
Rosenbrock::RosenbrockModel_impl::hessianSolve2_impl (
  /* in array<double> */::sidl::array<double> x,
  /* in array<double> */::sidl::array<double> v,
  /* in array<double> */::sidl::array<double> w ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.hessianSolve2)
  // insert implementation here
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.hessianSolve2)
}

/**
 * Optional --
 * getVariableBounds() is only necessary if any of the variable bounds are 
 * not at (-inf, inf).    If a solution method
 * is selected that does not use variable bounds, then this function
 * will not be called.
 */
void
Rosenbrock::RosenbrockModel_impl::getVariableBounds_impl (
  /* in array<double> */::sidl::array<double> xl,
  /* in array<double> */::sidl::array<double> xu ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.getVariableBounds)
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.getVariableBounds)
}

/**
 *  
 * Optional --
 * initializeVariables() is called from the Optimization Solver to
 * sets the solution vector to an initial value.
 */
void
Rosenbrock::RosenbrockModel_impl::initializeVariables_impl (
  /* in array<double> */::sidl::array<double> x ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.initializeVariables)
  int i;
  double *rawx = x._get_ior()->d_firstElement;

  // This should be done with an x.set(), but used the C API to allow both
  // babel-0.7.4 and earlier versions to work (0.7.4 changed arg. order)
  for (i=0;i<this->n;i++)
    rawx[i] = 0.0;
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.initializeVariables)
}

/**
 *  
 * Optional --
 * monitor() is called from TAO at every iteration.
 * If the application programmer wants to perform some kind of
 * visualization or other output throughout the solve, then this
 * method should be implemented.
 */
void
Rosenbrock::RosenbrockModel_impl::monitor_impl () 

{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.monitor)
  return;
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.monitor)
}

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
Rosenbrock::RosenbrockModel_impl::checkConvergence_impl (
  /* inout */int32_t& flag ) 
{
  // DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel.checkConvergence)
  // insert implementation here
  // DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel.checkConvergence)
}


// DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._misc)
// Put miscellaneous code here
// DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._misc)

