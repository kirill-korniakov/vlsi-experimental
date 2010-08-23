#include "VanGinnekenData.h"
#include "Utils.h"
#include "HDPGrid.h"
#include "VanGinnekenAlgorithm.h"

//VanGinnekenTreeNode

class VanGinnekenTreeNode;
class HVGAlgorithm;

VanGinnekenTreeNode::VanGinnekenTreeNode()
{
    x = 0;
    y = 0;
    type = 3;
    //sPoint = tree->design.SteinerPoints.Null();
    index = 0;
    tree = NULL;
    left = NULL;
    right = NULL;
}

bool VanGinnekenTreeNode::isSource()
{
    return (type == 0) ? true : false;
}

bool VanGinnekenTreeNode::isSink()
{
    return (type == 1) ? true : false;
}

bool VanGinnekenTreeNode::isBranchPoint()
{
    return (type == 2) ? true : false;
}

bool VanGinnekenTreeNode::isCandidate()
{
    return ((type == 3) || (type == 4)|| (type == 5)) ? true : false;
}

bool VanGinnekenTreeNode::isCandidateAndRealPoint()
{
    return (type == 4) ? true : false;
}

bool VanGinnekenTreeNode::isInternal()
{
    return (type == 5) ? true : false;  
}

bool VanGinnekenTreeNode::HasLeft()
{
    if (left != NULL)
        return true;
    return false;
}

bool VanGinnekenTreeNode::HasRight()
{
    if (right != NULL)
        return true;
    return false;
}

HSteinerPoint VanGinnekenTreeNode::GetSteinerPoint()
{
    return sPoint;
}

double VanGinnekenTreeNode::GetX()
{
    return x;
}

double VanGinnekenTreeNode::GetY()
{
    return y;
}

VanGinnekenTreeNode* VanGinnekenTreeNode::GetLeft()
{
    return left;
}

VanGinnekenTreeNode* VanGinnekenTreeNode::GetRight()
{
    return right;
}

int VanGinnekenTreeNode::GetIndex()
{
    return index;
}

VanGinnekenTree* VanGinnekenTreeNode::GetTree()
{
    return tree;
}

double VanGinnekenTreeNode::GetRAT()	
{		
    if (sPoint != tree->vGAlgorithm->data->design.SteinerPoints.Null())
    {
        return (tree->vGAlgorithm->data->design.TimingPoints[(sPoint,tree->vGAlgorithm->data->design).Pin()],tree->vGAlgorithm->data->design).RequiredTime();
    }
    else
        return 0;
}

double VanGinnekenTreeNode::GetC()
{
    if (sPoint != tree->vGAlgorithm->data->design.SteinerPoints.Null())
        return (((sPoint,tree->vGAlgorithm->data->design).Pin(),tree->vGAlgorithm->data->design).Type(),tree->vGAlgorithm->data->design).Capacitance();
    else
        return 0;
}

double VanGinnekenTreeNode::GetR()
{
    if (sPoint != tree->vGAlgorithm->data->design.SteinerPoints.Null())
        return Utils::GetDriverTimingPhisics(tree->vGAlgorithm->data->design, (sPoint,tree->vGAlgorithm->data->design).Pin(), SignalDirection_None).R;
    else
        return 0;
}

void VanGinnekenTreeNode::SetSteinerPoint(HSteinerPoint sp)  
{
    sPoint = sp;
}
void VanGinnekenTreeNode::SetX(double value)
{
    x = value;
}
void VanGinnekenTreeNode::SetY(double value)
{
    y = value;
}

void VanGinnekenTreeNode::SetLeft(VanGinnekenTreeNode* node)
{
    left = node;
}
void VanGinnekenTreeNode::SetRight(VanGinnekenTreeNode* node)
{
    right = node;
}
void VanGinnekenTreeNode::SetType(int t)
{
    type = t;
}

void VanGinnekenTreeNode::SetIndex(int i)
{
    index = i;
}

void VanGinnekenTreeNode::SetTree(VanGinnekenTree* t)
{
    tree = t;
}

VanGinnekenTreeNode::~VanGinnekenTreeNode()
{
    left = NULL;
    right = NULL;
    tree = NULL;
}
//VanGinnekenTreeNode

//VanGinnekenTree
VanGinnekenTree::VanGinnekenTree(HVGAlgorithm* vGA):  pGrid(vGA->data->design)
{
    vGAlgorithm = vGA;
    treeSize = 0;
    DPGrid = new HDPGrid(vGA->data->design);
    partitionPointCount = vGAlgorithm->data->design.cfg.ValueOf("Interval", 1);
    TypePartition = vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0);
}

VanGinnekenTree::VanGinnekenTree(HVGAlgorithm* vGA, int partitionCount): pGrid(vGA->data->design)
{
    vGAlgorithm = vGA;
    treeSize = 0;
    DPGrid = new HDPGrid(vGA->data->design);
    partitionPointCount = partitionCount;
    TypePartition = vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0);

}

VanGinnekenTree::VanGinnekenTree(HVGAlgorithm* vGA, int partitionCount, double sizeBuffer): pGrid(vGA->data->design, sizeBuffer)
{
    vGAlgorithm = vGA;
    treeSize = 0;
    DPGrid = new HDPGrid(vGA->data->design);
    partitionPointCount = partitionCount;
    TypePartition = vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0);
}

VanGinnekenTree::VanGinnekenTree(HVGAlgorithm* vGA, int partitionCount, HSteinerPoint& source): pGrid(vGA->data->design)
{
    vGAlgorithm = vGA;
    treeSize = 0;
    DPGrid = new HDPGrid(vGA->data->design);
    partitionPointCount = partitionCount;
    TypePartition = vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0);
}

void VanGinnekenTree::ClearTree()
{
    for (int i = 0; i < totalTreeSize; i++)
    {
        vGTree[i].SetLeft(NULL);
        vGTree[i].SetRight(NULL);
        vGTree[i].SetType(-1);
    }
}

int VanGinnekenTree::GetTreeSize()
{
    return treeSize;
}

