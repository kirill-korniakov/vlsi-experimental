#ifndef TAOESIMAT_H
#define TAOESIMAT_H

#include "esi/ESI.h"
#include "tao_solver.h"
#include "taomat.h"
#include "taolinearsolver.h"
#include "taolinearsolver_esi.h"

typedef esi::Operator<double, int> ESIMAT;

class TaoMatESI: public TaoMat{

 protected:


 public:

  esi::Operator<double, int> *esiobj;
  //  esi::Object *esiobj2;

  TaoMatESI( esi::Operator<double, int> * );

  ~TaoMatESI(){
    this->esiobj->deleteReference();
  }


  int Compatible(TaoVec*, TaoVec*, TaoTruth*);

  /** Get the dimension of the vector space */
  int GetDimensions( int*, int * );
  int Multiply(TaoVec*,TaoVec*);
  int MultiplyTranspose(TaoVec*,TaoVec*);

  int SetDiagonal(TaoVec*);
  int AddDiagonal(TaoVec*);
  int GetDiagonal(TaoVec*);
  int ShiftDiagonal(double);    

  int RowScale(TaoVec*);

  int Norm1(double *nm);
  int View();

};

#endif
