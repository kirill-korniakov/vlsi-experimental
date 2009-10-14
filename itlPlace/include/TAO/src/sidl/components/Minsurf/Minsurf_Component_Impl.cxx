// 
// File:          Minsurf_Component_Impl.cxx
// Symbol:        Minsurf.Component-v0.0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for Minsurf.Component
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 
#include "Minsurf_Component_Impl.hxx"

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
// DO-NOT-DELETE splicer.begin(Minsurf.Component._includes)
#include <iostream>
#include "sidl.hxx"
#include "sidlArray.h"
#include "Optimize_OptimizationModel.hxx"
#include "gov_cca_TypeMap.hxx"
#include "gov_cca_Services.hxx"
// DO-NOT-DELETE splicer.end(Minsurf.Component._includes)

// speical constructor, used for data wrapping(required).  Do not put code here unless you really know what you're doing!
Minsurf::Component_impl::Component_impl() : StubBase(reinterpret_cast< 
  void*>(::Minsurf::Component::_wrapObj(reinterpret_cast< void*>(this))),
  false) , _wrapped(true){ 
  // DO-NOT-DELETE splicer.begin(Minsurf.Component._ctor2)
  // Insert-Code-Here {Minsurf.Component._ctor2} (ctor2)
  // DO-NOT-DELETE splicer.end(Minsurf.Component._ctor2)
}

// user defined constructor
void Minsurf::Component_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(Minsurf.Component._ctor)
  this->mx = this->my = 10;
  this->n_iters=0;
  this->fvalue = 0.0;

  return;
  // DO-NOT-DELETE splicer.end(Minsurf.Component._ctor)
}

// user defined destructor
void Minsurf::Component_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(Minsurf.Component._dtor)
  return;
  // DO-NOT-DELETE splicer.end(Minsurf.Component._dtor)
}

// static class initializer
void Minsurf::Component_impl::_load() {
  // DO-NOT-DELETE splicer.begin(Minsurf.Component._load)
  // Insert-Code-Here {Minsurf.Component._load} (class initialization)
  // DO-NOT-DELETE splicer.end(Minsurf.Component._load)
}

// user defined static methods: (none)

// user defined non-static methods:
/**
 *  
 * Optional --
 * initialize() will be called from the Optimization Solver 
 * before solving in order to set up any data.
 */
int32_t
Minsurf::Component_impl::initialize_impl () 

{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.initialize)
  int info;

  info = this->MSA_BoundaryConditions(); 
  return info;
  // DO-NOT-DELETE splicer.end(Minsurf.Component.initialize)
}

/**
 * Optional --
 * finalize() will be called from the Opimization Solver
 * when it through using the model (ie, when solver is deleted)
 */
int32_t
Minsurf::Component_impl::finalize_impl () 

{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.finalize)
  free(this->bottom);
  free(this->top);
  free(this->left);
  free(this->right);

  return 0;
  // DO-NOT-DELETE splicer.end(Minsurf.Component.finalize)
}

/**
 * Required --
 * getNumberVariables()  lets the Optimization Solver know how much 
 * space to allocate for the various vectors and matrices.
 */
int32_t
Minsurf::Component_impl::getNumberVariables_impl () 

{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.getNumberVariables)
  return this->mx * this->my;
  // DO-NOT-DELETE splicer.end(Minsurf.Component.getNumberVariables)
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
Minsurf::Component_impl::evaluateObjectiveFunction_impl (
  /* in array<double> */::sidl::array<double> x,
  /* out */double& f ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.evaluateObjectiveFunction)
  sidl::array<double> dummy = 
    sidl::array<double>::create1d(this->getNumberVariables());
  this->evaluateObjectiveAndGradient(x,f,dummy);
  
  return;
  // DO-NOT-DELETE splicer.end(Minsurf.Component.evaluateObjectiveFunction)
}

/**
 *  
 * Required --
 * evaluateGradient(...) should be straightforward.
 * If you wish to only implement evaluateObjectiveAndGradient(), then
 * this function should call it explicitly. 
 */
void
Minsurf::Component_impl::evaluateGradient_impl (
  /* in array<double> */::sidl::array<double> x,
  /* in array<double> */::sidl::array<double> g ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.evaluateGradient)
  double f;
  this->evaluateObjectiveAndGradient(x,f,g);
  return;
  // DO-NOT-DELETE splicer.end(Minsurf.Component.evaluateGradient)
}

