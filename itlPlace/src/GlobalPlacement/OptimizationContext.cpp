#include "OptimizationContext.h"
#include "Spreading.h"

void AppCtx::Initialize(HDesign& ahd, ClusteringInformation& aci)
{
  hd = &ahd;
  ci = &aci;

  int j = 0;
  solutionIdx2clusterIdxLUT = new int[2*CalculateNumberOfActiveClusters(aci) + static_cast<int>(ci->clusters.size())]; 
  clusterIdx2solutionIdxLUT = solutionIdx2clusterIdxLUT + 2*CalculateNumberOfActiveClusters(aci); 
  for(int i = 0; i < static_cast<int>(ci->clusters.size()); i++)
  {
    if (!ci->clusters[i].isFake)
    {
      clusterIdx2solutionIdxLUT[i] = j;
      solutionIdx2clusterIdxLUT[2*j+0] = i;
      solutionIdx2clusterIdxLUT[2*j+1] = i;
      ++j;
    }
    else
      clusterIdx2solutionIdxLUT[i] = -1;
  }
  //x,y,ki
  nVariables = 2 * ci->mCurrentNumberOfClusters + ci->netList.size();

  gLSE = new double[4*nVariables];
  gSOD = gLSE + nVariables;
  gLR  = gSOD + nVariables;
  gQS  = gLR  + nVariables;

  ConstructBinGrid(ahd, *this, 
    ahd.cfg.ValueOf("Clustering.desiredNumberOfClustersAtEveryBin", 10));

  netListSizes = new int[ci->netList.size()];
  CalculateNetListSizes(ci->netList, netListSizes);
  
  useNetWeights         = ahd.cfg.ValueOf("NetWeighting.useNetWeights", false);
  useLogSumExp          = ahd.cfg.ValueOf("GlobalPlacement.useLogSumExp", false);
  useSumOfDelays        = ahd.cfg.ValueOf("GlobalPlacement.useSumOfDelays", false);
  useLR                 = ahd.cfg.ValueOf("GlobalPlacement.useLR", false);
  useQuadraticSpreading = ahd.cfg.ValueOf("GlobalPlacement.useQuadraticSpreading", false);
  useUnidirectSpreading = ahd.cfg.ValueOf("GlobalPlacement.useUnidirectSpreading", false);
  useLRSpreading        = ahd.cfg.ValueOf("GlobalPlacement.useLRSpreading", false);
  useBorderBounds       = ahd.cfg.ValueOf("GlobalPlacement.useBorderBounds", true);

  if (useLogSumExp || useSumOfDelays || useLR)
    LSEdata.Initialize(ahd, aci, spreadingData.binGrid.binWidth);
  if (useSumOfDelays)
    SODdata.Initialize(ahd);
  if (useLR)
    LRdata.Initialize(ahd);
}

void AppCtx::FreeMemory()
{
  for (int j = 0; j < spreadingData.binGrid.nBinRows; ++j)
  {
    delete [] spreadingData.binGrid.bins[j];
  }
  delete [] spreadingData.binGrid.bins;

  int maxAffectedRows = CalcMaxAffectedArea(spreadingData.potentialRadiusY, 
    spreadingData.binGrid.binHeight);
  for (int i = 0; i < maxAffectedRows; ++i)
  {
    delete [] spreadingData.clusterPotentialOverBins[i];
  }
  delete [] spreadingData.clusterPotentialOverBins;

  if (useLRSpreading)
  {
    delete [] spreadingData.binsPenaltyValues;
    delete [] spreadingData.muBinsPen;
    delete [] spreadingData.individualBinsDiscrepancy;
  }

  //TODO: check correctness of memory deletion
  delete[] LSEdata.precalcedExponents;
  delete[] netListSizes;
  delete[] solutionIdx2clusterIdxLUT;
  delete[] gLSE;
}