#include "Clustering.h"

#include <vector>
#include "math.h"
#include <stdlib.h>
#include <conio.h>

//TODO: do smth with this trash
#define MARK_NEIGHBORS_INVALID true
#define DONT_MARK_NEIGHBORS_INVALID false


//inline int CellToClusterIdx(ClusteringInformation& ci, HCell cell)
//{
//    return ::ToID(cell) - 1;
//}


void InitializeClusters(HDesign& hd, ClusteringInformation& ci)
{
    ASSERT(hd.Cluster.ClustersCount() >= hd.Cells.PlaceableCellsCount());
    //put clusters
    int clusterIdx = 0;
    HClusters::ClustersEnumeratorW cluster = hd.Cluster.GetEnumeratorW();
    cluster.MoveNext();
    for (HCells::PlaceableCellsEnumeratorW cell = hd.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
    {
        cluster.Cells()->push_back(cell);
        cell.SetCluster(cluster);
        cluster.SetArea(cell.Width() * cell.Height());
        cluster.SetIsPrimary(false);
        cluster.SetIsTerminals(false);
        cluster.SetIsValid(true);
        cluster.SetIsFake(false);

        //ASSERT(CellToClusterIdx(ci, cell) == clusterIdx);
        cluster.MoveNext();
    }
}

HCluster AssignClusterIdx(HDesign& hd, ClusteringInformation& ci, HPinWrapper& pin)
{
    HCluster clusterIdx;

    if (pin.IsPrimary())
    {
        //clusterIdx = SHIFT_NUMBER_FOR_PRIMARY_PINS + (int)ci.primaryPins.size();   
        clusterIdx = hd.Cluster.AllocateClusterUnreal();
        HClusterWrapper clust = hd[clusterIdx];
        clust.Setid((int)ci.primaryPins.size());        
        clust.SetIsPrimary(true);
        clust.SetIsTerminals(false);
        clust.SetIsValid(true);
        clust.SetIsFake(false);
        ci.primaryPins.push_back(pin);
    }
    else if (hd.GetBool<HCell::IsTerminal>(pin.Cell()))
    {
        //clusterIdx = SHIFT_NUMBER_FOR_TERMINALS + (int)ci.terminalCells.size();        
        clusterIdx = hd.Cluster.AllocateClusterUnreal();
        HClusterWrapper clust = hd[clusterIdx];
        clust.SetIsPrimary(false);
        clust.SetIsTerminals(true);
        clust.SetIsValid(true);
        clust.SetIsFake(false);
        clust.Setid((int)ci.terminalCells.size());
        ci.terminalCells.push_back(pin.Cell());
    }
    else
    {
        clusterIdx = hd[pin.Cell()].Cluster();
    }

    return clusterIdx;
}

void MoveValueToTheFirstPosition(std::vector<HCluster>& vector, HCluster value)
{
    std::vector<HCluster>::iterator iter = find(vector.begin(), vector.end(), value);
    ASSERT(iter != vector.end());
    vector.erase(iter);
    vector.insert(vector.begin(), value);
}

void AssignClusters(HDesign& design, ClusteringInformation& ci, HNetWrapper& net, HClusteredNet netIdx)
{
    std::vector<HCluster>& idxs = (*design[netIdx].clusterIdxs());

    //put pins (we also add terminals and primary pins for LogSumExp calculation)
    idxs.resize(net.PinsCount());
    int i = 0;
    HCluster sourceIdx; //WARNING: be careful with this source
    for (HNet::PinsEnumeratorW pin = net.GetPinsEnumeratorW(); pin.MoveNext(); ++i)
    {
        idxs[i] = AssignClusterIdx(design, ci, pin);
        if (i == 0) sourceIdx = idxs[i];
    }

    //remove duplicates
    sort(idxs.begin(), idxs.end());
    std::vector<HCluster>::iterator iter = unique(idxs.begin(), idxs.end());
    idxs.resize(iter - idxs.begin());

    //move source to the first position
    MoveValueToTheFirstPosition(idxs, sourceIdx); 
}

void AssignWeightForClusteredNet(HDesign& hd, ClusteringInformation& ci, HClusteredNet netIdx)
{
  double maxWeight = 0.0;
  //find original nets connected to all clusters in this clustered net
  HClusteredNetWrapper netIdxW = hd[netIdx];
  for (int i = 0; i < static_cast<int>(netIdxW.clusterIdxs()->size()); i++)
  {
    HClusterWrapper currClusterIdx = hd[(*netIdxW.clusterIdxs())[i]];

    if ((currClusterIdx.IsPrimary() || currClusterIdx.IsTerminals()) || currClusterIdx.IsFake())
      continue;

    //for all cells in this cluster ci.clusters[clusterIdx].isFake
    for (std::vector<HCell>::iterator cell = currClusterIdx.Cells()->begin(); 
        cell <  currClusterIdx.Cells()->end(); cell++)
    //for (int j = 0; j < currClusterIdx.Cells().size(); j++)
    {
      HCell currCell = *cell;

      for (HCell::PinsEnumerator pe = hd.Get<HCell::Pins, HCell::PinsEnumerator>(currCell);
           pe.MoveNext(); )
      {
        HNet currNet = hd.Get<HPin::Net, HNet>(pe);
        double netWeight = hd.GetDouble<HNet::Weight>(currNet);

        if (netWeight > maxWeight)
          maxWeight = netWeight;
      }
    }
  }

  ASSERT(maxWeight > 0);
    netIdxW.Setweight(maxWeight);
  //ci.netList[netIdx].weight = maxWeight;
}

void WriteWeightsToClusteredNets(HDesign& hd, ClusteringInformation& ci)
{
  HClusteredNets::ClusteredNetsEnumeratorW netIdx = hd.ClustersNetList.GetEnumeratorW();
  netIdx.MoveNext();

  for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); netIdx.MoveNext())
  {
    netIdx.Setweight(net.Weight());
    //ci.netList[netIdx].weight = net.Weight();
    //AssignWeightForClusteredNet(hd, ci, netIdx);
  }
}

