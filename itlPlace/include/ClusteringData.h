#ifndef __CLUSTERING_DATA__
#define __CLUSTERING_DATA__

struct Cluster
{
  std::vector<HCell> cells;
  double xCoord;  // его координаты
  double yCoord;
  double area;    // занимаемая площадь (= сумма площадей всех элементов кластера)
  double potentialMultiplier; 
  bool   isValid; //применяется в алгоритме кластеризации. если true, то пересчитывать score 
                  //для данного кластера не нужно, в противном случае score пересчитывается, 
                  //и элемент помещается в очередь (Heap) для кластеризации на то
                  //место, к-му соответствует его новый score. для этого используется ф-ия InsertInHeap
  bool   isFake;  //при слиянии двух кластеров результат помещается в первый из них, 
                  //а второй помечается как Fake
  
  Cluster()
  {
    xCoord = yCoord = 0.0;
    area = 0.0;
    isFake = false;
    isValid = true;
  }
};

struct MergeCandidate
{
  int clusterIdx;
  int bestNeighborIdx;
  double score;

  MergeCandidate()
  {
    score = 0.0;
    bestNeighborIdx = -1;
    clusterIdx = -1;
  }
};

// во время кластеризации ведется лог кластеризации (clusteringLog) для того, чтобы потом 
// по нему откатиться назад на предыдущий уровень кластеризации.
// данные, к-ые достаточно занести в журнал кластеризации при кластеризации двух кластеров, 
// можно представить следующей структурой
struct MergedCluster
{
  int cluster1Idx;
  int cluster2Idx;
  size_t nCellsInCluster1;
};

struct ClusteredNet
{
  std::vector<int> clusterIdxs;

  double weight;

  //SumOfDelays data
  double Lnet;
  double k;
  double braces;

  //Lagrangian Relaxation data
  double sourceAFactor;
  std::vector<double> sinkLoad;
  std::vector<double> muNetVector;

  ClusteredNet()
  {
    weight = 1.0;
    k = 0.0;
    sourceAFactor = 0.0;
  }
};

typedef std::list<MergedCluster> ClusteringLog;
typedef std::list<ClusteringLog>::reverse_iterator ClusteringLogIterator;

typedef std::vector<ClusteredNet> NetList;  //перечень существующих соединений между кластерами
typedef std::list<NetList>::reverse_iterator NetListIterator;

typedef std::vector<int> ConnectionsVector;// перечень индексов соединений, в которые входит текущий кластер

typedef double (*pAffinityFunction)(HDesign& hd, 
                                    const int& firstClusterIdx, const int& secondClusterIdx,
                                    std::vector<Cluster>& clusters, NetList& netList, int* netListSizes,
                                    std::vector<ConnectionsVector>& currTableOfAdjacentNets);

#endif