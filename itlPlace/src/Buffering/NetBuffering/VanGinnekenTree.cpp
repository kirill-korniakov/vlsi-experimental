#include "VanGinnekenTree.h"
#include "NetBufferingAlgorithm.h"
#include "HDPGrid.h"

VanGinnekenTree::VanGinnekenTree(VGAlgorithmData* data)
{
    vGAlgorithmData = data;

    if (data->typeNetListBuffering == BUFFERING_ALL_CRITICAL_PATH)
        vanGinnekenTreeCreate = new VanGinnekenTreeCreate(this);
    if (data->typeNetListBuffering == PATH_BASED)
        vanGinnekenTreeCreate = new VanGinnekenTreeCreatePathBased(this);

    if (data->typePartition == LINEAR)
    {
        updateVanGinnekenTree = new UpdateVanGinnekenTree(this);
        vanGinnekenTreeNodeCreate = new VanGinnekenTreeNodeCreate(this);    
        calculateVanGinnekenSubtree = new CalculateVanGinnekenSubtree(this);
        findMaxPointInEdgeVanGinnekenTree = new FindMaxPointInEdgeVanGinnekenTree(this);
    }

    if (data->typePartition == DYNAMIC)
    {
        updateVanGinnekenTree = new UpdateVanGinnekenTree(this);
        vanGinnekenTreeNodeCreate = new VanGinnekenTreeDynamicDistributionNodeCreate(this);
        calculateVanGinnekenSubtree = new CalculateVanGinnekenSubtree(this);
        findMaxPointInEdgeVanGinnekenTree = new FindMaxPointInEdgeVGTreeDynamicDistribution(this);
    }

    if (data->typePartition == LEGAL_POSITIONS_ONLY) 
    {
        pGrid = new HPlacementGrid(data->design, data->sizeBuffer);
        updateVanGinnekenTree = new UpdateVanGinnekenTree(this);
        vanGinnekenTreeNodeCreate = new VanGinnekenTreeLegalDynamicDistributionNodeCreate(this);
        calculateVanGinnekenSubtree = new CalculateVanGinnekenSubtree(this);
        findMaxPointInEdgeVanGinnekenTree = new FindMaxPointInEdgeVanGinnekenTreeLegalDynamicDistribution(this);
    }
    DPGrid = new HDPGrid(data->design);
    vanGinnekenTreeCreate->CreateTree();
}

VanGinnekenTree::~VanGinnekenTree()
{
    delete updateVanGinnekenTree;
    delete vanGinnekenTreeNodeCreate;
    delete vanGinnekenTreeCreate;
    delete calculateVanGinnekenSubtree;
    delete findMaxPointInEdgeVanGinnekenTree;
    delete DPGrid;
    delete [] vGTreeNodeList;
}

void VanGinnekenTree::ClearTree()
{
    for (int i = 0; i < vGAlgorithmData->totalTreeSize; i++)
    {
        vGTreeNodeList[i]->SetLeft(NULL);
        vGTreeNodeList[i]->SetRight(NULL);
        vGTreeNodeList[i]->type = NodeType(-1);
    }
}

int VanGinnekenTree::GetTreeSize()
{
    return updateVanGinnekenTree->treeSize;
}

VanGinnekenTreeNode* VanGinnekenTree::GetSource()
{
    return vGTreeNodeList[0];
}

double VanGinnekenTree::GetR()
{
    vGTreeNodeList[0];
    return vGTreeNodeList[0]->GetR();
}

UpdateVanGinnekenTree::UpdateVanGinnekenTree(VanGinnekenTree* tree)
{
    treeSize = 0;
    vGTree = tree;
}

void UpdateVanGinnekenTree::CreateNodeInLeftSubTree(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
                                                    TemplateTypes<int>::stack& rootIndexs,
                                                    TemplateTypes<int>::stack& isPoitnsVisits,
                                                    int& nodeIndex,
                                                    int& rootIndex,
                                                    int& isPoitnVisit,
                                                    HSteinerPoint& srcPoint,
                                                    HSteinerPoint& nextPoint, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot)
{
    isPoitnVisit = 1;
    nextPoint = vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Left, HSteinerPoint>(srcPoint);
    vGTree->vanGinnekenTreeNodeCreate->CreateNewPoint(net, nextPoint, BRANCH, nodeIndex, rootIndex, vanGinnekenTreeNodeRoot, false, vGTree);
    rootIndexs.push(nodeIndex);
    points.push(nextPoint);
    isPoitnsVisits.push(isPoitnVisit);
    isPoitnsVisits.push(0);
}