void InitializeNetList(HDesign& hd, ClusteringInformation& ci)
{
    //ci.netList.resize(hd.Nets.Count(NetKind_Active));
    hd.ClustersNetList = (*hd.NetLevels.AllocateNetLevel());
    hd.ClustersNetList.Resize(hd.Nets.Count(NetKind_Active));
    //put nets
    HClusteredNets::ClusteredNetsEnumeratorW netIdx = hd.ClustersNetList.GetEnumeratorW();
    netIdx.MoveNext();

    for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); netIdx.MoveNext())
    {
        AssignClusters(hd, ci, net, netIdx);
    }

    WriteWeightsToClusteredNets(hd, ci);
    hd.ClustersNetList = (*hd.NetLevels.CreateCopy());
    //ci.netLevels.push_back(ci.netList);
}

//we add terminals and primary pins for LogSumExp calculation
//used only when if clustering is loaded from file
void InitializeTerminalsAndPrimaryPins(HDesign& hd, ClusteringInformation& ci)
{
    for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
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
    ALERT("Currently there are %d clusters", ci.mCurrentNumberOfClusters);

    int upLimit = ci.mCurrentNumberOfClusters;
    if (nClusters >= 0)
        upLimit = nClusters;
    int i = 0;
    for (HClusters::ClustersEnumeratorW cluster = hd.Cluster.GetEnumeratorW(); cluster.MoveNext() && (i < upLimit); i++)
    {
        ALERT("cluster %d:\t", i);
        if (cluster.IsFake())
        {
            ALERT("FAKE");
            continue;
        }
        else
        {
            for (std::vector<HCell>::iterator cell = cluster.Cells()->begin(); cell <  cluster.Cells()->end(); cell++)
            {
                ALERT(hd[(*cell)].Name().c_str());
            }
        }
    }
}

