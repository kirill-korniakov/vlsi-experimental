#include "HDesign.h"
#include "AnalyticalPlacement.h"

using namespace AnalyticalGlobalPlacement;

double AnalyticalGlobalPlacement::CalculateSumOfK(HDesign& hd, ClusteringInformation& ci)
{
  double s = 0.0;
  //int netListSize = static_cast<int>(ci.netList.size());
  for (HClusteredNets::ClusteredNetsEnumeratorW i = hd.ClustersNetList.GetEnumeratorW(); i.MoveNext(); )
  //for (int i = 0; i < netListSize; i++)
  {
    s += i.k();
  }
  return s;
}

void AnalyticalGlobalPlacement::GetKValues(ClusteringInformation& ci, Vec x)
{
  int nK = ci.design.ClustersNetList.ClusteredNetCount();
  PetscScalar* values = new PetscScalar[nK];
  int* idxs = InitIdxs(nK, 2*ci.mCurrentNumberOfClusters);

  VecGetValues(x, PetscInt(nK), idxs, values);
  int j = 0;
    for (HClusteredNets::ClusteredNetsEnumeratorW i = ci.design.ClustersNetList.GetEnumeratorW(); i.MoveNext(); j++)
  //for (int i = 0; i < nK; i++)
  {
    i.Setk(values[j]);
  }

  delete[] values;
  delete[] idxs;
}

void AnalyticalGlobalPlacement::SetKValues(ClusteringInformation& ci, Vec& x)
{
  int nK = ci.design.ClustersNetList.ClusteredNetCount();

  PetscScalar* initValues = new PetscScalar[nK];
  int* idxs = InitIdxs(nK, 2*ci.mCurrentNumberOfClusters);
  //  for (HClusteredNets::ClusteredNetsEnumeratorW i = ci.design.ClustersNetList.GetEnumeratorW(); i.MoveNext(); j++)
  for (int i = 0; i < nK; i++)
  {
    initValues[i] = 0.0;
  }
  VecSetValues(x, nK, idxs, initValues, INSERT_VALUES);

  delete[] initValues;
  delete[] idxs;
}