/**
 * Required --
 * evaluateObjectiveAndGradient(...) should be straightforward.
 * If you wish to implement the function and gradient evaluation routines
 * separately, then this method should do so explicitly.
 */
void
Minsurf::Component_impl::evaluateObjectiveAndGradient_impl (
  /* in array<double> */::sidl::array<double> x,
  /* out */double& f,
  /* in array<double> */::sidl::array<double> g ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.evaluateObjectiveAndGradient)
  int nn;
  int i,j,row;
  double hx=1.0/(this->mx+1),hy=1.0/(this->my+1);
  double hydhx=hy/hx, hxdhy=hx/hy, area=0.5*hx*hy;
  double rhx=this->mx+1, rhy=this->my+1;
  double f1,f2,f3,f4,f5,f6,d1,d2,d3,d4,d5,d6,d7,d8,xc,xl,xr,xt,xb,xlt,xrb;
  double df1dxc,df2dxc,df3dxc,df4dxc,df5dxc,df6dxc;
  sidl_double__array *xior = x._get_ior();
  sidl_double__array *gior = g._get_ior();

  nn = this->getNumberVariables();
  

  f = 0;

  // Initialize vector to zero
  for (i=0;i<nn;i++)
    sidlArrayElem1(gior,i) = 0.0;

  for (j=0;j<this->my; j++) {
    for (i=0; i<this->mx; i++) {
      row = j*this->mx + i;
      xlt = xrb = xl = xb = xt = xc = sidlArrayElem1(xior,row);

      if (i==0) { // left side
        xl = this->left[j+1];
        xlt = this->left[j+2];
      } else {
        xl = sidlArrayElem1(xior,row-1);
      }

      if (j==0) { // bottom side
        xb = this->bottom[i+1];
        xrb = this->bottom[i+2];
      } else {
        xb = sidlArrayElem1(xior,row-this->mx);
      }

      if (i+1 == this->mx) { // right side
        xr = this->right[j+1];
        xrb = this->right[j];
      } else {
        xr = sidlArrayElem1(xior,row+1);
      }

      if (j+1 == this->my) { // top side
        xt = this->top[i+1];
        xlt = this->top[i];
      } else {
        xt = sidlArrayElem1(xior, row + this->mx);
      }

      if (i > 0  &&  j+1 < this->my) {
        xlt = sidlArrayElem1(xior, row-1+this->mx);
      }

      if (j > 0  &&  i+1 < this->mx) {
        xrb = sidlArrayElem1(xior, row + 1 - this->mx);
      }

      
        
      d1 = (xc-xl);
      d2 = (xc-xr);
      d3 = (xc-xt);
      d4 = (xc-xb);
      d5 = (xr-xrb);
      d6 = (xrb-xb);
      d7 = (xlt-xl);
      d8 = (xt-xlt);
      
      df1dxc = d1*hydhx;
      df2dxc = ( d1*hydhx + d4*hxdhy );
      df3dxc = d3*hxdhy;
      df4dxc = ( d2*hydhx + d3*hxdhy );
      df5dxc = d2*hydhx;
      df6dxc = d4*hxdhy;

      d1 *= rhx;
      d2 *= rhx;
      d3 *= rhy;
      d4 *= rhy;
      d5 *= rhy;
      d6 *= rhx;
      d7 *= rhy;
      d8 *= rhx;

      f1 = sqrt( 1.0 + d1*d1 + d7*d7);
      f2 = sqrt( 1.0 + d1*d1 + d4*d4);
      f3 = sqrt( 1.0 + d3*d3 + d8*d8);
      f4 = sqrt( 1.0 + d3*d3 + d2*d2);
      f5 = sqrt( 1.0 + d2*d2 + d5*d5);
      f6 = sqrt( 1.0 + d4*d4 + d6*d6);
      
      f2 = sqrt( 1.0 + d1*d1 + d4*d4);
      f4 = sqrt( 1.0 + d3*d3 + d2*d2);

      f += (f2 + f4);

      df1dxc /= f1;
      df2dxc /= f2;
      df3dxc /= f3;
      df4dxc /= f4;
      df5dxc /= f5;
      df6dxc /= f6;

      sidlArrayElem1(gior,row) = 
        (df1dxc+df2dxc+df3dxc+df4dxc+df5dxc+df6dxc ) * 0.5;

    }
  }

  // Compute triangular areas along the border of the domain
  // (If data is distributed, then need to check if on the border)
  
  // left side
  for (j=0;j<this->my; j++) {
    d3 = (this->left[j+1] - this->left[j+2])*rhy;
    d2 = (this->left[j+1] - sidlArrayElem1(xior,j*this->mx) )*rhx;
    f += sqrt(1.0 + d3*d3 + d2*d2);
  }
  
  // bottom side
  for (i=0;i<this->my;i++) {
    d2 = (this->bottom[i+1] - this->bottom[i+2])*rhx;
    d3 = (this->bottom[i+1] - sidlArrayElem1(xior,i) )*rhy;
    f += sqrt(1.0 + d3*d3 + d2*d2);
  }

  // right side
  for (j=0; j<this->my; j++) {
    d1 = (sidlArrayElem1(xior,(j+1)*this->mx -1) - this->right[j+1]) *rhx;
    d4 = (this->right[j] - this->right[j+1]) * rhy;
    f += sqrt(1.0 + d1*d1 + d4*d4);
  }

  // top side
  for (i=0; i<this->my; i++) {
    d1 = (sidlArrayElem1(xior,(this->my-1)*this->mx + i) - this->top[i+1]) *rhy;
    d4 = (this->top[i+1] - this->top[i]) * rhx;
    f += sqrt(1.0 + d1*d1 + d4*d4);
  }

  // bottomleft corner
  d1 = (this->left[0] - this->left[1])/hy;
  d2 = (this->bottom[0] - this->bottom[1])*rhx;
  f += sqrt(1.0 + d1*d1 + d2*d2);

  // topleft corner
  d1 = (this->right[this->my + 1] - this->right[this->my])*rhy;
  d2 = (this->top[this->mx + 1] - this->top[this->mx])*rhx;
  f += sqrt(1.0 + d1*d1 + d2*d2);

  f *= area;

  this->fvalue = f;

  return;
  // DO-NOT-DELETE splicer.end(Minsurf.Component.evaluateObjectiveAndGradient)
}

