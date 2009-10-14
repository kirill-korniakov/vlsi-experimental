#ifdef TAO_USE_ADIC
#include "ad_deriv.h"

extern int DAAppSetADElementFunctionGradient(TAO_APPLICATION, 
					     int (*)(int[2],DERIV_TYPE[4],DERIV_TYPE*,void*), 
			       	             int, void *);

extern int DAAppSetADElementFunctionGradient2(TAO_APPLICATION, 
					     int (*)(int[2],DERIV_TYPE[16],DERIV_TYPE*,void*), 
			       	             int, void *);

#else

typedef double InactiveDouble;
typedef double DERIV_TYPE;
#define DAAppSetADElementFunctionGradient(a,b,c,d)   0
#define DAAppSetADElementFunctionGradient2(a,b,c,d)   0

#endif
