/* taofevalwkr.h */
/* class declaration for myfevalwkr */
#ifndef TAOFEVALWKR_H
#define TAOFEVALWKR_H

#include "tao_solver.h"
#include "fevalwkr.H"
#include "apps_solver.h"

FevalWkr* myfevalwkr(void);

/* MyFevalWkr class  -- See APPS documentation */
class TaoFevalWkr: public FevalWkr
{


  TAO_SOLVER tao;
  TAO_APPS *appsPtr;
  
 public:
  TaoFevalWkr(TAO_SOLVER t, TAO_APPS *a): FevalWkr()
  {tao=t;appsPtr=a;  };
   
  TaoFevalWkr():FevalWkr() {};

  ~TaoFevalWkr() {};

  double doFeval(vector<double>& x, bool& isf);
  //  double doFeval(double * x, bool& isf);
};


#endif

