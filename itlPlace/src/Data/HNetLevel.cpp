#include "HDesign.h"

//properties specializations
//PROPERTYA(std::vector<HCell>*, HCluster::Cells, m_ld->Clusterization.cells)
PROPERTYA(HNetLevels, double, HNetLevel::weight, m_ld->Clusterization.weight[0])
PROPERTYA(HNetLevels, double, HNetLevel::k, m_ld->Clusterization.k[0])
PROPERTYA(HNetLevels, int,   HNetLevel::idNet, m_ld->Clusterization.idNet[0])

void HNetLevels::Initialize(int NetLevelLimit)
{
    CHECKIFINITIALIZED();
    m_ld->Clusterization.netLevels = new std::vector<HClusteredNets*>(0);
    this->Grow(NetLevelLimit);

    //m_ld->Clusterization.clusterIdxs[0]->resize(0);
    //m_ld->Clusterization.weight[id][0] = false;
    //m_ld->Clusterization.k[id][0] = false;
    //m_ld->Clusterization.idNet[id][0] = -1;
}
void HNetLevels::Grow(int GrowsLimit)
{
    int old_size = m_ld->Clusterization.nNetLevelsLimit;
    if (m_ld->Clusterization.netLevels == 0)
        m_ld->Clusterization.netLevels = new std::vector<HClusteredNets*>(0);

    if (GrowsLimit > old_size)
    {
        std::vector<HCluster>*** newCluster = new std::vector<HCluster>**[GrowsLimit];
        double** weight = new double*[GrowsLimit];
        double** k = new double* [GrowsLimit];
        int** idNet = new int* [GrowsLimit];
        IDType* nClusteredNetEnd = new IDType[GrowsLimit];
        IDType* nClusteredNetLimit = new IDType[GrowsLimit];

        for (int i = 0; i < old_size; i++)
        {
            newCluster[i] = m_ld->Clusterization.clusterIdxs[i];
            weight[i] = m_ld->Clusterization.weight[i];
            k[i] = m_ld->Clusterization.k[i];
            idNet[i] = m_ld->Clusterization.idNet[i];
            nClusteredNetEnd[i] = m_ld->Clusterization.nClusteredNetEnd[i];
            nClusteredNetLimit[i] = m_ld->Clusterization.nClusteredNetLimit[i];
        }
        for (int i = old_size; i < GrowsLimit; i++)
        {
            newCluster[i] = 0;
            weight[i] = 0;
            k[i] = 0;
            idNet[i] = 0;
            nClusteredNetEnd[i] = 0;
            nClusteredNetLimit[i] = 0;
        }

        m_ld->Clusterization.clusterIdxs = newCluster;
        m_ld->Clusterization.weight = weight;
        m_ld->Clusterization.k = k;
        m_ld->Clusterization.idNet = idNet;
        m_ld->Clusterization.nClusteredNetEnd = nClusteredNetEnd;
        m_ld->Clusterization.nClusteredNetLimit = nClusteredNetLimit;

        for (int i = old_size; i < GrowsLimit; i++)
        {
            HClusteredNets* nets = new HClusteredNets(m_hd);
            nets->Initialize(m_ld->Clusterization.maxNClusteredNetLimit, i);
            m_ld->Clusterization.netLevels->push_back(nets);
        }
    }
    m_ld->Clusterization.nNetLevelsLimit = GrowsLimit;
}
void HNetLevels::Swap(HNetLevel* first, HNetLevel* second)
{
    //BeforeSwap(first, second);

    //::Swap(m_ld->Clusterization.netLevels + ::ToID(first), m_ld->Clusterization.netLevels + ::ToID(second));

    //::Swap(&first, &second);
}
HClusteredNets* HNetLevels::AllocateNetLevel()
{
    int ind = m_ld->Clusterization.nNetLevelsEnd;
    while (ind >= m_ld->Clusterization.nNetLevelsLimit)
        Grow(m_ld->Clusterization.nNetLevelsLimit + 1);//GetNewSize(m_hd->cfg, m_ld->NetList.nCellsLimit, "data.cellsLimitGrow", 128, 0, 1.25));

    m_ld->Clusterization.nNetLevelsEnd++;
    HClusteredNets* res = (*m_ld->Clusterization.netLevels)[ind];

    //OnNetLevelAllocation(res);

    return res;
}
void HNetLevels::Resize(int newSize)
{
    if (m_ld->Clusterization.nNetLevelsLimit < newSize)
    {
        Grow(m_ld->Clusterization.nNetLevelsLimit + newSize + newSize / 2);
    }
    m_ld->Clusterization.nNetLevelsEnd = newSize + 1;
}

