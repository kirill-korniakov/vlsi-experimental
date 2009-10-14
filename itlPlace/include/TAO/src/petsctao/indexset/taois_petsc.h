#ifndef TAOPETSCIS_H
#define TAOPETSCIS_H

#include "taois.h"        /*I  "tao.h"  I*/
#include "petscmat.h"

#include "../include/taopetsc.h"

class TaoIndexSetPetsc: public TaoIndexSet{

protected:

  IS isp;             /* The underlying IS repesentation of the IndexSet                */
  Vec VSpace;         /* Instance of a vector from the full space that the IS describes */
  int nlocal;         /* Local size of VSpace        */
  int *iptr;          /* Work array of length nlocal */

  IS ispComplement;   /* Complement of isp              */

  IS isp2;                     /* Parallel redistribution of isp                                 */
  TaoPetscISType reducedtype;  /* Mask full space or use reduced data structures                 */
  IS ISGathered;               /* All of isp collected onto each matrix                          */
  VecScatter scatter;          /* Scatter from full space to reduced space                       */

  PetscViewer ispviewer;       /* View the IS  */

  int SetUp(Vec, IS);
  int clearit();

public:

  TaoIndexSetPetsc(Vec, IS SS);
  TaoIndexSetPetsc(Vec);

  ~TaoIndexSetPetsc();

  /* Special to TaoIndexSetPetsc */
  int SetIS(IS);
  inline IS GetIS(){return isp;}

  int GetReducedType(TaoPetscISType *);
  int GetWholeIS(IS*);
  int RedistributeIS(IS*);
  int GetReducedVecScatter(Vec,Vec,VecScatter*);

  int GetMask(Vec *);
  int GetComplementIS(IS*);

  /* These virtual methods of the TaoIndexSet are defined */
  int UnionOf(TaoIndexSet *, TaoIndexSet*);
  int IntersectionOf(TaoIndexSet*, TaoIndexSet*);
  int ComplementOf(TaoIndexSet*);

  int Duplicate(TaoIndexSet**);

  int IsSame(TaoIndexSet*, TaoTruth*);

  int WhichEqual(TaoVec*,TaoVec*);

  int WhichLessThan(TaoVec*,TaoVec*);

  int WhichGreaterThan(TaoVec*,TaoVec*);

  int WhichBetween(TaoVec*,TaoVec*,TaoVec*);

  int WhichBetweenOrEqual(TaoVec *, TaoVec *, TaoVec *);

  int GetSize(int *);

  int View();

};

int ISCreateComplement(IS, Vec, IS *);
int MatCreateSubMatrixFree(Mat,IS,IS,Mat *);
int VecISSetToConstant(IS, PetscScalar, Vec);

#endif
