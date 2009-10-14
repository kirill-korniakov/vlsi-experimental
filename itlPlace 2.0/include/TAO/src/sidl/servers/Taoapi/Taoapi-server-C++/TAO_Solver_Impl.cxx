// 
// File:          TAO_Solver_Impl.cxx
// Symbol:        TAO.Solver-v0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TAO.Solver
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 
#include "TAO_Solver_Impl.hxx"

// 
// Includes for all method dependencies.
// 
#ifndef included_LinearAlgebra_Factory_hxx
#include "LinearAlgebra_Factory.hxx"
#endif
#ifndef included_Optimize_OptimizationModel_hxx
#include "Optimize_OptimizationModel.hxx"
#endif
#ifndef included_Solver_TerminateReason_hxx
#include "Solver_TerminateReason.hxx"
#endif
#ifndef included_sidl_BaseInterface_hxx
#include "sidl_BaseInterface.hxx"
#endif
#ifndef included_sidl_ClassInfo_hxx
#include "sidl_ClassInfo.hxx"
#endif
#ifndef included_sidl_NotImplementedException_hxx
#include "sidl_NotImplementedException.hxx"
#endif
// DO-NOT-DELETE splicer.begin(TAO.Solver._includes)
#include "tao.h"
#include "taoapp_sidl.h"
#include "LinearAlgebra_Factory.hxx"
#include "taovec.h"
#include "src/petsctao/vector/taovec_petsc.h"
#include <iostream>
// DO-NOT-DELETE splicer.end(TAO.Solver._includes)

// speical constructor, used for data wrapping(required).  Do not put code here unless you really know what you're doing!
TAO::Solver_impl::Solver_impl() : StubBase(reinterpret_cast< 
  void*>(::TAO::Solver::_wrapObj(reinterpret_cast< void*>(this))),false) ,
  _wrapped(true){ 
  // DO-NOT-DELETE splicer.begin(TAO.Solver._ctor2)
  // Insert-Code-Here {TAO.Solver._ctor2} (ctor2)
  // DO-NOT-DELETE splicer.end(TAO.Solver._ctor2)
}

// user defined constructor
void TAO::Solver_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TAO.Solver._ctor)
  this->tao = NULL;
  this->lafactory = NULL;
  this->diag = 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver._ctor)
}

// user defined destructor
void TAO::Solver_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TAO.Solver._dtor)
  // add destruction details here
  // DO-NOT-DELETE splicer.end(TAO.Solver._dtor)
}

// static class initializer
void TAO::Solver_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TAO.Solver._load)
  // Insert-Code-Here {TAO.Solver._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TAO.Solver._load)
}

// user defined static methods: (none)

// user defined non-static methods:
/**
 * Method:  Create[]
 */
int32_t
TAO::Solver_impl::Create_impl (
  /* in */const ::std::string& method ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.Create)
  int info;
  info = TaoCreate(PETSC_COMM_SELF, (char*)method.c_str(),&this->tao); CHKERRQ(info);
  return 0;
 
  // DO-NOT-DELETE splicer.end(TAO.Solver.Create)
}

/**
 * Method:  Destroy[]
 */
int32_t
TAO::Solver_impl::Destroy_impl () 

{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.Destroy)
  int info;
  info = TaoDestroy(this->tao); CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.Destroy)
}

/**
 * Method:  SetTolerances[]
 */
int32_t
TAO::Solver_impl::SetTolerances_impl (
  /* in */double fatol,
  /* in */double frtol,
  /* in */double catol,
  /* in */double crtol ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetTolerances)
  int info;
  info = TaoSetTolerances(this->tao, fatol,frtol,catol,crtol);
  CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.SetTolerances)
}

/**
 * Method:  SolveApplication[]
 */
int32_t
TAO::Solver_impl::SolveApplication_impl (
  /* in */::Optimize::OptimizationModel app ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SolveApplication)
  int info;
  TaoSIDLApplication *sidlapp = new TaoSIDLApplication(app,this->lafactory);

  if (sidlapp != NULL) {
    info = TaoSetApplication(this->tao, static_cast<TaoApplication*>(sidlapp));
    CHKERRQ(info);
    PetscTruth flag;
    PetscOptionsGetReal(PETSC_NULL,"-tao_initH",&this->diag,&flag);
    if (flag == PETSC_TRUE && this->diag > 0) {
      TaoVec *tv;

      if (this->lafactory._not_nil()) 
	tv = (TaoVec*)this->lafactory.createVec(app.getNumberVariables());
      else {
	Vec V;
	info = VecCreateSeq(PETSC_COMM_SELF,app.getNumberVariables(),&V); CHKERRQ(info);
	TaoVecPetsc *tvp;
	info = TaoWrapPetscVec(V,&tvp);
	tv = static_cast<TaoVec*>(tvp);
      }
      tv->SetToConstant(this->diag);
      tv->View();
      std::cout << "Setting lmvm matrix" << std::endl;
      info = TaoInitializeLMVMmatrix(this->tao, tv);
    }
    info = sidlapp->setTao(this->tao); CHKERRQ(info);
    info=PetscOptionsHasName(PETSC_NULL,"-tao_lmvmh",&flag);
    if (flag){
      info=TaoLMVMSetH0(tao,TAO_TRUE); CHKERRQ(info);
      info=TaoBLMVMSetH0(tao,TAO_TRUE); CHKERRQ(info);
    }
    info = TaoSolve(this->tao); CHKERRQ(info);
  } 
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.SolveApplication)
}