void VanGinnekenTree::SetEdgePartitionCount(int partitionCount)
{
    partitionPointCount = partitionCount;
}

void VanGinnekenTree::UpdateTree(HSteinerPoint& source)
{

    ClearTree();

    TemplateTypes<HSteinerPoint>::stack points;
    TemplateTypes<int>::stack rootIndexs;
    TemplateTypes<int>::stack isPoitnsVisits;
    //создаем дерево

    int nodeIndex = 0;
    int rootIndex = 0;
    int isPoitnVisit = 0;
    HSteinerPoint srcPoint = vGAlgorithm->data->design[source];
    HSteinerPoint nextPoint = srcPoint;
    points.push(srcPoint);	
    rootIndexs.push(0);
    isPoitnsVisits.push(0);
    CreateNode(srcPoint, 0, nodeIndex, rootIndex, false, this);//, vGAlgorithm->data->design[vGAlgorithm->data->design.SteinerPoints.Null()], false, vGAlgorithm->data->design[vGAlgorithm->data->design.SteinerPoints.Null()]);


    while (!points.empty())
    {
        srcPoint = points.top();
        rootIndex = rootIndexs.top();
        isPoitnVisit = isPoitnsVisits.top();
        //points.pop();	
        //rootIndexs.pop();
        isPoitnsVisits.pop();
        if (isPoitnVisit == 0)
        {
            if (vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::HasLeft, bool>(srcPoint))
            {
                isPoitnVisit = 1;
                nextPoint = vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Left, HSteinerPoint>(srcPoint);
                CreateNode(nextPoint, 2, nodeIndex, rootIndex, false, this);
                rootIndexs.push(nodeIndex);
                points.push(nextPoint);
                isPoitnsVisits.push(isPoitnVisit);
                isPoitnsVisits.push(0);
            }
            else
            {
                isPoitnVisit = 2;
                isPoitnsVisits.push(isPoitnVisit);
            }
        }
        else
            if (isPoitnVisit == 1)
            {
                isPoitnVisit = 2;
                isPoitnsVisits.push(isPoitnVisit);  
                if (vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::HasRight, bool>(srcPoint))
                {
                    nextPoint = vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Right, HSteinerPoint>(srcPoint);
                    CreateNode(nextPoint, 2, nodeIndex, rootIndex, true, this);
                    rootIndexs.push(nodeIndex);
                    points.push(nextPoint);
                    isPoitnsVisits.push(0);
                }    
            }
            else
                if (isPoitnVisit == 2)
                {
                    points.pop();	
                    rootIndexs.pop();
                }
    }


    /*while (!points.empty())
    {
    srcPoint = points.top();
    rootIndex = rootIndexs.top();
    points.pop();	
    rootIndexs.pop();
    if (vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::HasLeft, bool>(srcPoint))
    {
    nextPoint = vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Left, HSteinerPoint>(srcPoint);

    CreateNode(nextPoint, 2, nodeIndex, rootIndex, false, this);
    rootIndexs.push(nodeIndex);
    points.push(nextPoint);			

    if (vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::HasRight, bool>(srcPoint))
    {
    nextPoint = vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Right, HSteinerPoint>(srcPoint);

    CreateNode(nextPoint, 2, nodeIndex, rootIndex, true, this);
    rootIndexs.push(nodeIndex);
    points.push(nextPoint);
    }
    }
    else
    {
    //sink
    //CreateNode(&srcPoint, 1, nodeIndex, rootIndex);
    }
    }*/
    //vGAlgorithm->data->design.Plotter.PlotVGTree(&vGTree[0], Color_Black);
    //vGAlgorithm->data->design.Plotter.Refresh();
    treeSize = nodeIndex + 1;
    if (nodeIndex >= totalTreeSize)
        ALERT("ERROR3!!!!! + totalTreeSize = %d\tnodeIndex = %d",totalTreeSize, nodeIndex);
}

VanGinnekenTreeNode VanGinnekenTree::GetSource()
{
    return vGTree[0];
}

double VanGinnekenTree::GetR()
{
    vGTree[0];
    return vGTree[0].GetR();
}

//VanGinnekenTree

//VGTreeUniformDistribution

VGTreeUniformDistribution::VGTreeUniformDistribution(HVGAlgorithm* vGA): VanGinnekenTree(vGA)
{
    CreateTree();
}

VGTreeUniformDistribution::VGTreeUniformDistribution(HVGAlgorithm* vGA, int partitionCount): VanGinnekenTree(vGA, partitionCount)
{
    CreateTree();
}

VGTreeUniformDistribution::VGTreeUniformDistribution(HVGAlgorithm* vGA, int partitionCount, HSteinerPoint& source): VanGinnekenTree(vGA, partitionCount, source)
{
    CreateTree();
}

void VGTreeUniformDistribution::CreateTree()
{
    int maxTree = 0;
    int treeSizeTemp = 0;
    for (HNets::ActiveNetsEnumeratorW netW = vGAlgorithm->data->design.Nets.GetActiveNetsEnumeratorW(); netW.MoveNext(); )
    {

        HSteinerPointWrapper srcPoint = vGAlgorithm->data->design[vGAlgorithm->data->design.SteinerPoints[netW.Source()]];
        HSteinerPointWrapper nextPoint = srcPoint;
        //считаем количество точек дерева штейнера.
        int pointCount = 0;
        TemplateTypes<HSteinerPoint>::stack points;
        int maxPos = partitionPointCount;
        points.push(srcPoint);

        while (!points.empty())
        {
            srcPoint = points.top();
            points.pop();

            if (srcPoint.HasLeft())
            {
                nextPoint = srcPoint.Left();
                pointCount++;
                points.push(nextPoint);

                if (srcPoint.HasRight())
                {
                    nextPoint = srcPoint.Right();
                    pointCount++;
                    points.push(nextPoint);
                }
            }
            else
            {
                //sink
                pointCount++;
            }
        }

        treeSizeTemp = pointCount * maxPos + pointCount + 1;
        if (treeSizeTemp > maxTree)
            maxTree = treeSizeTemp;
    }
    totalTreeSize = maxTree;
    vGTree = new VanGinnekenTreeNode [maxTree];
}

