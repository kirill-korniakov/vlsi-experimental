#ifndef __VanGinnekenTree_H__
#define __VanGinnekenTree_H__

#include "VanGinnekenTreeNode.h"
#include "Utils.h"

class HDPGrid;
class NetBufferingAlgorithm;
using namespace Utils;

class VanGinnekenTree
{
protected:
  int partitionPointCount;
  int totalTreeSize;  
  int treeSize;
  int TypePartition;
  virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos) = 0;
  virtual void CreateTree();
  virtual void CreateSource(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
  virtual void CreatePotentialBufferPoint(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
  virtual void CreateFirstPointInEdge(HSteinerPoint node, int type, int& index, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
  virtual void CreatePotentialBufferPointInEdge(HSteinerPoint node, int type, int& index, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
  virtual void CreateLastPointInEdgeAndSink(HSteinerPoint node, int type, int& index, int& rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
  virtual void CreateNewPoint(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
  virtual VanGinnekenTreeNode* CreateNode(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);

public:
  NetBufferingAlgorithm* vGAlgorithm;
  HDPGrid* DPGrid;
  VanGinnekenTreeNode* vGTree;
  HPlacementGrid pGrid;

  VanGinnekenTree(NetBufferingAlgorithm* vGA);
  VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount);
  VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount, double sizeBuffer);
  VanGinnekenTree(NetBufferingAlgorithm* vGA, int partitionCount, HSteinerPoint& source);

  virtual void ClearTree();
  virtual void UpdateTree(HSteinerPoint& source);
  virtual int GetTreeSize();

  virtual double GetR();
  virtual VanGinnekenTreeNode GetSource();
  virtual void SetEdgePartitionCount(int partitionCount);
};

class VGTreeUniformDistribution: public VanGinnekenTree
{
protected:
  virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
public:
  VGTreeUniformDistribution(NetBufferingAlgorithm* vGA);
  VGTreeUniformDistribution(NetBufferingAlgorithm* vGA, int partitionCount);
  VGTreeUniformDistribution(NetBufferingAlgorithm* vGA, int partitionCount, HSteinerPoint& source);

  virtual ~VGTreeUniformDistribution();
};

class VGTreeDynamicDistribution: public VanGinnekenTree
{
protected:
  virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
  virtual void CreatePotentialBufferPointInEdge(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
public:
  VGTreeDynamicDistribution(NetBufferingAlgorithm* vGA);
  VGTreeDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount);
  VGTreeDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount, HSteinerPoint& source);

  virtual ~VGTreeDynamicDistribution();
};

class VGTreeLegalDynamicDistribution: public VanGinnekenTree
{
protected:
  virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
  virtual void CreatePotentialBufferPointInEdge(HSteinerPoint node, int type, int& index, int rootIndex, bool isRight = false, VanGinnekenTree* tree = NULL);
public:
  VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA);
  VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount);
  VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount, double sizeBuffer);
  VGTreeLegalDynamicDistribution(NetBufferingAlgorithm* vGA, int partitionCount, HSteinerPoint& source);

  virtual ~VGTreeLegalDynamicDistribution();
};

#endif