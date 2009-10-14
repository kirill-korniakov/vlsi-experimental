#ifndef TAOMATRIX_H
#define TAOMATRIX_H

#include "tao_basictypes.h"

class TaoIndexSet;
class TaoVec;

class TaoMat {

protected:

public:

  virtual ~TaoMat() { };

  virtual int Compatible(TaoVec *, TaoVec *, TaoTruth *);

  /** Get the dimension of the vector spaces */
  virtual int GetDimensions(int *, int *);

  virtual int Multiply(TaoVec *, TaoVec *);
  virtual int MultiplyTranspose(TaoVec *, TaoVec *);

  virtual int SetDiagonal(TaoVec *);
  virtual int AddDiagonal(TaoVec *);
  virtual int GetDiagonal(TaoVec *);
  virtual int ShiftDiagonal(double);

  virtual int RowScale(TaoVec *);
  virtual int ColScale(TaoVec *);

  virtual int Norm1(double *);

  virtual int D_Fischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *, 
  		        TaoVec *, TaoVec *, TaoVec *, TaoVec *);
  virtual int D_SFischer(TaoVec *, TaoVec *, TaoVec *, TaoVec *, double,
                         TaoVec *, TaoVec *, TaoVec *, TaoVec *, TaoVec *);

  virtual int CreateReducedMatrix(TaoIndexSet*,TaoIndexSet*,TaoMat**);
  virtual int SetReducedMatrix(TaoMat*,TaoIndexSet*,TaoIndexSet*);

  virtual int View();

  /* These two routines are not needed.  A linear solver may be used instead */
  virtual int Presolve();
  virtual int Solve(TaoVec *, TaoVec *, TaoTruth *);
};

#endif
