// 
// File:          TaoSolver_Component_Impl.cxx
// Symbol:        TaoSolver.Component-v0.0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TaoSolver.Component
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 
#include "TaoSolver_Component_Impl.hxx"

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
// DO-NOT-DELETE splicer.begin(TaoSolver.Component._includes)
#include <iostream>
#include <ctype.h>
#include "sidl.hxx"
#include "Solver_OptimizationSolver.hxx"
#include "Solver_ProjectState.hxx"
#include "Solver_TerminateReason.hxx"
#include "gov_cca_Port.hxx"
#include "gov_cca_Services.hxx"

// DO-NOT-DELETE splicer.end(TaoSolver.Component._includes)

// speical constructor, used for data wrapping(required).  Do not put code here unless you really know what you're doing!
TaoSolver::Component_impl::Component_impl() : StubBase(reinterpret_cast< 
  void*>(::TaoSolver::Component::_wrapObj(reinterpret_cast< void*>(this))),
  false) , _wrapped(true){ 
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component._ctor2)
  // Insert-Code-Here {TaoSolver.Component._ctor2} (ctor2)
  // DO-NOT-DELETE splicer.end(TaoSolver.Component._ctor2)
}

// user defined constructor
void TaoSolver::Component_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component._ctor)
  this->tao_environment = 0;
  this->tao = 0;
  this->model = 0;
  this->myServices = 0;
  //  this->dynTestDone = false;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component._ctor)
}

// user defined destructor
void TaoSolver::Component_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component._dtor)
  
  // DO-NOT-DELETE splicer.end(TaoSolver.Component._dtor)
}

// static class initializer
void TaoSolver::Component_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component._load)
  // Insert-Code-Here {TaoSolver.Component._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TaoSolver.Component._load)
}

// user defined static methods: (none)

// user defined non-static methods:
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
TaoSolver::Component_impl::setServices_impl (
  /* in */::gov::cca::Services services ) 
// throws:
//     ::gov::cca::CCAException
//     ::sidl::RuntimeException
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.setServices)
  if (services._is_nil()) {
    return;
  }
  myPort = sidl::babel_cast<gov::cca::Port>(*this);
  std::cout << "cat myport=self..." << std::endl;
  if (myPort._is_nil()) {
    std::cerr << "unable to cast self as port (TaoSolver::Component_impl) in TaoSolver::Component_impl::setServices" << std::endl;
  }
  
  myServices = services;
  if (myServices._is_nil()) {
    std::cout << "myServices is nil" << std::endl;
    return;
  }


  try {
    myServices.addProvidesPort(myPort,
			       ::std::string("OptimizationSolver"),
			       ::std::string("OptimizationPorts.SolverPort"),
			       0);
  } catch (gov::cca::CCAException e) {
    std::cerr << "Error adding providesPort in TaoSolver: " << e.getNote() << std::endl;
  }


  try { 
    myServices.registerUsesPort(::std::string("OptimizationModel"),
				::std::string("OptimizationPorts.ModelPort"),
				0);
  } catch (gov::cca::CCAException e) {
    std::cerr << "Error adding providesPort in TaoSolver: " << e.getNote() << std::endl;
  }
    
  

  try {
    this->myServices.registerUsesPort("ppf",
                                      "gov.cca.ports.ParameterPortFactory",
                                      0);
  } catch (gov::cca::CCAException e) {
    std::cerr << "Error registering 'ppf' usesPort in TaoSolver: " << e.getNote() << std::endl;
  }
  // Read parameters
  ppf = sidl::babel_cast<gov::cca::ports::ParameterPortFactory>(
					      myServices.getPort("ppf") );
  if (ppf._is_nil()) {
    std::cerr << "ppf not connected!" << std::endl;
  }
  else {
    std::cout << "Calling up setupParameters()" << std::endl;
    setupParameters(ppf);
  }

  
							   
  try {
    myServices.registerUsesPort(::std::string("LAFactory"),
				::std::string("OptimizationPorts.LAFactoryPort"),
				0);
  } catch (gov::cca::CCAException e) {
    std::cerr << "Error registering UsesPort in TaoSolver: " << e.getNote() << std::endl;
  }

  // DO-NOT-DELETE splicer.end(TaoSolver.Component.setServices)
}

/**
 * Method:  Initialize[]
 */
int32_t
TaoSolver::Component_impl::Initialize_impl () 

