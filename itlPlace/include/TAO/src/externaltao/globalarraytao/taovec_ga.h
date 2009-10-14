/**************************************************************
File: taovec_ga.h

TAO/GA project

Author: Limin Zhang, Ph.D.
        Mathematics Department
        Columbia Basin College
        Pasco, WA 99301
        Limin.Zhang@cbc2.org

Mentor: Jarek Naplocha, Ph.D.
        Environmental Molecular Science Laboratory
        Pacific Northwest National Laboratory
        Richland, WA 99352

Date: 7/11/2002
Revised: (1) 7/15/02
         replace pv with this->pv  
         introduce GAVec as int so that it looks like PetscVec
         (2) 9/6/02
	 reformat it so that it has the same look as taovec.h. It also made clear
         about which member functions have been inherited from the base class TaoVec defined in taovec.h

Purpose:
      to design and implement a tao/ga vector using 
      global arrays.
**************************************************************/



#ifndef TAOVEC_GA_H
#define TAOVEC_GA_H

#include "taovec.h"
#include "ga.h"        //to define GA lib
#include "macdecls.h"  //to define GA constants like MT_C_DBL


typedef int GAVec;  //GAVec is the native global array vector. In fact, it is the global array handle.
typedef double GAScalar; //The only data we suppoort for the moment is double.

class TaoVecGa: public TaoVec{

 protected:

   GAVec pv; //pv is the global array handle that means the pointer to the vector.
 
 public:

  TaoVecGa( GAVec);
  ~TaoVecGa(){ if (pv) GA_Destroy(pv);}; 

  //void* VecObject; is inherited from TaoVec class.

  inline GAVec GetVec(){return pv;}

  virtual int Clone(TaoVec**);
  //int CloneVecs(int, TaoVec***); is inherited from TaoVec class.
  //int DestroyVecs(int, TaoVec**);is inherited from TaoVec class.

  virtual int  Compatible (TaoVec *v, TaoTruth *flag);

  /** Set all elements of this Tao Vector to zero. */
  virtual int SetToZero();
  /** Set all elements of this Tao Vector to the constant value c */
  virtual int SetToConstant( TaoScalar );

  /** Copy the elements of one vector to another */
  virtual int CopyFrom( TaoVec* );

  virtual int ScaleCopyFrom( TaoScalar, TaoVec* );

  /** Return the norm of this Tao Vector. */  
  virtual int NormInfinity(TaoScalar *);
  virtual int Norm1(TaoScalar *);
  virtual int Norm2(TaoScalar *);
  virtual int Norm2squared(TaoScalar *);

  /** Multiply the components of this Tao Vector by the components of v. */
  virtual int PointwiseMultiply( TaoVec*, TaoVec* );

  /** Divide the components of this Tao Vector by the components of v. */
  virtual int PointwiseDivide( TaoVec*, TaoVec* ); 

  /** Set the elements of one vector to the max/min of corresponding elements of two compatible vectors */
  virtual int PointwiseMinimum( TaoVec*, TaoVec* );
  virtual int PointwiseMaximum( TaoVec*, TaoVec* ); 

  /** Set the elements of one vector to the median of corresponding elements of three compatible vectors */
  virtual int Median( TaoVec* , TaoVec*, TaoVec* );

  /** Calculate the fischer function (using billups composition) given
      x, f, l, u
  */
  virtual int Fischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *);
  
  /** Scale each element of this Tao Vector by the constant alpha */
  virtual int Scale( TaoScalar );
   
  /** this += alpha * x */
  virtual int Axpy( TaoScalar, TaoVec* );
  
  /** this = alpha * this + x */
  virtual int Aypx  ( TaoScalar, TaoVec* );
  
  
  /*Adds a scalar multiple of a vector to a multiple of this vector. (this=alpha*xx + beta*this) */
  virtual int Axpby  ( TaoScalar , TaoVec*, TaoScalar  );

  /** this = alpha * x + beta *y */
  virtual int Waxpby  ( TaoScalar , TaoVec*, TaoScalar , TaoVec* );

  /** Take the absolute value of the elements */
  virtual int AbsoluteValue( );
 
  /** Take the minimum of the absolute value of the elements */
  virtual int MinElement(TaoScalar*);

  /** Add c to the elements of this Tao Vector */
  virtual int AddConstant( TaoScalar );
 
  /** Return the dot product of this Tao Vector with v */
  virtual int Dot( TaoVec*, TaoScalar *);

  /** Negate all the elements of this Tao Vector. */
  virtual int Negate();

  /** Invert (1/x) the elements of this Tao Vector. */
  virtual int Reciprocal();

  /* Replace each element with a -1, 0, or 1, depending on its sign.  */
  //  virtual int Sign();

  /** Get the dimension of the vector space */
  virtual int GetDimension(int *);

  /* View */
  virtual int View();

  /* stepMax */
  virtual int StepMax( TaoVec* , double* );
  virtual int StepMax2 (TaoVec *, TaoVec *, TaoVec *, TaoScalar *);
  virtual int StepBoundInfo(TaoVec* ,TaoVec*,TaoVec*,double*,double*,double*);
  virtual int BoundGradientProjection(TaoVec*,TaoVec*,TaoVec*, TaoVec*);

  /* Functionality for working in a reduced space */
  virtual int SetReducedVec(TaoVec*, TaoIndexSet*);
  virtual int ReducedCopyFromFull(TaoVec*, TaoIndexSet*);
  virtual int ReducedXPY(TaoVec*, TaoIndexSet*);
  virtual int CreateIndexSet(TaoIndexSet**S);

  /* Sets a pointer to the first element in the vector array on the local patch. */
  virtual int GetArray(TaoScalar **, int*);
 
  /* Returns a pointer to the first element in the vector array on the local patch.*/ 
  virtual int RestoreArray(TaoScalar **, int*);

};


int TaoWrapGaVec (GAVec V, TaoVecGa ** TV);
int TaoVecGetGaVec (TaoVec * TV, GAVec *V);

#endif