/**
 * Optional --
 * evaluateHessian() is only necessary if using second derivative methods for
 * solving the model. 
 */
void
Minsurf::Component_impl::evaluateHessian_impl (
  /* in array<double> */::sidl::array<double> x,
  /* in array<double,2> */::sidl::array<double> H ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.evaluateHessian)
  int i,j,row;
  double hx=1.0/(mx+1), hy=1.0/(my+1), hydhx=hy/hx, hxdhy=hx/hy;
  double rhx=mx+1, rhy=my+1;
  double f1,f2,f3,f4,f5,f6,d1,d2,d3,d4,d5,d6,d7,d8,xc,xl,xr,xt,xb,xlt,xrb;
  double hl,hr,ht,hb,hc,htl,hbr;
  sidl_double__array *xior = x._get_ior();
  sidl_double__array *Hior = H._get_ior();

  // zero hessian
  for (i=0;i<mx;i++) {
    for (j=0;j<my;j++) { 
      sidlArrayElem2(Hior,i,j) = 0;
    }
  }



  
  for (i=0; i<mx; i++) {
    for (j=0; j<my; j++) {
      row = j*mx + i;
      xc = xlt = xrb = xl = xr = xb = xt = sidlArrayElem1(xior, row);

      // Left side 
      if (i==0) {
	xl = this->left[j+1];
	xlt = this->left[j+2];
      }
      else {
	xl = sidlArrayElem1(xior, row-1);
      }

      // bottom
      if (j==0) {
	xb = this->bottom[i+1];
	xrb = this->bottom[i+2];
      }
      else {
	xb = sidlArrayElem1(xior, row-mx);
      }

      // right
      if (i+1 == mx) {
	xr = this->right[j+1];
	xrb = this->right[j];
      }
      else {
	xr = sidlArrayElem1(xior, row+1);
      }

      //top
      if (j+1 == my) {
	xt = this->top[i+1];
	xlt = this->top[i];
      }
      else {
	xt = sidlArrayElem1(xior, row+mx);
      }
      
      // corners
      if (i>0 && j+1<my) {
	xlt = sidlArrayElem1(xior, row-1+mx);
      }
      if (j>0 && i+1 <mx ) {
	xrb = sidlArrayElem1(xior, row+1-mx);
      }

      d1 = (xc-xl)*rhx;
      d2 = (xc-xr)*rhx;
      d3 = (xc-xt)*rhy;
      d4 = (xc-xb)*rhy;
      d5 = (xrb-xr)*rhy;
      d6 = (xrb-xb)*rhx;
      d7 = (xlt-xl)*rhy;
      d8 = (xlt-xt)*rhx;
      
      f1 = sqrt( 1.0 + d1*d1 + d7*d7);
      f2 = sqrt( 1.0 + d1*d1 + d4*d4);
      f3 = sqrt( 1.0 + d3*d3 + d8*d8);
      f4 = sqrt( 1.0 + d3*d3 + d2*d2);
      f5 = sqrt( 1.0 + d2*d2 + d5*d5);
      f6 = sqrt( 1.0 + d4*d4 + d6*d6);


      hl = (-hydhx*(1.0+d7*d7)+d1*d7)/(f1*f1*f1)+
	(-hydhx*(1.0+d4*d4)+d1*d4)/(f2*f2*f2);
      hr = (-hydhx*(1.0+d5*d5)+d2*d5)/(f5*f5*f5)+
	(-hydhx*(1.0+d3*d3)+d2*d3)/(f4*f4*f4);
      ht = (-hxdhy*(1.0+d8*d8)+d3*d8)/(f3*f3*f3)+
	(-hxdhy*(1.0+d2*d2)+d2*d3)/(f4*f4*f4);
      hb = (-hxdhy*(1.0+d6*d6)+d4*d6)/(f6*f6*f6)+
	(-hxdhy*(1.0+d1*d1)+d1*d4)/(f2*f2*f2);

      hbr = -d2*d5/(f5*f5*f5) - d4*d6/(f6*f6*f6);
      htl = -d1*d7/(f1*f1*f1) - d3*d8/(f3*f3*f3);

      hc = hydhx*(1.0+d7*d7)/(f1*f1*f1) + hxdhy*(1.0+d8*d8)/(f3*f3*f3) +
	hydhx*(1.0+d5*d5)/(f5*f5*f5) + hxdhy*(1.0+d6*d6)/(f6*f6*f6) +
	(hxdhy*(1.0+d1*d1)+hydhx*(1.0+d4*d4)-2*d1*d4)/(f2*f2*f2) +
	(hxdhy*(1.0+d2*d2)+hydhx*(1.0+d3*d3)-2*d2*d3)/(f4*f4*f4);

      hl*=0.5; hr*=0.5; ht*=0.5; hb*=0.5; hbr*=0.5; htl*=0.5;  hc*=0.5; 


      // Set the matrix values

      //bottom
      if (j>0){ 
	sidlArrayElem2(Hior,row,row-mx) = hb;
      }
      
      // bottom right
      if (j>0 && i < mx -1){
	sidlArrayElem2(Hior,row,row-mx+1) = hbr;
      }
      
      // left
      if (i>0){
	sidlArrayElem2(Hior,row,row-1)= hl;
      }

      // center
      sidlArrayElem2(Hior,row,row) = hc;
      
      // right
      if (i < mx-1 ){
	sidlArrayElem2(Hior,row,row+1) = hr;
      }
      
      // top left
      if (i>0 && j < my-1 ){
	sidlArrayElem2(Hior,row,row+mx-1) = htl;
      }

      // top
      if (j < my-1 ){
	sidlArrayElem2(Hior,row,row+mx) = ht;
      }
    }
  }      


  // DO-NOT-DELETE splicer.end(Minsurf.Component.evaluateHessian)
}

