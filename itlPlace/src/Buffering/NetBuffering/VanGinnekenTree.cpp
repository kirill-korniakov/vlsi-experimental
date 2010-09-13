#include "VanGinnekenTree.h"
#include "NetBufferingAlgorithm.h"
#include "HDPGrid.h"

VanGinnekenTree::VanGinnekenTree(NetBufferingAlgorithm* vGA):  pGrid(vGA->data->design)
{
  vGAlgorithm = vGA;
  treeSize = 0;
  DPGrid = new HDPGrid(vGA->data->design);
  partitionPointCount = vGAlgorithm->data->design.cfg.ValueOf("Interval", 1);
  TypePartition = PartitionType(vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0));
}

VanGinnekenTree::VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount): pGrid(vGA->data->design)
{
  vGAlgorithm = vGA;
  treeSize = 0;
  DPGrid = new HDPGrid(vGA->data->design);
  partitionPointCount = partitionCount;
  TypePartition = PartitionType(vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0));
}

VanGinnekenTree::VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount, double sizeBuffer): pGrid(vGA->data->design, sizeBuffer)
{
  vGAlgorithm = vGA;
  treeSize = 0;
  DPGrid = new HDPGrid(vGA->data->design);
  partitionPointCount = partitionCount;
  TypePartition = PartitionType(vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0));
}

VanGinnekenTree::VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount, HSteinerPoint& source): pGrid(vGA->data->design)
{
  vGAlgorithm = vGA;
  treeSize = 0;
  DPGrid = new HDPGrid(vGA->data->design);
  partitionPointCount = partitionCount;
  TypePartition = PartitionType(vGAlgorithm->data->design.cfg.ValueOf("TypePartition", 0));
}

template< class EnumeratorW >
HSteinerPoint VanGinnekenTree::GetSourceEnumerator(EnumeratorW enumerator)
{
  return vGAlgorithm->data->design.SteinerPoints[enumerator.Source()];
}

void VanGinnekenTree::CalculateLeftSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree)
{
  nextPoint = srcPoint.Left();
  FindNewMaxPointInEdge(srcPoint, nextPoint, maxTree);
  pointCount++;
  points.push(nextPoint);
}

void VanGinnekenTree::CalculateRightSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree)
{
  nextPoint = srcPoint.Right();
  FindNewMaxPointInEdge(srcPoint, nextPoint, maxTree);
  pointCount++;
  points.push(nextPoint);
}

template< class EnumeratorW >
void VanGinnekenTree::CalculatePinSubtree(EnumeratorW enumerator,TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree)
{
      pointCount++;
}

template< class EnumeratorW >
void VanGinnekenTree::CalculateNodeInEnumerator(EnumeratorW enumerator, int& maxTree, int& treeSizeTemp)
{
  HSteinerPointWrapper srcPoint = vGAlgorithm->data->design[GetSourceEnumerator<EnumeratorW>(enumerator)];
  HSteinerPointWrapper nextPoint = srcPoint;
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
      CalculateLeftSubtree(points, srcPoint, nextPoint, pointCount, maxTree);

      if (srcPoint.HasRight())
      {
        CalculateRightSubtree(points, srcPoint, nextPoint, pointCount, maxTree);
      }
    }
    else
    {
      CalculatePinSubtree<EnumeratorW>(enumerator, points, srcPoint, nextPoint, pointCount, maxTree);
    }
  }

  treeSizeTemp = pointCount * maxPos + pointCount + 1;
  if (treeSizeTemp > maxTree)
    maxTree = treeSizeTemp;
}

void VanGinnekenTree::CreateTree()
{
  int maxTree = 0;
  int treeSizeTemp = 0;
  for (HNets::ActiveNetsEnumeratorW enumerator = vGAlgorithm->data->design.Nets.GetActiveNetsEnumeratorW(); enumerator.MoveNext(); )
  {
    CalculateNodeInEnumerator<HNets::ActiveNetsEnumeratorW>(enumerator, maxTree, treeSizeTemp);
  }
  totalTreeSize = maxTree;
  vGTree = new VanGinnekenTreeNode [maxTree];
}

void VanGinnekenTree::CreateSource(HSteinerPoint node, NodeType type, int& nodeIndex, VanGinnekenTree* tree)
{
  HSteinerPointWrapper spw = vGAlgorithm->data->design[node];
  vGTree[nodeIndex].SetSteinerPoint(node);
  vGTree[nodeIndex].type = type;
  vGTree[nodeIndex].x = (spw.X());
  vGTree[nodeIndex].y = (spw.Y());
  vGTree[nodeIndex].index = (nodeIndex);
  vGTree[nodeIndex].SetTree(tree);
}

