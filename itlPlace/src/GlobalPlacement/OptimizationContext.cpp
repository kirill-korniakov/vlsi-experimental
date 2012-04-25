#include "OptimizationContext.h"
#include "Spreading.h"

void AppCtx::Initialize(HDesign& ahd, ClusteringInformation& aci)
{
  hd = &ahd;
  ci = &aci;

  int j = 0;
  solutionIdx2clusterIdxLUT = new HCluster[2*CalculateNumberOfActiveClusters(aci)]; 
  //clusterIdx2solutionIdxLUT = new int[static_cast<int>(ci->design.Cluster.ClustersCount())]; 
  int i = 0;
  for (HClusters::ClustersEnumeratorW en = ci->design.Cluster.GetEnumeratorW(); en.MoveNext(); i++)
  //for(int i = 0; i < static_cast<int>(ci->clusters.size()); i++)
  {
    if (!en.IsFake())
    {
        en.SetclusterIdx2solutionIdxLUT(j);
        //clusterIdx2solutionIdxLUT[::ToID(en) - 1] = j;
      solutionIdx2clusterIdxLUT[2*j+0] = en;
      solutionIdx2clusterIdxLUT[2*j+1] = en;
      ++j;
    }
    else
        en.SetclusterIdx2solutionIdxLUT(-1);
      //clusterIdx2solutionIdxLUT[::ToID(en) - 1] = -1;
  }
  //x,y,ki
  nVariables = 2 * ci->mCurrentNumberOfClusters + ci->design.ClustersNetList.ClusteredNetCount();

  criteriaValues.gLSE = new double[4*nVariables];
  criteriaValues.gSOD = criteriaValues.gLSE + nVariables;
  criteriaValues.gLR  = criteriaValues.gSOD + nVariables;
  criteriaValues.gQS  = criteriaValues.gLR  + nVariables;

  ConstructBinGrid(ahd, *this, 
    ahd.cfg.ValueOf("Clustering.desiredNumberOfClustersAtEveryBin", 10));

  //netListSizes = new int[ci->netList.size()];
  //CalculateNetListSizes(ci->netList, netListSizes);
  
  useLogSumExp          = ahd.cfg.ValueOf(".useLogSumExp", false);
  useSumOfDelays        = ahd.cfg.ValueOf(".useSumOfDelays", false);
  useLR                 = ahd.cfg.ValueOf(".useLR", false);
  useSpreading = ahd.cfg.ValueOf(".useSpreading", false);
  useBorderBounds       = ahd.cfg.ValueOf(".useBorderBounds", true);

  if (useLogSumExp || useSumOfDelays || useLR)
    LSEdata.Initialize(ahd, aci, sprData.binGrid.binWidth);
  if (useSumOfDelays)
    SODdata.Initialize(ahd);
  if (useLR)
    LRdata.Initialize(ahd);

  sprData.useUnidirectSpreading = ahd.cfg.ValueOf(".useUnidirectSpreading", false);
}

void AppCtx::FreeMemory()
{
  for (int j = 0; j < sprData.binGrid.nBinRows; ++j)
  {
    delete [] sprData.binGrid.bins[j];
  }
  delete [] sprData.binGrid.bins;

  int maxAffectedRows = CalcMaxAffectedArea(sprData.potentialRadiusY, 
    sprData.binGrid.binHeight);
  for (int i = 0; i < maxAffectedRows; ++i)
  {
    delete [] sprData.clusterPotentialOverBins[i];
  }
  delete [] sprData.clusterPotentialOverBins;

  //TODO: check correctness of memory deletion
  delete[] LSEdata.precalcedExponents;
  //delete[] netListSizes;
  delete[] solutionIdx2clusterIdxLUT;
  delete[] criteriaValues.gLSE;
}