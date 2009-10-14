#ifndef PETSCDAGRIDCTX_H
#define PETSCDAGRIDCTX_H

#include "petscmat.h"
#include "petscda.h"

typedef struct {

  DA  da;           /* The Structure Grid on which this problem is defined          */
  Vec X;            /* The Variable Vector for this problem                         */
  Vec XL,XU;        /* Lower and upper bounds on the variables                      */
  Mat H;            /* The Hessian Matrix for this problem                          */
  Vec R;            /* Work vector                                                  */
  Vec RHS;          /* Work vector                                                  */
  Vec W3;           /* Work vector                                                  */
  Mat Interpolate;  /* The Interplation Matrix from a coarser grid to this grid     */
  Vec CScale;       /* The Scaling Vector needed to restrict the solution on this grid to a coarser grid */
  ISColoring coloring; /* Matrix Coloring for finite differences                    */
  int mgrid;        /* true if multigrid preconditioner setup */
  KSP ksp;          /* KSP object for this level */
} GridCtx;

#endif
