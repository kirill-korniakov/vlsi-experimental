/*
 * File:          Rosenbrock_RosenbrockModel_Impl.h
 * Symbol:        Rosenbrock.RosenbrockModel-v0.0.2
 * Symbol Type:   class
 * Babel Version: 0.8.8
 * Description:   Server-side implementation for Rosenbrock.RosenbrockModel
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 * babel-version = 0.8.8
 */

#ifndef included_Rosenbrock_RosenbrockModel_Impl_h
#define included_Rosenbrock_RosenbrockModel_Impl_h

#ifndef included_SIDL_header_h
#include "SIDL_header.h"
#endif
#ifndef included_Rosenbrock_RosenbrockModel_h
#include "Rosenbrock_RosenbrockModel.h"
#endif

/* DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._includes) */
/* Put additional include files here... */
/* DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._includes) */

/*
 * Private data for class Rosenbrock.RosenbrockModel
 */

struct Rosenbrock_RosenbrockModel__data {
  /* DO-NOT-DELETE splicer.begin(Rosenbrock.RosenbrockModel._data) */
  /* Put private data members here... */
  int n;
  double alpha;
  /* DO-NOT-DELETE splicer.end(Rosenbrock.RosenbrockModel._data) */
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct Rosenbrock_RosenbrockModel__data*
Rosenbrock_RosenbrockModel__get_data(
  Rosenbrock_RosenbrockModel);

extern void
Rosenbrock_RosenbrockModel__set_data(
  Rosenbrock_RosenbrockModel,
  struct Rosenbrock_RosenbrockModel__data*);

extern void
impl_Rosenbrock_RosenbrockModel__ctor(
  Rosenbrock_RosenbrockModel);

extern void
impl_Rosenbrock_RosenbrockModel__dtor(
  Rosenbrock_RosenbrockModel);

/*
 * User-defined object methods
 */

extern void
impl_Rosenbrock_RosenbrockModel_setNumberVariables(
  Rosenbrock_RosenbrockModel,
  int32_t);

extern void
impl_Rosenbrock_RosenbrockModel_setAlpha(
  Rosenbrock_RosenbrockModel,
  double);

extern int32_t
impl_Rosenbrock_RosenbrockModel_initialize(
  Rosenbrock_RosenbrockModel);

extern int32_t
impl_Rosenbrock_RosenbrockModel_finalize(
  Rosenbrock_RosenbrockModel);

extern int32_t
impl_Rosenbrock_RosenbrockModel_getNumberVariables(
  Rosenbrock_RosenbrockModel);

extern void
impl_Rosenbrock_RosenbrockModel_evaluateObjectiveFunction(
  Rosenbrock_RosenbrockModel,
  struct SIDL_double__array*,
  double*);

extern void
impl_Rosenbrock_RosenbrockModel_evaluateGradient(
  Rosenbrock_RosenbrockModel,
  struct SIDL_double__array*,
  struct SIDL_double__array*);

extern void
impl_Rosenbrock_RosenbrockModel_evaluateObjectiveAndGradient(
  Rosenbrock_RosenbrockModel,
  struct SIDL_double__array*,
  double*,
  struct SIDL_double__array*);

extern void
impl_Rosenbrock_RosenbrockModel_evaluateHessian(
  Rosenbrock_RosenbrockModel,
  struct SIDL_double__array*,
  struct SIDL_double__array*);

extern void
impl_Rosenbrock_RosenbrockModel_getVariableBounds(
  Rosenbrock_RosenbrockModel,
  struct SIDL_double__array*,
  struct SIDL_double__array*);

extern void
impl_Rosenbrock_RosenbrockModel_initializeVariables(
  Rosenbrock_RosenbrockModel,
  struct SIDL_double__array*);

extern void
impl_Rosenbrock_RosenbrockModel_monitor(
  Rosenbrock_RosenbrockModel);

extern void
impl_Rosenbrock_RosenbrockModel_checkConvergence(
  Rosenbrock_RosenbrockModel,
  int32_t*);

#ifdef __cplusplus
}
#endif
#endif