void VanGinnekenTree::CreatePotentialBufferPoint(NodeType type, int& nodeIndex, VanGinnekenTree* tree)
{
  nodeIndex++;
  vGTree[nodeIndex].type = type;	
  vGTree[nodeIndex].index = (nodeIndex);
  vGTree[nodeIndex].SetTree(tree);
}

void VanGinnekenTree::CreateFirstPointInEdge(int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  if (isRight)
    vGTree[rootIndex].SetRight(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), CANDIDATE, nodeIndex, rootIndex, isRight, tree));
  else
    vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), CANDIDATE, nodeIndex, rootIndex, isRight, tree));

  vGTree[nodeIndex].x = (vGTree[rootIndex].x);
  vGTree[nodeIndex].y = (vGTree[rootIndex].y);

  vGTree[nodeIndex].type = CANDIDATE_INTERNAL;

  if (vGTree[rootIndex].isSource())
    vGTree[nodeIndex].type = CANDIDATE_ON_PIN;

  rootIndex = nodeIndex;
}

void VanGinnekenTree::CreatePotentialBufferPointInEdge(HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  double rx,ry,nx,ny;
  rx = vGTree[rootIndex].x;
  ry = vGTree[rootIndex].y;
  nx = vGAlgorithm->data->design[node].X();
  ny = vGAlgorithm->data->design[node].Y();

  for (int i = 1; i < (partitionPointCount - 1); i++)
  {
    vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), CANDIDATE, nodeIndex, rootIndex, isRight, tree));
    vGTree[nodeIndex].x = (rx + (nx - rx) / (partitionPointCount - 1) * i);
    vGTree[nodeIndex].y = (ry + (ny - ry) / (partitionPointCount - 1) * i);
    rootIndex = nodeIndex;
  }
}

void VanGinnekenTree::CreateLastPointInEdgeAndSink(HSteinerPoint node, NodeType type, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), CANDIDATE, nodeIndex, rootIndex, isRight, tree));
  vGTree[nodeIndex].x = (vGAlgorithm->data->design[node].X());
  vGTree[nodeIndex].y = (vGAlgorithm->data->design[node].Y());

  vGTree[nodeIndex].type = CANDIDATE_INTERNAL;

  if (!vGAlgorithm->data->design[node].IsInternal())
  {
    vGTree[nodeIndex].type = CANDIDATE_ON_PIN;
  }

  rootIndex = nodeIndex;

  nodeIndex++;
  vGTree[nodeIndex].SetSteinerPoint(node);
  vGTree[nodeIndex].x = (vGAlgorithm->data->design[node].X());
  vGTree[nodeIndex].y = (vGAlgorithm->data->design[node].Y());
  vGTree[rootIndex].SetLeft(&vGTree[nodeIndex]);
  vGTree[nodeIndex].index = (nodeIndex);
  vGTree[nodeIndex].SetTree(tree);

  if (!vGAlgorithm->data->design[node].IsInternal())
  {
    vGTree[nodeIndex].type = SINK;
  }
  else
  {
    vGTree[nodeIndex].type = type;	
  }

  if (nodeIndex >= totalTreeSize)
    ALERT("ERROR2!!!!!");
}

void VanGinnekenTree::CreateNewPoint(HSteinerPoint node, NodeType type, int& nodeIndex, int rootIndex, bool isRight, VanGinnekenTree* tree)
{  
  CreateFirstPointInEdge(nodeIndex, rootIndex, isRight, tree);
  CreatePotentialBufferPointInEdge(node, nodeIndex, rootIndex, isRight, tree);
  CreateLastPointInEdgeAndSink(node, type, nodeIndex, rootIndex, isRight, tree);
}

VanGinnekenTreeNode* VanGinnekenTree::CreateNode(HSteinerPoint node, NodeType type, int& nodeIndex, int rootIndex, bool isRight, VanGinnekenTree* tree)
{
  if (type == 0)
  {
    CreateSource(node, type, nodeIndex, tree);
  }
  if (type == 2)
  {
    CreateNewPoint(node, type, nodeIndex, rootIndex, isRight, tree);
  }
  if (type == 3)
  {
    CreatePotentialBufferPoint(type, nodeIndex, tree);
  }
  return &vGTree[nodeIndex];
}

