#include "VanGinnekenTreeCreate.h"
#include "VanGinnekenTree.h"
#include "VanGinnekenData.h"
#include "HDPGrid.h"

VanGinnekenTreeNodeCreate::VanGinnekenTreeNodeCreate(VanGinnekenTree* tree)
{
  vGTree = tree;
}

VanGinnekenTreeNode* VanGinnekenTreeNodeCreate::CreateSource(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, VanGinnekenTree* tree)
{
  HSteinerPointWrapper spw = vGTree->vGAlgorithmData->design[node];
  vGTree->vGTreeNodeList[nodeIndex]->SetSteinerPoint(node);
  vGTree->vGTreeNodeList[nodeIndex]->type = type;
  vGTree->vGTreeNodeList[nodeIndex]->x = (spw.X());
  vGTree->vGTreeNodeList[nodeIndex]->y = (spw.Y());
  vGTree->vGTreeNodeList[nodeIndex]->index = (nodeIndex);
  vGTree->vGTreeNodeList[nodeIndex]->SetTree(tree);
  vGTree->vGTreeNodeList[nodeIndex]->SetNet(net);
  return vGTree->vGTreeNodeList[nodeIndex];
}

VanGinnekenTreeNode* VanGinnekenTreeNodeCreate::CreatePotentialBufferPoint(HNet net, NodeType type, int& nodeIndex, VanGinnekenTree* tree)
{
  nodeIndex++;
  vGTree->vGTreeNodeList[nodeIndex]->type = type;	
  vGTree->vGTreeNodeList[nodeIndex]->index = (nodeIndex);
  vGTree->vGTreeNodeList[nodeIndex]->SetTree(tree);
  vGTree->vGTreeNodeList[nodeIndex]->SetNet(net);
  return vGTree->vGTreeNodeList[nodeIndex];
}

void VanGinnekenTreeNodeCreate::CreateFirstPointInEdge(HNet net, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  if (isRight)
    vGTree->vGTreeNodeList[rootIndex]->SetRight(CreatePotentialBufferPoint(net, CANDIDATE, nodeIndex, tree));
  else
    vGTree->vGTreeNodeList[rootIndex]->SetLeft(CreatePotentialBufferPoint(net, CANDIDATE, nodeIndex, tree));

  vGTree->vGTreeNodeList[nodeIndex]->x = (vGTree->vGTreeNodeList[rootIndex]->x);
  vGTree->vGTreeNodeList[nodeIndex]->y = (vGTree->vGTreeNodeList[rootIndex]->y);

  vGTree->vGTreeNodeList[nodeIndex]->type = CANDIDATE_INTERNAL;

  if (vGTree->vGTreeNodeList[rootIndex]->isSource())
    vGTree->vGTreeNodeList[nodeIndex]->type = CANDIDATE_ON_PIN;

  rootIndex = nodeIndex;
}

void VanGinnekenTreeNodeCreate::CreatePotentialBufferPointInEdge(HNet net, HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  double rx,ry,nx,ny;
  rx = vGTree->vGTreeNodeList[rootIndex]->x;
  ry = vGTree->vGTreeNodeList[rootIndex]->y;
  nx = vGTree->vGAlgorithmData->design[node].X();
  ny = vGTree->vGAlgorithmData->design[node].Y();

  for (int i = 1; i < (vGTree->vGAlgorithmData->partitionPointCount - 1); i++)
  {
    vGTree->vGTreeNodeList[rootIndex]->SetLeft(CreatePotentialBufferPoint(net, CANDIDATE, nodeIndex, tree));
    vGTree->vGTreeNodeList[nodeIndex]->x = (rx + (nx - rx) / (vGTree->vGAlgorithmData->partitionPointCount - 1) * i);
    vGTree->vGTreeNodeList[nodeIndex]->y = (ry + (ny - ry) / (vGTree->vGAlgorithmData->partitionPointCount - 1) * i);
    rootIndex = nodeIndex;
  }
}