/**
 *  
 * Optional -- Used only by LMVM and BLMVM solvers with the right option is set.
 * hessianSolve(...) solve linear system involved exact or approximate Hessian.
 */
void
Minsurf::Component_impl::hessianSolve_impl (
  /* in array<double> */::sidl::array<double> v,
  /* in array<double> */::sidl::array<double> w ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.hessianSolve)
  // insert implementation here
  int i,nn;
  sidl_double__array *vior = v._get_ior();
  sidl_double__array *wior = w._get_ior();

  nn = this->getNumberVariables();
  for (i=0;i<nn;i++){
    sidlArrayElem1(wior,i) = sidlArrayElem1(vior,i);
  }
  // DO-NOT-DELETE splicer.end(Minsurf.Component.hessianSolve)
}

/**
 * Method:  hessianSolve2[]
 */
void
Minsurf::Component_impl::hessianSolve2_impl (
  /* in array<double> */::sidl::array<double> x,
  /* in array<double> */::sidl::array<double> v,
  /* in array<double> */::sidl::array<double> w ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.hessianSolve2)

  this->hessianSolve(v,w);
  /*
  int i,nn;
  sidl_double__array *xior = x._get_ior();
  nn = this->getNumberVariables();

  printf("Approxmate Hessian2 x\n");
  sidl_double__array *wior = w._get_ior();

  for (i=0;i<nn;i++){
    printf(" %4.4e ",sidlArrayElem1(wior,i));
  }
  printf("\n");
  */
  // DO-NOT-DELETE splicer.end(Minsurf.Component.hessianSolve2)
}

