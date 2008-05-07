/*  
 * multilevel_framework.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2007, ITLab, Zhivoderov Artem
 * email: zhivoderov.a@gmail.com
 */

#ifndef _MULTILEVEL_FRAMEWORK_H_
#define _MULTILEVEL_FRAMEWORK_H_

#include "..\include\errors.h"
#include "..\include\global.h"
#include "..\include\random_numbers_generation.h"
#include "..\include\itlTiming.h"
#include "tao.h"
#include <vector>
#include <list>

#define IsNotTerminal(nodeIdx) ((nodeIdx < nNodes) ? true : false)
#define sign(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
#define MARK_NEIGHBORS_INVALID true
#define DONT_MARK_NEIGHBORS_INVALID false
#define USE_BORDER_PENALTY

using namespace std;
//using namespace stlport;

namespace MultilevelFramework
{
  const int CLUSTER_RATIO = 5; // decrease of the number of clusters after each iteration
  const int FINAL_NCLUSTERS = 2000;
  const double CLUSTERS_AREA_TOLERANCE = 1.5;

  struct Cluster
  {
    vector<int> cellIdxs;  // индексы исходных node'ов, которые попали в данный кластер
    double xCoord;         // его координаты
    double yCoord;
    double area;           // занимаемая площадь (= сумма площадей всех нодов из cellIdxs)
    double scalingCoefficient; 
    bool   isValid;        // применяется в алгоритме кластеризации. если true, то пересчитывать score для данного кластера
                           // не нужно, в противном случае score пересчитывается, и элемент помещается в очередь (Heap)
                           // для кластеризации на то
                           // место, к-му соответствует его новый score. для этого используется ф-ия InsertInHeap
    bool   isFake;         // при слиянии двух кластеров результат помещается в первый из них, а второй помечается как Fake
    Cluster()
    {
      xCoord  = yCoord = area = 0.0;
      isFake  = false;
      isValid = true;
    }
  };
  
  struct ClusterData
  {
    double score;
    int bestNeighborIdx;
    int clusterIdx;
    ClusterData()
    {
      score = 0.0;
      bestNeighborIdx = 0;
      clusterIdx = 0;
    }
  };

  // во время кластеризации ведется лог кластеризации (clusteringLog) для того, чтобы потом 
  // по нему откатиться назад на предыдущий уровень кластеризации.
  // данные, к-ые достаточно занести в журнал кластеризации при кластеризации двух кластеров, можно представить
  // следующей структурой
  struct ClusteredNodes
  {
    int firstClusterIdx;
    int secondClusterIdx;
    size_t size;  // end index of clusters in "vector<int> cellIdxs" of the result cluster
  };

  struct NetWithWeight 
  {
    vector<int> clusterIdxs;
    double      weight;
    NetWithWeight()
    {
      weight = 0.0;
    }
    /*size_t size()
    {
      return clusterIdxs.size();
    }*/
  };

  typedef list<ClusteredNodes> ClusteringInfoAtEachLevel;

  // перечень существующих соединений между кластерами
  //typedef vector<vector<int>> NetList;
  typedef vector<NetWithWeight> NetList;
  
  // перечень индексов соединений, в которые входит текущий кластер
  typedef vector<int> ConnectionsList;
  
  typedef list<ClusteringInfoAtEachLevel>::reverse_iterator ClusteringLogIterator;

  // User-defined application context - contains data needed by the 
  // application-provided call-back routines that evaluate the function and gradient
  typedef struct {
    Circuit*          circuit;
    vector<Cluster>*  clusters;
    NetList*          netList;
    int               nClusters;
    double            alpha;
    PetscInt*         lookUpTable;
    vector<ConnectionsList>* currTableOfConnections;
    double            binHeight;
    double            binWidth;
    int               nBinRows;
    int               nBinCols;
    double            mu0;
    double            mu;
    double            totalCellArea;
    double            meanBinArea;
    double            penAlpha;
    Bin               **binPenalties; // todo: check if we really need bin array, why not simple double**??
    double**          clusterPotentialOverBins;
    double            potentialRadiusX;
    double            potentialRadiusY;
    double            weight;        // вес для симметричной записи критерия
    double*           netWeights;
  } AppCtx;

  typedef MULTIPLACER_ERROR (*affinityFunc)(const int& firstClusterIdx, const int& secondClusterIdx,
                                            vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                                            vector<ConnectionsList>& currTableOfConnections,
                                            double* netAreas, double& result);
  
  void Merge(vector<int>& a, vector<int>& b, int result[]);

