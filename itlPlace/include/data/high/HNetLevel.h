#ifndef __HIGH_NET_LEVEL_H__
#define __HIGH_NET_LEVEL_H__
#include "HBaseDesignItem.h"
#include "Index.h"

BEGINITEM(HNetLevel, HNetLevelWrapper)

enum Properties
{
    weight,
    k,
    idNet
};

bool operator < (HNetLevel& net)
{
    return __uid < net.__uid;
}

bool operator > (HNetLevel& net)
{
    return __uid > net.__uid;
}

ENDITEM(HNetLevel)

    BEGINHCOLLECTION2(HNetLevels, HNetLevel)

    public:
        HNetLevels(class HDesign* hd): HBaseCollection(hd)
        {
        }

        int NetLevelsCount() const
        { 
            return m_ld->Clusterization.nNetLevelsEnd - 1;
        }

        ///events
        //Event<2, int, int> OnGrow; ///int nCellsLimit_before, int nCellsLimit_after
        //Event<2, HClusteredNets*, HClusteredNets*> BeforeSwap; ///HCell first, HCell second
        //Event<1, HClusteredNets*> OnNetLevelAllocation;

        typedef HEnumerator<HNetLevel> NetLevelsEnumerator;


        NetLevelsEnumerator GetEnumerator();


        ///events
        Event<2, int, int> OnGrow; ///int nCellsLimit_before, int nCellsLimit_after
        Event<2, HNetLevel, HNetLevel> BeforeSwap; ///HCell first, HCell second
        Event<1, HNetLevel> OnClusteredNetAllocation;
        ///operations
        void Initialize(int NetLevelLimit);
        void Grow(int GrowsLimit);
        void Swap(HNetLevel* first, HNetLevel* second);
        HClusteredNets* AllocateNetLevel();
        HClusteredNets* CreateCopy();
        HClusteredNets* RewritePastCopy();
        void Resize(int newSize);
        std::vector<HClusteredNets*>* GetNetLelev()
        {
            return m_ld->Clusterization.netLevels;
        }
        void PurifyNetList();

        GETTERS_SETTERS_DEFINITION()

ENDHCOLLECTION2(HNetLevels)

//properties specializations
//PROPERTYA(std::vector<HCell>*, HCluster::Cells, m_ld->Clusterization.cells)
PROPERTYADECL(HNetLevels, double, HNetLevel::weight, m_ld->Clusterization.weight[0])
PROPERTYADECL(HNetLevels, double, HNetLevel::k, m_ld->Clusterization.k[0])
PROPERTYADECL(HNetLevels, int,   HNetLevel::idNet, m_ld->Clusterization.idNet[0])

BEGINWRAPPER(HNetLevelWrapper, HNetLevels)

public:

    //properties
    //PROPERTYWD(std::vector<HCell>*, Cells)
        PROPERTYWD(double, weight)
        PROPERTYWD(double, k)
        PROPERTYWD(int, idNet)

        ENDWRAPPER(HNetLevelWrapper)

        inline HNetLevels::NetLevelsEnumerator HNetLevels::GetEnumerator()
    {
        return NetLevelsEnumerator(1, m_ld->Clusterization.nClusteredNetEnd[0]);
    }


#endif //__HIGH_NET_LEVEL_H__
