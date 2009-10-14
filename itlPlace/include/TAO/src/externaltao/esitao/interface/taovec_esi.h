#ifndef TAOESIVECTOR_H
#define TAOESIVECTOR_H

#include "esi/ESI.h"
//#include "esi/petsc/indexspace.h"

//#include "esi/Vector.h"
//#include "esi/VectorReplaceAccess.h"

#include "taovec.h"

typedef esi::Vector<double,int> ESIVCTR;
// typedef esi::VectorReplaceAccess<double,int> ESIVCTR;


class TaoVecESI: public TaoVec{

 protected:
  int nlocal;

 public:
  esi::Vector<double,int> *esivec;


  TaoVecESI(esi::Vector<double,int> *);
  ~TaoVecESI();


  virtual int  Compatible(TaoVec*, TaoTruth*);
  virtual int Clone( TaoVec **);
 
  virtual int SetToConstant( double );
  virtual int SetToZero();
  virtual int Norm1(double *);
  virtual int Norm2(double *);
  virtual int Norm2squared(double *);
  virtual int NormInfinity(double *);
  virtual int Dot( TaoVec*, double *);
  virtual int CopyFrom( TaoVec* );
  virtual int ScaleCopyFrom( double, TaoVec* );
  virtual int Scale( double );
  virtual int Negate();
  virtual int Axpy( double, TaoVec* );
  virtual int Aypx  ( double, TaoVec* );
  virtual int Waxpby  ( double , TaoVec*, double , TaoVec* );

  virtual int PointwiseMultiply( TaoVec*, TaoVec* );

  virtual int GetArray(TaoScalar **, int*);
  virtual int RestoreArray(TaoScalar **, int*);
  virtual int View();
  virtual int GetDimension( int* );

};

#endif


