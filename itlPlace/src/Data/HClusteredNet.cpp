#include "HDesign.h"

COLLECTIONCONSTRUCTOR(HClusteredNets)

//properties specializations
//PROPERTYA(std::vector<HCell>*, HCluster::Cells, m_ld->Clusterization.cells)
GETTERA(HClusteredNets, std::vector<HCluster>*, HClusteredNet::clusterIdxs, m_ld->Clusterization.clusterIdxs[id])
PROPERTYA(HClusteredNets, double, HClusteredNet::weight, m_ld->Clusterization.weight[id])
PROPERTYA(HClusteredNets, double, HClusteredNet::k, m_ld->Clusterization.k[id])
PROPERTYA(HClusteredNets, int,   HClusteredNet::idNet, m_ld->Clusterization.idNet[id])

void HClusteredNets::Initialize(int clusteredNetsLimit, int idLevel)
{
    CHECKIFINITIALIZED();
    id = idLevel;
    this->Grow(clusteredNetsLimit);

    m_ld->Clusterization.clusterIdxs[id][0]->resize(0);
    m_ld->Clusterization.weight[id][0] = false;
    m_ld->Clusterization.k[id][0] = false;
    m_ld->Clusterization.idNet[id][0] = -1;
}

void HClusteredNets::Grow(int clusteredNetsLimit)
{
    int old_size = m_ld->Clusterization.nClusteredNetLimit[id];

   
    if (clusteredNetsLimit > old_size)
    {
            
        std::vector<HCluster>** newCluster = new std::vector<HCluster>*[clusteredNetsLimit];
        
        for (int i = 0; i < old_size; i++)
        {
            newCluster[i] = m_ld->Clusterization.clusterIdxs[id][i];
        }
        for (int i = old_size; i < clusteredNetsLimit; i++)
        {
            newCluster[i] = new std::vector<HCluster>(0);
        }
        
        if (m_ld->Clusterization.clusterIdxs[id] != 0)
            delete []m_ld->Clusterization.clusterIdxs[id];
        m_ld->Clusterization.clusterIdxs[id] = newCluster;       
        
    }
    //::Grow(&m_ld->Clusterization.cells, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.weight[id], old_size, clusteredNetsLimit);
    ::Grow(&m_ld->Clusterization.k[id], old_size, clusteredNetsLimit);
    ::Grow(&m_ld->Clusterization.idNet[id], old_size, clusteredNetsLimit);

    m_ld->Clusterization.nClusteredNetLimit[id] = clusteredNetsLimit;
    if (m_ld->Clusterization.maxNClusteredNetLimit < clusteredNetsLimit)
        m_ld->Clusterization.maxNClusteredNetLimit = clusteredNetsLimit;
}

void HClusteredNets::Swap(HClusteredNet& first, HClusteredNet& second)
{
    BeforeSwap(first, second);

    ::Swap(m_ld->Clusterization.clusterIdxs[id] + ::ToID(first),                      m_ld->Clusterization.clusterIdxs[id] + ::ToID(second));
    ::Swap(m_ld->Clusterization.weight[id] + ::ToID(first),                     m_ld->Clusterization.weight[id] + ::ToID(second));
    ::Swap(m_ld->Clusterization.k[id] + ::ToID(first),                    m_ld->Clusterization.k[id] + ::ToID(second));
    ::Swap(m_ld->Clusterization.idNet[id] + ::ToID(first),                   m_ld->Clusterization.idNet[id] + ::ToID(second));

    ::Swap(&first, &second);
}

HClusteredNet HClusteredNets::AllocateClusteredNet()
{
    int ind = m_ld->Clusterization.nClusteredNetEnd[id];
    while (ind >= m_ld->Clusterization.nClusteredNetLimit[id])
        Grow(m_ld->Clusterization.nClusteredNetLimit[id] + 100);//GetNewSize(m_hd->cfg, m_ld->NetList.nCellsLimit, "data.cellsLimitGrow", 128, 0, 1.25));

    m_ld->Clusterization.nClusteredNetEnd[id]++;
    HClusteredNet res = ::__ConstructPtr<HClusteredNet>(ind);

    OnClusteredNetAllocation(res);

    return res;
}
HClusteredNet HClusteredNets::GetClusteredNetByIndex(int idnet)
{
    return ::__ConstructPtr<HClusteredNet>(idnet);
}


void HClusteredNets::Resize(int newSize)
{
    if (m_ld->Clusterization.nClusteredNetLimit[id] < newSize)
    {
        Grow(m_ld->Clusterization.nClusteredNetLimit[id] + newSize + newSize / 2);
    }
    m_ld->Clusterization.nClusteredNetEnd[id] = newSize + 1;
}
