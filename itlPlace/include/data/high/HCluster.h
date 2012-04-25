#ifndef __HIGH_CLUSTER_H__
#define __HIGH_CLUSTER_H__
#include "HBaseDesignItem.h"
#include "Index.h"

class HCell;
class HClusteredNet;

BEGINITEM(HCluster, HClusterWrapper)

enum Properties
{
    Cells,
    XCoord,
    YCoord,
    Area,
    PotentialMultiplier,
    IsValid,
    IsFake,
    IsTerminals,
    IsPrimary,
    id,
    tableOfAdjacentNets,
    clusterIdx2solutionIdxLUT, 
    isCPoint
};

bool operator < (HCluster& cluster)
{
    return __uid < cluster.__uid;
}

ENDITEM(HCluster)

BEGINHCOLLECTION(HClusters, HCluster)

public:
    int ClustersCount() const
    { 
        return m_ld->Clusterization.nClusterEnd - 1;
    }

    //Enumerators
    typedef HEnumerator<HCluster> ClustersEnumerator;
    typedef HEnumeratorW<HClusterWrapper, HClusters> ClustersEnumeratorW;
    typedef HEnumeratorW<HClusterWrapper, HClusters> ClustersUnrealEnumeratorW;
    class ClustersNotFakeEnumeratorW;
    

    ClustersEnumerator GetEnumerator();
    ClustersEnumeratorW GetEnumeratorW();
    ClustersUnrealEnumeratorW GetUnrealEnumeratorW();
    ClustersNotFakeEnumeratorW GetNotFakeEnumeratorW();

    ///events
    Event<2, int, int> OnGrow; ///int nCellsLimit_before, int nCellsLimit_after
    Event<2, HCluster, HCluster> BeforeSwap; ///HCell first, HCell second
    Event<1, HCluster> OnClusterAllocation;

    ///operations
    void Initialize(int ClustersLimit);
    void Grow(int GrowsLimit);
    void Swap(HCluster& first, HCluster& second);
    HCluster AllocateCluster();
    HCluster AllocateClusterUnreal();
    void BuildTerminalsIndex();
    void Resize(int newSize);
    HCluster GetClusterByIndex(int idCluster);
    void ClearTableOfAdjacentNets();

    //Getters & Setters
    GETTERS_SETTERS_DEFINITION()

        //properties specializations
        //PROPERTYA(std::vector<HCell>*, HCluster::Cells, m_ld->Clusterization.cells)
        GETTERA(std::vector<HCell>*, HCluster::Cells, m_ld->Clusterization.cells)
        GETTERA(std::vector<HClusteredNet>*, HCluster::tableOfAdjacentNets, m_ld->Clusterization.tableOfAdjacentNets)
        PROPERTYA(double, HCluster::XCoord, m_ld->Clusterization.xCoord)
        PROPERTYA(double, HCluster::YCoord, m_ld->Clusterization.yCoord)
        PROPERTYA(double, HCluster::Area, m_ld->Clusterization.area)
        PROPERTYA(double, HCluster::PotentialMultiplier, m_ld->Clusterization.potentialMultiplier)
        PROPERTYA(bool,   HCluster::IsValid, m_ld->Clusterization.isValid)
        PROPERTYA(bool,   HCluster::IsFake, m_ld->Clusterization.isFake)
        PROPERTYA(bool,   HCluster::IsTerminals, m_ld->Clusterization.isTerminals)
        PROPERTYA(bool,   HCluster::IsPrimary, m_ld->Clusterization.isPrimary)
        PROPERTYA(int,   HCluster::id, m_ld->Clusterization.id)
        PROPERTYA(int,   HCluster::clusterIdx2solutionIdxLUT, m_ld->Clusterization.clusterIdx2solutionIdxLUT)
        PROPERTYA(bool,   HCluster::isCPoint, m_ld->Clusterization.isCPoint)

        ENDHCOLLECTION(HClusters)

        BEGINWRAPPER(HClusterWrapper, HClusters)

        //properties
        //PROPERTYWD(std::vector<HCell>*, Cells)
        GETTERWD(std::vector<HCell>*, Cells)
        GETTERWD(std::vector<HClusteredNet>*, tableOfAdjacentNets)
        PROPERTYWD(double, XCoord)
        PROPERTYWD(double, YCoord)
        PROPERTYWD(double, Area)
        PROPERTYWD(double, PotentialMultiplier)
        PROPERTYWD(bool, IsValid)
        PROPERTYWD(bool, IsFake)
        PROPERTYWD(bool, IsTerminals)
        PROPERTYWD(bool, IsPrimary)
        PROPERTYWD(int, id)
        PROPERTYWD(int, clusterIdx2solutionIdxLUT)
        PROPERTYWD(bool, isCPoint)

        ENDWRAPPER(HClusterWrapper)

//typedef HEnumeratorW<HClusterWrapper, HClusters> ClustersNotFakeEnumeratorW;
    class HClusters::ClustersNotFakeEnumeratorW : public HEnumeratorW<HClusterWrapper, HClusters, 1>
    {
        typedef HEnumeratorW<HClusterWrapper, HClusters, 1> BaseEnumeratorW;
    public:
        ClustersNotFakeEnumeratorW(HClusters* cl, IDType start, IDType end)
            : BaseEnumeratorW(cl, start, end)
        {
        }

        bool MoveNext()
        {
            while(BaseEnumeratorW::MoveNext())
                if(this->IsFake() == false)
                    return true;
            return false;
        }
    };

    inline HClusters::ClustersEnumerator HClusters::GetEnumerator()
    {
        return ClustersEnumerator(1, m_ld->Clusterization.nClusterEnd);
    }
    inline HClusters::ClustersEnumeratorW HClusters::GetEnumeratorW()
    {
        return ClustersEnumeratorW(this, 1, m_ld->Clusterization.nClusterEnd);
    };
    inline HClusters::ClustersNotFakeEnumeratorW HClusters::GetNotFakeEnumeratorW()
    {
        return ClustersNotFakeEnumeratorW(this, 1, m_ld->Clusterization.nClusterEnd);
    };
    inline HClusters::ClustersUnrealEnumeratorW HClusters::GetUnrealEnumeratorW()
    {
        return ClustersUnrealEnumeratorW(this, m_ld->Clusterization.nClusterEnd, 
            m_ld->Clusterization.nClusterEnd + m_ld->Clusterization.nClusterUnreal);
    };

    

#endif //__HIGH_CLUSTER_H__
