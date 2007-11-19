#ifndef TAOPETSCMULTIMAT_H
#define TAOPETSCMULTIMAT_H

#include "../../matrix/taomat_petsc.h"
#include "../interface/dagridctx.h"

#define PETSCDAAPPMAXGRIDS 20

class TaoMultiGridMatPetsc: public TaoMatPetsc{

 public:

  int ndamax;        /* Max number of levels */
  int nda;           /* Number of levels in current application */

  //  TaoMultiGridMatPetsc( GridCtx*,int );
  TaoMultiGridMatPetsc( Mat );

  ~TaoMultiGridMatPetsc();

  GridCtx grid[PETSCDAAPPMAXGRIDS];

  int SetUpMultiGrid(GridCtx*,int );
  int TakeDown();

  virtual int SetDiagonal(TaoVec*);
  virtual int AddDiagonal(TaoVec*);
  virtual int ShiftDiagonal(double);    

  virtual int RowScale(TaoVec*);
  virtual int ColScale(TaoVec*);

  virtual int CreateReducedMatrix(TaoIndexSet*,TaoIndexSet*,TaoMat**);
  virtual int SetReducedMatrix(TaoMat*,TaoIndexSet*,TaoIndexSet*);

};

#endif