{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.Initialize)
  int info;   
  sidl::DLL envdll = sidl::Loader::findLibrary(std::string("TAO.Environment"), std::string("ior/impl"), sidl::Scope_GLOBAL, sidl::Resolve_SCLRESOLVE);
  sidl::DLL soldll = sidl::Loader::findLibrary(std::string("TAO.Solver"), std::string("ior/impl"), sidl::Scope_GLOBAL, sidl::Resolve_SCLRESOLVE);
  if (envdll._not_nil() && soldll._not_nil()) {
    this->tao_environment = sidl::babel_cast<Solver::ProjectState>(envdll.createClass(std::string("TAO.Environment")));
    this->tao = sidl::babel_cast<Solver::OptimizationSolver>(soldll.createClass(std::string("TAO.Solver")));
  } else {
    std::cout <<"Could not load TAO.Solver. Check SIDL_DLL_PATH." << std::endl;
    return -1;
  }
  if (this->tao_environment._is_nil() || this->tao._is_nil()) {
    std::cout <<"Could not load TAO.Solver. Check SIDL_DLL_PATH." << std::endl;
    return -1;
  }    


  gov::cca::Port myModelPort = myServices.getPort(std::string("OptimizationModel"));
  
  this->model = sidl::babel_cast<OptimizationPorts::ModelPort>(myModelPort);
  if (this->model._is_nil()) {
    std::cerr << "Could not connect with OptimizationPorts::ModelPort!" << std::endl;
    return -1;
  }
  gov::cca::Port myFactoryPort = myServices.getPort(std::string("LAFactory"));
  if (myFactoryPort._is_nil()) {
    std::cout << "Could not find LAFactory component -- using default PETSc" << std::endl;
    this->factory = 0;
  } else {
    this->factory = sidl::babel_cast<OptimizationPorts::LAFactoryPort>(myFactoryPort);
    if (this->factory._is_nil()) {
      std::cout << "Attempt to connect with OptimizationPorts::LAFactoryPort unsuccessful!  Using default PETSc" << std::endl;
      this->factory = 0;
    } 
    else 
      this->factory.initialize();
  }
  std::string options_str;
  // Set parameter info

  if (pp._not_nil()) {
    tm = pp.readConfigurationMap();
    options_str =  tm.getString("tao_options","");
  }
  else {
    options_str=std::string("");
  }
  
  // parse options string (% delimited string)
  sidl::array<std::string> args;
  
  int options_str_size = options_str.size();
  if (options_str_size) {
    char *options=(char*)malloc(options_str_size+1);

    options_str.copy(options,options_str_size);
    options[options_str_size]='\0';

    if (strncmp(options,"",1)) {
      int i,argc=1;
      char *charPtr=&options[0];
      // Count # of delimiters
      while (*charPtr && !isspace(*charPtr)) {
	if (*charPtr == '%') {
	  *charPtr = 0;
	  if (charPtr != options && *(charPtr-1))
	    argc++;
	} 
	charPtr++;
      }
    
      //reset charPtr to start
      charPtr = options;
      args = sidl::array<std::string>::create1d(argc+1);
      args.set(0,"TaoSolverComponent");
      args.set(1,charPtr);
      for (i=2;i<=argc;i++) {
	while (*charPtr)
	  charPtr++;
	while (!*charPtr)
	  charPtr++;
	args.set(i,charPtr);
      }
    } else {
      args = sidl::array<std::string>::create1d(1);
      args.set(0,"TaoSolverComponent");
    }
  }
  else {
    args = sidl::array<std::string>::create1d(1);
    args.set(0,"TaoSolverComponent");
  }
  //Initialize PETSc and TAO
  info = tao_environment.Initialize(args);//(args);

  // create the solver
  info = Create("tao_lmvm");
  if (pp._not_nil()) {
    tm = pp.readConfigurationMap();
    SetMethod(tm.getString("tao_method","tao_lmvm"));
    SetMaximumIterates(tm.getInt("tao_max_its",500));
    SetMaximumFunctionEvaluations(tm.getInt("tao_max_funcs",10000000));
    SetTolerances(tm.getDouble("tao_fatol",0), tm.getDouble("tao_frtol",0),0,0);
    SetGradientTolerances(tm.getDouble("tao_gatol",1e-8),tm.getDouble("tao_grtol",1e-8),
			  tm.getDouble("tao_gttol",1e-8));
  }

  
  SetLAFactory(this->factory);

  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.Initialize)
}

/**
 * Method:  Solve[]
 */
int32_t
TaoSolver::Component_impl::Solve_impl () 

{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.Solve)
  int info;
  info = SetFromOptions();
  info = SolveApplication(this->model);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.Solve)
}

/**
 * Method:  Finalize[]
 */
int32_t
TaoSolver::Component_impl::Finalize_impl () 

