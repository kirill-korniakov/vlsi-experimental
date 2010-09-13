#ifndef __VanGinnekenTree_H__
#define __VanGinnekenTree_H__

#include "VanGinnekenTreeNode.h"
#include "Utils.h"

class HDPGrid;
class NetBufferingAlgorithm;
using namespace Utils;



class VanGinnekenTree
{
public:

    enum PartitionType
    {
        LINEAR               = 0, //0 - обычное линейное разбиение
        DYNAMIC              = 1, //1 - динамическое распределение точек,
        LEGAL_POSITIONS_ONLY = 2  //2 - легальное распределение точек (использовать только с 1 или 2 типом буферизации)
    };

    NetBufferingAlgorithm* vGAlgorithm;
    HDPGrid* DPGrid;
    VanGinnekenTreeNode* vGTree;
    HPlacementGrid pGrid;
	HCriticalPath criticalPath;

    VanGinnekenTree(NetBufferingAlgorithm* vGA);
    VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount);
    VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount, double sizeBuffer);
    VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount, HSteinerPoint& source);

    virtual void ClearTree();
    virtual int GetTreeSize();

	template< class StartPoint >
	void UpdateTree(StartPoint source)
	{
	  ClearTree();
	  TemplateTypes<HSteinerPoint>::stack points;
	  TemplateTypes<int>::stack rootIndexs;
	  TemplateTypes<int>::stack isPoitnsVisits;

	  int nodeIndex = 0;
	  int rootIndex = 0;
	  int isPoitnVisit = 0;
	  HSteinerPoint srcPoint = GetSteinerPointByStartPoint(source);
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
			  CreateNodeInLeftSubTree(points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint);
		  }
		  else
		  {
			CreateNodeInSink(points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint);
		  }
		}
		else
		  if (isPoitnVisit == 1)
		  {
			isPoitnVisit = 2;
			isPoitnsVisits.push(isPoitnVisit);  
			if (vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::HasRight, bool>(srcPoint))
			{
			  CreateNodeInRightSubTree(points, rootIndexs, isPoitnsVisits, nodeIndex, rootIndex, isPoitnVisit, srcPoint, nextPoint);
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

    virtual double GetR();
    virtual VanGinnekenTreeNode GetSource();
    virtual void SetEdgePartitionCount(int partitionCount);

protected:
    int partitionPointCount;
    int totalTreeSize;  
    int treeSize;
    PartitionType TypePartition;

template< class StartPoint >
HSteinerPoint GetSteinerPointByStartPoint(StartPoint point)
{
	return HSteinerPoint(point);
}

	void CreateNodeInLeftSubTree(  TemplateTypes<HSteinerPoint>::stack& points,
  TemplateTypes<int>::stack& rootIndexs,
  TemplateTypes<int>::stack& isPoitnsVisits,
  int& nodeIndex,
  int& rootIndex,
  int& isPoitnVisit,
  HSteinerPoint& srcPoint,
  HSteinerPoint& nextPoint);
	void CreateNodeInSink(  TemplateTypes<HSteinerPoint>::stack& points,
  TemplateTypes<int>::stack& rootIndexs,
  TemplateTypes<int>::stack& isPoitnsVisits,
  int& nodeIndex,
  int& rootIndex,
  int& isPoitnVisit,
  HSteinerPoint& srcPoint,
  HSteinerPoint& nextPoint);
void CreateNodeInRightSubTree(  TemplateTypes<HSteinerPoint>::stack& points,
  TemplateTypes<int>::stack& rootIndexs,
  TemplateTypes<int>::stack& isPoitnsVisits,
  int& nodeIndex,
  int& rootIndex,
  int& isPoitnVisit,
  HSteinerPoint& srcPoint,
  HSteinerPoint& nextPoint);

    template< class EnumeratorW >
    HSteinerPoint GetSourceEnumerator(EnumeratorW enumerator);
    void CalculateLeftSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree);
    void CalculateRightSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree);
    template< class EnumeratorW >
    void CalculatePinSubtree(EnumeratorW enumerator,TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree);
    template< class EnumeratorW >
    void CalculateNodeInEnumerator(EnumeratorW enumerator, int& maxTree, int& treeSizeTemp);
    virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos) = 0;
    virtual void CreateTree();
    virtual void CreateSource(HSteinerPoint node, NodeType type, int& nodeIndex, VanGinnekenTree* tree = NULL);
    virtual void CreatePotentialBufferPoint(NodeType type, int& nodeIndex, VanGinnekenTree* tree = NULL);
    virtual void CreateFirstPointInEdge(int& nodeIndex, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void CreatePotentialBufferPointInEdge(HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void CreateLastPointInEdgeAndSink(HSteinerPoint node, NodeType type, int& nodeIndex, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void CreateNewPoint(HSteinerPoint node, NodeType type, int& nodeIndex, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual VanGinnekenTreeNode* CreateNode(HSteinerPoint node, NodeType type, int& nodeIndex, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
};

class VGTreeUniformDistribution: public VanGinnekenTree
{
public:
    VGTreeUniformDistribution(NetBufferingAlgorithm* vGA, int partitionCount);
    virtual ~VGTreeUniformDistribution();

protected:
    virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
};

class VGTreeDynamicDistribution: public VanGinnekenTree
{
public:
    VGTreeDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount);
    virtual ~VGTreeDynamicDistribution();

protected:
    virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
    virtual void CreatePotentialBufferPointInEdge(HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
};

class VGTreeLegalDynamicDistribution: public VanGinnekenTree
{
public:
    VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount);
    VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount, double sizeBuffer);

    virtual ~VGTreeLegalDynamicDistribution();

protected:
    virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
    virtual void CreatePotentialBufferPointInEdge(HSteinerPoint node, int& nodeIndex, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
};

#endif