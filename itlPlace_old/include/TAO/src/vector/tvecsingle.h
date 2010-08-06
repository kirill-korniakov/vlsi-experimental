#ifndef SIMPLEFLOATVECTOR_H
#define SIMPLEFLOATVECTOR_H

#include "tao_basictypes.h"
#include "taovec.h"

class TaoVecFloatArray: public TaoVec{

 protected:

  int n;
  float *v;
  int fallocated;
 public:
  
  inline int GetData(float**dd,int*nn){*dd=v;*nn=n; return 0;}

  TaoVecFloatArray( int nn );
  TaoVecFloatArray( int nn , float *vv);
  ~TaoVecFloatArray(){if (n>0 && fallocated) delete [] v;};

  virtual int Clone(TaoVec**);
  virtual int Compatible(TaoVec*, TaoTruth*);

  /** Set all elements of this Tao Vector to zero. */
  virtual int SetToZero();
  /** Set all elements of this Tao Vector to the constant value c */
  virtual int SetToConstant( double );

  /** Copy the elements of one vector to another */
  virtual int CopyFrom( TaoVec* );

  virtual int ScaleCopyFrom( double, TaoVec* );

  /** Return the norm of this Tao Vector. */
  virtual int NormInfinity( double* );
  virtual int Norm1( double* );
  virtual int Norm2( double* );
  virtual int Norm2squared( double* );

  /** Multiply the components of this Tao Vector by the components of v. */
  virtual int PointwiseMultiply( TaoVec* , TaoVec* );

  /** Divide the components of this Tao Vector by the components of v. */
  virtual int PointwiseDivide( TaoVec* , TaoVec* );

  /** Set the elements of one vector to the mi of corresponding elements of two compatible vectors */
  virtual int PointwiseMaximum( TaoVec* , TaoVec*);

  virtual int PointwiseMinimum( TaoVec* , TaoVec*);

  virtual int Median( TaoVec* , TaoVec* , TaoVec* );

  /** Calculate the Billups composition form of the Fischer function given
      x, f, l, u.  The smoothed version takes the smoothing parameter for each
      component as an additional argument.
  */
  virtual int  Fischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *);
  virtual int SFischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *, double);

  /** Scale each element of this Tao Vector by the constant alpha */
  virtual int Scale( double );

  /** this += alpha * x */
  virtual int Axpy  ( double , TaoVec* );

  /** this = alpha * this + x */
  virtual int Aypx  ( double , TaoVec* );

  virtual int Axpby  ( double , TaoVec*, double  );

  /** this = alpha * x + beta *y */
  virtual int Waxpby( double , TaoVec*, double , TaoVec* );

  /** Take the absolute value of the elements */
  virtual int AbsoluteValue( );

  /** Take the minimum of the absolute value of the elements */
  virtual int MinElement(double*);

  /** Add c to the elements of this Tao Vector */
  virtual int AddConstant( double );

  /** Return the dot product of this Tao Vector with v */
  virtual int Dot( TaoVec* , double *);

  /** Negate all the elements of this Tao Vector. */
  virtual int Negate();

  /** Invert (1/x) the elements of this Tao Vector. */
  virtual int Reciprocal();

  /** Take square root of the elements of this Tao Vector. */
  virtual int Sqrt();
  virtual int Pow( double );

  /** Get the dimension of the vector space */
  virtual int GetDimension( int* );

  /* View */
  virtual int View();

  /* stepMax */
  virtual int StepMax2( TaoVec* , TaoVec*, TaoVec*, double* );

  virtual int StepMax( TaoVec* , double* );

  virtual int BoundGradientProjection(TaoVec*,TaoVec*,TaoVec*, TaoVec*);

  /* Functionality for working in a reduced space */

  int GetFloats(float **, int*);
  int RestoreFloats(float **, int*);

};

#endif

