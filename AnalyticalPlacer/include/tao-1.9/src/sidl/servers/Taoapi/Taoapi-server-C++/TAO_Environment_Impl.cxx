// 
// File:          TAO_Environment_Impl.cxx
// Symbol:        TAO.Environment-v0.3
// Symbol Type:   class
// Babel Version: 1.0.0
// Description:   Server-side implementation for TAO.Environment
// 
// WARNING: Automatically generated; only changes within splicers preserved
// 
// 
#include "TAO_Environment_Impl.hxx"

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
// DO-NOT-DELETE splicer.begin(TAO.Environment._includes)
#include "tao.h"
#include <iostream>
//TODO: what about these?
static int hasBeenInitialized;
static int nargs;
static char **args;
// DO-NOT-DELETE splicer.end(TAO.Environment._includes)

// speical constructor, used for data wrapping(required).  Do not put code here unless you really know what you're doing!
TAO::Environment_impl::Environment_impl() : StubBase(reinterpret_cast< 
  void*>(::TAO::Environment::_wrapObj(reinterpret_cast< void*>(this))),false) ,
  _wrapped(true){ 
  // DO-NOT-DELETE splicer.begin(TAO.Environment._ctor2)
  // Insert-Code-Here {TAO.Environment._ctor2} (ctor2)
  // DO-NOT-DELETE splicer.end(TAO.Environment._ctor2)
}

// user defined constructor
void TAO::Environment_impl::_ctor() {
  // DO-NOT-DELETE splicer.begin(TAO.Environment._ctor)
  // add construction details here
  // DO-NOT-DELETE splicer.end(TAO.Environment._ctor)
}

// user defined destructor
void TAO::Environment_impl::_dtor() {
  // DO-NOT-DELETE splicer.begin(TAO.Environment._dtor)
  // add destruction details here
  // DO-NOT-DELETE splicer.end(TAO.Environment._dtor)
}

// static class initializer
void TAO::Environment_impl::_load() {
  // DO-NOT-DELETE splicer.begin(TAO.Environment._load)
  // Insert-Code-Here {TAO.Environment._load} (class initialization)
  // DO-NOT-DELETE splicer.end(TAO.Environment._load)
}

// user defined static methods: (none)

// user defined non-static methods:
/**
 * Method:  Initialize[]
 */
int32_t
TAO::Environment_impl::Initialize_impl (
  /* in array<string> */::sidl::array< ::std::string> argv ) 
{
  // DO-NOT-DELETE splicer.begin(TAO.Environment.Initialize)
  int info;
  int i;
  if (hasBeenInitialized)
    return 0;

  nargs = argv.upper(0) - argv.lower(0) +1;  
  args = (char**)malloc(sizeof(char*)*nargs);
  for (i=0;i<nargs;i++) {
    args[i] = (char*)malloc(sizeof(char)*argv.get(i).length()+1);
    strncpy(args[i], argv.get(i).c_str(), argv.get(i).length()+1);
  }

  info = PetscInitialize(&nargs,&args,0,0);
  info = ::TaoInitialize(&nargs,&args,0,0);
  CHKERRQ(info);

  hasBeenInitialized = 1;
  return 0;

  // DO-NOT-DELETE splicer.end(TAO.Environment.Initialize)
}

/**
 * Method:  InitializeNoArgs[]
 */
int32_t
TAO::Environment_impl::InitializeNoArgs_impl () 

{
  // DO-NOT-DELETE splicer.begin(TAO.Environment.InitializeNoArgs)
  int info;
  int nargs;
  char **args;
    
  nargs = 1;  
  args = (char**)malloc(sizeof(char*));
  args[0] = (char*)malloc(8);
  strncpy(args[0], "unknown", 8);

  if (hasBeenInitialized)
    return 0;
  info = PetscInitialize(&nargs,&args,PETSC_NULL,PETSC_NULL); CHKERRQ(info);
  info = ::TaoInitialize(&nargs,&args,0,0); CHKERRQ(info);
  hasBeenInitialized = 1;
  return 0;

  // DO-NOT-DELETE splicer.end(TAO.Environment.InitializeNoArgs)
}

/**
 * Method:  Finalize[]
 */
int32_t
TAO::Environment_impl::Finalize_impl () 

{
  // DO-NOT-DELETE splicer.begin(TAO.Environment.Finalize)
  TaoFinalize();
  PetscFinalize();
  return 0;
  // DO-NOT-DELETE splicer.end(TAO.Environment.Finalize)
}


// DO-NOT-DELETE splicer.begin(TAO.Environment._misc)
// Put miscellaneous code here
// DO-NOT-DELETE splicer.end(TAO.Environment._misc)

