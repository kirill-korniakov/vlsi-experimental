#include "SumOfDelays.h"
#include "LogSumExp.h"

extern timetype expTime;

double GetNetKOpt(PetscScalar* coordinates, int nClusterCoordinates, int i, AppCtx* context)
{
  double kopt = (CalcNetLSE(context, coordinates, i) + context->ci->netList[i].Lnet - context->Lbuf) / context->Dbuf - 1;
  return kopt;
}

double GetNetDelay(PetscScalar* coordinates, int nClusterCoordinates, int i, AppCtx* context)
{
  double ki = 0.0;
  ki = coordinates[nClusterCoordinates + i]; //WARNING: this index is correct
  
  context->ci->netList[i].braces = 
    CalcNetLSE(context, coordinates, i) + 
    context->ci->netList[i].Lnet + 
    ki * context->Lbuf;

  //NOTE: debugging variables
  double x = 0.0;
  double y = 0.0;

  x = context->ci->netList[i].braces;
  x = x * x;
  x = x / (ki + 1);
  y = ki * context->DbufLbufDifferenceOfSquares;

  return x + y;
}

double SumOfDelays(void* data, PetscScalar* coordinates)
{
  AppCtx* context = (AppCtx*)data;

  double sumOfDelays = 0.0;
  int netListSize = static_cast<int>(context->ci->netList.size());
  int nClusterCoordinates = 2 * context->ci->mCurrentNumberOfClusters;

  for (int i = 0; i < netListSize; ++i)
  {
    sumOfDelays = sumOfDelays + GetNetDelay(coordinates, nClusterCoordinates, i, context);
  }

  return sumOfDelays;
}

void AddSumOfDelaysGradient(AppCtx* context, PetscScalar* coordinates, PetscScalar* gradient)
{
  int nAdjacentNets;
  int netIdx;
  int netListSize = static_cast<int>(context->ci->netList.size());

  int nClusterCoordinates = 2*context->ci->mCurrentNumberOfClusters;

  for (int i = 0; i < nClusterCoordinates; i++)
  {
    int clusterIdx = context->solutionIdx2clusterIdxLUT[i];
    nAdjacentNets = static_cast<int>(context->ci->tableOfAdjacentNets[clusterIdx].size());

    for (int j = 0; j < nAdjacentNets; ++j)
    {//consider all adjacent nets
      netIdx = context->ci->tableOfAdjacentNets[clusterIdx][j];
      gradient[i] += 2 * context->ci->netList[netIdx].braces * 
        CalcNetLSEGradient(context, netIdx, i);
    }
  }

  double ki = 0.0;

  for (int i = nClusterCoordinates; i < nClusterCoordinates + netListSize; i++)
  {
    ki = coordinates[i]; //WARNING: this index is correct
    netIdx = i - nClusterCoordinates;
    gradient[i] = context->DbufLbufDifferenceOfSquares + 
      (2 * context->ci->netList[netIdx].braces * context->Lbuf * (ki + 1) - 
      context->ci->netList[netIdx].braces * context->ci->netList[netIdx].braces) / 
      ((ki + 1) * (ki + 1));
  }
}

void SOD_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution, double*& f)
{
  timetype start;
  timetype finish;

  start = GET_TIME_METHOD();
  PrecalcExponents(context, solution);
  finish = GET_TIME_METHOD();
  expTime += finish - start;

  *f += SumOfDelays(context, solution);;
  //ALERTFORMAT(("SOD = %f", addValue));
  AddSumOfDelaysGradient(context, solution, context->gSOD);
}