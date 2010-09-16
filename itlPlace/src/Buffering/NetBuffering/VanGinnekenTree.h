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
  VanGinnekenTreeNode* vGTreeNodeList;
  HPlacementGrid pGrid;

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
  virtual VanGinnekenTreeNode GetSource();

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

    int nodeIndex = 0;
    int rootIndex = 0;
    int isPoitnVisit = 0;
    HSteinerPoint srcPoint = GetSteinerPointByStartPoint(source);
    HNet net = vGTree->vGAlgorithmData->design.Pins.Get<HPin::Net, HNet>(vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(srcPoint));
    HSteinerPoint nextPoint = srcPoint;
    points.push(srcPoint);	
    rootIndexs.push(0);
    isPoitnsVisits.push(0);
    //CreateNode(srcPoint, SOURCE, nodeIndex, rootIndex, false, this);
    vGTree->vanGinnekenTreeNodeCreate->CreateSource(net, srcPoint, SOURCE, nodeIndex, vGTree);

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
          CreateNodeInLeftSubTree(net, points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint);
        }
        else
        {
          CreateNodeInSink(net, points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint, source);
        }
      }
      else
        if (isPoitnVisit == 1)
        {
          isPoitnVisit = 2;
          isPoitnsVisits.push(isPoitnVisit);  
          if (vGTree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::HasRight, bool>(srcPoint))
          {
            CreateNodeInRightSubTree(net, points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint);
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
    if (nodeIndex >= vGTree->vGAlgorithmData->totalTreeSize)
      ALERT("ERROR3!!!!! + totalTreeSize = %d\tnodeIndex = %d", vGTree->vGAlgorithmData->totalTreeSize, nodeIndex);
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

  void CreateNodeInLeftSubTree(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
    TemplateTypes<int>::stack& rootIndexs,
    TemplateTypes<int>::stack& isPoitnsVisits,
    int& nodeIndex,
    int& rootIndex,
    int& isPoitnVisit,
    HSteinerPoint& srcPoint,
    HSteinerPoint& nextPoint);
  void CreateNodeInSink(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
    TemplateTypes<int>::stack& rootIndexs,
    TemplateTypes<int>::stack& isPoitnsVisits,
    int& nodeIndex,
    int& rootIndex,
    int& isPoitnVisit,
    HSteinerPoint& srcPoint,
    HSteinerPoint& nextPoint, HSteinerPoint& source);
  void CreateNodeInRightSubTree(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
    TemplateTypes<int>::stack& rootIndexs,
    TemplateTypes<int>::stack& isPoitnsVisits,
    int& nodeIndex,
    int& rootIndex,
    int& isPoitnVisit,
    HSteinerPoint& srcPoint,
    HSteinerPoint& nextPoint);

  void CreateNodeInSink(HNet& net, TemplateTypes<HSteinerPoint>::stack& points,
    TemplateTypes<int>::stack& rootIndexs,
    TemplateTypes<int>::stack& isPoitnsVisits,
    int& nodeIndex,
    int& rootIndex,
    int& isPoitnVisit,
    HSteinerPoint& srcPoint,
    HSteinerPoint& nextPoint, HCriticalPath::PointsEnumeratorW& source);
};

#endif