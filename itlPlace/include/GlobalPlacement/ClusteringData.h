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
  HCluster clusterIdx;
  HCluster bestNeighborIdx;
  double score;

  MergeCandidate()
  {
    score = 0.0;
    //bestNeighborIdx = HClusters::Null();
    //clusterIdx = HClusters::Null();
  }
};

// во время кластеризации ведется лог кластеризации (clusteringLog) для того, чтобы потом 
// по нему откатиться назад на предыдущий уровень кластеризации.
// данные, к-ые достаточно занести в журнал кластеризации при кластеризации двух кластеров, 
// можно представить следующей структурой
struct MergedCluster
{
  HCluster cluster1Idx;
  HCluster cluster2Idx;
  size_t nCellsInCluster1;
};

class ClusteredNet
{
public:
  std::vector<HCluster> clusterIdxs;
  
  double weight;
  double k;

  ClusteredNet()
  {
    weight = 1.0;
    k      = 1;
  }
  ClusteredNet& operator=(const ClusteredNet& cn)
  {
    this->weight      = cn.weight;
    this->k           = cn.k;
    this->clusterIdxs = cn.clusterIdxs;
    return *this;
  }
};

typedef std::list<MergedCluster> ClusteringLog;
typedef std::list<ClusteringLog>::reverse_iterator ClusteringLogIterator;

//typedef std::vector<ClusteredNet> ClusteringNetList0;  //перечень существующих соединений между кластерами
//typedef std::list<NetList>::reverse_iterator NetListIterator;

typedef std::vector<HClusteredNet> ConnectionsVector;// перечень индексов соединений, в которые входит текущий кластер

typedef double (*pAffinityFunction)(HDesign& hd, HCluster& firstClusterIdx, HCluster& secondClusterIdx, 
                                                   std::vector<Cluster>& clusters, HClusteredNets* netList,
                                                   std::vector<ConnectionsVector>& currTableOfAdjacentNets);

#endif