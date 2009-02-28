/*$Id$*/

// Context for a Newton trust region method (unconstrained minimization)

#ifndef __TAO_NTR_H
#define __TAO_NTR_H
#include "tao_solver.h"
#include "src/matrix/lmvmmat.h"

typedef struct {
  TaoLMVMMat *M;

  TaoVec *G;
  TaoVec *D;
  TaoVec *W;

  TaoVec *Diag;

  // Parameters when updating the trust-region radius based on reduction
  double eta1;          // used to compute trust-region radius
  double eta2;          // used to compute trust-region radius
  double eta3;          // used to compute trust-region radius
  double eta4;          // used to compute trust-region radius

  double alpha1;	// factor used for trust-region update
  double alpha2;	// factor used for trust-region update
  double alpha3;	// factor used for trust-region update
  double alpha4;	// factor used for trust-region update
  double alpha5;	// factor used for trust-region update

  // kappa = ared / pred
  // if   kappa < eta1          (very bad step)
  //   radius = alpha1 * min(norm(d), radius)
  // elif kappa < eta2          (bad step)
  //   radius = alpha2 * min(norm(d), radius)
  // elif kappa < eta3          (okay step)
  //   radius = alpha3 * radius;
  // elif kappa < eta4          (good step)
  //   radius = max(alpha4 * norm(d), radius)
  // else                       (very good step)
  //   radius = max(alpha5 * norm(d), radius)
  // fi

  // Parameters when updating the trust-region radius based on interpolation
  double mu1;		// used for model agreement in radius update
  double mu2;		// used for model agreement in radius update

  double gamma1;	// factor used for radius update
  double gamma2;	// factor used for radius update
  double gamma3;	// factor used for radius update
  double gamma4;	// factor used for radius update

  double theta;		// factor used for radius update

  // kappa = ared / pred
  // if   kappa >= 1.0 - mu1    (very good step)
  //   choose tau in [gamma3, gamma4]
  //   radius = max(tau * norm(d), radius)
  // elif kappa >= 1.0 - mu2    (good step)
  //   choose tau in [gamma2, gamma3]
  //   if (tau >= 1.0)
  //     radius = max(tau * norm(d), radius)
  //   else
  //     radius = tau * min(norm(d), radius)
  //   fi
  // else                       (bad step)
  //   choose tau in [gamma1, 1.0]
  //   radius = tau * min(norm(d), radius)
  // fi

  // Parameters when initializing trust-region radius based on interpolation
  double mu1_i;         // used for model agreement in interpolation
  double mu2_i;         // used for model agreement in interpolation

  double gamma1_i;      // factor used for interpolation
  double gamma2_i;      // factor used for interpolation
  double gamma3_i;      // factor used for interpolation
  double gamma4_i;      // factor used for interpolation

  double theta_i;       // factor used for interpolation

  double min_radius;	// lower bound on initial radius value
  double max_radius;	// upper bound on trust region radius
  double epsilon;	// tolerance used when computing actred/prered

  int ksp_type;         // KSP method for the code
  int pc_type;          // Preconditioner for the code
  int bfgs_scale_type;  // Scaling matrix for the bfgs preconditioner
  int init_type;	// Trust-region initialization method
  int update_type;	// Trust-region update method
} TAO_NTR;

#endif
