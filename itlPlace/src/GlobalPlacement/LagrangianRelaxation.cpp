#include "LagrangianRelaxation.h"
#include "LogSumExp.h"

extern timetype expTime;

double CalcSourceMu(double mu)
{
  int nArcs = 1;
  return mu * (double)nArcs;
}

double GetBraces(AppCtx* context, PetscScalar* solution, int netIdx)
{
  //FIXME: use correct values
  double sourceDrive = 1.0;
  double mu = 1.0;

  double A = context->alphaTWL + context->c * CalcSourceMu(mu) * sourceDrive;
  
  double termB = 0.0;
  int netSize = context->netListSizes[netIdx];
  for (int s = 1; s < netSize; s++)
  {
    double doi = CalcDistance2Sink(context, solution, netIdx, s);

    double C = context->r * context->c * mu; //FIXME: use correct values
    termB += C * doi;
  }
  termB *= 0.5;

  return A + termB;
}

double GetGreenTerm(AppCtx* context, PetscScalar* solution, int netIdx)
{
  double termG = 0.0;
  int netSize = context->netListSizes[netIdx];
  for (int s = 1; s < netSize; s++)
  {
    //FIXME: use correct values
    double sinkLoad = 1.0;
    double mu = 1.0;

    double doi = CalcDistance2Sink(context, solution, netIdx, s);

    double B = context->r * mu * sinkLoad; //FIXME: use correct values
    termG += B * doi;
  }

  return termG;
}

double LR(AppCtx* context, PetscScalar* solution)
{
  double LRValue = 0.0;

  int netListSize = static_cast<int>(context->ci->netList.size());
  for (int netIdx = 0; netIdx < netListSize; ++netIdx)
  {
    double braces = GetBraces(context, solution, netIdx);
    double LSE = CalcNetLSE(context, solution, netIdx);
    double greenTerm = GetGreenTerm(context, solution, netIdx);

    LRValue += braces * LSE + greenTerm;
    //ALERTFORMAT(("%d LRValue =\t%f", netIdx, LRValue));
  }

  return LRValue;
}

void AddLRGradient(AppCtx* context, int nCoordinates, PetscScalar* coordinates, PetscScalar* g)
{
  for (int idxInSolutionVector = 0; idxInSolutionVector < nCoordinates; idxInSolutionVector++)
  {
    int clusterIdx = context->solutionIdx2clusterIdxLUT[idxInSolutionVector];
    int tableSize = static_cast<int>(context->ci->tableOfAdjacentNets[clusterIdx].size());
    //consider all adjacent nets
    for (int j = 0; j < tableSize; ++j)
    {
      g[idxInSolutionVector] += CalcNetLSEGradient(context, 
        context->ci->tableOfAdjacentNets[clusterIdx][j],
        idxInSolutionVector);
    }
  }
}

void LR_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double*& f)
{
  PrecalcExponents(context, solution);

  *f += LR(context, solution);

  int nClusterVariables = 2 * context->ci->mCurrentNumberOfClusters;
  AddLRGradient(context, nClusterVariables, solution, context->gLR);
}