#ifndef __VanGinnekenTree_H__
#define __VanGinnekenTree_H__

//#include "VanGinnekenTreeNode.h"
#include "VanGinnekenTreeCreate.h"
#include "Utils.h"

class HDPGrid;
class VGAlgorithmData;
class UpdateVanGinnekenTree;
using namespace Utils;

enum PartitionType
{
    LINEAR               = 0, //0 - обычное линейное разбиение
    DYNAMIC              = 1, //1 - динамическое распределение точек,
    LEGAL_POSITIONS_ONLY = 2  //2 - легальное распределение точек (использовать только с 1 или 2 типом буферизации)
};

class VanGinnekenTree
{
public:
    VGAlgorithmData* vGAlgorithmData;
    HDPGrid* DPGrid;
    VanGinnekenTreeNode** vGTreeNodeList;
    HPlacementGrid* pGrid;

    UpdateVanGinnekenTree* updateVanGinnekenTree;
    VanGinnekenTreeNodeCreate* vanGinnekenTreeNodeCreate;
    VanGinnekenTreeCreate* vanGinnekenTreeCreate;
    CalculateVanGinnekenSubtree* calculateVanGinnekenSubtree;
    FindMaxPointInEdgeVanGinnekenTree* findMaxPointInEdgeVanGinnekenTree;

    VanGinnekenTree(VGAlgorithmData* data);
    ~VanGinnekenTree();

    virtual void ClearTree();
    virtual int GetTreeSize();
    virtual double GetR();
    virtual VanGinnekenTreeNode* GetSource();

};

class UpdateVanGinnekenTree
{
public:
    VanGinnekenTree* vGTree;
    int treeSize;

    UpdateVanGinnekenTree(VanGinnekenTree* tree);

    template< class StartPoint >
    void UpdateTree(StartPoint source)
    {
        vGTree->ClearTree();
        TemplateTypes<HSteinerPoint>::stack points;
        TemplateTypes<int>::stack rootIndexs;
        TemplateTypes<int>::stack isPoitnsVisits;
        TemplateTypes<HNet>::stack netInCriticalPath;
        TemplateTypes<VanGinnekenTreeNode*>::stack vanGinnekenTreeNodeRoot;
        int pc = 1;
        int nodeIndex = 0;
        int rootIndex = 0;
        int isPoitnVisit = 0;
        HSteinerPoint srcPoint = GetSteinerPointByStartPoint(source);
        HNet net = vGTree->vGAlgorithmData->design.Pins.Get<HPin::Net, HNet>(vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(srcPoint));
        netInCriticalPath.push(net);
        HSteinerPoint nextPoint = srcPoint;
        points.push(srcPoint);	
        rootIndexs.push(0);
        isPoitnsVisits.push(0);
        int pinInNet = vGTree->vGAlgorithmData->design.Nets.GetInt<HNet::PinsCount>(net);
        VanGinnekenTreeNode* root = vGTree->vanGinnekenTreeNodeCreate->CreateSource(net, srcPoint, SOURCE, nodeIndex, vGTree);
        vanGinnekenTreeNodeRoot.push(root);
        int pininnet = vGTree->vGAlgorithmData->design.Nets.GetInt<HNet::PinsCount>(net);
        while (!points.empty())
        {
            srcPoint = points.top();
            rootIndex = rootIndexs.top();
            isPoitnVisit = isPoitnsVisits.top();
            isPoitnsVisits.pop();
            if (isPoitnVisit == 0)
            {
                if (vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::HasLeft, bool>(srcPoint))
                {
                    CreateNodeInLeftSubTree(net, points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint, vanGinnekenTreeNodeRoot);
                }
                else
                {
                    CreateNodeInSink(net, points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint, source, netInCriticalPath, vanGinnekenTreeNodeRoot, pc);
                }
            }
            else
                if (isPoitnVisit == 1)
                {
                    isPoitnVisit = 2;
                    isPoitnsVisits.push(isPoitnVisit);  
                    if (vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::HasRight, bool>(srcPoint))
                    {
                        CreateNodeInRightSubTree(net, points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint, vanGinnekenTreeNodeRoot);
                    }    
                }
                else
                    if (isPoitnVisit == 2)
                    {
                        points.pop();	
                        rootIndexs.pop();
                        if (rootIndex == vanGinnekenTreeNodeRoot.top()->index)
                        {
                            vanGinnekenTreeNodeRoot.pop();
                            netInCriticalPath.pop();
                            if (netInCriticalPath.size() > 0)
                                net = netInCriticalPath.top();
                        }
                    }
        }

        treeSize = nodeIndex + 1;
        if (nodeIndex >= vGTree->vGAlgorithmData->totalTreeSize)
            ALERT("ERROR  totalTreeSize = %d\t< nodeIndex = %d", vGTree->vGAlgorithmData->totalTreeSize, nodeIndex);
    }

    template< class StartPoint >
    HSteinerPoint GetSteinerPointByStartPoint(StartPoint point)
    {
        return HSteinerPoint(point);
    }

    HSteinerPoint GetSteinerPointByStartPoint(HSteinerPoint point)
    {
        return point;
    }

    HSteinerPoint GetSteinerPointByStartPoint(HCriticalPath::PointsEnumeratorW point);


    void CreateNodeInLeftSubTree(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
        TemplateTypes<int>::stack& rootIndexs,
        TemplateTypes<int>::stack& isPoitnsVisits,
        int& nodeIndex,
        int& rootIndex,
        int& isPoitnVisit,
        HSteinerPoint& srcPoint,
        HSteinerPoint& nextPoint, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot);
    void CreateNodeInSink(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
        TemplateTypes<int>::stack& rootIndexs,
        TemplateTypes<int>::stack& isPoitnsVisits,
        int& nodeIndex,
        int& rootIndex,
        int& isPoitnVisit,
        HSteinerPoint& srcPoint,
        HSteinerPoint& nextPoint, HSteinerPoint& source,
        TemplateTypes<HNet>::stack& netInCriticalPath,
        TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, int& pc);
    void CreateNodeInRightSubTree(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
        TemplateTypes<int>::stack& rootIndexs,
        TemplateTypes<int>::stack& isPoitnsVisits,
        int& nodeIndex,
        int& rootIndex,
        int& isPoitnVisit,
        HSteinerPoint& srcPoint,
        HSteinerPoint& nextPoint, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot);

    void CreateNodeInSink(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
        TemplateTypes<int>::stack& rootIndexs,
        TemplateTypes<int>::stack& isPoitnsVisits,
        int& nodeIndex,
        int& rootIndex,
        int& isPoitnVisit,
        HSteinerPoint& srcPoint,
        HSteinerPoint& nextPoint, HCriticalPath::PointsEnumeratorW& source,
        TemplateTypes<HNet>::stack& netInCriticalPath,
        TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, int& pc);
};

#endif