void CreateTableOfAdjacentNets(HDesign& hd, ClusteringInformation& ci)
{
    //prepare table
    //ci.tableOfAdjacentNets.clear();
    //ci.tableOfAdjacentNets.resize(hd.Cluster.ClustersCount());
    hd.Cluster.ClearTableOfAdjacentNets();
    //fill the table
    for (HClusteredNets::ClusteredNetsEnumeratorW netIdx = hd.ClustersNetList.GetEnumeratorW(); netIdx.MoveNext(); )
    //for (int netIdx = 0; netIdx < (int)ci.netList.size(); ++netIdx)
    {

        for (int j = 0; j < (int)netIdx.clusterIdxs()->size(); ++j)
        {
            HClusterWrapper clusterIdx = hd[(*netIdx.clusterIdxs())[j]];
            if (!clusterIdx.IsTerminals() && !clusterIdx.IsPrimary() && !clusterIdx.IsFake())
            
                clusterIdx.tableOfAdjacentNets()->push_back(netIdx);

                //ci.tableOfAdjacentNets[::ToID(clusterIdx) - 1].push_back(netIdx);
        }
    }

    //remove duplicated nets
    for (HClusters::ClustersEnumeratorW i = hd.Cluster.GetEnumeratorW(); i.MoveNext();)
    //for (unsigned int i = 0; i < ci.tableOfAdjacentNets.size(); i++)
    {
        std::sort(i.tableOfAdjacentNets()->begin(), i.tableOfAdjacentNets()->end());
        ConnectionsVector::iterator newEnd
            = unique(i.tableOfAdjacentNets()->begin(), i.tableOfAdjacentNets()->end());
        i.tableOfAdjacentNets()->resize(newEnd - i.tableOfAdjacentNets()->begin());
    }
}

//NOTE: you have to set initial value -1 for clusterIdx if
//you want to go through all clusters
bool GetNextActiveClusterIdx(ClusteringInformation* ci, /*ref*/ int& clusterIdx)
{
    //CHECK:what about terminals and primary pins?
    //do
    //{
    //    ++clusterIdx;
    //} while (clusterIdx < (int)ci->clusters.size() && ci->clusters[clusterIdx].isFake);
    return false;//clusterIdx < (int)ci->clusters.size();
}

double AffinityLite(HDesign& hd, std::vector<HClusteredNet>& a, std::vector<HClusteredNet>& b, 
                    HCluster& ac, HCluster& bc)
{
    double result = 0.0;
    long size1 = static_cast<int>(a.size());
    long size2 = static_cast<int>(b.size());
    long pos1=0;
    long pos2=0;

    while (pos1 < size1 && pos2 < size2)
    {
        if (a[pos1] < b[pos2])
            ++pos1;
        else if (a[pos1] > b[pos2])
            ++pos2;
        else
        {
            if (hd[a[pos1]].clusterIdxs()->size() > 1)
                result += hd._Design.NetList.netWeight[pos1] / (( hd[a[pos1]].clusterIdxs()->size() - 1) *
                hd[ac].Area() * hd[bc].Area());
            ++pos1;
            ++pos2;
        }
    }
    return result;
}

//double Affinity(HDesign& hd, HCluster& firstClusterIdx, HCluster& secondClusterIdx, 
//                std::vector<Cluster>& clusters, HClusteredNets* netList, 
//                std::vector<ConnectionsVector>& currTableOfAdjacentNets)
//{
//    return AffinityLite(hd, currTableOfAdjacentNets[::ToID(firstClusterIdx) - 1],
//        currTableOfAdjacentNets[::ToID(secondClusterIdx) - 1],
//        firstClusterIdx,
//        secondClusterIdx);
//}

//void CalculateNetListSizes(HClusteredNets* netList, int* netListSizes)
//{
//    //int netListSize = (int)netList.size();
//    //for (int i = 0; i < netListSize; i++)
//    //    netListSizes[i] = (int)netList[i].clusterIdxs.size();
//}

bool AreClustersMergeable(ClusteringInformation& ci, HCluster clusterIdx, HCluster neighborCandidateIdx)
{
    if (clusterIdx == neighborCandidateIdx) 
        return false;

    //WARNING: do not join following 2 is statement, because we have
    //to call IsMovableCell() first
    if (ci.design[neighborCandidateIdx].IsTerminals() || ci.design[neighborCandidateIdx].IsPrimary()) return false;
    return !ci.design[neighborCandidateIdx].IsFake();
}