void UpdateVanGinnekenTree::CreateNodeInSink(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
                                             TemplateTypes<int>::stack& rootIndexs,
                                             TemplateTypes<int>::stack& isPoitnsVisits,
                                             int& nodeIndex,
                                             int& rootIndex,
                                             int& isPoitnVisit,
                                             HSteinerPoint& srcPoint,
                                             HSteinerPoint& nextPoint, HSteinerPoint& source,
                                             TemplateTypes<HNet>::stack& netInCriticalPath,
                                             TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, int& pc)
{
    isPoitnVisit = 2;
    isPoitnsVisits.push(isPoitnVisit);
}

void UpdateVanGinnekenTree::CreateNodeInRightSubTree(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
                                                     TemplateTypes<int>::stack& rootIndexs,
                                                     TemplateTypes<int>::stack& isPoitnsVisits,
                                                     int& nodeIndex,
                                                     int& rootIndex,
                                                     int& isPoitnVisit,
                                                     HSteinerPoint& srcPoint,
                                                     HSteinerPoint& nextPoint, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot)
{
    nextPoint = vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Right, HSteinerPoint>(srcPoint);
    vGTree->vanGinnekenTreeNodeCreate->CreateNewPoint(net, nextPoint, BRANCH, nodeIndex, rootIndex, vanGinnekenTreeNodeRoot, true, vGTree);
    rootIndexs.push(nodeIndex);
    points.push(nextPoint);
    isPoitnsVisits.push(0);
}

void UpdateVanGinnekenTree::CreateNodeInSink(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
                                             TemplateTypes<int>::stack& rootIndexs,
                                             TemplateTypes<int>::stack& isPoitnsVisits,
                                             int& nodeIndex,
                                             int& rootIndex,
                                             int& isPoitnVisit,
                                             HSteinerPoint& srcPoint,
                                             HSteinerPoint& nextPoint, HCriticalPath::PointsEnumeratorW& source,
                                             TemplateTypes<HNet>::stack& netInCriticalPath,
                                             TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, int& pc)
{

    HCriticalPath::PointsEnumeratorW nextCriticalPathPointW = source;
    nextCriticalPathPointW.MoveNext();
    HSteinerPointWrapper nextSteinerPoint = vGTree->vGAlgorithmData->design[vGTree->vGAlgorithmData->design.SteinerPoints[vGTree->vGAlgorithmData->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(nextCriticalPathPointW.TimingPoint())]];

    bool isCriticalPathLeaf = true;

    if (nextSteinerPoint == srcPoint)
    {
        if (source.MoveNext())
        {
            if (source.MoveNext())
            {
                pc+=2;
                isCriticalPathLeaf = false;
                srcPoint = vGTree->vGAlgorithmData->design[vGTree->vGAlgorithmData->design.SteinerPoints[vGTree->vGAlgorithmData->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(source.TimingPoint())]];
                int pinInNet = vGTree->vGAlgorithmData->design.Nets.GetInt<HNet::PinsCount>(net);
                net = vGTree->vGAlgorithmData->design.Pins.Get<HPin::Net, HNet>(vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin,HPin>(srcPoint));
                int pinInNet2 = vGTree->vGAlgorithmData->design.Nets.GetInt<HNet::PinsCount>(net);
                HSteinerPointWrapper o = vGTree->vGAlgorithmData->design[srcPoint];
                HNetWrapper oo = vGTree->vGAlgorithmData->design[net];
                string netName0 = oo.Name();
                HPinWrapper pw = vGTree->vGAlgorithmData->design[oo.Source()];
                HSteinerPointWrapper spw = vGTree->vGAlgorithmData->design[vGTree->vGAlgorithmData->design.SteinerPoints[pw]];
                netInCriticalPath.push(net);
                vanGinnekenTreeNodeRoot.push(vGTree->vGTreeNodeList[rootIndex]);
                vGTree->vanGinnekenTreeNodeCreate->RefreshSoursAndSinkPoint(net, srcPoint, SOURCE_AND_SINK, rootIndex);
                vGTree->vGTreeNodeList[rootIndex]->signalDirection = source.SignalDirection();

                CreateNodeInLeftSubTree(net, points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint, vanGinnekenTreeNodeRoot);
            }
        }
    }
    if (isCriticalPathLeaf)
    {
        isPoitnVisit = 2;
        isPoitnsVisits.push(isPoitnVisit);
    }
}

HSteinerPoint UpdateVanGinnekenTree::GetSteinerPointByStartPoint(HCriticalPath::PointsEnumeratorW point)
{
    return vGTree->vGAlgorithmData->design.SteinerPoints[vGTree->vGAlgorithmData->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(point.TimingPoint())];
}