#include "private/matimpl.h"
#include "private/vecimpl.h"

typedef struct{

  Mat A;
  Vec VC; /* Work vector in the column space */
  Vec VR; /* Work vector in the row space */
  //  IS Row, Col;
  IS RowComplement, ColComplement;

} _p_MatSubMatFreeCtx;

typedef  _p_MatSubMatFreeCtx* MatSubMatFreeCtx;

int MatCreateSubMatrixFree(Mat,IS,IS,Mat*);
int MatSMFResetRowColumn(Mat,IS,IS);
int MatMult_SMF(Mat,Vec,Vec);
int MatMultTranspose_SMF(Mat,Vec,Vec);
int MatDiagonalSet_SMF(Mat,Vec,InsertMode);
int MatDestroy_SMF(Mat);
int MatView_SMF(Mat,PetscViewer);
int MatShift_SMF(Mat,PetscScalar);
int MatDuplicate_SMF(Mat,MatDuplicateOption,Mat*);
int MatEqual_SMF(Mat,Mat,PetscTruth*);
int MatScale_SMF(Mat,PetscScalar);
int MatGetSubMatrix_SMF(Mat,IS,IS,int,MatReuse,Mat *);
int MatGetSubMatrices_SMF(Mat,int,IS*,IS*,MatReuse,Mat**);
int MatTranspose_SMF(Mat,Mat*);
int MatGetDiagonal_SMF(Mat,Vec);
int MatGetColumnVector_SMF(Mat,Vec, int);
int MatNorm_SMF(Mat,NormType,PetscReal *);
int MatGetRowMax_SMF(Mat, Vec);
int MatGetRow_SMF(Mat,int,int *,const int **,const PetscScalar **);
int MatRestoreRow_SMF(Mat,int,int *,const int **,const PetscScalar **);
