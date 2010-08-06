#ifndef QPGENTAOLINSYS
#define QPGENTAOLINSYS

#include "QpGenLinsys.h"
#include "SparseSymMatrixHandle.h"

class DoubleLinearSolver;

class QpGenTaoLinsys : public QpGenLinsys {
protected:
  SparseSymMatrixHandle Mat;
  DoubleLinearSolver * solver;
public:
  QpGenTaoLinsys(  QpGen * factory,
		QpGenData * data,
		LinearAlgebraPackage * la, SparseSymMatrix * Mat,
		DoubleLinearSolver * solver );
  virtual void solveCompressed( OoqpVector& rhs );

  virtual void putXDiagonal( OoqpVector& xdiag );
  virtual void putZDiagonal( OoqpVector& zdiag );
  virtual void factor(Data *prob, Variables *vars);
  
  virtual ~QpGenTaoLinsys();
};

#endif
