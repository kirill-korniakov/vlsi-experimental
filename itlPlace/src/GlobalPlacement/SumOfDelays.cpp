#include "SumOfDelays.h"
#include "LogSumExp.h"
///!!!ни где не используется
double GetNetKOpt(PetscScalar* coordinates, int nClusterCoordinates, HClusteredNet i, AppCtx* context)
{
  double kopt = (CalcNetLSE(context, coordinates, i) 
      + context->SODdata.netSODData[::ToID(i) - 1].Lnet - context->SODdata.Lbuf) / context->SODdata.Dbuf - 1;
  return kopt;
}

double GetNetDelay(PetscScalar* coordinates, int nClusterCoordinates, HClusteredNet i, AppCtx* context)
{
  double ki = 0.0;
  ki = coordinates[nClusterCoordinates + ::ToID(i) - 1]; //WARNING: this index is correct
  
  context->SODdata.netSODData[::ToID(i) - 1].braces = 
    CalcNetLSE(context, coordinates, i) + 
    context->SODdata.netSODData[::ToID(i) - 1].Lnet + 
    ki * context->SODdata.Lbuf;

  //NOTE: debugging variables
  double x = 0.0;
  double y = 0.0;

  x = context->SODdata.netSODData[::ToID(i) - 1].braces;
  x = x * x;
  x = x / (ki + 1);
  y = ki * context->SODdata.DbufLbufDifferenceOfSquares;

  return x + y;
}

double SumOfDelays(void* data, PetscScalar* coordinates)
{
  AppCtx* context = (AppCtx*)data;

  double sumOfDelays = 0.0;
  //int netListSize = static_cast<int>( context->ci->netList.size());
  int nClusterCoordinates = 2 * context->ci->mCurrentNumberOfClusters;
    for (HClusteredNets::ClusteredNetsEnumeratorW i = context->hd->ClustersNetList.GetEnumeratorW(); i.MoveNext(); )
  //for (int i = 0; i < netListSize; ++i)
  {
    sumOfDelays = sumOfDelays + GetNetDelay(coordinates, nClusterCoordinates, i, context);
  }

  return sumOfDelays;
}

void AddSumOfDelaysGradient(AppCtx* context, PetscScalar* coordinates, PetscScalar* gradient)
{
  int nAdjacentNets;
  HClusteredNet netIdx;
  int netListSize = static_cast<int>(context->hd->ClustersNetList.ClusteredNetCount());

  int nClusterCoordinates = 2*context->ci->mCurrentNumberOfClusters;

  for (int i = 0; i < nClusterCoordinates; i++)
  {
      HCluster clusterIdx = context->solutionIdx2clusterIdxLUT[i];
    nAdjacentNets = static_cast<int>(context->ci->design[clusterIdx].tableOfAdjacentNets()->size());

    for (int j = 0; j < nAdjacentNets; ++j)
    {//consider all adjacent nets
      netIdx = (*context->ci->design[clusterIdx].tableOfAdjacentNets())[j];
      gradient[i] += 2 * context->SODdata.netSODData[::ToID(netIdx) - 1].braces * 
        CalcNetLSEGradient(context, netIdx, i);
    }
  }

  double ki = 0.0;
    for (HClusteredNets::ClusteredNetsEnumeratorW i = context->hd->ClustersNetList.GetEnumeratorW(); i.MoveNext(); )
  //for (int i = nClusterCoordinates; i < nClusterCoordinates + netListSize; i++)
  {
    ki = coordinates[::ToID(i) - 1 + nClusterCoordinates]; //NOTE: this index is correct
    netIdx = i;// - nClusterCoordinates;
    gradient[::ToID(i) - 1 + nClusterCoordinates] = context->SODdata.DbufLbufDifferenceOfSquares + 
      (2 * context->SODdata.netSODData[::ToID(netIdx) - 1].braces * context->SODdata.Lbuf * (ki + 1) - 
      context->SODdata.netSODData[::ToID(netIdx) - 1].braces * context->SODdata.netSODData[::ToID(netIdx) - 1].braces) / 
      ((ki + 1) * (ki + 1));
  }
}

void SOD_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution)
{
  context->criteriaValues.sod = SumOfDelays(context, solution);;
  AddSumOfDelaysGradient(context, solution, context->criteriaValues.gSOD);
}