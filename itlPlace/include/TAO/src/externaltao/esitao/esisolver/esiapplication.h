
#ifndef ESIPROBLEMOBJECT_H
#define ESIPROBLEMOBJECT_H


namespace esi {

  // class ESIApplication : public virtual Object{

class ESIApplication {
  
 public:
  virtual ~ESIApplication(){};

  virtual esi::ErrorCode evaluateObjectiveFunction(esi::Vector<double,int> *xx, double *ff)=0;
  virtual esi::ErrorCode evaluateGradient(esi::Vector<double,int> *xx, esi::Vector<double,int> *gg)=0;
  virtual esi::ErrorCode evaluateObjectiveAndGradient(esi::Vector<double,int> *xx, double *ff, esi::Vector<double,int> *gg)=0;
  virtual esi::ErrorCode evaluateHessian(esi::Vector<double,int> *xx, esi::Operator<double,int> *HH)=0;

  virtual esi::ErrorCode initializeVariables(esi::Vector<double,int> *x)=0;
  virtual esi::ErrorCode evaluateBounds(esi::Vector<double,int> *,esi::Vector<double,int> *)=0;

  virtual esi::ErrorCode getVariableVector(esi::Vector<double,int> **xx)=0;
  virtual esi::ErrorCode getHessianMatrix(esi::Operator<double,int> **HH)=0;
  virtual esi::ErrorCode monitor()=0;
  
  //  virtual esi::ErrorCode getLinearSolver(ESILinearSolver **tksp);

}; // esi::Vector class
}; // esi namespace




#endif