VanGinnekenTreeNode* VGTreeUniformDistribution::CreateNode(HSteinerPoint node, int type, int& nodeIndex, int rootIndex, bool isRight, VanGinnekenTree* tree)
//тип звена: 0 - source; 1 - sink; 2 - точка ветвления; 3 - кандидат на вставку буфера
{
    if (type == 0)
    {
        HSteinerPointWrapper spw = vGAlgorithm->data->design[node];
        vGTree[nodeIndex].SetSteinerPoint(node);
        vGTree[nodeIndex].SetType(type);
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);
    }
    if (type == 2)
    {
        HSteinerPointWrapper spw = vGAlgorithm->data->design[node];
        double rx,ry,nx,ny;
        rx = vGTree[rootIndex].GetX();
        ry = vGTree[rootIndex].GetY();
        nx = spw.X();
        ny = spw.Y();

        if (isRight)
            vGTree[rootIndex].SetRight(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
        else
            vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));

        vGTree[nodeIndex].SetX(vGTree[rootIndex].GetX());
        vGTree[nodeIndex].SetY(vGTree[rootIndex].GetY());

        vGTree[nodeIndex].SetType(5);

        if (vGTree[rootIndex].isSource())
            vGTree[nodeIndex].SetType(4);

        rootIndex = nodeIndex;

        for (int i = 1; i < (partitionPointCount - 1); i++)
        {
            vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
            vGTree[nodeIndex].SetX(rx + (nx - rx) / (partitionPointCount - 1) * i);
            vGTree[nodeIndex].SetY(ry + (ny - ry) / (partitionPointCount - 1) * i);
            rootIndex = nodeIndex;
        }


        vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());

        vGTree[nodeIndex].SetType(5);

        if (!spw.IsInternal())
        {
            vGTree[nodeIndex].SetType(4);
        }

        rootIndex = nodeIndex;

        nodeIndex++;
        vGTree[nodeIndex].SetSteinerPoint(node);
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());
        vGTree[rootIndex].SetLeft(&vGTree[nodeIndex]);
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);

        if (!spw.IsInternal())
        {
            vGTree[nodeIndex].SetType(1);
        }
        else
        {
            vGTree[nodeIndex].SetType(type);	
        }

        if (nodeIndex >= totalTreeSize)
            ALERT("ERROR2!!!!!");

    }
    if (type == 3)
    {
        nodeIndex++;
        vGTree[nodeIndex].SetType(type);	
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);
    }
    return &vGTree[nodeIndex];
}

VGTreeUniformDistribution::~VGTreeUniformDistribution()
{
    delete [] vGTree;
}

//VGTreeUniformDistribution

//VGTreeDynamicDistribution

VGTreeDynamicDistribution::VGTreeDynamicDistribution(HVGAlgorithm* vGA): VanGinnekenTree(vGA)
{
    CreateTree();
}

VGTreeDynamicDistribution::VGTreeDynamicDistribution(HVGAlgorithm* vGA, int partitionCount): VanGinnekenTree(vGA, partitionCount)
{
    CreateTree();
}

VGTreeDynamicDistribution::VGTreeDynamicDistribution(HVGAlgorithm* vGA, int partitionCount, HSteinerPoint& source): VanGinnekenTree(vGA, partitionCount, source)
{
    CreateTree();
}

void VGTreeDynamicDistribution::CreateTree()
{
    int maxTree = 0;
    for (HNets::ActiveNetsEnumeratorW netW = vGAlgorithm->data->design.Nets.GetActiveNetsEnumeratorW(); netW.MoveNext(); )
    {
        HSteinerPointWrapper srcPoint = vGAlgorithm->data->design[vGAlgorithm->data->design.SteinerPoints[netW.Source()]];
        HSteinerPointWrapper nextPoint = srcPoint;
        //считаем количество точек дерева штейнера.
        int pointCount = 0;
        TemplateTypes<HSteinerPoint>::stack points;
        int maxPos = partitionPointCount;
        points.push(srcPoint);

        while (!points.empty())
        {
            srcPoint = points.top();
            points.pop();

            if (srcPoint.HasLeft())
            {
                nextPoint = srcPoint.Left();

                double rx,ry,nx,ny;
                rx = srcPoint.X();
                ry = srcPoint.Y();
                nx = nextPoint.X();
                ny = nextPoint.Y();

                int rsy = DPGrid->FindRow(ry);
                int rfy = DPGrid->FindRow(ny);
                int rsx = DPGrid->FindColumn(rx);
                int rfx = DPGrid->FindColumn(nx);

                int RowCount = abs(rsy - rfy + 1);
                int ColumnCount = abs(rsx - rfx + 1);
                int n = RowCount + ColumnCount;

                int rowInd = 0;
                int pointInRowCount = 0;
                int newPartitionPointCount = 0;
                if (((partitionPointCount - 1.0) / n) < 1)
                {
                    newPartitionPointCount = n + 1;
                    pointInRowCount = 1;
                }
                else
                {
                    pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
                    newPartitionPointCount = pointInRowCount * n + 1;
                }

                if (newPartitionPointCount > maxPos)
                    maxPos = newPartitionPointCount;


                pointCount++;
                points.push(nextPoint);

                if (srcPoint.HasRight())
                {
                    nextPoint = srcPoint.Right();

                    double rx,ry,nx,ny;
                    rx = srcPoint.X();
                    ry = srcPoint.Y();
                    nx = nextPoint.X();
                    ny = nextPoint.Y();

                    int rsy = DPGrid->FindRow(ry);
                    int rfy = DPGrid->FindRow(ny);
                    int rsx = DPGrid->FindColumn(rx);
                    int rfx = DPGrid->FindColumn(nx);

                    int RowCount = abs(rsy - rfy + 1);
                    int ColumnCount = abs(rsx - rfx + 1);
                    int n = RowCount + ColumnCount;


                    int rowInd = 0;
                    int pointInRowCount = 0;
                    int newPartitionPointCount = 0;
                    if (((partitionPointCount - 1.0) / n) < 1)
                    {
                        newPartitionPointCount = n + 1;
                        pointInRowCount = 1;
                    }
                    else
                    {
                        pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
                        newPartitionPointCount = pointInRowCount * n + 1;
                    }

                    if (newPartitionPointCount > maxPos)
                        maxPos = newPartitionPointCount;

                    pointCount++;
                    points.push(nextPoint);
                }
            }

            else
            {
                //sink
                pointCount++;
            }
        }
        maxPos *= 2;


        totalTreeSize = pointCount * maxPos + pointCount + 1;
        if (totalTreeSize > maxTree)
            maxTree = totalTreeSize;
    }
    totalTreeSize = maxTree;
    vGTree = new VanGinnekenTreeNode [maxTree];
}

