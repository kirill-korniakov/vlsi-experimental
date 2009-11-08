#include "Clustering.h"

#include <vector>
#include "math.h"
#include <stdlib.h>
#include <conio.h>

//TODO: do smth with this trash
#define MARK_NEIGHBORS_INVALID true
#define DONT_MARK_NEIGHBORS_INVALID false

bool IsMovableCell(int idx)
{
  if (idx < SHIFT_NUMBER_FOR_TERMINALS)
    return true;
  else
    return false;
}

bool IsTerminal(int idx)
{
  if (idx >= SHIFT_NUMBER_FOR_TERMINALS && idx < SHIFT_NUMBER_FOR_PRIMARY_PINS)
    return true;
  else
    return false;
}

bool IsPrimaryPin(int idx)
{
  if (idx >= SHIFT_NUMBER_FOR_PRIMARY_PINS)
    return true;
  else
    return false;
}

void InitializeClusters(HDesign& hd, ClusteringInformation& ci)
{
  //put clusters
  int clusterIdx = 0;
  for (HCells::PlaceableCellsEnumeratorW cell = hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
  {
    ci.clusters[clusterIdx].cells.push_back(cell);
    ci.clusters[clusterIdx].area = cell.Width() * cell.Height();
    clusterIdx++;
  }
}

inline int CellToClusterIdx(ClusteringInformation& ci, HCell cell)
{
  return ::ToID(cell) - 1;
  /*for (unsigned int i = 0; i < ci.clusters.size(); i++)
  {
    if (cell == ci.clusters[i].cells[0])
    {
      return i;
    }
  }
  return -1;*/
}

int AssignClusterIdx(HDesign& hd, ClusteringInformation& ci, HNet::PinsEnumeratorW& pin)
{
  int clusterIdx = 0;
  static int primaryPinCount = 0;
  static int terminalCount = 0;

  if (pin.IsPrimary())
  {
    clusterIdx = SHIFT_NUMBER_FOR_PRIMARY_PINS + primaryPinCount;
    ci.primaryPins.push_back(pin);
    primaryPinCount++;
  }
  else if (hd.GetBool<HCell::IsTerminal>(pin.Cell()))
  {
    clusterIdx = SHIFT_NUMBER_FOR_TERMINALS + terminalCount;
    ci.terminalCells.push_back(pin.Cell());
    terminalCount++;
  }
  else
  {
    clusterIdx = CellToClusterIdx(ci, pin.Cell());
  }

  return clusterIdx;
}

void AssignClusters(HDesign& hd, ClusteringInformation &ci, HNets::NetsEnumeratorW& net, int netIdx)
{
  //put pins (we also add terminals and primary pins for LogSumExp calculation)
  for (HNet::PinsEnumeratorW pin = net.GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    int clusterIdx = AssignClusterIdx(hd, ci, pin);
    ci.netList[netIdx].clusterIdxs.push_back(clusterIdx);
  }

  //remove duplicates
  std::vector<int>::iterator iter;
  sort(ci.netList[netIdx].clusterIdxs.begin(), ci.netList[netIdx].clusterIdxs.end());
  iter = unique(ci.netList[netIdx].clusterIdxs.begin(), ci.netList[netIdx].clusterIdxs.end());
  ci.netList[netIdx].clusterIdxs.resize(iter - ci.netList[netIdx].clusterIdxs.begin());
}

void InitializeNetList(HDesign& hd, ClusteringInformation& ci)
{
  ci.netList.resize(hd.Nets.Count());

  //put nets
  int netIdx = 0;
  for (HNets::NetsEnumeratorW net = hd.Nets.GetNetsEnumeratorW(); net.MoveNext(); )
  {
    AssignClusters(hd, ci, net, netIdx);

    ci.netList[netIdx].weight = net.Weight();
    
    netIdx++;
  }

  ci.netLevels.push_back(ci.netList);
}

//we add terminals and primary pins for LogSumExp calculation
void InitializeTerminalsAndPrimaryPins(HDesign& hd, ClusteringInformation& ci)
{
  for (HNets::NetsEnumeratorW net = hd.Nets.GetNetsEnumeratorW(); net.MoveNext(); )
  {
    for (HNet::PinsEnumeratorW pin = net.GetPinsEnumeratorW(); pin.MoveNext(); )
    {
      if (pin.IsPrimary())
      {
        ci.primaryPins.push_back(pin);
      }
      else if (hd.GetBool<HCell::IsTerminal>(pin.Cell()))
      {
        ci.terminalCells.push_back(pin.Cell());
      }
    }
  }
}

void PrintClusteringInfo(HDesign& hd, ClusteringInformation& ci, int nClusters)
{
  ALERTFORMAT(("Currently there are %d clusters", ci.mCurrentNumberOfClusters));

  int upLimit = ci.mCurrentNumberOfClusters;
  if (nClusters >= 0)
    upLimit = nClusters;

  for (int i = 0; i<upLimit; i++)
  {
    ALERTFORMAT(("cluster %d:\t", i));
    if (ci.clusters[i].isFake)
    {
      ALERT("FAKE");
      continue;
    }
    else
    {
      for (int j = 0; j < static_cast<int>(ci.clusters[i].cells.size()); j++)
      {
        ALERT(hd[ci.clusters[i].cells[j]].Name().c_str());
      }
    }
  }
}

void CreateTableOfAdjacentNets(HDesign& hd, ClusteringInformation& ci)
{
  //prepare table
  ci.tableOfAdjacentNets.clear();
  ci.tableOfAdjacentNets.resize(ci.clusters.size());

  //fill the table
  for (int netIdx = 0; netIdx < static_cast<int>(ci.netList.size()); ++netIdx)
  {
    for (int j = 0; j < static_cast<int>(ci.netList[netIdx].clusterIdxs.size()); ++j)
    {
      int clusterIdx = ci.netList[netIdx].clusterIdxs[j];
      if (IsMovableCell(clusterIdx))
      {
        if (!ci.clusters[clusterIdx].isFake)
        {
          ci.tableOfAdjacentNets[clusterIdx].push_back(netIdx);
        }
      }
    }
  }

  //remove duplicated nets
  for (unsigned int i = 0; i < ci.tableOfAdjacentNets.size(); i++)
  {
    ConnectionsVector::iterator newEnd = unique(ci.tableOfAdjacentNets[i].begin(), 
      ci.tableOfAdjacentNets[i].end());
    ci.tableOfAdjacentNets[i].resize(newEnd - ci.tableOfAdjacentNets[i].begin());
  }
}

//NOTE: you have to set initial value -1 for clusterIdx if
//you want to go through all clusters
bool GetNextActiveClusterIdx(ClusteringInformation* ci, int& clusterIdx)
{
  bool retCode = true;

  clusterIdx++;

  for (; clusterIdx < static_cast<int>(ci->clusters.size()); ++clusterIdx)
  {
    if (ci->clusters[clusterIdx].isFake) 
      continue;
    else
      break;
  }
  if (clusterIdx == static_cast<int>(ci->clusters.size()))
    retCode = false;

  return retCode;
}

void Merge(std::vector<int>& a, std::vector<int>& b, int result[])
{
  long size1 = static_cast<int>(a.size());
  long size2 = static_cast<int>(b.size());
  // текущая позиция чтения из первой последовательности
  long pos1=0;
  // текущая позиция чтения из второй последовательности
  long pos2=0;
  // текущая позиция записи в temp
  long pos3=0;

  // идет слияние, пока есть хоть один элемент в каждой последовательности
  while (pos1 < size1 && pos2 < size2) {
    if (a[pos1] < b[pos2])
      result[pos3++] = a[pos1++];
    else
      result[pos3++] = b[pos2++];
  }

  // одна последовательность закончилась - 
  // копировать остаток другой в конец буфера 
  while (pos2 < size2)   // пока вторая последовательность непуста 
    result[pos3++] = b[pos2++];
  while (pos1 < size1)  // пока первая последовательность непуста
    result[pos3++] = a[pos1++];
}

double AffinityLite(HDesign& hd, std::vector<int>& a, std::vector<int>& b, int* netListSizes, Cluster& ac, Cluster& bc)
{
  double result = 0.0;
  long size1 = static_cast<int>(a.size());
  long size2 = static_cast<int>(b.size());
  long pos1=0;
  long pos2=0;

  while (pos1 < size1 && pos2 < size2) {
    if (a[pos1] < b[pos2])
      ++pos1;
    else if (a[pos1] > b[pos2])
      ++pos2;
    else
    {
      if (netListSizes[a[pos1]] > 1)
        result += hd._Design.NetList.netWeight[pos1] / ((netListSizes[a[pos1]] - 1) * ac.area * bc.area);
      ++pos1;
      ++pos2;
    }
  }
  return result;
}

double Affinity(HDesign& hd, const int& firstClusterIdx, const int& secondClusterIdx, 
             std::vector<Cluster>& clusters, NetList& netList, int* netListSizes,
             std::vector<ConnectionsVector>& currTableOfAdjacentNets)
{
  return AffinityLite(hd, currTableOfAdjacentNets[firstClusterIdx],
    currTableOfAdjacentNets[secondClusterIdx],
    netListSizes,
    clusters[firstClusterIdx],
    clusters[secondClusterIdx]);
  //double result = 0.0;

  //static std::vector<int> commonNetsIdxs(10000000); //HACK: magic number, looks like bad hack
  //int netIdx = 0;
  //int theSize = static_cast<int>(currTableOfAdjacentNets[firstClusterIdx].size() + currTableOfAdjacentNets[secondClusterIdx].size());
  //static int* tmpIdxsInt = new int[10000000];

  //// looking for common nets for these clusters:
  //Merge(currTableOfAdjacentNets[firstClusterIdx], currTableOfAdjacentNets[secondClusterIdx], tmpIdxsInt);
  //int counter = 0;
  //for (int i = 0; i < theSize - 1; ++i)
  //{
  //  if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netListSizes[tmpIdxsInt[i]] > 1)
  //    commonNetsIdxs[counter++] = tmpIdxsInt[++i];
  //}

  //for (int i = 0; i < counter; ++i)
  //{
  //  netIdx = commonNetsIdxs[i];
  //  result += 1 / ((netListSizes[netIdx] - 1) * clusters[firstClusterIdx].area * clusters[secondClusterIdx].area);
  //}

  //return result;
}

//double dist(const int& firstClusterIdx, const int& secondClusterIdx, std::vector<Cluster>& clusters)
//{
//  return sqrt(pow(clusters[firstClusterIdx].xCoord - clusters[secondClusterIdx].xCoord, 2) + 
//    pow(clusters[firstClusterIdx].yCoord - clusters[secondClusterIdx].yCoord, 2));
//}
//
//double AffinitySP(const int& firstClusterIdx, const int& secondClusterIdx, 
//               std::vector<Cluster>& clusters, NetList& netList, int* netListSizes,
//               std::vector<ConnectionsVector>& currTableOfAdjacentNets)
//{
//  double result = 0.0;
//
//  std::vector<int> commonNetsIdxs;
//  int netIdx = 0;
//  int theSize = static_cast<int>(currTableOfAdjacentNets[firstClusterIdx].size() + 
//    currTableOfAdjacentNets[secondClusterIdx].size());
//  int* tmpIdxsInt = new int[theSize];
//
//  // looking for common nets for these clusters:
//  Merge(currTableOfAdjacentNets[firstClusterIdx], currTableOfAdjacentNets[secondClusterIdx], tmpIdxsInt);
//  for (int i = 0; i < theSize - 1; ++i)
//  {
//    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netList[tmpIdxsInt[i]].clusterIdxs.size() > 1)
//      commonNetsIdxs.push_back(tmpIdxsInt[++i]);
//  }
//
//  for (int i = 0; i < static_cast<int>(commonNetsIdxs.size()); ++i)
//  {
//    netIdx = commonNetsIdxs[i];
//    result += 1 / ((netList[netIdx].clusterIdxs.size() - 1) * clusters[firstClusterIdx].area * 
//      clusters[secondClusterIdx].area * dist(firstClusterIdx, secondClusterIdx, clusters));
//  }
//
//  delete[] tmpIdxsInt;
//  return result;
//}

void CalculateNetListSizes(NetList& netList, int* netListSizes)
{
  int netListSize = static_cast<int>(netList.size());
  for (int i = 0; i < netListSize; i++)
  {
    netListSizes[i] = static_cast<int>(netList[i].clusterIdxs.size());
  }
}

////TODO: check if we really need this function
//double AffinityInterp(const int& firstClusterIdx, const int& secondClusterIdx, 
//                   std::vector<Cluster>& clusters, NetList& netList, int* netListSizes,
//                   std::vector<ConnectionsVector>& currTableOfAdjacentNets)
//{
//  double result = 0.0;
//
//  std::vector<int> commonNetsIdxs;
//  int netIdx = 0;
//  int theSize = static_cast<int>(currTableOfAdjacentNets[firstClusterIdx].size() + currTableOfAdjacentNets[secondClusterIdx].size());
//  int* tmpIdxsInt = new int[theSize];
//
//  // looking for common nets for these clusters:
//  Merge(currTableOfAdjacentNets[firstClusterIdx], currTableOfAdjacentNets[secondClusterIdx], tmpIdxsInt);
//  for (int i = 0; i < theSize - 1; ++i)
//  {
//    if (tmpIdxsInt[i] == tmpIdxsInt[i+1] && netList[tmpIdxsInt[i]].clusterIdxs.size() > 1)
//      commonNetsIdxs.push_back(tmpIdxsInt[++i]);
//  }
//
//  for (int i = 0; i < static_cast<int>(commonNetsIdxs.size()); ++i)
//  {
//    netIdx = commonNetsIdxs[i];
//    result += 1 / ((netList[netIdx].clusterIdxs.size() - 1));
//  }
//
//  delete[] tmpIdxsInt;
//  return result;
//}

bool AreClustersMergeable(ClusteringInformation& ci, int clusterIdx, int neighborCandidateIdx)
{
  if (clusterIdx == neighborCandidateIdx) 
    return false;

  //WARNING: do not join following 2 is statement, because we have
  //to call IsMovableCell() first
  if (!IsMovableCell(neighborCandidateIdx))
    return false;

  if (ci.clusters[neighborCandidateIdx].isFake)
    return false;

  return true;
}

int FindBestNeighbour(HDesign& hd, ClusteringInformation& ci, int* netListSizes, MergeCandidate& mergeCandidate)
{
  mergeCandidate.score = 0.0;

  for (int j = 0; j < static_cast<int>(ci.tableOfAdjacentNets[mergeCandidate.clusterIdx].size()); ++j)
  {
    int netIdx = ci.tableOfAdjacentNets[mergeCandidate.clusterIdx][j];
    for (int k = 0; k < static_cast<int>(ci.netList[netIdx].clusterIdxs.size()); ++k)
    {
      int neighborCandidateIdx = ci.netList[netIdx].clusterIdxs[k];

      if (!AreClustersMergeable(ci, mergeCandidate.clusterIdx, neighborCandidateIdx))
        continue;
      
      double score = Affinity(hd, mergeCandidate.clusterIdx, neighborCandidateIdx, ci.clusters, ci.netList, 
                              netListSizes, ci.tableOfAdjacentNets);

      if (mergeCandidate.score < score)
      {
        mergeCandidate.score = score;
        mergeCandidate.bestNeighborIdx = neighborCandidateIdx;
      }

      /*if (Affinity != AffinityInterp && mergeCandidate.score < score)
      {
        mergeCandidate.score = score;
        mergeCandidate.bestNeighborIdx = neighborCandidateIdx;
      }
      else
      {
        if (Affinity == AffinityInterp)
          mergeCandidate.score += score;
      }*/
    }
  }

  return OK;
}

int MarkNeighboursAsInvalid(ClusteringInformation& ci, int clusterIdx)
{
  for (int j = 0; j < static_cast<int>(ci.tableOfAdjacentNets[clusterIdx].size()); ++j)
  {
    int netIdx = ci.tableOfAdjacentNets[clusterIdx][j];
    for (int k = 0; k < static_cast<int>(ci.netList[netIdx].clusterIdxs.size()); ++k)
    {
      int neighborCandidateIdx = ci.netList[netIdx].clusterIdxs[k];
      
      if (neighborCandidateIdx == clusterIdx || !IsMovableCell(neighborCandidateIdx))
        continue;

      ci.clusters[neighborCandidateIdx].isValid = false;
    }
  }

  return OK;
}

int CalculateNumberOfActiveClusters(ClusteringInformation& ci)
{
  int nActiveClusters = 0;
  int clusterIdx = -1;
  while (GetNextActiveClusterIdx(&ci, clusterIdx))
  {
    nActiveClusters++;
  }
  return nActiveClusters;
}

void UnclusterLevelUp(HDesign& hd, ClusteringInformation& ci, ClusteringLogIterator& clusteringLogIterator)
{
  std::list<MergedCluster>::reverse_iterator iter = clusteringLogIterator->rbegin();

  for (; iter != clusteringLogIterator->rend(); ++iter)
  {
    ci.clusters[iter->cluster1Idx].cells.resize(iter->nCellsInCluster1);
    ci.clusters[iter->cluster1Idx].area -= ci.clusters[iter->cluster2Idx].area;
    ci.clusters[iter->cluster2Idx].isFake = false;
    ci.clusters[iter->cluster2Idx].xCoord = ci.clusters[iter->cluster1Idx].xCoord;
    ci.clusters[iter->cluster2Idx].yCoord = ci.clusters[iter->cluster1Idx].yCoord;
  }

  ci.mCurrentNumberOfClusters = CalculateNumberOfActiveClusters(ci);

  CreateTableOfAdjacentNets(hd, ci);
}

int ShiftHeadAccordingToScore(std::list<MergeCandidate>& mergeCandidates)
{
  std::list<MergeCandidate>::iterator headCandidate = mergeCandidates.begin();
  
  //find proper place for head of the list
  std::list<MergeCandidate>::iterator iter = headCandidate;
  iter++;
  for (; iter != mergeCandidates.end(); iter++)
  {
    if (headCandidate->score >= iter->score)
      break;
  }

  //move the head
  mergeCandidates.insert(iter, *headCandidate);
  mergeCandidates.pop_front();

  return OK;
}

void MergeCurrTableOfAdjacentNets(std::vector<ConnectionsVector>& currTableOfAdjacentNets, 
                                  const int cluster1Idx, const int cluster2Idx)
{
  ConnectionsVector tempMergedVector(currTableOfAdjacentNets[cluster1Idx].size() 
    + currTableOfAdjacentNets[cluster2Idx].size());

  merge(currTableOfAdjacentNets[cluster1Idx].begin(), currTableOfAdjacentNets[cluster1Idx].end(), 
        currTableOfAdjacentNets[cluster2Idx].begin(), currTableOfAdjacentNets[cluster2Idx].end(),
        tempMergedVector.begin());

  ConnectionsVector::iterator newEnd = unique(tempMergedVector.begin(), tempMergedVector.end());
  currTableOfAdjacentNets[cluster1Idx].resize(newEnd - tempMergedVector.begin());
  copy(tempMergedVector.begin(), newEnd, currTableOfAdjacentNets[cluster1Idx].begin());
}

void MergeCellIdxs(std::vector<Cluster>& clusters, const int& clusterIdx1, const int& clusterIdx2)
{
  int size1 = clusters[clusterIdx1].cells.size();
  int size2 = clusters[clusterIdx2].cells.size();
  
  std::vector<HCell> tempVector;
  tempVector.resize(size1 + size2);
  copy(clusters[clusterIdx1].cells.begin(), clusters[clusterIdx1].cells.end(), tempVector.begin());
  copy(clusters[clusterIdx2].cells.begin(), clusters[clusterIdx2].cells.end(), tempVector.begin() + size1);
  
  clusters[clusterIdx1].cells.resize(size1 + size2);
  copy(tempVector.begin(), tempVector.end(), clusters[clusterIdx1].cells.begin());
}

void UpdateNetList(std::vector<ConnectionsVector>& currTableOfAdjacentNets, NetList& netList, 
                   const int firstClusterIdx, const int secondClusterIdx)
{
  std::vector<int>::iterator iter;
  for (int j = 0; j < static_cast<int>(currTableOfAdjacentNets[secondClusterIdx].size()); ++j)
  {
    int netIdx = currTableOfAdjacentNets[secondClusterIdx][j];
    if (netList[netIdx].clusterIdxs.size() == 0)
      continue;

    //rewrite second cluster idx with first cluster idx
    iter = find(netList[netIdx].clusterIdxs.begin(), netList[netIdx].clusterIdxs.end(), secondClusterIdx);
    if (iter == netList[netIdx].clusterIdxs.end())
    {
      LOGERRORFORMAT(("can't find cluster in net. Cluster index: %d, Net index: %d", secondClusterIdx, netIdx));
    }
    (*iter) = firstClusterIdx;

    //remove duplicates
    sort(netList[netIdx].clusterIdxs.begin(), netList[netIdx].clusterIdxs.end());
    iter = unique(netList[netIdx].clusterIdxs.begin(), netList[netIdx].clusterIdxs.end());
    netList[netIdx].clusterIdxs.resize(iter - netList[netIdx].clusterIdxs.begin());

    //clear net if it is totally inside cluster
    if (netList[netIdx].clusterIdxs.size() == 1)
    {
      netList[netIdx].clusterIdxs.clear();
    }
  }
}

// the result is stored in clusterIdx1
int MergePairOfClusters(const int cluster1Idx, const int cluster2Idx, 
                        std::vector<Cluster>& clusters, NetList& netList,
                        std::vector<ConnectionsVector>& currTableOfAdjacentNets)
{
  MergeCurrTableOfAdjacentNets(currTableOfAdjacentNets, cluster1Idx, cluster2Idx);
  MergeCellIdxs(clusters, cluster1Idx, cluster2Idx);
  UpdateNetList(currTableOfAdjacentNets, netList, cluster1Idx, cluster2Idx);

  clusters[cluster1Idx].xCoord = (clusters[cluster1Idx].xCoord * clusters[cluster1Idx].area + 
                                  clusters[cluster2Idx].xCoord * clusters[cluster2Idx].area) / 
                                  (clusters[cluster1Idx].area + clusters[cluster2Idx].area);
  clusters[cluster1Idx].yCoord = (clusters[cluster1Idx].yCoord * clusters[cluster1Idx].area + 
                                  clusters[cluster2Idx].yCoord * clusters[cluster2Idx].area) / 
                                  (clusters[cluster1Idx].area + clusters[cluster2Idx].area);
  clusters[cluster1Idx].area += clusters[cluster2Idx].area;
  clusters[cluster2Idx].isFake = true;

  return OK;
}

bool PredicateMergePairGreater(MergeCandidate elem1, MergeCandidate elem2)
{
  return elem1.score > elem2.score;
}

bool PredicateNetListLess(ClusteredNet elem1, ClusteredNet elem2)
{
  int size1 = static_cast<int>(elem1.clusterIdxs.size());
  int size2 = static_cast<int>(elem2.clusterIdxs.size());

  if (size1 < size2)
    return true;
  else if (size1 > size2)
    return false;

  for (int i = 0; i < size1; ++i)
  {
    if (elem1.clusterIdxs[i] < elem2.clusterIdxs[i])
      return true;
    else if (elem1.clusterIdxs[i] > elem2.clusterIdxs[i])
      return false;
  }

  // сортировка по весам в обратном порядке
  // при применении функции unique автоматически будет выбран первый нет
  // при таком способе сортировки у этого нета будет максимальный вес
  if (elem1.weight > elem2.weight)
    return true;
  else if (elem1.weight < elem2.weight)
    return false;

  return false;
}

bool IsEqualNetListBinaryPredicate(ClusteredNet elem1, ClusteredNet elem2)
{
  int size1 = static_cast<int>(elem1.clusterIdxs.size());
  int size2 = static_cast<int>(elem2.clusterIdxs.size());

  if (size1 != size2)
    return false;

  for (int i = 0; i < size2; ++i)
  {
    if (elem1.clusterIdxs[i] != elem2.clusterIdxs[i])
      return false;
  }

  return true;
}

double CalculateTotalCellArea(HDesign &hd)
{
  double totalCellArea = 0.0;

  for (HCells::PlaceableCellsEnumeratorW cell = hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
  {
    totalCellArea += cell.Width() * cell.Height();
  }  

  return totalCellArea;
}

void InitializeMergeCandidates(ClusteringInformation& ci, std::list<MergeCandidate>& mergeCandidates)
{
  mergeCandidates.clear();
  mergeCandidates.resize(ci.clusters.size());

  std::list<MergeCandidate>::iterator mcIter;
  int clusterIdx = 0;
  for (mcIter = mergeCandidates.begin(); mcIter != mergeCandidates.end(); mcIter++)
  {
    mcIter->clusterIdx = clusterIdx;
    mcIter->bestNeighborIdx = -1; 
    mcIter->score = 0.0;
    clusterIdx++;
  }
}

void CalculateScores(HDesign& hd, ClusteringInformation& ci, 
                     std::list<MergeCandidate>& mergeCandidates, int*& netListSizes)
{
  std::list<MergeCandidate>::iterator mcIter;
  for (mcIter = mergeCandidates.begin(); mcIter != mergeCandidates.end(); mcIter++)
  {
    MergeCandidate& mc = *mcIter;
    FindBestNeighbour(hd, ci, netListSizes, mc);
  }
}

void InitializeClustersData(ClusteringInformation& ci, std::vector<MergeCandidate>& clustersData)
{
  for (int i = 0; i < static_cast<int>(ci.clusters.size()); ++i)
  {
    clustersData[i].clusterIdx = i;
    clustersData[i].score = 0.0;
  }
}

void CalculateScores(HDesign& hd, ClusteringInformation& ci, 
                        std::vector<MergeCandidate>& clustersData, int*& netListSizes)
{
  InitializeClustersData(ci, clustersData);

  std::vector<MergeCandidate>::iterator mcIter;
  for (mcIter = clustersData.begin(); mcIter != clustersData.end(); mcIter++)
  {
    MergeCandidate& mc = *mcIter;
    FindBestNeighbour(hd, ci, netListSizes, mc);
  }
}

void PurifyNetList(HDesign& hd, ClusteringInformation& ci)
{
  NetList::iterator netListIterator;

  sort(ci.netList.begin(), ci.netList.end(), PredicateNetListLess);

  //delete duplicated nets
  if (hd.cfg.ValueOf("Clustering.deleteDuplicatingNets", true))
  {
    ALERTFORMAT(("NetList size before duplicates removing: %d", ci.netList.size()));
    netListIterator = unique(ci.netList.begin(), ci.netList.end(), IsEqualNetListBinaryPredicate);
    ci.netList.resize(netListIterator - ci.netList.begin());
    ALERTFORMAT(("NetList size after duplicates removing: %d", ci.netList.size()));
  }

  //delete nets with less than 2 clusters
  netListIterator = ci.netList.begin();
  while (netListIterator->clusterIdxs.size() < 2) 
    ++netListIterator;
  ci.netList.erase(ci.netList.begin(), netListIterator);
  ALERTFORMAT(("NetList size after one cluster nets removing: %d", ci.netList.size()));
}

void EraseZeroScorePairs(std::list<MergeCandidate>& mergeCandidates)
{
  std::list<MergeCandidate>::iterator mcIter;
  std::list<MergeCandidate>::iterator eraseIter;
  for (mcIter = mergeCandidates.begin(); mcIter != mergeCandidates.end(); )
  {
    if (mcIter->score != 0.0)
    {
      mcIter++;
    }
    else
    {
      eraseIter = mcIter;
      mcIter++;
      mergeCandidates.erase(eraseIter);
    }
  }
}

void CreateListOfMergeCandidates(HDesign& hd, ClusteringInformation& ci, 
                                 int*& netListSizes, std::list<MergeCandidate>& mergeCandidates)
{
  InitializeMergeCandidates(ci, mergeCandidates);
  CalculateScores(hd, ci, mergeCandidates, netListSizes);
  mergeCandidates.sort(PredicateMergePairGreater);
  EraseZeroScorePairs(mergeCandidates);
}

bool IsMergedAreaAcceptable(ClusteringInformation &ci, int currClusterIdx, int bestNeighborIdx, double maxClusterArea)
{
  return ci.clusters[currClusterIdx].area + ci.clusters[bestNeighborIdx].area <= maxClusterArea;
}

void PrintMergeCandidates(std::list<MergeCandidate>& mergeCandidates)
{
  for (std::list<MergeCandidate>::iterator iter = mergeCandidates.begin(); 
       iter != mergeCandidates.end(); iter++)
  {
    WRITE("(%d, %d)\t", iter->clusterIdx, iter->bestNeighborIdx);
  }
  WRITELINE("");
}

void MergeClusters(HDesign& hd, ClusteringInformation& ci)
{
  std::list<MergeCandidate> mergeCandidates;
  ClusteringLog thisLevelClusteringLog;

  int targetNClusters = static_cast<int>(ci.mCurrentNumberOfClusters / ci.mClusterRatio);
  double maxClusterArea = (CalculateTotalCellArea(hd) / targetNClusters) * ci.mClustersAreaTolerance;

  int* netListSizes = new int[ci.netList.size()];
  CalculateNetListSizes(ci.netList, netListSizes);
  
  CreateListOfMergeCandidates(hd, ci, netListSizes, mergeCandidates);

  //PrintMergeCandidates(mergeCandidates);

  //main loop of merging
  int mergedCount = 0;
  while (mergeCandidates.size() && ci.mCurrentNumberOfClusters - mergedCount > targetNClusters)
  {
    std::list<MergeCandidate>::iterator headCandidate = mergeCandidates.begin();
    int currClusterIdx  = headCandidate->clusterIdx;
    int bestNeighborIdx = headCandidate->bestNeighborIdx;

    if (ci.clusters[currClusterIdx].isValid && 
        IsMergedAreaAcceptable(ci, currClusterIdx, bestNeighborIdx, maxClusterArea)) 
    {//merge
      MergePairOfClusters(currClusterIdx, bestNeighborIdx, ci.clusters, ci.netList, ci.tableOfAdjacentNets);
      mergedCount++;

      //save merge info to the log
      MergedCluster mergedCluster;
      mergedCluster.cluster1Idx = currClusterIdx;
      mergedCluster.cluster2Idx = bestNeighborIdx;
      mergedCluster.nCellsInCluster1 = ci.clusters[currClusterIdx].cells.size();
      thisLevelClusteringLog.push_back(mergedCluster);
      
      CalculateNetListSizes(ci.netList, netListSizes);

      MarkNeighboursAsInvalid(ci, bestNeighborIdx);
      MarkNeighboursAsInvalid(ci, currClusterIdx);
      
      //delete second cluster merge pair - it is already clustered
      for (std::list<MergeCandidate>::iterator iter = mergeCandidates.begin(); 
           iter != mergeCandidates.end(); iter++)
      {
        if (iter->clusterIdx == bestNeighborIdx)
        {
          mergeCandidates.erase(iter);
          break;
        }
      }

      //shift head of the list
      MergeCandidate& mc = *headCandidate;
      FindBestNeighbour(hd, ci, netListSizes, mc);
      ShiftHeadAccordingToScore(mergeCandidates);
      //reset validity - now cluster is valid, mark it
      ci.clusters[currClusterIdx].isValid = true;
    }
    else if (!ci.clusters[currClusterIdx].isValid)
    {
      //shift head of the list
      MergeCandidate& mc = *headCandidate;
      FindBestNeighbour(hd, ci, netListSizes, mc);
      ShiftHeadAccordingToScore(mergeCandidates);
      //reset validity - now cluster is valid, mark it
      ci.clusters[currClusterIdx].isValid = true;
    }
    else
    {//we shouldn't merge this pair of clusters - they are too fat
      mergeCandidates.pop_front();
    }
  }

  ci.clusteringLog.push_back(thisLevelClusteringLog);
  delete[] netListSizes;
}

void MarkClustersAsValid(ClusteringInformation& ci)
{
  for (int i = 0; i < static_cast<int>(ci.clusters.size()); ++i)
  {
    ci.clusters[i].isValid = true;
  }
}

string GetClusteringInformationFileName(HDesign& hd)
{
  string fileName = hd.cfg.ValueOf("Clustering.clusteringInformationLoadFileName");
  if (fileName == "")
  {
    fileName = ".\\ClusteringInformation\\" + hd.Circuit.Name() + ".ci";
  }
  return fileName;
}

int Clustering(HDesign& hd, ClusteringInformation& ci)
{
  WRITELINE("");
  ALERT("CLUSTERING STARTED");

  ci.mCurrentNumberOfClusters = hd.Cells.MovableCellsCount();
  ci.clusters.resize(ci.mCurrentNumberOfClusters);
  
  if (hd.cfg.ValueOf("Clustering.useClusteringInformationFromFile", false))
  {//LOAD CLUSTERING INFO AND ESCAPE
    string fileName = GetClusteringInformationFileName(hd);
    if (ci.LoadFromFile(fileName.c_str(), hd.Circuit.Name().c_str(), hd))
    {
      InitializeTerminalsAndPrimaryPins(hd, ci);
      ALERT("CLUSTERING INFORMATION LOADED");
      return OK;
    }
  }

  //PERFORM CLUSTERING
  InitializeClusters(hd, ci);
  InitializeNetList(hd, ci);
  CreateTableOfAdjacentNets(hd, ci);

  PrintClusteringInfo(hd, ci, 0);
  while (ci.mCurrentNumberOfClusters > ci.mDesiredFinalNumberOfClusters)
  {
    MergeClusters(hd, ci);

    PurifyNetList(hd, ci);
    ci.netLevels.push_back(ci.netList);
    CreateTableOfAdjacentNets(hd, ci);

    ci.mCurrentNumberOfClusters = CalculateNumberOfActiveClusters(ci);
    MarkClustersAsValid(ci);

    PrintClusteringInfo(hd, ci, 0);
  }
  ALERT("CLUSTERING FINISHED");
  ALERTFORMAT(("The number of levels is %u", ci.netLevels.size()));

  //EXPORT CLUSTERING
  string fileName = GetClusteringInformationFileName(hd);
  ci.SaveToFile(fileName.c_str(), hd.Circuit.Name().c_str(), hd);
  ALERT("CLUSTERING INFORMATION EXPORTED");
  
  return OK;
}





