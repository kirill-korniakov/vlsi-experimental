#ifndef QPGENTAO_H
#define QPGENTAO_H

#include "QpGen.h"

class TaoVec;
class TaoMat;
class PetscVector;

class QpGenTao : public QpGen {
  int nnzQ, nnzA, nnzC;
public:
  QpGenTao( int nx, int my, int mz,
  		     int nnzQ, int nnzA, int nnzC );
  virtual LinearSystem * makeLinsys( Data * prob_in );

  virtual void joinRHS( OoqpVector& rhs,  OoqpVector& rhs1,
			OoqpVector& rhs2, OoqpVector& rhs3 );
  virtual void separateVars( OoqpVector& vars1, OoqpVector& vars2,
			     OoqpVector& vars3, OoqpVector& vars );

  //virtual Data          * makeData();
  virtual Data * makeData( TaoVec * c,     TaoMat * Q,
			   TaoVec * xlow,  TaoVec * xupp,  
			   TaoMat * A,     TaoVec * b,
			   TaoMat * C,     TaoVec * clow,  TaoVec * cupp );

  virtual Variables     *makeVariables( Data * problem, 
					TaoVec * x,
					TaoVec * y,
					TaoVec * z,
					TaoVec * gamma,
					TaoVec * phi );

};

PetscVector * TaoVecAsOoqpVec( TaoVec * v );

#endif