void VanGinnekenTree::ClearTree()
{
  for (int i = 0; i < totalTreeSize; i++)
  {
    vGTree[i].SetLeft(NULL);
    vGTree[i].SetRight(NULL);
    vGTree[i].type = NodeType(-1);
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

  int nodeIndex = 0;
  int rootIndex = 0;
  int isPoitnVisit = 0;
  HSteinerPoint srcPoint = vGAlgorithm->data->design[source];
  HSteinerPoint nextPoint = srcPoint;
  points.push(srcPoint);	
  rootIndexs.push(0);
  isPoitnsVisits.push(0);
  CreateNode(srcPoint, SOURCE, nodeIndex, rootIndex, false, this);

  while (!points.empty())
  {
    srcPoint = points.top();
    rootIndex = rootIndexs.top();
    isPoitnVisit = isPoitnsVisits.top();
    isPoitnsVisits.pop();
    if (isPoitnVisit == 0)
    {
      if (vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::HasLeft, bool>(srcPoint))
      {
        isPoitnVisit = 1;
        nextPoint = vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Left, HSteinerPoint>(srcPoint);
        CreateNode(nextPoint, BRANCH, nodeIndex, rootIndex, false, this);
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
          CreateNode(nextPoint, BRANCH, nodeIndex, rootIndex, true, this);
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

VGTreeUniformDistribution::VGTreeUniformDistribution(NetBufferingAlgorithm* vGA, int partitionCount): VanGinnekenTree(vGA, partitionCount)
{
  CreateTree();
}

void VGTreeUniformDistribution::FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos)
{
}

VGTreeUniformDistribution::~VGTreeUniformDistribution()
{
  delete [] vGTree;
}

VGTreeDynamicDistribution::VGTreeDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount): VanGinnekenTree(vGA, partitionCount)
{
  CreateTree();
}

void VGTreeDynamicDistribution::FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos)
{
  double rx,ry,nx,ny;
  rx = vGAlgorithm->data->design[srcPoint].X();
  ry = vGAlgorithm->data->design[srcPoint].Y();
  nx = vGAlgorithm->data->design[nextPoint].X();
  ny = vGAlgorithm->data->design[nextPoint].Y();

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
}

void VGTreeDynamicDistribution::CreatePotentialBufferPointInEdge(HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  double rx,ry,nx,ny;
  rx = vGTree[rootIndex].x;
  ry = vGTree[rootIndex].y;
  nx = vGAlgorithm->data->design[node].X();
  ny = vGAlgorithm->data->design[node].Y();

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
    vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), CANDIDATE, nodeIndex, rootIndex, isRight, tree));
    vGTree[nodeIndex].x = (rx + (nx - rx) / (pPC - 1) * i);
    vGTree[nodeIndex].y = (ry + (ny - ry) / (pPC - 1) * i);
    rootIndex = nodeIndex;
  }
}

VGTreeDynamicDistribution::~VGTreeDynamicDistribution()
{
  delete [] vGTree;
}

VGTreeLegalDynamicDistribution::VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount): VanGinnekenTree(vGA, partitionCount)
{
  CreateTree();
}

VGTreeLegalDynamicDistribution::VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount, double sizeBuffer): VanGinnekenTree(vGA, partitionCount, sizeBuffer)
{
  CreateTree();
}

void VGTreeLegalDynamicDistribution::FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos)
{
  double rx,ry,nx,ny;
  rx = vGAlgorithm->data->design[srcPoint].X();
  ry = vGAlgorithm->data->design[srcPoint].Y();
  nx = vGAlgorithm->data->design[nextPoint].X();
  ny = vGAlgorithm->data->design[nextPoint].Y();

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
}

void VGTreeLegalDynamicDistribution::CreatePotentialBufferPointInEdge(HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  int lenPath = 0;
  double rx,ry,nx,ny;
  rx = vGTree[rootIndex].x;
  ry = vGTree[rootIndex].y;
  nx = vGAlgorithm->data->design[node].X();
  ny = vGAlgorithm->data->design[node].Y();
  PlacementGridNode **path = pGrid.GetPath(lenPath, rx, ry, nx, ny);    
  for (int i = 0; i < lenPath; i++)
  {
    vGTree[rootIndex].SetLeft(CreateNode(tree->vGAlgorithm->data->design.SteinerPoints.Null(), CANDIDATE, nodeIndex, rootIndex, isRight, tree));
    vGTree[nodeIndex].x = (path[i]->GetX());
    vGTree[nodeIndex].y = (path[i]->GetY());
    rootIndex = nodeIndex;
  }
}

VGTreeLegalDynamicDistribution::~VGTreeLegalDynamicDistribution()
{
  delete [] vGTree;
}