#ifndef TAOPETSCVECTOR_H
#define TAOPETSCVECTOR_H

#include "taovec.h"
#include "petscksp.h"


class TaoVecPetsc: public TaoVec {

 protected:

  Vec pv;
  PetscViewer pvecviewer;

 public:

  TaoVecPetsc( Vec);
  ~TaoVecPetsc();

  inline Vec GetVec(){return pv;}
  int SetVec(Vec);
  int SetPetscViewer(PetscViewer);

  /* 
     The following methods are implementations of 
     the virtual method in the base class 
  */
  int Clone(TaoVec**);
         
  int Compatible (TaoVec *v, TaoTruth*);

  int SetToZero();
  int SetToConstant( double );
  int Norm1(double *);
  int Norm2(double *);
  int Norm2squared(double *);
  int NormInfinity(double *);
  int Dot( TaoVec*, double *);

  int CopyFrom( TaoVec* );
  int ScaleCopyFrom( double, TaoVec* );
  int Scale( double );
  int AddConstant( double );
  int Negate();
  int Reciprocal();
  int Sqrt();
  int Pow(double);

  int GetDimension(int *);

  int Axpy( double, TaoVec* );
  int Aypx( double, TaoVec* );
  int Axpby( double , TaoVec*, double  );

  int Waxpby( double , TaoVec*, double , TaoVec* );

  int AbsoluteValue( );
  int MinElement(double*);

  int PointwiseMultiply( TaoVec*, TaoVec* );
  int PointwiseDivide( TaoVec*, TaoVec* ); 

  int PointwiseMinimum( TaoVec*, TaoVec* );
  int PointwiseMaximum( TaoVec*, TaoVec* ); 
  int Median( TaoVec* , TaoVec*, TaoVec* );

  int Fischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *);
  int SFischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *, double);

  int View();
  int StepMax( TaoVec* , double* );
  int StepMax2( TaoVec* , TaoVec* , TaoVec* , double* );
  int StepBoundInfo(TaoVec* ,TaoVec*,TaoVec*,double*,double*,double*);

  int BoundGradientProjection(TaoVec*,TaoVec*,TaoVec*, TaoVec*);

  int GetArray(TaoScalar **, int*);
  int RestoreArray(TaoScalar **, int*);


  int CreateIndexSet(TaoIndexSet**S);

  int SetReducedVec(TaoVec*, TaoIndexSet*);
  int ReducedCopyFromFull(TaoVec*, TaoIndexSet*);
  int ReducedXPY(TaoVec*, TaoIndexSet*);

};

#endif