/**
 * Optional --
 * getVariableBounds() is only necessary if any of the variable bounds are 
 * not at (-inf, inf).    If a solution method
 * is selected that does not use variable bounds, then this function
 * will not be called.
 */
void
Minsurf::Component_impl::getVariableBounds_impl (
  /* in array<double> */::sidl::array<double> xl,
  /* in array<double> */::sidl::array<double> xu ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.getVariableBounds)
  int i,j,row,nn;
  int t1,t2,t3;
  int cylinder;
  sidl_double__array *xlior = xl._get_ior();
  sidl_double__array *xuior = xu._get_ior();

  this->bmx = this->mx/2;
  this->bmy=this->my/2;
  this->bheight=0.3;

  // Here is where we would check for command line parameters
  nn = this->getNumberVariables();

  for (i=0;i<nn;i++) {
    sidlArrayElem1(xlior,i) = -2.0; 
    sidlArrayElem1(xuior,i) =  2.0; 
  }

  // Compute the optional lower box
  if (0 && cylinder) { // not implemented until can read command line
    for (i=0;i<this->mx;i++) {
      for (j=0; j<this->my; j++) {
        row = j*this->mx + this->my;
        t1 = (2*i - this->mx)*this->bmy;
        t2 = (2*j - this->my)*this->bmy;
        t3 = this->bmx*this->bmx * this->bmy*this->bmy;
        if (t1*t1 + t2*t2 <= t3) {
          sidlArrayElem1(xlior,row) = this->bheight;
        }

      }
    }
  }
  else {
    for (i=0;i<this->mx; i++) {
      for (j=0;j<this->my;j++) {
        row = j*this->mx + i;
        if (i >= (this->mx - this->bmx)/2 && 
            i < this->mx - (this->mx-this->bmx)/2 &&
            j >= (this->my - this->bmy)/2 &&
            j < this->my - (this->my - this->bmy)/2 ) {
          sidlArrayElem1(xlior,row) = this->bheight;
        }
      }
    }
  }
  
  return;
  // DO-NOT-DELETE splicer.end(Minsurf.Component.getVariableBounds)
}

/**
 *  
 * Optional --
 * initializeVariables() is called from the Optimization Solver to
 * sets the solution vector to an initial value.
 */
void
Minsurf::Component_impl::initializeVariables_impl (
  /* in array<double> */::sidl::array<double> x ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.initializeVariables)
  int i,j;
  sidl_double__array *xior = x._get_ior();
  int row;
  

  // Take an average of the boundary conditions

  for (j=0;j<this->my;j++) {
    for (i=0;i<this->mx; i++) {
      row = j*this->mx + i;
      sidlArrayElem1(xior,row) = ( (j+1)*this->bottom[i+1]/this->my 
                     + (this->my - j + 1)*this->top[i+1]/(this->my + 2) 
                     + (i+1)*this->left[j+1]/this->mx
                     + (this->mx-i+1)*this->right[j+1]/(this->mx+2)   ) / 2.0;
    }
  }
  /*
  int nn;
  nn = this->getNumberVariables();
  printf("Approxmate Hessian x\n");
  for (i=0;i<nn;i++){
    printf(" %4.4e ",sidlArrayElem1(xior,i));
  }
  printf("\n");
  */  
  // DO-NOT-DELETE splicer.end(Minsurf.Component.initializeVariables)
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
Minsurf::Component_impl::monitor_impl () 