VanGinnekenTreeNode* VGTreeDynamicDistribution::CreateNode(HSteinerPoint node, int type, int& nodeIndex, int rootIndex, bool isRight, VanGinnekenTree* tree)
//тип звена: 0 - source; 1 - sink; 2 - точка ветвления; 3 - кандидат на вставку буфера
{

    if (type == 0)
    {
        HSteinerPointWrapper spw = vGAlgorithm->data->design[node];
        vGTree[nodeIndex].SetSteinerPoint(node);
        vGTree[nodeIndex].SetType(type);
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);
    }
    if (type == 2)
    {
        HSteinerPointWrapper spw = vGAlgorithm->data->design[node];
        double rx,ry,nx,ny;
        rx = vGTree[rootIndex].GetX();
        ry = vGTree[rootIndex].GetY();
        nx = spw.X();
        ny = spw.Y();

        if (isRight)
            vGTree[rootIndex].SetRight(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
        else
            vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));

        vGTree[nodeIndex].SetX(vGTree[rootIndex].GetX());
        vGTree[nodeIndex].SetY(vGTree[rootIndex].GetY());
        vGTree[nodeIndex].SetType(5);

        if (vGTree[rootIndex].isSource())
            vGTree[nodeIndex].SetType(4);

        rootIndex = nodeIndex;
        int pPC = partitionPointCount;

        int rsy = DPGrid->FindRow(ry);
        int rfy = DPGrid->FindRow(ny);
        int rsx = DPGrid->FindColumn(rx);
        int rfx = DPGrid->FindColumn(nx);

        int RowCount = abs(rsy - rfy + 1);
        int ColumnCount = abs(rsx - rfx + 1);
        int n = RowCount + ColumnCount;

        int pointInRowCount = 0;
        int newPartitionPointCount = 0;
        if (((partitionPointCount - 1.0) / n) < 1)
        {
            newPartitionPointCount = n + 1;
        }
        else
        {
            pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
            newPartitionPointCount = pointInRowCount * n + 1;
        }
        pPC = newPartitionPointCount;


        for (int i = 1; i < (pPC - 1); i++)
        {
            vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
            vGTree[nodeIndex].SetX(rx + (nx - rx) / (pPC - 1) * i);
            vGTree[nodeIndex].SetY(ry + (ny - ry) / (pPC - 1) * i);
            rootIndex = nodeIndex;
        }


        vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());
        vGTree[nodeIndex].SetType(5);
        if (!spw.IsInternal())
        {
            vGTree[nodeIndex].SetType(4);
        }

        rootIndex = nodeIndex;

        nodeIndex++;
        vGTree[nodeIndex].SetSteinerPoint(node);
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());
        vGTree[rootIndex].SetLeft(&vGTree[nodeIndex]);
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);

        if (!spw.IsInternal())
        {
            vGTree[nodeIndex].SetType(1);
        }
        else
        {
            vGTree[nodeIndex].SetType(type);	
        }

        if (nodeIndex >= totalTreeSize)
            ALERT("ERROR1!!!!!");


    }
    if (type == 3)
    {
        nodeIndex++;
        vGTree[nodeIndex].SetType(type);	
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);
    }
    return &vGTree[nodeIndex];
}

VGTreeDynamicDistribution::~VGTreeDynamicDistribution()
{
    delete [] vGTree;
}
//VGTreeDynamicDistribution

//VGTreeLegalDynamicDistribution

VGTreeLegalDynamicDistribution::VGTreeLegalDynamicDistribution(HVGAlgorithm* vGA): VanGinnekenTree(vGA)
{
    CreateTree();
}

VGTreeLegalDynamicDistribution::VGTreeLegalDynamicDistribution(HVGAlgorithm* vGA, int partitionCount): VanGinnekenTree(vGA, partitionCount)
{
    CreateTree();
}

VGTreeLegalDynamicDistribution::VGTreeLegalDynamicDistribution(HVGAlgorithm* vGA, int partitionCount, double sizeBuffer): VanGinnekenTree(vGA, partitionCount, sizeBuffer)
{
    CreateTree();
}

VGTreeLegalDynamicDistribution::VGTreeLegalDynamicDistribution(HVGAlgorithm* vGA, int partitionCount, HSteinerPoint& source): VanGinnekenTree(vGA, partitionCount, source)
{
    CreateTree();
}