  MULTIPLACER_ERROR Clusterize(Circuit& circuit, vector<Cluster>& clusters, NetList& netList, list<NetList>& netLevels,
                               affinityFunc Affinity, list<ClusteringInfoAtEachLevel>& clusteringLog, int& currNClustrers);

  void InitializeDataStructures(Circuit& circuit, vector<Cluster>& clusters, 
                                NetList& netList, const int& nNodes, int& nClusters);

  MULTIPLACER_ERROR Relaxation(Circuit& circuit, vector<Cluster>& clusters, NetList& netList,
                               vector<ConnectionsList>& currTableOfConnections, 
                               int& nInnerIters, int& nOuterIters);

  int ReadClustersCoords(Vec x, int nClusters, int* idxs, PetscScalar* initValues,
                         vector<Cluster> &clusters);

  void InitializeOptimizationProblemParameters(AppCtx &user, Circuit& circuit, 
    vector<Cluster>& clusters, NetList& netList, int nClusters, PetscInt** lookUpTable, 
    vector<ConnectionsList>& currTableOfConnections);
  void DeinitializeOptimizationProblemParameters(AppCtx &user);
  
  MULTIPLACER_ERROR Interpolation(Circuit& circuit, vector<Cluster>& clusters, 
                                  vector<ConnectionsList>& currTableOfConnections, NetList& netList);

  void UnclusterLevelUp(vector<Cluster>& clusters, vector<ConnectionsList>& currTableOfConnections,
                        NetList& netList, ClusteringLogIterator clusteringLogIterator, const int& nNodes);

  void UpdateCoords(Circuit& circuit, vector<Cluster>& clusters);

  MULTIPLACER_ERROR MergeClusters(const int& firstClusterIdx, const int& secondClusterIdx,
                                  vector<Cluster>& clusters, NetList& netList,
                                  vector<ConnectionsList>& currTableOfConnections,
                                  double* netAreas);
  MULTIPLACER_ERROR AffinityInterp(const int& firstClusterIdx, const int& secondClusterIdx,
                                   vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                                   vector<ConnectionsList>& currTableOfConnections,
                                   double* netAreas, double& result);
  MULTIPLACER_ERROR Affinity(const int& firstClusterIdx, const int& secondClusterIdx, 
                             vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                             vector<ConnectionsList>& currTableOfConnections, double* netAreas, double& result);
  // affinity function that considers spacial proximity
  MULTIPLACER_ERROR AffinitySP(const int& firstClusterIdx, const int& secondClusterIdx, 
                               vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                               vector<ConnectionsList>& currTableOfConnections, double* netAreas, double& result);
  MULTIPLACER_ERROR CalculateScore(const int& currClusterIdx, 
                                   vector<ConnectionsList>& currTableOfConnections,
                                   NetList& netList, int* netListSizes, double* netAreas, 
                                   vector<Cluster>& clusters,
                                   double& score, int& bestNeighborIdx, bool isToMark,
                                   affinityFunc Affinity, int nNodes);
  MULTIPLACER_ERROR InsertInHeap(list<ClusterData>& clustersDataList);

  void CreateTableOfConnections(vector<Cluster>& clusters, vector<ConnectionsList>& currTableOfConnections,
                                NetList& netList, const int& nNodes);

  double LogSumExpForClusters(PetscScalar *coordinates, void* data);

  void   CalcLogSumExpForClustersGrad(PetscScalar *coordinates, PetscScalar *grad, void* data);
  
  double TestObjectiveFunc(PetscScalar *coordinates, void* data);
  
  void   SetInitialState(vector<Cluster>& clusters, Circuit& circuit, const int& numOfClusters);

  void CalcBinGrid(Circuit& circuit, vector<Cluster>& clusters, const int& nClusters,
    int& nBinRows, int& nBinCols,
    double& binWidth, double& binHeight);

  double CalcPenalty(PetscScalar *x, void* data);

  void DetermineBordersOfClusterPotential(int &min_col, int &max_col, int &min_row, int &max_row,
                                          int i, PetscScalar * x, AppCtx* userData);

  void CalcPenaltyGrad(PetscScalar *x, PetscScalar *grad, void* data);

  void CalcBellShapedFuncAndDerivative(AppCtx* userData, int solutionIdx, int clusterIdx, int colIdx, int rowIdx, PetscScalar * x, 
                                       double &potX, double &gradX, double &potY, double &gradY);
  void CalcMu0(PetscScalar *x, void* data);
  double GetDiscrepancy(PetscScalar *x, void* data);
  void MakeClustersFromBins(Circuit& circuit, vector<Cluster>& clusters, NetList& netList);
  void ComputeNetWeights(Circuit& circuit, vector<Cluster>& clusters, NetList& netList, double* netWeights);
}
#endif