#include "LogSumExp.h"
//#include "mkl_vml.h"

extern timetype expTime;
extern timetype lseTime;
extern timetype lseGradTime;

int GetPrecalcedExponentsIdx(AppCtx* context, HCluster clusterIdx)
{
    HClusterWrapper cw = (*context->hd)[clusterIdx];
  if (!cw.IsTerminals() && !cw.IsPrimary())
  {
      return cw.clusterIdx2solutionIdxLUT();
  }
  else if (cw.IsTerminals())
  {
    return context->ci->mCurrentNumberOfClusters + (*context->hd)[clusterIdx].id();
  }
  else
  {
    return context->ci->mCurrentNumberOfClusters + (*context->hd)[clusterIdx].id() + context->ci->terminalCells.size();
  }
}

void GetClusterCoordinates(HCluster clusterIdx, PetscScalar* coordinates, 
                           AppCtx* context, double& x, double& y)
{
  HClusterWrapper cw = (*context->hd)[clusterIdx];
  if (cw.IsTerminals())
  {
    HCell terminal = context->ci->terminalCells[cw.id()];
    x = context->hd->GetDouble<HCell::X>(terminal);
    y = context->hd->GetDouble<HCell::Y>(terminal);
  }
  else
  if (cw.IsPrimary())
  {
    HPin pin = context->ci->primaryPins[cw.id()];
    x = context->hd->GetDouble<HPin::X>(pin);
    y = context->hd->GetDouble<HPin::Y>(pin);
  }
  else
  {
      int clusterIdxInCoordinatesArray = cw.clusterIdx2solutionIdxLUT();
      CRITICAL_ASSERT(clusterIdxInCoordinatesArray != -1);
      x = coordinates[2*clusterIdxInCoordinatesArray+0];
      y = coordinates[2*clusterIdxInCoordinatesArray+1];
  }
}

double CalcNetLSE(AppCtx* context, PetscScalar* coordinates, HClusteredNet netIdx)
{
  double sumExp1 = 0.0;
  double sumExp2 = 0.0;
  double sumExp3 = 0.0;
  double sumExp4 = 0.0;

  int precalcedExponentsIdx;
  HCluster realClusterIdx;

  //int netSize = context->netListSizes[netIdx];

  HClusteredNetWrapper net = (*context->hd)[netIdx];  

  for (int j = 0; j < net.clusterIdxs()->size(); ++j)
  {
    realClusterIdx = (*net.clusterIdxs())[j];
    precalcedExponentsIdx = GetPrecalcedExponentsIdx(context, realClusterIdx);

    sumExp1 += context->LSEdata.precalcedExponents[4*precalcedExponentsIdx + 0];
    sumExp2 += context->LSEdata.precalcedExponents[4*precalcedExponentsIdx + 1];
    sumExp3 += context->LSEdata.precalcedExponents[4*precalcedExponentsIdx + 2];
    sumExp4 += context->LSEdata.precalcedExponents[4*precalcedExponentsIdx + 3];
  }
  context->LSEdata.SUM1[::ToID(netIdx) - 1] = sumExp1;
  context->LSEdata.SUM2[::ToID(netIdx) - 1] = sumExp2;
  context->LSEdata.SUM3[::ToID(netIdx) - 1] = sumExp3;
  context->LSEdata.SUM4[::ToID(netIdx) - 1] = sumExp4;

  return context->LSEdata.alpha * log(sumExp1 * sumExp2 * sumExp3 * sumExp4);
}

double CalcNetLSEGradient(AppCtx* context, HClusteredNet netIdx, int idxInSolutionVector)
{
  double sum1 = 0.0;
  double sum2 = 0.0;

  if (idxInSolutionVector % 2 == 0)
  {
    sum1 = context->LSEdata.SUM1[::ToID(netIdx) - 1];
    sum2 = context->LSEdata.SUM2[::ToID(netIdx) - 1];
  }
  else
  {
    sum1 = context->LSEdata.SUM3[::ToID(netIdx) - 1];
    sum2 = context->LSEdata.SUM4[::ToID(netIdx) - 1];
  }

  //TODO [issue 131]: we need alpha here, but parameters tuning will follow
  return /*context->alpha * */
    (context->LSEdata.precalcedExponents[2*idxInSolutionVector + 0] / sum1 - 
    context->LSEdata.precalcedExponents[2*idxInSolutionVector + 1] / sum2) * 
    (*context->hd)[netIdx].weight();
}