bool PredicateNetListLess2(HClusteredNetWrapper elem1, HClusteredNetWrapper elem2)
{
    int size1 = static_cast<int>(elem1.clusterIdxs()->size());
    int size2 = static_cast<int>(elem2.clusterIdxs()->size());

    if (size1 < size2)
        return true;
    else if (size1 > size2)
        return false;

    for (int i = 0; i < size1; ++i)
    {
        if ((*elem1.clusterIdxs())[i] < (*elem2.clusterIdxs())[i])
            return true;
        else if ((*elem2.clusterIdxs())[i] < (*elem1.clusterIdxs())[i])
            return false;
    }

    // сортировка по весам в обратном порядке
    // при применении функции unique автоматически будет выбран первый нет
    // при таком способе сортировки у этого нета будет максимальный вес
    if (elem1.weight() > elem2.weight())
        return true;
    else if (elem1.weight() < elem2.weight())
        return false;

    return false;
}
bool IsEqualNetListBinaryPredicate2(HClusteredNetWrapper elem1, HClusteredNetWrapper elem2)
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

void HNetLevels::PurifyNetList()
{
    std::vector<HClusteredNetWrapper> netList(m_hd->ClustersNetList.ClusteredNetCount());
    int i = 0;
    for (HClusteredNets::ClusteredNetsEnumeratorW j = m_hd->ClustersNetList.GetEnumeratorW(); j.MoveNext(); i++)
    {
        netList[i] = j;
    }
    // Points to where the unique vector ends (and the duplicating part starts)
    //HClusteredNets::ClusteredNetsEnumeratorW netListIter;
    string aggregationMethod = m_hd->cfg.ValueOf(".NetWeighting.APlace.aggregationMethod", "max");

    sort(netList.begin(), netList.end(), PredicateNetListLess2);
    ALERT("NetList size before one cluster nets removing: %d", netList.size());
    //delete nets with less than 2 clusters
    std::vector<HClusteredNetWrapper>::iterator netListIter = netList.begin();
    while ( (*m_hd)[(*netListIter)].clusterIdxs()->size() < 2) 
        ++netListIter;
    netList.erase(netList.begin(), netListIter);
    ALERT("NetList size after  one cluster nets removing: %d", netList.size());

    //delete duplicated nets
    if (m_hd->cfg.ValueOf(".Clustering.deleteDuplicatingNets", true))
    {
        // This will point to the last element of unique part of the vector
        std::vector<HClusteredNetWrapper>::iterator lastUniqueIdx = netList.begin();
        if (aggregationMethod == "sum-1")
        {
            ALERT("Aggregation method for net weights inheritance is SUM-1");
            for (netListIter = netList.begin(); netListIter != netList.end(); ++netListIter)
            {
                while (netListIter != netList.end() && 
                    IsEqualNetListBinaryPredicate2((*m_hd)[(*netListIter)], (*m_hd)[(*lastUniqueIdx)]))
                {
                    (*m_hd)[(*lastUniqueIdx)].Setweight((*m_hd)[(*lastUniqueIdx)].weight() + (*m_hd)[(*netListIter)].weight() - 1);
                    ++netListIter;
                }
                if (netListIter != netList.end())
                {
                    *(++lastUniqueIdx) = *netListIter;
                } else
                    break;
            }
            /* Now lastUniqueIdx points to the last unique value, for instance for vector<int>
            was:
            4 5 5 5 5 5 5 6 7 7 7 7 7 9 9 9 9
            now:
            4 5 6 7 9 5 5 6 7 7 7 7 7 9 9 9 9
            ^
            |
            lastUniqueIdx
            so we need to increment it
            */
            ++lastUniqueIdx;
        } else if (aggregationMethod == "sum")
        {
            ALERT("Aggregation method for net weights inheritance is SUM");
            for (netListIter = netList.begin(); netListIter != netList.end(); ++netListIter)
            {
                while (netListIter != netList.end() && 
                    IsEqualNetListBinaryPredicate2((*m_hd)[(*netListIter)], (*m_hd)[(*lastUniqueIdx)]))
                {
                    (*m_hd)[(*lastUniqueIdx)].Setweight((*m_hd)[(*lastUniqueIdx)].weight() + (*m_hd)[(*netListIter)].weight());
                    ++netListIter;
                }
                if (netListIter != netList.end())
                {
                    *(++lastUniqueIdx) = *netListIter;
                } else
                    break;
            }
            /* Now lastUniqueIdx points to the last unique value, for instance for vector<int>
            was:
            4 5 5 5 5 5 5 6 7 7 7 7 7 9 9 9 9
            now:
            4 5 6 7 9 5 5 6 7 7 7 7 7 9 9 9 9
            ^
            |
            lastUniqueIdx
            so we need to increment it
            */
            ++lastUniqueIdx;
        } else
            /* In this case the net with maximum weight will be inherited */
        {
            ALERT("Aggregation method for net weights inheritance is MAX");
            lastUniqueIdx = unique(netList.begin(), netList.end(), IsEqualNetListBinaryPredicate2);
        }
        netList.resize(lastUniqueIdx - netList.begin());
        ALERT("NetList size after duplicates removing: %d", netList.size());
    }
    HClusteredNets* clustersNetList = m_hd->NetLevels.AllocateNetLevel();
    clustersNetList->Resize(netList.size());
    i = 0;
    for (HClusteredNets::ClusteredNetsEnumeratorW j = clustersNetList->GetEnumeratorW(); j.MoveNext(); i++)
    {
        (*j.clusterIdxs()) = (*(*m_hd)[netList[i]].clusterIdxs());
        j.Setweight((*m_hd)[netList[i]].weight());
        j.Setk((*m_hd)[netList[i]].k());
    }
    //m_hd->ClustersNetList = (*clustersNetList);
    RewritePastCopy();
    CreateCopy();
    m_hd->ClustersNetList = (*m_ld->Clusterization.netLevels->back());
}