void VGTreeLegalDynamicDistribution::CreateTree()
{

    int maxTree = 0;
    for (HNets::ActiveNetsEnumeratorW netW = vGAlgorithm->data->design.Nets.GetActiveNetsEnumeratorW(); netW.MoveNext(); )
    {
        HSteinerPointWrapper srcPoint = vGAlgorithm->data->design[vGAlgorithm->data->design.SteinerPoints[netW.Source()]];
        HSteinerPointWrapper nextPoint = srcPoint;
        //считаем количество точек дерева штейнера.
        int pointCount = 0;
        TemplateTypes<HSteinerPoint>::stack points;
        int maxPos = partitionPointCount;
        points.push(srcPoint);

        while (!points.empty())
        {
            srcPoint = points.top();
            points.pop();

            if (srcPoint.HasLeft())
            {
                nextPoint = srcPoint.Left();

                double rx,ry,nx,ny;
                rx = srcPoint.X();
                ry = srcPoint.Y();
                nx = nextPoint.X();
                ny = nextPoint.Y();

                int rsy = pGrid.GetRow(ry);
                int rfy = pGrid.GetRow(ny);
                int rsx = pGrid.GetColumn(rx);
                int rfx = pGrid.GetColumn(nx);

                int RowCount = abs(rsy - rfy) + 1;
                int ColumnCount = abs(rsx - rfx) + 1;
                int n = RowCount * ColumnCount;

                int rowInd = 0;
                int pointInRowCount = 0;
                int newPartitionPointCount = 0;
                if (((partitionPointCount - 1.0) / n) < 1)
                {
                    newPartitionPointCount = n + 1;
                    pointInRowCount = 1;
                }
                else
                {
                    pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
                    newPartitionPointCount = pointInRowCount * n + 1;
                }

                if (newPartitionPointCount > maxPos)
                    maxPos = newPartitionPointCount;


                pointCount++;
                points.push(nextPoint);

                if (srcPoint.HasRight())
                {
                    nextPoint = srcPoint.Right();

                    double rx,ry,nx,ny;
                    rx = srcPoint.X();
                    ry = srcPoint.Y();
                    nx = nextPoint.X();
                    ny = nextPoint.Y();

                    int rsy = pGrid.GetRow(ry);
                    int rfy = pGrid.GetRow(ny);
                    int rsx = pGrid.GetColumn(rx);
                    int rfx = pGrid.GetColumn(nx);

                    int RowCount = abs(rsy - rfy + 1);
                    int ColumnCount = abs(rsx - rfx + 1);
                    int n = RowCount * ColumnCount;


                    int rowInd = 0;
                    int pointInRowCount = 0;
                    int newPartitionPointCount = 0;
                    if (((partitionPointCount - 1.0) / n) < 1)
                    {
                        newPartitionPointCount = n + 1;
                        pointInRowCount = 1;
                    }
                    else
                    {
                        pointInRowCount = int(ceil(((partitionPointCount - 1.0) / n)));
                        newPartitionPointCount = pointInRowCount * n + 1;
                    }

                    if (newPartitionPointCount > maxPos)
                        maxPos = newPartitionPointCount;

                    pointCount++;
                    points.push(nextPoint);
                }
            }

            else
            {
                //sink
                pointCount++;
            }
        }
        maxPos *= 2;


        totalTreeSize = pointCount * maxPos + pointCount + 1;
        if (totalTreeSize > maxTree)
            maxTree = totalTreeSize;
    }

    totalTreeSize = maxTree;
    vGTree = new VanGinnekenTreeNode [maxTree];
}

VanGinnekenTreeNode* VGTreeLegalDynamicDistribution::CreateNode(HSteinerPoint node, int type, int& nodeIndex, int rootIndex, bool isRight, VanGinnekenTree* tree)
//тип звена: 0 - source; 1 - sink; 2 - точка ветвления; 3 - кандидат на вставку буфера
{

    if (type == 0)
    {
        HSteinerPointWrapper spw = vGAlgorithm->data->design[node];
        vGTree[nodeIndex].SetSteinerPoint(node);
        vGTree[nodeIndex].SetType(type);
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);
    }
    if (type == 2)
    {
        HSteinerPointWrapper spw = vGAlgorithm->data->design[node];
        double rx,ry,nx,ny;
        rx = vGTree[rootIndex].GetX();
        ry = vGTree[rootIndex].GetY();
        nx = spw.X();
        ny = spw.Y();

        if (isRight)
            vGTree[rootIndex].SetRight(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
        else
            vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));

        vGTree[nodeIndex].SetX(vGTree[rootIndex].GetX());
        vGTree[nodeIndex].SetY(vGTree[rootIndex].GetY());

        vGTree[nodeIndex].SetType(5);

        if (vGTree[rootIndex].isSource())
            vGTree[nodeIndex].SetType(4);

        rootIndex = nodeIndex;
        int lenPath = 0;
        PlacementGridNode **path = pGrid.GetPath(lenPath, rx, ry, nx, ny);    
        for (int i = 0; i < lenPath; i++)
        {
            vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
            vGTree[nodeIndex].SetX(path[i]->GetX());
            vGTree[nodeIndex].SetY(path[i]->GetY());
            rootIndex = nodeIndex;

        }
        vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), 3, nodeIndex, rootIndex, isRight, tree));
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());

        vGTree[nodeIndex].SetType(5);

        if (!spw.IsInternal())
        {
            vGTree[nodeIndex].SetType(4);
        }

        rootIndex = nodeIndex;

        nodeIndex++;
        vGTree[nodeIndex].SetSteinerPoint(node);
        vGTree[nodeIndex].SetX(spw.X());
        vGTree[nodeIndex].SetY(spw.Y());
        vGTree[rootIndex].SetLeft(&vGTree[nodeIndex]);
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);

        if (!spw.IsInternal())
        {
            vGTree[nodeIndex].SetType(1);
        }
        else
        {
            vGTree[nodeIndex].SetType(type);	
        }

        if (nodeIndex >= totalTreeSize)
            ALERT("ERROR1!!!!!");

    }
    if (type == 3)
    {
        nodeIndex++;
        vGTree[nodeIndex].SetType(type);	
        vGTree[nodeIndex].SetIndex(nodeIndex);
        vGTree[nodeIndex].SetTree(tree);
    }
    return &vGTree[nodeIndex];
}

VGTreeLegalDynamicDistribution::~VGTreeLegalDynamicDistribution()
{
    delete [] vGTree;
}
//VGTreeLegalDynamicDistribution

//BufferPositions
BufferPositions::BufferPositions()
{
    index = 0;
    position = NULL;
    bufferInfo = NULL;
}

BufferPositions::BufferPositions(VanGinnekenTreeNode* pos, BufferInfo* bufInfo, int i)
{
    index = i;
    position = pos;
    bufferInfo = bufInfo;
}

bool BufferPositions::operator > (BufferPositions& element)
{
    if (position->GetIndex() > element.GetPosition()->GetIndex())
        return true;
    return false;
}

bool BufferPositions::operator < (BufferPositions& element)
{
    if (position->GetIndex() < element.GetPosition()->GetIndex())
        return true;
    return false;
}