void VanGinnekenTreeNodeCreate::CreateSinkOrBranchPoint(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  nodeIndex++;
  vGTree->vGTreeNodeList[nodeIndex]->SetSteinerPoint(node);
  vGTree->vGTreeNodeList[nodeIndex]->x = (vGTree->vGAlgorithmData->design[node].X());
  vGTree->vGTreeNodeList[nodeIndex]->y = (vGTree->vGAlgorithmData->design[node].Y());
  vGTree->vGTreeNodeList[rootIndex]->SetLeft(vGTree->vGTreeNodeList[nodeIndex]);
  vGTree->vGTreeNodeList[nodeIndex]->index = (nodeIndex);
  vGTree->vGTreeNodeList[nodeIndex]->SetTree(tree);
  vGTree->vGTreeNodeList[nodeIndex]->SetNet(net);

  if (!vGTree->vGAlgorithmData->design[node].IsInternal())
  {
    vGTree->vGTreeNodeList[nodeIndex]->type = SINK;
  }
  else
  {
    vGTree->vGTreeNodeList[nodeIndex]->type = type;	
  }

  if (nodeIndex >= vGTree->vGAlgorithmData->totalTreeSize)
    ALERT("ERROR2!!!!!");
}

void VanGinnekenTreeNodeCreate::CreateLastPointInEdge(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  vGTree->vGTreeNodeList[rootIndex]->SetLeft(CreatePotentialBufferPoint(net, CANDIDATE, nodeIndex, tree));
  vGTree->vGTreeNodeList[nodeIndex]->x = vGTree->vGAlgorithmData->design[node].X();
  vGTree->vGTreeNodeList[nodeIndex]->y = vGTree->vGAlgorithmData->design[node].Y();

  vGTree->vGTreeNodeList[nodeIndex]->type = CANDIDATE_INTERNAL;

  if (!vGTree->vGAlgorithmData->design[node].IsInternal())
  {
    vGTree->vGTreeNodeList[nodeIndex]->type = CANDIDATE_ON_PIN;
  }

  rootIndex = nodeIndex;
}

void VanGinnekenTreeNodeCreate::CreateNewPoint(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, int rootIndex, bool isRight, VanGinnekenTree* tree)
{  
  CreateFirstPointInEdge(net, nodeIndex, rootIndex, isRight, tree);
  CreatePotentialBufferPointInEdge(net, node, nodeIndex, rootIndex, isRight, tree);
  CreateLastPointInEdge(net, node, type, nodeIndex, rootIndex, isRight, tree);
  CreateSinkOrBranchPoint(net, node, type, nodeIndex, rootIndex, isRight, tree);
}

void VanGinnekenTreeNodeCreate::RefreshSoursAndSinkPoint(HNet net, HSteinerPoint node, NodeType type, int& rootIndex)
{
  vGTree->vGTreeNodeList[rootIndex]->SetSteinerPoint(node, false);
  vGTree->vGTreeNodeList[rootIndex]->SetNet(net, false);
  vGTree->vGTreeNodeList[rootIndex]->type = type;
}

