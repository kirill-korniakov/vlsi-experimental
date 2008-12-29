#ifndef SIMPLEVECTOR_H
#define SIMPLEVECTOR_H

// #include "tao_basictypes.h"
#include "taovec.h"

class TaoVecDoubleArray: public TaoVec{

 protected:

  int n;
  double *v;
  int dallocated;
 public:
  
  inline int GetData(double**dd,int*nn){*dd=v;*nn=n; return 0;}

  TaoVecDoubleArray( int nn );
  TaoVecDoubleArray( int nn , double *vv);
  ~TaoVecDoubleArray(){if (n>0 && dallocated) delete [] v;};


  int Compatible (TaoVec *v, TaoTruth*);
  int GetArray(TaoScalar **, int*);
  int RestoreArray(TaoScalar **, int*);
  int Clone(TaoVec**);
  int GetDimension(int *);

  int View();

  int GetDoubles(double **, int*);
  int RestoreDoubles(double **, int*);

};

#endif

