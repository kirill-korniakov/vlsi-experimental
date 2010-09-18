#include "VanGinnekenTreeNode.h"
#include "HBaseDesignItem.h"
#include "VanGinnekenTree.h"
#include "NetBufferingAlgorithm.h"

VanGinnekenTreeNode::VanGinnekenTreeNode()
{
  x = 0;
  y = 0;
  type = CANDIDATE;
  index = 0;
  tree = NULL;
  left = NULL;
  right = NULL;
}

VanGinnekenTreeNode::~VanGinnekenTreeNode()
{
  left = NULL;
  right = NULL;
  tree = NULL;
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

HSteinerPoint VanGinnekenTreeNode::GetSteinerPoint(bool first)
{
  return sPoint;
}

HNet VanGinnekenTreeNode::GetNet(bool first)
{
  return net;
}

VanGinnekenTreeNode* VanGinnekenTreeNode::GetLeft()
{
  return left;
}

VanGinnekenTreeNode* VanGinnekenTreeNode::GetRight()
{
  return right;
}

VanGinnekenTree* VanGinnekenTreeNode::GetTree()
{
  return tree;
}

double VanGinnekenTreeNode::GetRAT()	
{		
  if (sPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
  {
    return (tree->vGAlgorithmData->design.TimingPoints[(sPoint,tree->vGAlgorithmData->design).Pin()],tree->vGAlgorithmData->design).RequiredTime();
  }
  else
    return 0;
}

double VanGinnekenTreeNode::GetC()
{
  if (sPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
    return (((sPoint,tree->vGAlgorithmData->design).Pin(),tree->vGAlgorithmData->design).Type(),tree->vGAlgorithmData->design).Capacitance();
  else
    return 0;
}

double VanGinnekenTreeNode::GetR()
{
  if (sPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
    return Utils::GetDriverTimingPhisics(tree->vGAlgorithmData->design, (sPoint,tree->vGAlgorithmData->design).Pin(), SignalDirection_None).R;
  else
    return 0;
}

double VanGinnekenTreeNode::GetT()
{
  return 0;
}

void VanGinnekenTreeNode::SetSteinerPoint(HSteinerPoint sp, bool first)  
{
  sPoint = sp;
}

void VanGinnekenTreeNode::SetNet(HNet n, bool first)
{
  net = n;
}

void VanGinnekenTreeNode::SetLeft(VanGinnekenTreeNode* node)
{
  left = node;
}
void VanGinnekenTreeNode::SetRight(VanGinnekenTreeNode* node)
{
  right = node;
}

void VanGinnekenTreeNode::SetTree(VanGinnekenTree* t)
{
  tree = t;
}

bool VanGinnekenTreeNode::isSource()
{
  return (type == SOURCE) ? true : false;
}

bool VanGinnekenTreeNode::isSink()
{
  return (type == SINK) ? true : false;
}

bool VanGinnekenTreeNode::isBranchPoint()
{
  return ((type == BRANCH) || (type == SOURCE_AND_SINK)) ? true : false;
}

bool VanGinnekenTreeNode::isCandidate()
{
  return ((type == CANDIDATE) || (type == CANDIDATE_ON_PIN)|| (type == CANDIDATE_INTERNAL)) ? true : false;
}

bool VanGinnekenTreeNode::isCandidateAndRealPoint()
{
  return (type == CANDIDATE_ON_PIN) ? true : false;
}

bool VanGinnekenTreeNode::isInternal()
{
  return (type == CANDIDATE_INTERNAL) ? true : false;  
}

bool VanGinnekenTreeNode::isSteinerTreeLeaf()
{
  return ((type == SOURCE_AND_SINK) || (type == SINK)) ? true : false;  
}

 bool VanGinnekenTreeNode::isSteinerTreeSource()
 {
return ((type == SOURCE_AND_SINK) || (type == SOURCE)) ? true : false; 
 }

VanGinnekenTreeNodePathBased::VanGinnekenTreeNodePathBased():VanGinnekenTreeNode()
{
}

double VanGinnekenTreeNodePathBased::GetR()
{
  if (secondSPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
    return Utils::GetDriverTimingPhisics(tree->vGAlgorithmData->design, (secondSPoint,tree->vGAlgorithmData->design).Pin(), SignalDirection_None).R;
  else
    return 0;
}

double VanGinnekenTreeNodePathBased::GetT()
{
  Utils::DriverPhisics dph = Utils::GetDriverAveragePhisics(tree->vGAlgorithmData->design, tree->vGAlgorithmData->design[secondSPoint].Pin(), signalDirection);
  return dph.T;
}

HSteinerPoint VanGinnekenTreeNodePathBased::GetSteinerPoint(bool first)
{
  if (first)
    return sPoint;
  else
    return secondSPoint;

}

HNet VanGinnekenTreeNodePathBased::GetNet(bool first)
{
  if (first)
    return net;
  else
    return secondNet;
}

void VanGinnekenTreeNodePathBased::SetSteinerPoint(HSteinerPoint sp, bool first)
{
  if (first)
    sPoint = sp;
  else
    secondSPoint = sp;
}

void VanGinnekenTreeNodePathBased::SetNet(HNet n, bool first)
{
  if (first)
    net = n;
  else
    secondNet = n;
}