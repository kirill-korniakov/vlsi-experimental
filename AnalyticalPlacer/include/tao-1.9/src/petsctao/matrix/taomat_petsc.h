#ifndef TAOPETSCMAT_H
#define TAOPETSCMAT_H

#include "taomat.h"
#include "petscksp.h"

class TaoMatPetsc: public TaoMat {

 protected:

  Mat pm;
  MatStructure preflag;
  Mat pm_pre;
  PetscViewer pmatviewer;

 public:

  TaoMatPetsc( Mat M );
  ~TaoMatPetsc();

  int SetPetscViewer(PetscViewer);
  int SetMatrix(Mat, Mat, MatStructure);
  int GetMatrix(Mat*, Mat*, MatStructure*);

  
  /*
     The following methods are implementations of 
     the virtual methods in the base class 
  */

  int Compatible(TaoVec*, TaoVec*, TaoTruth*);

  int GetDimensions( int*, int * );
  int Multiply(TaoVec*,TaoVec*);
  int MultiplyTranspose(TaoVec*,TaoVec*);

  int SetDiagonal(TaoVec*);
  int AddDiagonal(TaoVec*);
  int GetDiagonal(TaoVec*);
  int ShiftDiagonal(double);    

  int RowScale(TaoVec*);
  int ColScale(TaoVec*);

  int CreateReducedMatrix(TaoIndexSet*,TaoIndexSet*,TaoMat**);
  int SetReducedMatrix(TaoMat*,TaoIndexSet*,TaoIndexSet*);

  int D_Fischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *, 
		TaoVec *, TaoVec *, TaoVec *, TaoVec *);
  int D_SFischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *, double,
                 TaoVec *, TaoVec *, TaoVec *, TaoVec *, TaoVec *);

  int Norm1(double*);

  int View();

  /* 
     The following methods are not currently needed by TAO
     but may be needed in the future.
  */
  int Clone(TaoMat**);
  int CopyFrom(TaoMat*);
  int MultiplyAdd(TaoVec*,TaoVec*,TaoVec*);
  int MultiplyTransposeAdd(TaoVec*,TaoVec*,TaoVec*);

};

#endif

