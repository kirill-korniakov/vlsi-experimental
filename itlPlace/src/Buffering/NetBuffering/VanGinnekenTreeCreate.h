#ifndef __VanGinnekenTreeCreate_H__
#define __VanGinnekenTreeCreate_H__
#include "VanGinnekenTreeNode.h"
#include "Utils.h"

class VanGinnekenTree;

class VanGinnekenTreeNodeCreate
{
public:
    VanGinnekenTree* vGTree;

    VanGinnekenTreeNodeCreate(VanGinnekenTree* tree);

    virtual VanGinnekenTreeNode* CreateSource(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, VanGinnekenTree* tree = NULL);
    virtual VanGinnekenTreeNode* CreatePotentialBufferPoint(HNet net, NodeType type, int& nodeIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, VanGinnekenTree* tree = NULL);
    virtual void CreateFirstPointInEdge(HNet net, int& nodeIndex, int& rootIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void CreatePotentialBufferPointInEdge(HNet net, HSteinerPoint node, int& nodeIndex, int& rootIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void CreateSinkOrBranchPoint(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, int& rootIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void CreateLastPointInEdge(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, int& rootIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void CreateNewPoint(HNet net, HSteinerPoint node, NodeType type, int& nodeIndex, int rootIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, bool isRight = false, VanGinnekenTree* tree = NULL);
    virtual void RefreshSoursAndSinkPoint(HNet net, HSteinerPoint node, NodeType type, int& rootIndex);
};

class VanGinnekenTreeDynamicDistributionNodeCreate: public VanGinnekenTreeNodeCreate
{
public:
    VanGinnekenTreeDynamicDistributionNodeCreate(VanGinnekenTree* tree):VanGinnekenTreeNodeCreate(tree)
    {
    };
    virtual void CreatePotentialBufferPointInEdge(HNet net, HSteinerPoint node, int& nodeIndex, int& rootIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, bool isRight = false, VanGinnekenTree* tree = NULL);
};

class VanGinnekenTreeLegalDynamicDistributionNodeCreate: public VanGinnekenTreeNodeCreate
{
public:
    VanGinnekenTreeLegalDynamicDistributionNodeCreate(VanGinnekenTree* tree):VanGinnekenTreeNodeCreate(tree)
    {
    };
    virtual void CreatePotentialBufferPointInEdge(HNet net, HSteinerPoint node, int& nodeIndex, int& rootIndex, TemplateTypes<VanGinnekenTreeNode*>::stack& vanGinnekenTreeNodeRoot, bool isRight = false, VanGinnekenTree* tree = NULL);
};

class VanGinnekenTreeCreate
{
public:
    VanGinnekenTree* vGTree;
    VanGinnekenTreeCreate(VanGinnekenTree* tree);

    virtual void CreateTree();
    template< class EnumeratorW >
    void CalculateNodeInEnumerator(EnumeratorW enumerator, int& maxTree, int& treeSizeTemp);
    virtual HSteinerPoint GetSourceEnumerator(HNets::ActiveNetsEnumeratorW enumerator);
    virtual HSteinerPoint GetSourceEnumerator(HCriticalPaths::EnumeratorW enumerator);
};

class VanGinnekenTreeCreatePathBased: public VanGinnekenTreeCreate
{
public:
    VanGinnekenTreeCreatePathBased(VanGinnekenTree* tree):VanGinnekenTreeCreate(tree)
    {
    };
    virtual void CreateTree();
};

class CalculateVanGinnekenSubtree
{
public:
    VanGinnekenTree* vGTree;
    CalculateVanGinnekenSubtree(VanGinnekenTree* tree);

    void CalculateLeftSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree);
    void CalculateRightSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree);
    void CalculatePinSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree, HNets::ActiveNetsEnumeratorW enumerator);
    void CalculatePinSubtree(TemplateTypes<HSteinerPoint>::stack& points, HSteinerPointWrapper& srcPoint, HSteinerPointWrapper& nextPoint, int& pointCount, int& maxTree, HCriticalPaths::EnumeratorW enumerator);

};

class FindMaxPointInEdgeVanGinnekenTree
{
public:
    VanGinnekenTree* vGTree;

    FindMaxPointInEdgeVanGinnekenTree(VanGinnekenTree* tree);
    virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos) {};
};

class FindMaxPointInEdgeVGTreeDynamicDistribution: public FindMaxPointInEdgeVanGinnekenTree
{
public:
    FindMaxPointInEdgeVGTreeDynamicDistribution(VanGinnekenTree* tree):FindMaxPointInEdgeVanGinnekenTree(tree)
    {
    };
    virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
};

class FindMaxPointInEdgeVanGinnekenTreeLegalDynamicDistribution: public FindMaxPointInEdgeVanGinnekenTree
{
public:
    FindMaxPointInEdgeVanGinnekenTreeLegalDynamicDistribution(VanGinnekenTree* tree):FindMaxPointInEdgeVanGinnekenTree(tree)
    {
    };
    virtual void FindNewMaxPointInEdge(HSteinerPoint srcPoint, HSteinerPoint nextPoint, int& maxPos);
};

#endif