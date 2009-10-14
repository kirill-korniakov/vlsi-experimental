#ifndef TAOINDEXSET_H
#define TAOINDEXSET_H

#include "tao_basictypes.h"

/** 
    An abstract class representing the implementation of a TAO Index Set.
*/

class TaoVec;

class TaoIndexSet {

protected:
  
public:

  virtual ~TaoIndexSet(void){};

  virtual int UnionOf(TaoIndexSet *, TaoIndexSet*);
  virtual int IntersectionOf(TaoIndexSet*, TaoIndexSet*);
  virtual int ComplementOf(TaoIndexSet*);

  virtual int Duplicate(TaoIndexSet**);

  virtual int IsSame(TaoIndexSet*, TaoTruth*);

  /** Get the dimension of the vector spaces */
  virtual int WhichEqual(TaoVec*,TaoVec*);

  virtual int WhichLessThan(TaoVec*,TaoVec*);

  virtual int WhichGreaterThan(TaoVec*,TaoVec*);

  virtual int WhichBetween(TaoVec*,TaoVec*,TaoVec*);

  virtual int WhichBetweenOrEqual(TaoVec *,TaoVec *,TaoVec *);

  virtual int GetSize(int *);

  virtual int View();

};

#endif
