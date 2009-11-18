#include "HDesign.h"
#include "Clustering.h"
#include "OptimizationContext.h"

#include "tao.h"

namespace AnalyticalGlobalPlacement
{
  void ReportTimes();
  void ReportIterationInfo(ClusteringInformation& ci, AppCtx& user);
  void ReportBinGridInfo(AppCtx& context);

  void InitializeDataStructures(std::vector<Cluster>& clusters, NetList& netList);  
  int InitializeTAO(HDesign& hd, ClusteringInformation& ci, AppCtx &context, 
    Vec& x, Vec& xl, Vec& xu, 
    TAO_SOLVER& tao, TAO_APPLICATION& taoapp);
  
  int Relaxation(HDesign& hd, ClusteringInformation& ci);
  int Interpolation(HDesign& hd, ClusteringInformation& ci);
  
  int Solve(HDesign& hd, ClusteringInformation& ci, AppCtx& context, TAO_APPLICATION taoapp, 
    TAO_SOLVER tao, Vec x);

  int ReportTerminationReason( int retCode, TAO_SOLVER tao );

  void SetVariablesValues(ClusteringInformation& ci, Vec& x);
  void GetVariablesValues(ClusteringInformation& ci, Vec x);
  void PlaceToTheCenterIntially(HDesign& hd, ClusteringInformation& ci);
  void UpdateCellsCoordinates(HDesign& hd, ClusteringInformation& ci);
  void WriteCellsCoordinates2Clusters(HDesign& hd, ClusteringInformation& ci);
  void SetClustersCoords(ClusteringInformation& ci, Vec& x);
  void GetClusterCoordinates(ClusteringInformation& ci, Vec x);
  void SetKValues(ClusteringInformation& ci, Vec& x);
  void GetKValues(ClusteringInformation& ci, Vec x);
  void SetBounds(HDesign& hd, ClusteringInformation& ci, Vec& xl, Vec& xu);

  double CalculateSumOfK(HDesign& hd, ClusteringInformation& ci);

  //auxiliary
  int* InitIdxs(int nVariables, int shift);
}