void VanGinnekenTreeDynamicDistributionNodeCreate::CreatePotentialBufferPointInEdge(HNet net, HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  double rx,ry,nx,ny;
  rx = vGTree->vGTreeNodeList[rootIndex]->x;
  ry = vGTree->vGTreeNodeList[rootIndex]->y;
  nx = vGTree->vGAlgorithmData->design[node].X();
  ny = vGTree->vGAlgorithmData->design[node].Y();

  int pPC = vGTree->vGAlgorithmData->partitionPointCount;

  int rsy = vGTree->DPGrid->FindRow(ry);
  int rfy = vGTree->DPGrid->FindRow(ny);
  int rsx = vGTree->DPGrid->FindColumn(rx);
  int rfx = vGTree->DPGrid->FindColumn(nx);

  int RowCount = abs(rsy - rfy + 1);
  int ColumnCount = abs(rsx - rfx + 1);
  int n = RowCount + ColumnCount;

  int pointInRowCount = 0;
  int newPartitionPointCount = 0;
  if (((vGTree->vGAlgorithmData->partitionPointCount - 1.0) / n) < 1)
  {
    newPartitionPointCount = n + 1;
  }
  else
  {
    pointInRowCount = int(ceil(((vGTree->vGAlgorithmData->partitionPointCount - 1.0) / n)));
    newPartitionPointCount = pointInRowCount * n + 1;
  }
  pPC = newPartitionPointCount;

  for (int i = 1; i < (pPC - 1); i++)
  {
    vGTree->vGTreeNodeList[rootIndex]->SetLeft(CreatePotentialBufferPoint(net, CANDIDATE, nodeIndex, tree));
    vGTree->vGTreeNodeList[nodeIndex]->x = (rx + (nx - rx) / (pPC - 1) * i);
    vGTree->vGTreeNodeList[nodeIndex]->y = (ry + (ny - ry) / (pPC - 1) * i);
    rootIndex = nodeIndex;
  }
}

void VanGinnekenTreeLegalDynamicDistributionNodeCreate::CreatePotentialBufferPointInEdge(HNet net, HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight, VanGinnekenTree* tree)
{
  int lenPath = 0;
  double rx,ry,nx,ny;
  rx = vGTree->vGTreeNodeList[rootIndex]->x;
  ry = vGTree->vGTreeNodeList[rootIndex]->y;
  nx = vGTree->vGAlgorithmData->design[node].X();
  ny = vGTree->vGAlgorithmData->design[node].Y();
  PlacementGridNode **path = vGTree->pGrid.GetPath(lenPath, rx, ry, nx, ny);    
  for (int i = 0; i < lenPath; i++)
  {
    vGTree->vGTreeNodeList[rootIndex]->SetLeft(CreatePotentialBufferPoint(net, CANDIDATE, nodeIndex, tree));
    vGTree->vGTreeNodeList[nodeIndex]->x = (path[i]->GetX());
    vGTree->vGTreeNodeList[nodeIndex]->y = (path[i]->GetY());
    rootIndex = nodeIndex;
  }
}

VanGinnekenTreeCreate::VanGinnekenTreeCreate(VanGinnekenTree* tree)
{
  vGTree = tree;
}

void VanGinnekenTreeCreate::CreateTree()
{
  int maxTree = 0;
  int treeSizeTemp = 0;
  for (HNets::ActiveNetsEnumeratorW enumerator = vGTree->vGAlgorithmData->design.Nets.GetActiveNetsEnumeratorW(); enumerator.MoveNext(); )
  {
    CalculateNodeInEnumerator<HNets::ActiveNetsEnumeratorW>(enumerator, maxTree, treeSizeTemp);
  }
  vGTree->vGAlgorithmData->totalTreeSize = maxTree;
  vGTree->vGTreeNodeList = new VanGinnekenTreeNode* [maxTree];
  for (int i = 0; i < maxTree; i++)
    vGTree->vGTreeNodeList[i] = new VanGinnekenTreeNode();
}

template< class EnumeratorW >
void VanGinnekenTreeCreate::CalculateNodeInEnumerator(EnumeratorW enumerator, int& maxTree, int& treeSizeTemp)
{
  HSteinerPointWrapper srcPoint = vGTree->vGAlgorithmData->design[GetSourceEnumerator(enumerator)];
  HSteinerPointWrapper nextPoint = srcPoint;
  int pointCount = 0;
  TemplateTypes<HSteinerPoint>::stack points;
  int maxPos = vGTree->vGAlgorithmData->partitionPointCount;
  points.push(srcPoint);

  while (!points.empty())
  {
    srcPoint = points.top();
    points.pop();

    if (srcPoint.HasLeft())
    {
      vGTree->calculateVanGinnekenSubtree->CalculateLeftSubtree(points, srcPoint, nextPoint, pointCount, maxTree);

      if (srcPoint.HasRight())
      {
        vGTree->calculateVanGinnekenSubtree->CalculateRightSubtree(points, srcPoint, nextPoint, pointCount, maxTree);
      }
    }
    else
    {
      vGTree->calculateVanGinnekenSubtree->CalculatePinSubtree(points, srcPoint, nextPoint, pointCount, maxTree, enumerator);
    }
  }

  treeSizeTemp = pointCount * maxPos + pointCount + 1;
  if (treeSizeTemp > maxTree)
    maxTree = treeSizeTemp;
}