VanGinnekenTreeNode* BufferPositions::GetPosition()
{
    return position;
}

BufferInfo* BufferPositions::GetBufferInfo()
{
    return bufferInfo;
}

int BufferPositions::GetIndex()
{
    return index;
}

void BufferPositions::SetPosition(VanGinnekenTreeNode* pos)
{
    position = pos;
}

void BufferPositions::SetBufferInfo(BufferInfo* bufInfo)
{
    bufferInfo = bufInfo;
}

void BufferPositions::SetIndex(int i)
{
    index = i;
}


//BufferPositions

//VGVariantsListElement
VGVariantsListElement::VGVariantsListElement()
{
    RAT = 0;
    c = 0;
    positionCount = 0;
    index = 0;
}
bool VGVariantsListElement::operator > (VGVariantsListElement& element)
{
    if ((c > element.c) && (RAT > element.RAT))
        return true;
    else
        return false;
}
bool VGVariantsListElement::operator < (VGVariantsListElement& element)
{
    if ((c < element.c) && (RAT < element.RAT))
        return false;
    else
        return true;
}

bool VGVariantsListElement::operator == (VGVariantsListElement& element)
{
    if (index == element.index)
        return true;
    else
        return false;
}

VGVariantsListElement& VGVariantsListElement::operator = (VGVariantsListElement& element)
{
    if(&element == this )
        return *this;
    RAT = element.GetRAT();
    c = element.GetC();
    bufferPositions = element.bufferPositions;
    positionCount = element.GetPositionCount();
    index = element.index;
    return *this;
}

void VGVariantsListElement::SortBufferPosition()
{
    bufferPositions.sort();
}

double VGVariantsListElement::GetRAT()
{
    return RAT;
}
double VGVariantsListElement::GetC()
{
    return c;
}
BufferPositions VGVariantsListElement::GetStartBufferPosition()
{
    if (positionCount > 0)
    {
        index = 0;
        return *bufferPositions.begin();
    }
    BufferPositions pos;
    return pos;
}
BufferPositions VGVariantsListElement::GetEndBufferPosition()
{
    if (positionCount > 0)
    {
        index = positionCount - 1;
        return *bufferPositions.end();
    }
    BufferPositions pos;
    return pos;
}
BufferPositions VGVariantsListElement::GetNextBufferPosition()
{
    if ((positionCount > 0) && (index < (positionCount )))
    {
        index++;
        return GetbufferPosition(index);
    }
    BufferPositions pos;
    return pos;
}
BufferPositions VGVariantsListElement::GetbufferPosition(int i)
{
    if ((positionCount > 0) && (i < (positionCount - 1)) && (i >= 0))
    {
        index = i;
        TemplateTypes<BufferPositions>::list::iterator iter = bufferPositions.begin();
        while (iter->GetIndex() != i)
            iter++;
        return *iter;
    }
    BufferPositions pos;
    return pos;
}

int VGVariantsListElement::GetPositionCount()
{
    return positionCount;
}

TemplateTypes<BufferPositions>::list* VGVariantsListElement::GetBufferPosition()
{
    return &bufferPositions;
}

int VGVariantsListElement::GetIndex()
{
    return index;
}

void VGVariantsListElement::SetBufferPosition(BufferPositions position)
{
    bufferPositions.push_back(position);
    positionCount = int (bufferPositions.size());
}
void VGVariantsListElement::SetBufferPosition(TemplateTypes<BufferPositions>::list position)
{
    bufferPositions = position;
    positionCount = int (bufferPositions.size());
}

void VGVariantsListElement::SetRAT(double rat)
{
    RAT = rat;
}
void VGVariantsListElement::SetC(double capacity)
{
    c = capacity;
}

void VGVariantsListElement::SetIndex(int i)
{
    index = i;
}

VGAlgorithmData::VGAlgorithmData(HDesign& hd, HVGAlgorithm* vGA): design(hd), WirePhisics(hd.RoutingLayers.Physics)
{
    vGAlgorithm = vGA;
    partitionCount = 0;
    plotVGTree = false;
    plotNets = false;
    printNetInfo = false;
    plotSteinerPoint = false;
    printVariantsList = false;
    plotterWaitTime = 1;
    isInitialize = false;
    isInsertInSourceAndSink = false;
    typeBufferingAlgorithm = 0;
    sizeBuffer = 0; 
    maxIndex = 0;
    vGTree = NULL;
    netVisit = NULL;
    maxBufferCount = 0;
    typeModificationVanGinnekenList = 0;
    countPinInBufferingNet = 0;
    isTypeLimitationOnCountPinInBufferingNetEquality = false;
    nameBufferingNet = "";
    typeBuferAddition = 0;
    sizeBufferMultiplier = 0;
    isBufferingNetContainPrimaryPin = false;
    totalAreaOfBuffersInRelationToAllCells = 0;
    totalAreaCells = 0;
    totalAreaBuffer = 0;
    plotBuffer = false;
    plotBinGridValue = false;
}

VGAlgorithmData::~VGAlgorithmData()
{
    if (netVisit != NULL)
        delete [] netVisit;
}

void VGAlgorithmData::ReportParameters() 
{
    ALERT("Partition type: %d", typePartition);
    if (typePartition == 0)
        ALERT("Partition count: %d", partitionCount);

    ALERT("Buffering algorithm type type: %d", typeBufferingAlgorithm);
    ALERT("Type modification van Ginneken list: %d", typeModificationVanGinnekenList);
    ALERT("Maximum insert buffer in net: %d", maxBufferCount);
    ALERT("Type bufer addition: %d", typeBuferAddition);
    ALERT("Size buffer multiplier: %f", sizeBufferMultiplier);
}

