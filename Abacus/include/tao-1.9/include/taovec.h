#ifndef TAOVECTOR_H
#define TAOVECTOR_H


#include "tao_basictypes.h"


/** 
    An abstract class representing the implementation of a TAO Vector. 
*/
class TaoIndexSet;

class TaoVec {

protected:
  
public:

  virtual ~TaoVec(void){};
  
  virtual int Clone(TaoVec**);
  int CloneVecs(int, TaoVec***);
  int DestroyVecs(int, TaoVec**);

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
  virtual int MinElement(double *);

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

  /* Replace each element with a -1, 0, or 1, depending on its sign.  */
  virtual int Sign();

  /** Get the dimension of the vector space */
  virtual int GetDimension( int* );

  /* View */
  virtual int View();

  /* stepMax */
  virtual int StepMax( TaoVec* , double* );
  virtual int StepBoundInfo(TaoVec* ,TaoVec*,TaoVec*,double*,double*,double*);
  virtual int BoundGradientProjection(TaoVec*,TaoVec*,TaoVec*, TaoVec*);

  /* Functionality for working in a reduced space */
  virtual int SetReducedVec(TaoVec*, TaoIndexSet*);

  virtual int ReducedCopyFromFull(TaoVec*, TaoIndexSet*);
  virtual int ReducedXPY(TaoVec *, TaoIndexSet*);

  virtual int CreateIndexSet(TaoIndexSet**);

  virtual int GetArray(TaoScalar **, int*);
  virtual int RestoreArray(TaoScalar **, int*);
};

#endif

