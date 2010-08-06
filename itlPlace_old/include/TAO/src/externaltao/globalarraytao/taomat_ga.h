//File: taomat_ga.h
/**************************************************************

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

Purpose:
      to design and implement TaoMatrix using 
      global arrays.

Revise history:

8/8/02 
	To clean Petsc function calls and marcos.

**************************************************************/



#ifndef TAOGAMAT_H
#define TAOGAMAT_H

typedef int GAMat;

//ga header files
#include "taovec_ga.h" //to define tao/ga vector class
#include "ga.h"        //to define GA lib functions

//tao header files
#include "taolinearsolver.h"
#include "tao_solver.h"
#include "taomat.h"

class TaoMatGa:public TaoMat
{

protected:

  GAMat pm;
  GAMat pm_pre;

public:

    TaoMatGa (GAMat M);
   ~TaoMatGa ()
  {
    if (this->pm)
      GA_Destroy (this->pm);
  }

  inline GAMat GetMat ()
  {
    return this->pm;
  }

/*The user of this function is responsible to free the memory of "this" 
before resplace it with a different vector. i
Otherwise, there is a possible memory leak.
*/
  inline int replaceMat (GAMat M)
  {
    this->pm = M;
    return 0;
  }

  int Compatible (TaoVec *, TaoVec *, TaoTruth*);
  int Clone (TaoMat **);
  int CopyFrom (TaoMat *);

  int Fill (TaoScalar);
  int Zero ();
  int GetDimensions (int *, int *);

  int Multiply (TaoVec *, TaoVec *);
  int MultiplyAdd (TaoVec *, TaoVec *, TaoVec *);
  int MultiplyTranspose (TaoVec *, TaoVec *);
  int MultiplyTransposeAdd (TaoVec *, TaoVec *, TaoVec *);

  int SetDiagonal (TaoVec *);
  int AddDiagonal (TaoVec *);
  int GetDiagonal (TaoVec *);
  int ShiftDiagonal (double);

  int View ();

  /* These routines not implemented */
  int RowScale (TaoVec *);
  int ColScale (TaoVec *);

  int Presolve ();
  int Solve (TaoVec *, TaoVec *, TaoTruth *);

  int CreateReducedMatrix (TaoIndexSet *, TaoIndexSet *, TaoMat **);
  int SetReducedMatrix (TaoMat *, TaoIndexSet *, TaoIndexSet *);

  int D_Fischer (TaoVec *, TaoVec *, TaoVec *, TaoVec *,
		 TaoVec *, TaoVec *, TaoVec *, TaoVec *);

  int Norm1 (double *);

  friend class TaoGAApplication;

};

int TaoWrapGaMat (GAMat, TaoMatGa **);

#endif