{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.Finalize)
  //  tao_environment.Finalize();
  if (this->model._not_nil()) {
    this->model.finalize();
    this->myServices.releasePort(std::string("OptimizationModel"));
  }

  if (this->factory._not_nil()) {
    this->factory.finalize();
    this->myServices.releasePort(std::string("LAFactory"));
  }
  std::cout << "Calling tao_environment.Finalize()" << std::endl;
  
  return 0;

  // DO-NOT-DELETE splicer.end(TaoSolver.Component.Finalize)
}

/**
 * Method:  Create[]
 */
int32_t
TaoSolver::Component_impl::Create_impl (
  /* in */const ::std::string& method ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.Create)
  int info;
  info = tao.Create(method);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.Create)
}

/**
 * Method:  Destroy[]
 */
int32_t
TaoSolver::Component_impl::Destroy_impl () 

{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.Destroy)
  int info;
  info = tao.Destroy();
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.Destroy)
}

/**
 * Method:  SetTolerances[]
 */
int32_t
TaoSolver::Component_impl::SetTolerances_impl (
  /* in */double fatol,
  /* in */double frtol,
  /* in */double catol,
  /* in */double crtol ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetTolerances)
  int info;
  info = tao.SetTolerances(fatol, frtol, catol, crtol);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetTolerances)
}

/**
 * Method:  SolveApplication[]
 */
int32_t
TaoSolver::Component_impl::SolveApplication_impl (
  /* in */::Optimize::OptimizationModel app ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SolveApplication)
  int info;
  info = tao.SolveApplication(app);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SolveApplication)
}

/**
 * Method:  View[]
 */
int32_t
TaoSolver::Component_impl::View_impl () 

{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.View)
  int info;
  info = tao.View();
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.View)
}

/**
 * Method:  GetIterationData[]
 */
int32_t
TaoSolver::Component_impl::GetIterationData_impl (
  /* out */int32_t& iterate,
  /* out */double& f,
  /* out */double& gnorm,
  /* out */double& cnorm,
  /* out */double& xdiff,
  /* out */::Solver::TerminateReason& reason ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.GetIterationData)
  int info;
  info = tao.GetIterationData(iterate,f,gnorm,cnorm,xdiff,reason);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.GetIterationData)
}

/**
 * Method:  GetMethod[]
 */
int32_t
TaoSolver::Component_impl::GetMethod_impl (
  /* out */::std::string& type ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.GetMethod)
  int info;
  info = tao.GetMethod(type);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.GetMethod)
}

/**
 * Method:  SetFunctionLowerBound[]
 */
int32_t
TaoSolver::Component_impl::SetFunctionLowerBound_impl (
  /* in */double fmin ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetFunctionLowerBound)
  int info;
  info = tao.SetFunctionLowerBound(fmin);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetFunctionLowerBound)
}

/**
 * Method:  GetGradientTolerances[]
 */
int32_t
TaoSolver::Component_impl::GetGradientTolerances_impl (
  /* out */double& gatol,
  /* out */double& grtol,
  /* out */double& gttol ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.GetGradientTolerances)
  int info;
  info = tao.GetGradientTolerances(gatol, grtol, gttol);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.GetGradientTolerances)
}

/**
 * Method:  SetGradientTolerances[]
 */
int32_t
TaoSolver::Component_impl::SetGradientTolerances_impl (
  /* in */double gatol,
  /* in */double grtol,
  /* in */double gttol ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetGradientTolerances)
  int info;
  info = tao.SetGradientTolerances(gatol, grtol, gttol);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetGradientTolerances)
}

/**
 * Method:  SetMaximumFunctionEvaluations[]
 */
int32_t
TaoSolver::Component_impl::SetMaximumFunctionEvaluations_impl (
  /* in */int32_t nfcn ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetMaximumFunctionEvaluations)
  int info;
  info = tao.SetMaximumFunctionEvaluations(nfcn);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetMaximumFunctionEvaluations)
}

/**
 * Method:  SetMaximumIterates[]
 */
int32_t
TaoSolver::Component_impl::SetMaximumIterates_impl (
  /* in */int32_t maxits ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetMaximumIterates)
  int info;
  info = tao.SetMaximumIterates(maxits);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetMaximumIterates)
}

/**
 * Method:  SetMethod[]
 */
int32_t
TaoSolver::Component_impl::SetMethod_impl (
  /* in */const ::std::string& type ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetMethod)
  int info;
  info = tao.SetMethod(type);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetMethod)
}

/**
 * Method:  SetLAFactory[]
 */
int32_t
TaoSolver::Component_impl::SetLAFactory_impl (
  /* in */::LinearAlgebra::Factory factory ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetLAFactory)
  int info;
  info = tao.SetLAFactory(factory);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetLAFactory)
}