HSteinerPoint VanGinnekenTreeCreate::GetSourceEnumerator(HNets::ActiveNetsEnumeratorW enumerator)
{
  return vGTree->vGAlgorithmData->design.SteinerPoints[enumerator.Source()];
}

void VanGinnekenTreeCreatePathBased::CreateTree()
{
  int maxTree = 0;
  int treeSizeTemp = 0;
  for (HCriticalPaths::EnumeratorW enumerator = vGTree->vGAlgorithmData->design.CriticalPaths.GetEnumeratorW(); enumerator.MoveNext();)
  {
    /*ALERT("riticalPath = %d; all point:", ::ToID(enumerator));
    for (HCriticalPath::PointsEnumeratorW en = enumerator.GetEnumeratorW(); en.MoveNext();)
      ALERT("\tPoint = %d", ::ToID(en));
    */
    CalculateNodeInEnumerator<HCriticalPaths::EnumeratorW>(enumerator, maxTree, treeSizeTemp);
  }
  vGTree->vGAlgorithmData->totalTreeSize = maxTree;
  vGTree->vGTreeNodeList = new VanGinnekenTreeNode* [maxTree];
  for (int i = 0; i < maxTree; i++)
    vGTree->vGTreeNodeList[i] = new VanGinnekenTreeNodePathBased();
}

HSteinerPoint VanGinnekenTreeCreate::GetSourceEnumerator(HCriticalPaths::EnumeratorW enumerator)
{
  return vGTree->vGAlgorithmData->design.SteinerPoints[
    vGTree->vGAlgorithmData->design.Nets.Get<HNet::Source, HPin>(vGTree->vGAlgorithmData->design.Pins.Get<HPin::OriginalNet, 
      HNet>(vGTree->vGAlgorithmData->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(
      vGTree->vGAlgorithmData->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, 
      HTimingPoint>(enumerator.StartPoint()))))];
}

CalculateVanGinnekenSubtree::CalculateVanGinnekenSubtree(VanGinnekenTree* tree)
{
  vGTree = tree;
}

void CalculateVanGinnekenSubtree::CalculateLeftSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree)
{
  nextPoint = srcPoint.Left();
  vGTree->findMaxPointInEdgeVanGinnekenTree->FindNewMaxPointInEdge(srcPoint, nextPoint, maxTree);
  pointCount++;
  points.push(nextPoint);
}

void CalculateVanGinnekenSubtree::CalculateRightSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree)
{
  nextPoint = srcPoint.Right();
  vGTree->findMaxPointInEdgeVanGinnekenTree->FindNewMaxPointInEdge(srcPoint, nextPoint, maxTree);
  pointCount++;
  points.push(nextPoint);
}

void CalculateVanGinnekenSubtree::CalculatePinSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree, HNets::ActiveNetsEnumeratorW enumerator)
{
  pointCount++;
}