{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.monitor)

  // DO-NOT-DELETE splicer.end(Minsurf.Component.monitor)
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
Minsurf::Component_impl::checkConvergence_impl (
  /* inout */int32_t& flag ) 
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.checkConvergence)
  // insert implementation here
  // DO-NOT-DELETE splicer.end(Minsurf.Component.checkConvergence)
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
Minsurf::Component_impl::setServices_impl (
  /* in */::gov::cca::Services services ) 
// throws:
//     ::gov::cca::CCAException
//     ::sidl::RuntimeException
{
  // DO-NOT-DELETE splicer.begin(Minsurf.Component.setServices)
  if (services._is_nil()) {
    return;
  }
  gov::cca::Port port=sidl::babel_cast<gov::cca::Port>(*this);
  if (port._is_nil()) {
    std::cout << "unable to cast self as port (Minsurf::Component_impl) in Minsurf::Component_impl::setServices" << std::endl;
  }
    
  this->myServices = services;
  try {
    this->myServices.addProvidesPort(port,std::string("OptimizationModel"),std::string("OptimizationPorts.ModelPort"),0);
  }
  catch (gov::cca::CCAException e) {
    std::cout << "Error adding providesPort in Minsurf: " << e.getNote() << std::endl;
  }


  // DO-NOT-DELETE splicer.end(Minsurf.Component.setServices)
}


// DO-NOT-DELETE splicer.begin(Minsurf.Component._misc)
int
Minsurf::Component_impl::MSA_BoundaryConditions()
{
  int        i,j,k,limit=0,maxits=5;
  int        xs,ys,xm,ym,gxs,gys,gxm,gym;
  int        bsize=0, lsize=0, tsize=0, rsize=0;
  double     one=1.0, two=2.0, three=3.0, tol=1e-10;
  double     fnorm,det,hx,hy,xt=0,yt=0;
  double     u1,u2,nf1,nf2,njac11,njac12,njac21,njac22;
  double     b=-0.5, t=0.5, l=-0.5, r=0.5;
  double     *boundary;

  /* Get local mesh boundaries */
  bsize = tsize = this->mx+2;
  lsize = rsize = this->my+2;

  /* Since not using DA (only one processor) */
  xs = ys = gxs = gys = 0;
  xm = gxm =  this->mx;
  ym = gym = this->my;
  

  this->bottom = (double*)malloc(bsize*sizeof(double)); 
  this->top = (double*)malloc(tsize*sizeof(double)); 
  this->left = (double*)malloc(lsize*sizeof(double)); 
  this->right = (double*)malloc(rsize*sizeof(double)); 


  hx= (r-l)/(this->mx+1); hy=(t-b)/(this->my+1);

  for (j=0; j<4; j++){
    if (j==0){
      yt=b;
      xt=l+hx*xs;
      limit=bsize;
      boundary=this->bottom;
    } else if (j==1){
      yt=t;
      xt=l+hx*xs;
      limit=tsize;
      boundary=this->top;
    } else if (j==2){
      yt=b+hy*ys;
      xt=l;
      limit=lsize;
      boundary=this->left;
    } else /* j==3 */ {
      yt=b+hy*ys;
      xt=r;
      limit=rsize;
      boundary=this->right;
    }

    for (i=0; i<limit; i++){
      u1=xt;
      u2=-yt;
      for (k=0; k<maxits; k++){
        nf1=u1 + u1*u2*u2 - u1*u1*u1/three-xt;
        nf2=-u2 - u1*u1*u2 + u2*u2*u2/three-yt;
        fnorm=sqrt(nf1*nf1+nf2*nf2);
        if (fnorm <= tol) break;
        njac11=one+u2*u2-u1*u1;
        njac12=two*u1*u2;
        njac21=-two*u1*u2;
        njac22=-one - u1*u1 + u2*u2;
        det = njac11*njac22-njac21*njac12;
        u1 = u1-(njac22*nf1-njac12*nf2)/det;
        u2 = u2-(njac11*nf2-njac21*nf1)/det;
      }

      boundary[i]=u1*u1-u2*u2;
      if (j==0 || j==1) {
        xt=xt+hx;
      } else if (j==2 || j==3){
        yt=yt+hy;
      }
      
    }

  }
  return 0;
}
// DO-NOT-DELETE splicer.end(Minsurf.Component._misc)

