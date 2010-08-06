#ifndef TAOKERNEL_H
#define TAOKERNEL_H

#include "tao_general.h"
/*

THIS IS NOT USED FOR ANYTHING RIGHT NOW.

IT DOES, HOWEVER, LIST THE ROUTINES NEEDED BY
THE TAO SOLVERS OTHER THAN NUMERICAL OPERATIONS.

USUALLY THESE ROUTINES WOULD BE DONE WITH PETSC

*/

class TaoKernel {

protected:
  
public:

  virtual ~TaoKernel(){};

  virtual int TaoNew(int, void*);
  virtual int TaoFree(void*);
  virtual int TaoMalloc(int, void*);

  virtual int TaoPrintf(const char *str);
  virtual int TaoPrintfDouble(const char *str,double);
  virtual int TaoPrintfInt(const char *str,double);
  virtual int TaoPrintfString(const char *str,const char *st);
  virtual int TaoPrintfMessage(const char *str);

  virtual int TaoLogInfo();

  virtual int TaoOptionsBegin();
  virtual int TaoOptionsList();
  virtual int TaoOptionsName();
  virtual int TaoOptionsDouble();
  virtual int TaoOptionsInt(const char *option, const char *help, const char *routine,int current, int* newvalue, TaoTruth*flg);
  virtual int TaoOptionsString();
  virtual int TaoOptionsHasName();
  virtual int TaoOptionsEnd();
  virtual int TaoOptionsHead();
  virtual int TaoOptionsTail();

  virtual int TaoStrcmp(const char *,const char *,PetscTruth*);

  virtual int TaoFListFind(MPI_Comm,PetscFList,const char*,int (**)(void *));
  virtual int TaoFListDestroy(PetscFList*);
  virtual int TaoFListConcat(const char*,const char* ,char* fullname);
  virtual int TaoFListAdd(PetscFList *,const char*,const char*,int (*)(void *));

};

#endif