/**
 * Method:  View[]
 */
int32_t
TAO::Solver_impl::View_impl () 

{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.View)
  int info;
  info = TaoView(this->tao); CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.View)
}

/**
 * Method:  GetIterationData[]
 */
int32_t
TAO::Solver_impl::GetIterationData_impl (
  /* out */int32_t& iterate,
  /* out */double& f,
  /* out */double& gnorm,
  /* out */double& cnorm,
  /* out */double& xdiff,
  /* out */::Solver::TerminateReason& reason ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.GetIterationData)
  int info;
  ::TaoTerminateReason r;
  info = TaoGetSolutionStatus(this->tao,&iterate,&f,&gnorm,&cnorm,&xdiff,
			     &r); CHKERRQ(info);
  reason = (::Solver::TerminateReason) r;
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.GetIterationData)
}

/**
 * Method:  GetMethod[]
 */
int32_t
TAO::Solver_impl::GetMethod_impl (
  /* out */::std::string& type ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.GetMethod)
  int info;
  TaoMethod m;
  type.erase();
  info = TaoGetMethod(this->tao, &m); CHKERRQ(info);
  type += std::string(m);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.GetMethod)
}

/**
 * Method:  SetFunctionLowerBound[]
 */
int32_t
TAO::Solver_impl::SetFunctionLowerBound_impl (
  /* in */double fmin ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetFunctionLowerBound)
  int info;
  info = TaoSetFunctionLowerBound(this->tao, fmin); CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.SetFunctionLowerBound)
}

/**
 * Method:  GetGradientTolerances[]
 */
int32_t
TAO::Solver_impl::GetGradientTolerances_impl (
  /* out */double& gatol,
  /* out */double& grtol,
  /* out */double& gttol ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.GetGradientTolerances)
  int info;
  info = TaoGetGradientTolerances(this->tao, &gatol,&grtol,&gttol); 
  CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.GetGradientTolerances)
}

/**
 * Method:  SetGradientTolerances[]
 */
int32_t
TAO::Solver_impl::SetGradientTolerances_impl (
  /* in */double gatol,
  /* in */double grtol,
  /* in */double gttol ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetGradientTolerances)
  int info;
  info = TaoSetGradientTolerances(this->tao,gatol,grtol,gttol); CHKERRQ(info);
  return 0;  // DO-NOT-DELETE splicer.end(TAO.Solver.SetGradientTolerances)
}

/**
 * Method:  SetMaximumFunctionEvaluations[]
 */
int32_t
TAO::Solver_impl::SetMaximumFunctionEvaluations_impl (
  /* in */int32_t nfcn ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetMaximumFunctionEvaluations)
  int info;
  info = TaoSetMaximumFunctionEvaluations(this->tao, nfcn); CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.SetMaximumFunctionEvaluations)
}

/**
 * Method:  SetMaximumIterates[]
 */
int32_t
TAO::Solver_impl::SetMaximumIterates_impl (
  /* in */int32_t maxits ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetMaximumIterates)
  int info;
  info = TaoSetMaximumIterates(this->tao, maxits); CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.SetMaximumIterates)
}

/**
 * Method:  SetMethod[]
 */
int32_t
TAO::Solver_impl::SetMethod_impl (
  /* in */const ::std::string& type ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetMethod)
  int info;
  info = TaoSetMethod(this->tao,(char*)type.c_str() ); CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.SetMethod)
}

/**
 * Method:  SetLAFactory[]
 */
int32_t
TAO::Solver_impl::SetLAFactory_impl (
  /* in */::LinearAlgebra::Factory factory ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetLAFactory)
  if (factory._not_nil()) {
    this->lafactory = factory;
    return 0;
  } else {
   return 1;
  }

  // DO-NOT-DELETE splicer.end(TAO.Solver.SetLAFactory)
}

/**
 * Method:  SetFromOptions[]
 */
int32_t
TAO::Solver_impl::SetFromOptions_impl () 

{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.SetFromOptions)
  int info;

  info = TaoSetFromOptions(this->tao); CHKERRQ(info);
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.SetFromOptions)
}

/**
 * Method:  GetSolverHandle[]
 */
int32_t
TAO::Solver_impl::GetSolverHandle_impl (
  /* out */void*& handle ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Solver.GetSolverHandle)
  handle = &this->tao;
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Solver.GetSolverHandle)
}


// DO-NOT-DELETE splicer.begin(TAO.Solver._misc)
// Put miscellaneous code here
// DO-NOT-DELETE splicer.end(TAO.Solver._misc)