HClusteredNets* HNetLevels::CreateCopy()
{
    int oldId = (m_ld->Clusterization.netLevels->operator [](m_ld->Clusterization.nNetLevelsEnd - 1))->id;    
    int n = m_ld->Clusterization.nClusteredNetEnd[m_ld->Clusterization.nNetLevelsEnd - 1];
    HClusteredNets* result = AllocateNetLevel();
    result->Resize(n - 1);
    int curId = result->id;
    
    for(int i = 0; i < n; i++)
    {
        (*m_ld->Clusterization.clusterIdxs[curId][i]) = (*m_ld->Clusterization.clusterIdxs[oldId][i]);
        m_ld->Clusterization.weight[curId][i] = m_ld->Clusterization.weight[oldId][i];
        m_ld->Clusterization.k[curId][i] = m_ld->Clusterization.k[oldId][i];
        m_ld->Clusterization.idNet[curId][i] = m_ld->Clusterization.idNet[oldId][i];
    }
    return result;
}
HClusteredNets* HNetLevels::RewritePastCopy()
{
    std::vector<HClusteredNets*>::reverse_iterator netLevelsIterator = m_ld->Clusterization.netLevels->rbegin();
    ++netLevelsIterator;
    HClusteredNets* result = (*netLevelsIterator);
    int n = m_ld->Clusterization.nClusteredNetEnd[m_ld->Clusterization.nNetLevelsEnd - 1];
    result->Resize(n - 1);
    int curId = result->id;
    int oldId = (*m_ld->Clusterization.netLevels->back()).id;

    for(int i = 0; i < n; i++)
    {
        (*m_ld->Clusterization.clusterIdxs[curId][i]) = (*m_ld->Clusterization.clusterIdxs[oldId][i]);
        m_ld->Clusterization.weight[curId][i] = m_ld->Clusterization.weight[oldId][i];
        m_ld->Clusterization.k[curId][i] = m_ld->Clusterization.k[oldId][i];
        m_ld->Clusterization.idNet[curId][i] = m_ld->Clusterization.idNet[oldId][i];
    }

    m_ld->Clusterization.nNetLevelsEnd--;
    //m_ld->Clusterization.netLevels->pop_back();

    return result;
}
