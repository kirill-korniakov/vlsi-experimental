// A wrapper class for the SIDL-created tao application class
#ifndef TAOAPP_SIDL_H
#define TAOAPP_SIDL_H

#include "Optimize_OptimizationModel.hxx"
#include "LinearAlgebra_Factory.hxx"
class TaoSIDLApplication : public TaoApplication {

 protected:
  int RegisterEvents();
  int Tao_ObjectiveEval;
  int Tao_GradientEval;
  int Tao_HessianEval;
  int Tao_ObjectiveAndGradientEval;
  int Tao_Monitor;
  int Tao_Convergence;

 public:
  TaoSIDLApplication(Optimize::OptimizationModel m);
  TaoSIDLApplication(Optimize::OptimizationModel m, LinearAlgebra::Factory factory);
  ~TaoSIDLApplication();

  // interface we are wrapping
  Optimize::OptimizationModel model;

  LinearAlgebra::Factory LAFactory;

  //Bookkeeping
  int setup; //boolean -- not used yet
  TAO_SOLVER tao; //used for setting convergence


  // Variable vector
  int n;

  
  TaoVec *taox, *taoxl, *taoxu;

  TaoMat *taoH;
  KSP ksptmp;
  virtual int GetVariableVector(TaoVec **xx);
  virtual int InitializeVariables(TaoVec *xx);

  // Function evaluation
  virtual int EvaluateObjectiveFunction(TaoVec *xx, double *ff);

  // Gradient Evaluation
  virtual int EvaluateGradient(TaoVec *, TaoVec *);
  
  // Function and Gradient
  virtual int EvaluateObjectiveAndGradient(TaoVec *, double *, TaoVec *);

  // Hessian evaluation

  virtual int GetHessianMatrix(TaoMat **);
  virtual int EvaluateHessian(TaoVec *, TaoMat *);

  // Action on Approximate Hessian 
  virtual int HessianSolve(TaoVec *, TaoVec *, TaoTruth*);
  TaoTruth useh2;

  // Variable bounds
  virtual int EvaluateVariableBounds(TaoVec *, TaoVec *);
  
  virtual int GetLinearSolver(TaoMat *H, int flag, TaoLinearSolver **tksp);

  // jacobian

  // constraints

  // monitor
  virtual int Monitor();


  int setTao(TAO_SOLVER);

};


#endif
