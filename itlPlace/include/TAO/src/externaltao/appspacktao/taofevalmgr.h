#ifndef TAOFEVALMGR_H
#define TAOFEVALMGR_H
#include "fevalmgr.H"
#include "tao_solver.h"
#include "apps_solver.h"
/* the MyFevalMgr class is descended from the FevalMgr class.  It is used
   to help set initial values and bounds for the MyFevalWkr's */

class TaoFevalMgr: public FevalMgr
{

  bool usebounds;
public:
  static TAO_SOLVER tao;
  static TAO_APPS *appsPtr;
  TaoVec *xl, *xu;
public:
  /* All these functions are in the ancestor */
  TaoFevalMgr();
  ~TaoFevalMgr();
  void setInitX();  // except this one
  void setBounds(); // and this one
  bool setFevalMgr(const vector<string>& fargs, bool issingle);
  void InitWorkers();
  static int setTao(TAO_SOLVER tao, TAO_APPS *appsPtr); // and this one
};

#endif
