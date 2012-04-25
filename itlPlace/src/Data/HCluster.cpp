#include "HDesign.h"



COLLECTIONCONSTRUCTOR(HClusters)

void HClusters::Initialize(int clusterLimit)
{
    CHECKIFINITIALIZED();
    
    this->Grow(clusterLimit);

    m_ld->Clusterization.area[0] = 0;
    m_ld->Clusterization.cells[0]->resize(0);
    m_ld->Clusterization.tableOfAdjacentNets[0]->resize(0);
    m_ld->Clusterization.isFake[0] = false;
    m_ld->Clusterization.isValid[0] = false;
    m_ld->Clusterization.isTerminals[0] = false;
    m_ld->Clusterization.isPrimary[0] = false;
    m_ld->Clusterization.potentialMultiplier[0] = 0;
    m_ld->Clusterization.xCoord[0] = 0;
    m_ld->Clusterization.yCoord[0] = 0;
    m_ld->Clusterization.id[0] = -1;
    m_ld->Clusterization.clusterIdx2solutionIdxLUT[0] = -1;
    m_ld->Clusterization.isCPoint[0] = -1;
}

void HClusters::Grow(int clusterLimit)
{
    int old_size = m_ld->Clusterization.nClusterLimit;

    ::Grow(&m_ld->Clusterization.area, old_size, clusterLimit);
    
    if (clusterLimit > old_size)
    {
            
        std::vector<HCell>** newCell = new std::vector<HCell>*[clusterLimit];
        std::vector<HClusteredNet>** tableOfAdjacentNets = new std::vector<HClusteredNet>*[clusterLimit];
        for (int i = 0; i < old_size; i++)
        {
            newCell[i] = m_ld->Clusterization.cells[i];
            tableOfAdjacentNets[i] = m_ld->Clusterization.tableOfAdjacentNets[i];
        }
        for (int i = old_size; i < clusterLimit; i++)
        {
            newCell[i] = new std::vector<HCell>(0);
            tableOfAdjacentNets[i] = new std::vector<HClusteredNet>(0);
        }
        
        if (m_ld->Clusterization.cells != 0)
            delete []m_ld->Clusterization.cells;
        m_ld->Clusterization.cells = newCell;

        if (m_ld->Clusterization.tableOfAdjacentNets != 0)
            delete []m_ld->Clusterization.tableOfAdjacentNets;
        m_ld->Clusterization.tableOfAdjacentNets = tableOfAdjacentNets;
        
    }
    //::Grow(&m_ld->Clusterization.cells, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.isFake, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.isValid, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.isTerminals, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.isPrimary, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.potentialMultiplier, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.xCoord, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.yCoord, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.id, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.clusterIdx2solutionIdxLUT, old_size, clusterLimit);
    ::Grow(&m_ld->Clusterization.isCPoint, old_size, clusterLimit);

    m_ld->Clusterization.nClusterLimit = clusterLimit;
}

void HClusters::Swap(HCluster& first, HCluster& second)
{
    BeforeSwap(first, second);

    ::Swap(m_ld->Clusterization.area + ::ToID(first),                      m_ld->Clusterization.area + ::ToID(second));
    ::Swap(m_ld->Clusterization.cells + ::ToID(first),                     m_ld->Clusterization.cells + ::ToID(second));
    ::Swap(m_ld->Clusterization.tableOfAdjacentNets + ::ToID(first),       m_ld->Clusterization.tableOfAdjacentNets + ::ToID(second));

    ::Swap(m_ld->Clusterization.isFake + ::ToID(first),                    m_ld->Clusterization.isFake + ::ToID(second));
    ::Swap(m_ld->Clusterization.isValid + ::ToID(first),                   m_ld->Clusterization.isValid + ::ToID(second));
    ::Swap(m_ld->Clusterization.isTerminals + ::ToID(first),               m_ld->Clusterization.isTerminals + ::ToID(second));
    ::Swap(m_ld->Clusterization.isPrimary + ::ToID(first),                 m_ld->Clusterization.isPrimary + ::ToID(second));
    ::Swap(m_ld->Clusterization.potentialMultiplier + ::ToID(first),       m_ld->Clusterization.potentialMultiplier + ::ToID(second));
    ::Swap(m_ld->Clusterization.xCoord + ::ToID(first),                    m_ld->Clusterization.xCoord + ::ToID(second));
    ::Swap(m_ld->Clusterization.yCoord + ::ToID(first),                    m_ld->Clusterization.yCoord + ::ToID(second));
    ::Swap(m_ld->Clusterization.id + ::ToID(first),                        m_ld->Clusterization.id + ::ToID(second));
    ::Swap(m_ld->Clusterization.clusterIdx2solutionIdxLUT + ::ToID(first), m_ld->Clusterization.clusterIdx2solutionIdxLUT + ::ToID(second));
    ::Swap(m_ld->Clusterization.isCPoint + ::ToID(first), m_ld->Clusterization.isCPoint + ::ToID(second));

    ::Swap(&first, &second);
}

HCluster HClusters::AllocateCluster()
{
    int ind = m_ld->Clusterization.nClusterEnd;
    while (ind >= m_ld->Clusterization.nClusterLimit)
        Grow(m_ld->Clusterization.nClusterLimit + 100);//GetNewSize(m_hd->cfg, m_ld->NetList.nCellsLimit, "data.cellsLimitGrow", 128, 0, 1.25));

    m_ld->Clusterization.nClusterEnd++;
    HCluster res = ::__ConstructPtr<HCluster>(ind);

    OnClusterAllocation(res);

    return res;
}

HCluster HClusters::GetClusterByIndex(int idCluster)
{
    return ::__ConstructPtr<HCluster>(idCluster);
}

HCluster HClusters::AllocateClusterUnreal()
{
    int ind = m_ld->Clusterization.nClusterEnd + m_ld->Clusterization.nClusterUnreal;
    while (ind >= m_ld->Clusterization.nClusterLimit)
        Grow(m_ld->Clusterization.nClusterLimit + 100);//GetNewSize(m_hd->cfg, m_ld->NetList.nCellsLimit, "data.cellsLimitGrow", 128, 0, 1.25));

    //m_ld->Clusterization.nClusterEnd++;
    m_ld->Clusterization.nClusterUnreal++;
    HCluster res = ::__ConstructPtr<HCluster>(ind);

    OnClusterAllocation(res);

    return res;
}

void HClusters::BuildTerminalsIndex()
{
    /*m_terminals.Clear();
    for (CellsEnumerator i = GetEnumerator(); i.MoveNext(); )
        if (GetBool<HCell::IsTerminal>(i) && Get<HCell::PlacementStatus, PlacementStatus>(i) != PlacementStatus_Fictive)
        {
            m_terminals.Add(::ToID(i));
        }*/
}
void HClusters::Resize(int newSize)
{
    if (m_ld->Clusterization.nClusterLimit < newSize)
    {
        Grow(m_ld->Clusterization.nClusterLimit + newSize + newSize / 2);
    }
    m_ld->Clusterization.nClusterEnd = newSize + 1;
}

void HClusters::ClearTableOfAdjacentNets()
{
    for (int i = 0; i < m_ld->Clusterization.nClusterLimit; i++)
    {
        m_ld->Clusterization.tableOfAdjacentNets[i]->clear();
    }
}