/**
 * Method:  SetFromOptions[]
 */
int32_t
TaoSolver::Component_impl::SetFromOptions_impl () 

{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.SetFromOptions)
  int info;
  info = tao.SetFromOptions();
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.SetFromOptions)
}

/**
 * Method:  GetSolverHandle[]
 */
int32_t
TaoSolver::Component_impl::GetSolverHandle_impl (
  /* out */void*& handle ) 
{
  // DO-NOT-DELETE splicer.begin(TaoSolver.Component.GetSolverHandle)
  int info;
  info = tao.GetSolverHandle(handle);
  return info;
  // DO-NOT-DELETE splicer.end(TaoSolver.Component.GetSolverHandle)
}


// DO-NOT-DELETE splicer.begin(TaoSolver.Component._misc)

void TaoSolver::Component_impl::setupParameters(gov::cca::ports::ParameterPortFactory &ppf)
{
  gov::cca::TypeMap tmp;
  tmp = myServices.createTypeMap();
  if (tmp._is_nil()) {
    std::cerr << "Could not create type map.  Parameter setup failed." << std::endl;
    return;
  }
  ppf.initParameterData(tmp,"CONFIG");
  ppf.setBatchTitle(tmp,"TAO Solver Options");
  ppf.setGroupName(tmp,"General Solver Options");
  ppf.addRequestInt(tmp,"tao_max_its","Maximum iterations for TAO solver",
		    "tao_max_its",500,0,100000000);
  ppf.addRequestInt(tmp,"tao_max_func",
		    "Maximum function evaluations for TAO solver",
		    "tao_max_func",10000000,0,10000000);
  
  ppf.addRequestString(tmp,"tao_options","'%'-delimited options",
		       "tao_options","");
  ppf.addRequestString(tmp,"tao_method","Choose algorithm to use in TAO solver",
		       "tao_method","tao_lmvm");

  sidl::array<std::string> methods = sidl::array<std::string>::create1d(11);
  methods[0] = "tao_lmvm";
  methods[1] = "tao_cg";
  methods[2] = "tao_ntr";
  methods[3] = "tao_nls";
  methods[4] = "tao_blmvm";
  methods[5] = "tao_tron";
  methods[6] = "tao_bqpip";
  methods[7] = "tao_gpcg";
  methods[8] = "tao_ssils";
  methods[9] = "tao_ssfls";
  methods[10] = "tao_kt";
  ppf.addStringChoices(tmp,"tao_method",methods);

  ppf.addRequestDouble(tmp,"tao_fatol","Absolute objective value tolerance",
		       "tao_fatol", 0,0,10000000);
  ppf.addRequestDouble(tmp,"tao_frtol","Relative objective value tolerance",
		       "tao_frtol", 0,0,10000000);
  ppf.addRequestDouble(tmp,"tao_gatol","Absolute gradient norm tolerance",
		       "tao_gatol", 1e-8,0,10000000);
  ppf.addRequestDouble(tmp,"tao_grtol","Relative gradient norm tolerance",
		       "tao_grtol", 1e-8,0,10000000);
  ppf.addRequestDouble(tmp,"tao_gttol","Total reduction gradient norm tolerance",
		       "tao_gttol", 1e-8,0,10000000);
  ppf.addRequestDouble(tmp,"tao_steptol","Minimum Trust Region radius",
		       "tao_steptol", 0,0,10000000);
  ppf.addRequestInt(tmp,"tao_lmvm_lm","The number of previous points and gradients to be used",
		    "tao_lmvm_lm",20,1,10000);
  ppf.addRequestDouble(tmp,"tao_lmvm_initH","Diagonal for initial Hessian",
		       "tao_lmvm_initH",0,1e20,0);
  std::cout << "created parameters..." << std::endl;
  ppf.addParameterPort(tmp,myServices);
  myServices.releasePort("ppf");
  pp = babel_cast<gov::cca::ports::ParameterPort>( 
	   myServices.getPort("CONFIG") );
  if (pp._is_nil()) {
    std::cerr << "Could not connect with parameter port" << std::endl;
  }

}


/*
bool
TaoSolver::Component_impl::updateParameterPort(ConfigurableParameterPort *opp)
{
  std::cout << "updating ParameterPort..." << std::endl;
  if (!(this->cpp == opp)) {
    return false;
  }

  if (!this->dynTestDone) {
    this->cpp->addRequest(utest);
    this->dynTestDone = true;
    return true;
  }
  
  return false;
}
*/
// DO-NOT-DELETE splicer.end(TaoSolver.Component._misc)