void PerformArgsAssignment(AppCtx* context, PetscScalar* coordinates, double* arguments)
{
  int i;
  int nClusters    = context->ci->mCurrentNumberOfClusters;
  int nTerminals   = context->ci->terminalCells.size();
  int nPrimaryPins = context->ci->primaryPins.size();
  HCluster realClusterIdx;
  double x = 0.0;
  double y = 0.0;
  double alpha = context->LSEdata.alpha;

  i = 0;
  //for (HClusters::ClustersEnumeratorW ue = context->hd->Cluster.GetEnumeratorW(); ue.MoveNext();)
  for (i = 0; i < nClusters; ++i)
  {
      realClusterIdx = context->solutionIdx2clusterIdxLUT[2 * i];
    GetClusterCoordinates(realClusterIdx, coordinates, context, x, y);
    arguments[4*i + 0] = (+x / alpha);
    arguments[4*i + 1] = (-x / alpha);
    arguments[4*i + 2] = (+y / alpha);
    arguments[4*i + 3] = (-y / alpha);    
  }

  i = 0;
  int j = 0;
  for (HClusters::ClustersUnrealEnumeratorW ue = context->hd->Cluster.GetUnrealEnumeratorW(); ue.MoveNext();)
  //for (i = 0; i < nTerminals; ++i)
  {
      if (ue.IsTerminals())
      {
        GetClusterCoordinates(ue, coordinates, context, x, y);
        arguments[4*(i + nClusters) + 0] = (+x / alpha);
        arguments[4*(i + nClusters) + 1] = (-x / alpha);
        arguments[4*(i + nClusters) + 2] = (+y / alpha);
        arguments[4*(i + nClusters) + 3] = (-y / alpha);
        i++;
      }
      if (ue.IsPrimary())
      {
          GetClusterCoordinates(ue, coordinates, context, x, y);
          arguments[4*(j + nClusters + nTerminals) + 0] = (+x / alpha);
          arguments[4*(j + nClusters + nTerminals) + 1] = (-x / alpha);
          arguments[4*(j + nClusters + nTerminals) + 2] = (+y / alpha);
          arguments[4*(j + nClusters + nTerminals) + 3] = (-y / alpha);
          j++;
      }
  }

  /*for (i = 0; i < nPrimaryPins; ++i)
  {
    GetClusterCoordinates(i + SHIFT_NUMBER_FOR_PRIMARY_PINS, coordinates, context, x, y);
    arguments[4*(i + nClusters + nTerminals) + 0] = (+x / alpha);
    arguments[4*(i + nClusters + nTerminals) + 1] = (-x / alpha);
    arguments[4*(i + nClusters + nTerminals) + 2] = (+y / alpha);
    arguments[4*(i + nClusters + nTerminals) + 3] = (-y / alpha);
  }*/
}

void PrecalcExponents(AppCtx* context, PetscScalar* coordinates)
{
  int i;
  int nClusters    = context->ci->mCurrentNumberOfClusters;
  int nTerminals   = context->ci->terminalCells.size();
  int nPrimaryPins = context->ci->primaryPins.size();

  int count = (nClusters + nTerminals + nPrimaryPins) * 4; 
  double* arguments = context->LSEdata.argsForPrecalcedExponents;

  PerformArgsAssignment(context, coordinates, arguments);

#ifdef __MKL_VML_H__
  int batchSize = context->LSEdata.batchSize;
  for (i = 0; i < count / batchSize; ++i)
    vdExp(batchSize, &arguments[i * batchSize], &context->LSEdata.precalcedExponents[i * batchSize]);
  vdExp(count % batchSize, &arguments[count - count % batchSize], 
    &context->LSEdata.precalcedExponents[count - count % batchSize]);
#else
  for (i = 0; i < count; ++i)
    context->LSEdata.precalcedExponents[i] = exp(arguments[i]);
#endif
}

double LogSumExp(void* data, PetscScalar* coordinates)
{
  AppCtx* context = (AppCtx*)data;

  double logSumExp = 0.0;
  int netListSize = static_cast<int>(context->hd->ClustersNetList.ClusteredNetCount());

  for (HClusteredNets::ClusteredNetsEnumeratorW i = context->hd->ClustersNetList.GetEnumeratorW(); i.MoveNext(); )
  //for (int i = 0; i < netListSize; ++i)
  {
    logSumExp += CalcNetLSE(context, coordinates, i) * i.weight();
    //ALERT("%d logSumExp =\t%f", i, logSumExp));
  }

  return logSumExp;
}

void AddLogSumExpGradient(AppCtx* context, int nCoordinates, PetscScalar* coordinates, PetscScalar* grad)
{
  //#pragma omp parallel for
  for (int idxInSolutionVector = 0; idxInSolutionVector < nCoordinates; idxInSolutionVector++)
  {
      HCluster clusterIdx = context->solutionIdx2clusterIdxLUT[idxInSolutionVector];
    int tableSize = static_cast<int>(context->ci->design[clusterIdx].tableOfAdjacentNets()->size());
    //consider all adjacent nets
    for (int j = 0; j < tableSize; ++j)
    {
      grad[idxInSolutionVector] += CalcNetLSEGradient(context, 
        (*context->ci->design[clusterIdx].tableOfAdjacentNets())[j],
        //context->ci->tableOfAdjacentNets[clusterIdx][j],
        idxInSolutionVector);
    }
  }
}

void LSE_AddObjectiveAndGradient(AppCtx* context, PetscScalar* solution)
{
  timetype start;
  timetype finish;

  start = GET_TIME_METHOD();
  context->criteriaValues.lse = LogSumExp(context, solution);
  //ALERT("LSE = %f", addValue));
  finish = GET_TIME_METHOD();
  lseTime += finish - start;

  start = GET_TIME_METHOD();
  int nClusterVariables = 2 * context->ci->mCurrentNumberOfClusters;
  AddLogSumExpGradient(context, nClusterVariables, solution, context->criteriaValues.gLSE);
  finish = GET_TIME_METHOD();
  lseGradTime += finish - start;
}