int FindBestNeighbour(HDesign& hd, ClusteringInformation& ci, MergeCandidate& mergeCandidate)
{
    mergeCandidate.score = 0.0;
    //IntVector& mrgNets = ci.tableOfAdjacentNets[::ToID(mergeCandidate.clusterIdx) - 1];
    for(std::vector<HClusteredNet>::iterator j = hd[mergeCandidate.clusterIdx].tableOfAdjacentNets()->begin(); 
        j != hd[mergeCandidate.clusterIdx].tableOfAdjacentNets()->end(); ++j)
    {
        std::vector<HCluster>& mrgClusters = (*hd[(*j)].clusterIdxs()); //ci.netList[*j].clusterIdxs;
        for (std::vector<HCluster>::iterator k = mrgClusters.begin(); k != mrgClusters.end(); k++)
        {
            if (!AreClustersMergeable(ci, mergeCandidate.clusterIdx, *k)) continue;

            //double score = Affinity(hd, mergeCandidate.clusterIdx, *k, ci.clusters, ci.netList, 
            //                        netListSizes, ci.tableOfAdjacentNets);

            double score = AffinityLite(hd, 
                (*hd[mergeCandidate.clusterIdx].tableOfAdjacentNets()),
                (*hd[(*k)].tableOfAdjacentNets()),
                //ci.tableOfAdjacentNets[::ToID(mergeCandidate.clusterIdx) - 1],
                //ci.tableOfAdjacentNets[::ToID(*k) - 1],                
                mergeCandidate.clusterIdx,
                *k);

            if (mergeCandidate.score < score)
            {
                ASSERT(score > 0.0);
                mergeCandidate.score = score;
                mergeCandidate.bestNeighborIdx = (*k);
            }
        }
    }
    return OK;
}

int MarkNeighboursAsInvalid(ClusteringInformation& ci, HCluster clusterIdx)
{
    //IntVector& mrgNets = ci.tableOfAdjacentNets[clusterIdx];
    //for(IntIterator j = mrgNets.begin(); j != mrgNets.end(); ++j)
    for(std::vector<HClusteredNet>::iterator j = ci.design[clusterIdx].tableOfAdjacentNets()->begin(); 
        j != ci.design[clusterIdx].tableOfAdjacentNets()->end(); ++j)
    {
        std::vector<HCluster>& mrgClusters = (*ci.design[(*j)].clusterIdxs()); // ci.netList[*j].clusterIdxs;
        for (std::vector<HCluster>::iterator k = mrgClusters.begin(); k != mrgClusters.end(); ++k)
            if ((clusterIdx != *k) && (!ci.design[(*k)].IsTerminals() && !ci.design[(*k)].IsPrimary()))
                ci.design[(*k)].SetIsValid(false);
    }
    return OK;
}

int CalculateNumberOfActiveClusters(ClusteringInformation& ci)
{
    int nActiveClusters = 0;
    int clusterIdx = 0;
    bool f = 0;
    for (HClusters::ClustersNotFakeEnumeratorW cluster = ci.design.Cluster.GetNotFakeEnumeratorW(); cluster.MoveNext();)
    //for (HClusters::ClustersEnumeratorW cluster = ci.design.Cluster.GetEnumeratorW(); cluster.MoveNext(); clusterIdx++)
    {
        
        /*while (cluster.IsFake() && !f)
        {
            if (!cluster.MoveNext())
                f = 1;
            clusterIdx++;
        }*/
    //while (GetNextActiveClusterIdx(&ci, clusterIdx))
        //if (!f)
        nActiveClusters++;
    }
    return nActiveClusters;
}

