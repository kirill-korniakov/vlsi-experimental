#ifndef TAOESIS_H
#define TAOESIS_H

#include <string.h>
#include <iostream.h>
#include "taois.h"
#include "tao_general.h"

class TaoESIIndexSet: public TaoIndexSet{

protected:

  int nlocalmax;
  int nlocal;
  int n;
  int *iptr;

public:

  TaoESIIndexSet(int);
  TaoESIIndexSet(int, int);

  ~TaoIndexSetPetsc(){
    delete iptr; 
  };

  int duplicate(TaoIndexSet**);

  int isSame(TaoIndexSet*, TaoTruth*);

  /** Get the dimension of the vector spaces */
  int whichEqual(TaoVec*,TaoVec*);

  int whichLessThan(TaoVec*,TaoVec*);

  int whichGreaterThan(TaoVec*,TaoVec*);

  int whichBetween(TaoVec*,TaoVec*,TaoVec*);

  int getSize(int *);

};

#endif