void VGAlgorithmData::Initialize()
{
    isInitialize = true;
    LoadBuffers();

    plotSteinerPoint = design.cfg.ValueOf("Plotting.PlotSteinerPoint", false);
    plotVGTree = design.cfg.ValueOf("Plotting.PlotVGTree", false);
    plotNets = design.cfg.ValueOf("Plotting.PlotNets", false);
    plotterWaitTime = design.cfg.ValueOf("Plotting.PlotterWaitTime", 1);
    plotBuffer = design.cfg.ValueOf("Plotting.PlotBuffer", false);
    plotBinGridValue = design.cfg.ValueOf("Plotting.PlotBinGridValue", false);

    printNetInfo = design.cfg.ValueOf("Reporting.PrintNetInfo", false);
    printVariantsList = design.cfg.ValueOf("Reporting.PrintVGVariantsList", false);

    partitionCount = design.cfg.ValueOf("Interval", 1);  
    isInsertInSourceAndSink = design.cfg.ValueOf("IsInsertInSourceAndSink", true);
    typeBufferingAlgorithm = design.cfg.ValueOf("TypeBufferingAlgorithm", 0);
    typePartition = design.cfg.ValueOf("TypePartition", 0);

    maxBufferCount = design.cfg.ValueOf("MaxBufferCount", 0);
    typeModificationVanGinnekenList = design.cfg.ValueOf("TypeModificationVanGinnekenList", 0);

    countPinInBufferingNet = design.cfg.ValueOf("CountPinInBufferingNet", 0);
    isTypeLimitationOnCountPinInBufferingNetEquality = design.cfg.ValueOf("IsTypeLimitationOnCountPinInBufferingNetEquality", false);
    nameBufferingNet = design.cfg.ValueOf("NameBufferingNet", "");
    typeBuferAddition = design.cfg.ValueOf("TypeBuferAddition", 0);
    sizeBufferMultiplier = design.cfg.ValueOf("SizeBufferMultiplier", 1.0);
    isBufferingNetContainPrimaryPin = design.cfg.ValueOf("IsBufferingNetContainPrimaryPin", false);
    totalAreaOfBuffersInRelationToAllCells = design.cfg.ValueOf("TotalAreaOfBuffersInRelationToAllCells", 0.0);

    totalAreaCells = 0;
    totalAreaBuffer = 0;
    for (HCells::CellsEnumeratorW cw = design.Cells.GetEnumeratorW(); cw.MoveNext();)
        totalAreaCells += cw.Width() * cw.Height();

    netVisit = new bool [design.Nets.Count() * 2];
    for (int i = 0; i < design.Nets.Count() * 2; i++)
    {
        netVisit[i] = false;
    }

    if (design.cfg.ValueOf("Reporting.reportParameters", false))
        ReportParameters();
}

void VGAlgorithmData::ReportBufferInfo(BufferInfo buf) 
{
    if (design.cfg.ValueOf("Reporting.reportBufferInfo", false) == false)
        return;

    ALERT("Buffer type: %s\t input pin: %s\t output pin: %s", (design, buf.Type()).Name().c_str(),
        (design, buf.InPin()).Name().c_str(), (design, buf.OutPin()).Name().c_str());
    sizeBuffer = design[buf.Type()].SizeX() * design[buf.Type()].SizeY();
}

void VGAlgorithmData::LoadBuffers()
{
    if (design.cfg.ValueOf("UseOnlyDefaultBuffer", false))
    {
        BufferInfo buf = BufferInfo::Create(design);    
        Buffers.push_back(buf);
        sizeBuffer = design[buf.Type()].SizeX() * design[buf.Type()].SizeY();
        ReportBufferInfo(buf);
    }
    else
    {
        string sBufferList = design.cfg.ValueOf("BufferList", "");
        unsigned int n = design.cfg.ValueOf("BufferListLength", 0);
        string* bufferList = NULL;
        if (n > 0)
        {
            bufferList = new string [n];     
            for (unsigned int i = 0, j = 0, t = 0; (i < sBufferList.length()) && (j < n); i++, t++)
            {
                if(sBufferList[i] != ',')
                    bufferList[j].push_back(sBufferList[i]);
                else
                {
                    t = -1;
                    //bufferList[j].push_back(0);
                    j++;
                }
            }
            //bufferList[n - 1].push_back(0);
        }

        Buffers = BufferInfo::CreateVector(design, bufferList);
        for (unsigned int i = 0; i < Buffers.size(); i++)
        {
            if ((design[Buffers[i].Type()].SizeX() * design[Buffers[i].Type()].SizeY()) > sizeBuffer)
                sizeBuffer = design[Buffers[i].Type()].SizeX() * design[Buffers[i].Type()].SizeY();
            ReportBufferInfo(Buffers[i]);
        }
    }
}


HDesign& VGAlgorithmData::GetDesign()
{
    return design;
}

HWirePhysicalParams& VGAlgorithmData::GetWirePhisics()
{
    return WirePhisics;
}

TemplateTypes<BufferInfo>::vector VGAlgorithmData::GetBuffers()
{
    return Buffers;
}

int VGAlgorithmData::GetPartitionCount()
{
    return partitionCount;
}

bool VGAlgorithmData::GetPlotVGTree()
{
    return plotVGTree;
}

bool VGAlgorithmData::GetPlotNets()
{
    return plotNets;
}

bool VGAlgorithmData::GetPrintNetInfo()
{
    return printNetInfo;
}

bool VGAlgorithmData::GetPlotSteinerPoint()
{
    return plotSteinerPoint;
}

bool VGAlgorithmData::GetPrintVariantsList()
{
    return printVariantsList;
}

int VGAlgorithmData::GetPlotterWaitTime()
{
    return plotterWaitTime;
}

bool VGAlgorithmData::GetIsInitialize()
{
    return isInitialize;
}

bool VGAlgorithmData::GetIsInsertInSourceAndSink()
{
    return isInsertInSourceAndSink;
}

int VGAlgorithmData::GetTypeBufferingAlgorithm()
{
    return typeBufferingAlgorithm;
}

int VGAlgorithmData::GetTypePartition()
{
    return typePartition;
}

double VGAlgorithmData::GetSizeBuffer()
{
    return sizeBuffer;
}

int VGAlgorithmData::GetMaxIndex()
{
    return maxIndex;
}

VanGinnekenTree* VGAlgorithmData::GetVGTree()
{
    return vGTree;
}

bool* VGAlgorithmData::GetNetVisit()
{
    return netVisit;
}