void UnclusterLevelUp(HDesign& hd, ClusteringInformation& ci, ClusteringLogIterator& clusteringLogIterator)
{
    std::list<MergedCluster>::reverse_iterator iter = clusteringLogIterator->rbegin();

    for (; iter != clusteringLogIterator->rend(); ++iter)
    {
        HClusterWrapper cluster1Idx = hd[iter->cluster1Idx];
        HClusterWrapper cluster2Idx = hd[iter->cluster2Idx];
        cluster1Idx.Cells()->resize(iter->nCellsInCluster1);
        cluster1Idx.SetArea(cluster1Idx.Area() - cluster2Idx.Area());
        cluster2Idx.SetIsFake(false);
        cluster2Idx.SetXCoord(cluster1Idx.XCoord());
        cluster2Idx.SetYCoord(cluster1Idx.YCoord());
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

void MergeCurrTableOfAdjacentNets(HDesign& design, 
                                  const HCluster cluster1Idx, const HCluster cluster2Idx)
{
    
    ConnectionsVector tempMergedVector(design[cluster1Idx].tableOfAdjacentNets()->size() 
        + design[cluster2Idx].tableOfAdjacentNets()->size());

    merge(design[cluster1Idx].tableOfAdjacentNets()->begin(), design[cluster1Idx].tableOfAdjacentNets()->end(), 
        design[cluster2Idx].tableOfAdjacentNets()->begin(), design[cluster2Idx].tableOfAdjacentNets()->end(),
        tempMergedVector.begin());

    ConnectionsVector::iterator newEnd = unique(tempMergedVector.begin(), tempMergedVector.end());
    design[cluster1Idx].tableOfAdjacentNets()->resize(newEnd - tempMergedVector.begin());
    copy(tempMergedVector.begin(), newEnd, design[cluster1Idx].tableOfAdjacentNets()->begin());
}

void MergeCellIdxs(ClusteringInformation& ci, HCluster clusterIdx1, HCluster clusterIdx2)
{
    int size1 = ci.design[clusterIdx1].Cells()->size();
    int size2 = ci.design[clusterIdx2].Cells()->size();

    std::vector<HCell> tempVector;
    tempVector.resize(size1 + size2);
    copy(ci.design[clusterIdx1].Cells()->begin(), ci.design[clusterIdx1].Cells()->end(), tempVector.begin());
    copy(ci.design[clusterIdx2].Cells()->begin(), ci.design[clusterIdx2].Cells()->end(), tempVector.begin() + size1);

    ci.design[clusterIdx1].Cells()->resize(size1 + size2);
    copy(tempVector.begin(), tempVector.end(), ci.design[clusterIdx1].Cells()->begin());
}

void UpdateNetList(HDesign& design, HClusteredNets* netList, 
                   HCluster firstClusterIdx, HCluster secondClusterIdx)
{
    std::vector<HCluster>::iterator iter;
    for (int j = 0; j < static_cast<int>(design[secondClusterIdx].tableOfAdjacentNets()->size()); ++j)
    {
        HClusteredNetWrapper netIdx = netList->MakeWrapper((*design[secondClusterIdx].tableOfAdjacentNets())[j]);
        std::vector<HCluster>& clusterIdxs = (*netIdx.clusterIdxs());

        if (clusterIdxs.size() == 0)
            continue;

        //rewrite second cluster idx with first cluster idx
        iter = find(clusterIdxs.begin(), clusterIdxs.end(), secondClusterIdx);
        ASSERT(iter != clusterIdxs.end());
        (*iter) = firstClusterIdx;

        //save head of the vector
        HCluster head = clusterIdxs[0];

        //remove duplicates
        sort(clusterIdxs.begin(), clusterIdxs.end());
        iter = unique(clusterIdxs.begin(), clusterIdxs.end());
        clusterIdxs.resize(iter - clusterIdxs.begin());

        //restore head
        MoveValueToTheFirstPosition(clusterIdxs, head);

        //clear net if it is totally inside cluster
        //if (netList[netIdx].clusterIdxs.size() == 1)
        //{
        //  netList[netIdx].clusterIdxs.clear();
        //}
    }
}

// the result is stored in clusterIdx1
int MergePairOfClusters(HCluster cluster1Idx, HCluster cluster2Idx, 
                        ClusteringInformation& ci, HClusteredNets* netList)
{
    MergeCurrTableOfAdjacentNets(ci.design, cluster1Idx, cluster2Idx);
    MergeCellIdxs(ci, cluster1Idx, cluster2Idx);
    UpdateNetList(ci.design, netList, cluster1Idx, cluster2Idx);

    ci.design[cluster1Idx].SetXCoord((ci.design[cluster1Idx].XCoord() * ci.design[cluster1Idx].Area() + 
        ci.design[cluster2Idx].XCoord() * ci.design[cluster2Idx].Area()) / 
        (ci.design[cluster1Idx].Area() + ci.design[cluster2Idx].Area()));

    ci.design[cluster1Idx].SetYCoord((ci.design[cluster1Idx].YCoord() * ci.design[cluster1Idx].Area() + 
        ci.design[cluster2Idx].YCoord() * ci.design[cluster2Idx].Area()) / 
        (ci.design[cluster1Idx].Area() + ci.design[cluster2Idx].Area()));
    ci.design[cluster1Idx].SetArea(ci.design[cluster1Idx].Area() + ci.design[cluster2Idx].Area());
    ci.design[cluster2Idx].SetIsFake(true);

    return OK;
}

bool PredicateMergePairGreater(MergeCandidate elem1, MergeCandidate elem2)
{
    return elem1.score > elem2.score;
}

bool IsEqualNetListBinaryPredicate(HClusteredNetWrapper& elem1, HClusteredNetWrapper& elem2)
{
    int size1 = static_cast<int>(elem1.clusterIdxs()->size());
    int size2 = static_cast<int>(elem2.clusterIdxs()->size());

    if (size1 != size2)
        return false;

    for (int i = 0; i < size2; ++i)
    {
        if ((*elem1.clusterIdxs())[i] != (*elem2.clusterIdxs())[i])
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
    mergeCandidates.resize(ci.design.Cluster.ClustersCount());

    std::list<MergeCandidate>::iterator mcIter;
    int clusterIdx = 0;
    HClusters::ClustersEnumeratorW en = ci.design.Cluster.GetEnumeratorW();
    en.MoveNext();
    for (mcIter = mergeCandidates.begin(); mcIter != mergeCandidates.end(); mcIter++)
    {
        mcIter->clusterIdx = en;
        mcIter->bestNeighborIdx = ci.design.Cluster.Null(); 
        mcIter->score = 0.0;
        clusterIdx++;
        en.MoveNext();
    }
}

void CalculateScores(HDesign& hd, ClusteringInformation& ci, 
                     std::list<MergeCandidate>& mergeCandidates)
{
    std::list<MergeCandidate>::iterator mcIter;
    for (mcIter = mergeCandidates.begin(); mcIter != mergeCandidates.end(); mcIter++)
    {
        MergeCandidate& mc = *mcIter;
        FindBestNeighbour(hd, ci, mc);
    }
}

void InitializeClustersData(ClusteringInformation& ci, std::vector<MergeCandidate>& clustersData)
{
    int i = 0;
    for (HClusters::ClustersEnumeratorW en = ci.design.Cluster.GetEnumeratorW(); en.MoveNext(); i++)
    //for (int i = 0; i < static_cast<int>(ci.clusters.size()); ++i)
    {
        clustersData[i].clusterIdx = en;
        clustersData[i].score = 0.0;
    }
}

void CalculateScores(HDesign& hd, ClusteringInformation& ci, 
                     std::vector<MergeCandidate>& clustersData)
{
    InitializeClustersData(ci, clustersData);

    std::vector<MergeCandidate>::iterator mcIter;
    for (mcIter = clustersData.begin(); mcIter != clustersData.end(); mcIter++)
    {
        MergeCandidate& mc = *mcIter;
        FindBestNeighbour(hd, ci, mc);
    }
}

//HClusteredNets::ClusteredNetsEnumeratorW FindInVector(HClusteredNets::ClusteredNetsEnumeratorW start, 
//                               HClusteredNets::ClusteredNetsEnumeratorW end, 
//                               HClusteredNetWrapper item)
//{
//    HClusteredNets::ClusteredNetsEnumeratorW iter = start;
//    while (iter != end && !IsEqualNetListBinaryPredicate(iter, item))
//    {
//        ++iter;
//    }
//    return iter;
//}

void Swap(HClusteredNet& i1, HClusteredNet& i2)
{
    HClusteredNet tmp;
    tmp = i1;
    i1 = i2;
    i2 = tmp;
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
                                 std::list<MergeCandidate>& mergeCandidates)
{
    InitializeMergeCandidates(ci, mergeCandidates);
    CalculateScores(hd, ci, mergeCandidates);
    mergeCandidates.sort(PredicateMergePairGreater);
    EraseZeroScorePairs(mergeCandidates);
}

bool IsMergedAreaAcceptable(ClusteringInformation &ci, HCluster currClusterIdx, HCluster bestNeighborIdx, double maxClusterArea)
{
    //ASSERT(bestNeighborIdx >= 0);
    return ci.design[currClusterIdx].Area() + ci.design[bestNeighborIdx].Area() <= maxClusterArea;
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

    //int* netListSizes = new int[ci.netList.size()];
    //CalculateNetListSizes(ci.netList, netListSizes);

    CreateListOfMergeCandidates(hd, ci, mergeCandidates);

    //PrintMergeCandidates(mergeCandidates);

    //main loop of merging
    int mergedCount = 0;
    while (mergeCandidates.size() && ci.mCurrentNumberOfClusters - mergedCount > targetNClusters)
    {
        if (mergedCount == 244)
            int yy = 0;
        std::list<MergeCandidate>::iterator headCandidate = mergeCandidates.begin();
        HCluster currClusterIdx  = headCandidate->clusterIdx;
        HCluster bestNeighborIdx = headCandidate->bestNeighborIdx;

        if (hd[currClusterIdx].IsValid() && 
            IsMergedAreaAcceptable(ci, currClusterIdx, bestNeighborIdx, maxClusterArea)) 
        {//merge
            MergePairOfClusters(currClusterIdx, bestNeighborIdx, ci, 
                &ci.design.ClustersNetList);
            mergedCount++;

            //save merge info to the log
            MergedCluster mergedCluster;
            mergedCluster.cluster1Idx = currClusterIdx;
            mergedCluster.cluster2Idx = bestNeighborIdx;
            mergedCluster.nCellsInCluster1 = hd[currClusterIdx].Cells()->size();
            thisLevelClusteringLog.push_back(mergedCluster);

            //CalculateNetListSizes(ci.netList, netListSizes);

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
            FindBestNeighbour(hd, ci, mc);
            ShiftHeadAccordingToScore(mergeCandidates);
            //reset validity - now cluster is valid, mark it
            hd[currClusterIdx].SetIsValid(true);
        }
        else if (!hd[currClusterIdx].IsValid())
        {
            //shift head of the list
            MergeCandidate& mc = *headCandidate;
            FindBestNeighbour(hd, ci, mc);
            ShiftHeadAccordingToScore(mergeCandidates);
            //reset validity - now cluster is valid, mark it
            hd[currClusterIdx].SetIsValid(true);
        }
        else
        {//we shouldn't merge this pair of clusters - they are too fat
            mergeCandidates.pop_front();
        }
    }

    ci.clusteringLog.push_back(thisLevelClusteringLog);
    //delete[] netListSizes;
}

void MarkClustersAsValid(ClusteringInformation& ci)
{
    for (HClusters::ClustersEnumeratorW en = ci.design.Cluster.GetEnumeratorW(); en.MoveNext();)
    //for (int i = 0; i < static_cast<int>(ci.clusters.size()); ++i)
    {
        en.SetIsValid(true);
        //ci.clusters[i].isValid = true;
    }
}



int Clustering(HDesign& hd, ClusteringInformation& ci)
{
    WRITELINE("");
    ALERT("CLUSTERING STARTED");

    ci.mCurrentNumberOfClusters = hd.Cells.MovableCellsCount();
    ci.design.Cluster.Resize(ci.mCurrentNumberOfClusters);
    if (hd.cfg.ValueOf(".Clustering.useClusteringInformationFromFile", false))
    {//LOAD CLUSTERING INFO AND ESCAPE
        string fileName = ci.GetClusteringInformationFileName(hd);
        if (ci.LoadFromFile(fileName.c_str(), hd.Circuit.Name().c_str(), hd))
        {
            InitializeTerminalsAndPrimaryPins(hd, ci);
            ALERT("CLUSTERING INFORMATION LOADED");
            goto ClusteringExport;
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

        hd.NetLevels.PurifyNetList();
        //ci.netLevels.push_back(ci.netList);
        CreateTableOfAdjacentNets(hd, ci);

        ci.mCurrentNumberOfClusters = CalculateNumberOfActiveClusters(ci);
        MarkClustersAsValid(ci);

        PrintClusteringInfo(hd, ci, 0);
    }
    hd.ClustersNetList = (*hd.NetLevels.RewritePastCopy());
    hd.NetLevels.GetNetLelev()->pop_back();
    ALERT("CLUSTERING FINISHED");
    ALERT("The number of levels is %u", ci.design.NetLevels.NetLevelsCount());

ClusteringExport:
    //EXPORT CLUSTERING
    if (ci.design.NetLevels.NetLevelsCount() > 1)
    {//do not export 1 level clustering
        string fileName = ci.GetClusteringInformationFileName(hd);
        ci.SaveToFile(fileName.c_str(), hd.Circuit.Name().c_str(), hd);
        ALERT("CLUSTERING INFORMATION EXPORTED");
    }

    return OK;
}