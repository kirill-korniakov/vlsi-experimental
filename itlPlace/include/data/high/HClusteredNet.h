#ifndef __HIGH_CLUSTERED_NET_H__
#define __HIGH_CLUSTERED_NET_H__
#include "HBaseDesignItem.h"
#include "Index.h"

class HCluster;

BEGINITEM(HClusteredNet, HClusteredNetWrapper)

enum Properties
{
    clusterIdxs,
    weight,
    k,
    idNet
};

bool operator < (HClusteredNet& net)
{
    return __uid < net.__uid;
};

bool operator > (HClusteredNet& net)
{
    return __uid > net.__uid;
};

ENDITEM(HClusteredNet)

BEGINHCOLLECTION(HClusteredNets, HClusteredNet)

    public:
        friend class HNetLevels;
        int id;
        enum Properties
        {
            netLevel,
            idLevel
        };

        int ClusteredNetCount() const
        { 
            return m_ld->Clusterization.nClusteredNetEnd[id] - 1;
        }

        HClusteredNets() : HBaseCollection(NULL)
        {
        }

        HClusteredNets& operator= (HClusteredNets& value) 
        { 
            if (this->id != value.id)
            {
                this->m_ld = value.m_ld;
                this->m_hd = value.m_hd;
                this->m_initialized = value.m_initialized;
                this->id = value.id;
            }
            return *this; 
        }

        //Enumerators
        typedef HEnumerator<HClusteredNet> ClusteredNetsEnumerator;
        typedef HEnumeratorW<HClusteredNetWrapper, HClusteredNets> ClusteredNetsEnumeratorW;
        typedef HREnumeratorW<HClusteredNetWrapper, HClusteredNets> RClusteredNetsEnumeratorW;

        ClusteredNetsEnumerator GetEnumerator();
        ClusteredNetsEnumeratorW GetEnumeratorW();
        RClusteredNetsEnumeratorW GetREnumeratorW();

        ///events
        Event<2, int, int> OnGrow; ///int nCellsLimit_before, int nCellsLimit_after
        Event<2, HClusteredNet, HClusteredNet> BeforeSwap; ///HCell first, HCell second
        Event<1, HClusteredNet> OnClusteredNetAllocation;

        ///operations
        void Initialize(int ClustersLimit, int idLevel);
        void Grow(int GrowsLimit);
        void Swap(HClusteredNet& first, HClusteredNet& second);
        HClusteredNet AllocateClusteredNet();
        void Resize(int newSize);
        HClusteredNet GetClusteredNetByIndex(int idnet);

        //Getters & Setters
        GETTERS_SETTERS_DEFINITION()

            //properties specializations
            //PROPERTYA(std::vector<HCell>*, HCluster::Cells, m_ld->Clusterization.cells)
            GETTERA(std::vector<HCluster>*, HClusteredNet::clusterIdxs, m_ld->Clusterization.clusterIdxs[id])
            PROPERTYA(double, HClusteredNet::weight, m_ld->Clusterization.weight[id])
            PROPERTYA(double, HClusteredNet::k, m_ld->Clusterization.k[id])
            PROPERTYA(int,   HClusteredNet::idNet, m_ld->Clusterization.idNet[id])

            ENDHCOLLECTION(HClusteredNets)

            BEGINWRAPPER(HClusteredNetWrapper, HClusteredNets)
public:
    HClusteredNetWrapper()
    {
    }
    //properties
    //PROPERTYWD(std::vector<HCell>*, Cells)
    GETTERWD(std::vector<HCluster>*, clusterIdxs)
        PROPERTYWD(double, weight)
        PROPERTYWD(double, k)
        PROPERTYWD(int, idNet)

        ENDWRAPPER(HClusteredNetWrapper)

        inline HClusteredNets::ClusteredNetsEnumerator HClusteredNets::GetEnumerator()
    {
        return ClusteredNetsEnumerator(1, m_ld->Clusterization.nClusteredNetEnd[id]);
    }
    inline HClusteredNets::ClusteredNetsEnumeratorW HClusteredNets::GetEnumeratorW()
    {
        return ClusteredNetsEnumeratorW(this, 1, m_ld->Clusterization.nClusteredNetEnd[id]);
    };
    inline HClusteredNets::RClusteredNetsEnumeratorW HClusteredNets::GetREnumeratorW()
    {
        return RClusteredNetsEnumeratorW(this, 1, m_ld->Clusterization.nClusteredNetEnd[id]);
    };

#endif //__HIGH_CLUSTER_H__
