#ifndef LMVMMAT_H
#define LMVMMAT_H

#include "taomat.h"

class TaoLMVMMat: public TaoMat {

 protected:
  int lm;
  double eps;

  int limitType;	// Limit type on change in scaling matrix
  int scaleType;	// Scaling type (scalar or broyden)
  int rScaleType;	// Additional rescaling to use for diagonal

  double s_alpha;	// Factor for scalar scaling
  double r_alpha;	// Factor on scalar for rescaling diagonal matrix
  double r_beta;	// Factor on diagonal for rescaling diagonal matrix
  double mu;		// Factor for using historical information
  double nu;		// Factor for using historical information
  double phi;		// Factor for Broyden scaling

  int scalar_history;	// Amount of history to keep for scalar scaling
  double *yy_history;	// Past information for scalar scaling
  double *ys_history;	// Past information for scalar scaling
  double *ss_history;	// Past information for scalar scaling

  int rescale_history;  // Amount of history to keep for rescaling diagonal
  double *yy_rhistory;	// Past information for scalar rescaling
  double *ys_rhistory;	// Past information for scalar rescaling
  double *ss_rhistory;	// Past information for scalar rescaling

  double delta_max;	// Maximum value for delta
  double delta_min;	// Minimum value for delta

  int lmnow;
  int iter;
  int updates;
  int rejects;

  TaoVec **S;
  TaoVec **Y;
  TaoVec *Gprev;
  TaoVec *Xprev;

  TaoVec *D;
  TaoVec *U;
  TaoVec *V;
  TaoVec *W;
  TaoVec *P;
  TaoVec *Q;

  double delta;
  double sigma;
  double *rho;
  double *beta;

  TaoTruth H0default;
  TaoMat *H0;

  TaoVec *scale;

 public:
  
  TaoLMVMMat(TaoVec *);
  ~TaoLMVMMat();

  int Reset();
  int Presolve();
  int Solve(TaoVec *, TaoVec *, TaoTruth *);
  int Update(TaoVec *, TaoVec *);

  int View();

  int SetDelta(double);
  int SetScale(TaoVec *s);

  inline int GetRejects() { return(rejects); }

  int SetH0(TaoMat *HH0);
  int GetX0(TaoVec *x);
  int InitialApproximation(TaoVec *x);

  int Refine(TaoLMVMMat *coarse, TaoMat *op, TaoVec *fineX, TaoVec *fineG);
};

#endif