int VGAlgorithmData::GetMaxBufferCount()
{
    return maxBufferCount;
}

int VGAlgorithmData::GetTypeModificationVanGinnekenList()
{
    return typeModificationVanGinnekenList;
}

int VGAlgorithmData::GetCountPinInBufferingNet()
{
    return countPinInBufferingNet;
}

bool VGAlgorithmData::GetIsTypeLimitationOnCountPinInBufferingNetEquality()
{
    return isTypeLimitationOnCountPinInBufferingNetEquality;
}

string VGAlgorithmData::GetNameBufferingNet()
{
    return nameBufferingNet;
}

int VGAlgorithmData::GetTypeBuferAddition()
{
    return typeBuferAddition;
}

double VGAlgorithmData::GetSizeBufferMultiplier()
{
    return sizeBufferMultiplier;
}

bool VGAlgorithmData::GetIsBufferingNetContainPrimaryPin()
{
    return isBufferingNetContainPrimaryPin;
}

double VGAlgorithmData::GetTotalAreaOfBuffersInRelationToAllCells()
{
    return totalAreaOfBuffersInRelationToAllCells;
}

double VGAlgorithmData::GetTotalAreaCells()
{
    return totalAreaCells;
}

double VGAlgorithmData::GetTotalAreaBuffer()
{
    return totalAreaBuffer;
}

bool VGAlgorithmData::GetIsBuffering()
{
    if (totalAreaOfBuffersInRelationToAllCells != 0)
        if (totalAreaBuffer > totalAreaOfBuffersInRelationToAllCells * totalAreaCells)
            return false;  
    return true;
}

double VGAlgorithmData::GetPercentAreaComposeBuffers()
{
    return totalAreaBuffer / totalAreaCells * 100.0;
}

bool VGAlgorithmData::GetPlotBuffer()
{
    return plotBuffer;
}
bool VGAlgorithmData::GetPlotBinGridValue()
{
    return plotBinGridValue;
}

void VGAlgorithmData::SetWirePhisics(HWirePhysicalParams& wPP)
{
    WirePhisics = wPP;
}

void VGAlgorithmData::SetBuffers(TemplateTypes<BufferInfo>::vector buf)
{
    Buffers = buf;
}

void VGAlgorithmData::SetPartitionCount(int pC)
{
    partitionCount = pC;
}

void VGAlgorithmData::SetPlotVGTree(bool pVGT)
{
    plotVGTree = pVGT;
}

void VGAlgorithmData::SetPlotNets(bool pN)
{
    plotNets = pN;
}

void VGAlgorithmData::SetPrintNetInfo(bool pNI)
{
    printNetInfo = pNI;
}

void VGAlgorithmData::SetPlotSteinerPoint(bool pSP)
{
    plotSteinerPoint = pSP;
}

void VGAlgorithmData::SetPrintVariantsList(bool pVL)
{
    printVariantsList = pVL;
}

void VGAlgorithmData::SetPlotterWaitTime(int pWT)
{
    plotterWaitTime = pWT;
}

void VGAlgorithmData::SetIsInitialize(bool iIL)
{
    isInitialize = iIL;
}

void VGAlgorithmData::SetIsInsertInSourceAndSink(bool iIISAS)
{
    isInsertInSourceAndSink = iIISAS;
}

void VGAlgorithmData::SetTypeBufferingAlgorithm(int tBA)
{
    typeBufferingAlgorithm = tBA;
}

void VGAlgorithmData::SetTypePartition(int tP)
{
    typePartition = tP;
}

void VGAlgorithmData::SetSizeBuffer(double sB)
{
    sizeBuffer = sB;
}

void VGAlgorithmData::SetMaxIndex(int mI)
{
    maxIndex = mI;
}

void VGAlgorithmData::SetVGTree(VanGinnekenTree* vgT)
{
    vGTree = vgT;
}

void VGAlgorithmData::SetNetVisit(bool* nV)
{
    netVisit = nV;
}

void VGAlgorithmData::SetMaxBufferCount(int mBC)
{
    maxBufferCount = mBC;
}

void VGAlgorithmData::SetTypeModificationVanGinnekenList(int tMVGL)
{
    typeModificationVanGinnekenList = tMVGL;
}

void VGAlgorithmData::SetCountPinInBufferingNet(int cPIBN)
{
    countPinInBufferingNet = cPIBN;
}

void VGAlgorithmData::SetIsTypeLimitationOnCountPinInBufferingNetEquality(bool iTLCPIBNE)
{
    isTypeLimitationOnCountPinInBufferingNetEquality = iTLCPIBNE;
}

void VGAlgorithmData::SetNameBufferingNet(string nBN)
{
    nameBufferingNet = nBN;
}

void VGAlgorithmData::SetTypeBuferAddition(int tBA)
{
    typeBuferAddition = tBA;
}
void VGAlgorithmData::SetSizeBufferMultiplier(double sBM)
{
    sizeBufferMultiplier = sBM;
}

void VGAlgorithmData::SetIsBufferingNetContainPrimaryPin(bool iBNCPP)
{
    isBufferingNetContainPrimaryPin = iBNCPP;
}

void VGAlgorithmData::SetTotalAreaOfBuffersInRelationToAllCells(double tAOBIRTAC)
{
    totalAreaOfBuffersInRelationToAllCells = tAOBIRTAC;
}

void VGAlgorithmData::SetTotalAreaCells()
{
    totalAreaCells = 0;
    for (HCells::CellsEnumeratorW cw = design.Cells.GetEnumeratorW(); cw.MoveNext();)
        totalAreaCells += cw.Width() * cw.Height();
}

void VGAlgorithmData::AddAreaBuffer(double bA)
{
    totalAreaBuffer += bA;
}

void VGAlgorithmData::SetTotalAreaBuffer(double tAB)
{
    totalAreaBuffer = tAB;
}

void VGAlgorithmData::SetPlotBuffer(bool pB)
{
    plotBuffer = pB;
}

void VGAlgorithmData::SetPlotBinGridValue(bool pBGV)
{
    plotBinGridValue = pBGV;
}