void CalculateVanGinnekenSubtree::CalculatePinSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree, HCriticalPaths::EnumeratorW enumerator)
{
  bool isCriticalPathPoint = false;
  for (HCriticalPath::PointsEnumeratorW point = enumerator.GetEnumeratorW(); point.MoveNext();)
  {
    if (srcPoint == vGTree->vGAlgorithmData->design[vGTree->vGAlgorithmData->design.SteinerPoints[
      vGTree->vGAlgorithmData->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(point.TimingPoint())]])
      {
        bool isEndCriticalPath = point.MoveNext();
        if (isEndCriticalPath)
          isCriticalPathPoint = true;
        else
          break;
        
        nextPoint = vGTree->vGAlgorithmData->design[vGTree->vGAlgorithmData->design.SteinerPoints[
          vGTree->vGAlgorithmData->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(point.TimingPoint())]];
          vGTree->findMaxPointInEdgeVanGinnekenTree->FindNewMaxPointInEdge(srcPoint, nextPoint, maxTree);
          pointCount++;
          points.push(nextPoint);
          break;
      }
  }
  if (!isCriticalPathPoint)
    pointCount++;
}

FindMaxPointInEdgeVanGinnekenTree::FindMaxPointInEdgeVanGinnekenTree(VanGinnekenTree* tree)
{
  vGTree = tree;
}

void FindMaxPointInEdgeVGTreeDynamicDistribution::FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos)
{
  double rx,ry,nx,ny;
  rx = vGTree->vGAlgorithmData->design[srcPoint].X();
  ry = vGTree->vGAlgorithmData->design[srcPoint].Y();
  nx = vGTree->vGAlgorithmData->design[nextPoint].X();
  ny = vGTree->vGAlgorithmData->design[nextPoint].Y();

  int rsy = vGTree->DPGrid->FindRow(ry);
  int rfy = vGTree->DPGrid->FindRow(ny);
  int rsx = vGTree->DPGrid->FindColumn(rx);
  int rfx = vGTree->DPGrid->FindColumn(nx);

  int RowCount = abs(rsy - rfy + 1);
  int ColumnCount = abs(rsx - rfx + 1);
  int n = RowCount + ColumnCount;

  int rowInd = 0;
  int pointInRowCount = 0;
  int newPartitionPointCount = 0;
  if (((vGTree->vGAlgorithmData->partitionPointCount - 1.0) / n) < 1)
  {
    newPartitionPointCount = n + 1;
    pointInRowCount = 1;
  }
  else
  {
    pointInRowCount = int(ceil(((vGTree->vGAlgorithmData->partitionPointCount - 1.0) / n)));
    newPartitionPointCount = pointInRowCount * n + 1;
  }

  if (newPartitionPointCount > maxPos)
    maxPos = newPartitionPointCount;
}

void FindMaxPointInEdgeVanGinnekenTreeLegalDynamicDistribution::FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos)
{
  double rx,ry,nx,ny;
  rx = vGTree->vGAlgorithmData->design[srcPoint].X();
  ry = vGTree->vGAlgorithmData->design[srcPoint].Y();
  nx = vGTree->vGAlgorithmData->design[nextPoint].X();
  ny = vGTree->vGAlgorithmData->design[nextPoint].Y();

  int rsy = vGTree->pGrid.GetRow(ry);
  int rfy = vGTree->pGrid.GetRow(ny);
  int rsx = vGTree->pGrid.GetColumn(rx);
  int rfx = vGTree->pGrid.GetColumn(nx);

  int RowCount = abs(rsy - rfy) + 1;
  int ColumnCount = abs(rsx - rfx) + 1;
  int n = RowCount * ColumnCount;

  int rowInd = 0;
  int pointInRowCount = 0;
  int newPartitionPointCount = 0;
  if (((vGTree->vGAlgorithmData->partitionPointCount - 1.0) / n) < 1)
  {
    newPartitionPointCount = n + 1;
    pointInRowCount = 1;
  }
  else
  {
    pointInRowCount = int(ceil(((vGTree->vGAlgorithmData->partitionPointCount - 1.0) / n)));
    newPartitionPointCount = pointInRowCount * n + 1;
  }

  if (newPartitionPointCount > maxPos)
    maxPos = newPartitionPointCount;
}