#include "VanGinnekenTreeNode.h"
#include "HBaseDesignItem.h"
#include "VanGinnekenTree.h"
#include "NetBufferingAlgorithm.h"
#include "Timing.h"

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

VanGinnekenTreeNode* VanGinnekenTreeNode::GetRoot()
{
  return root;
}

double VanGinnekenTreeNode::GetSinkRAT()	
{		
  if (sPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
  {
    return (tree->vGAlgorithmData->design.TimingPoints[(sPoint,tree->vGAlgorithmData->design).Pin()],tree->vGAlgorithmData->design).RequiredTime();
  }
  else
    return 0;
}

double VanGinnekenTreeNode::GetSourceRAT()
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

double VanGinnekenTreeNode::GetR(bool isRequiredArc)
{
  if (sPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
  {
    
    if (isRequiredArc)
    {
      HPin driver = tree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(sPoint);
      bool inverted;
      double arcTime;
      HTimingArcType arc = FindRequiredArc(tree->vGAlgorithmData->design, tree->vGAlgorithmData->design.TimingPoints[driver], SignalDirection_Rise, arcTime, inverted);
      double r = tree->vGAlgorithmData->design.GetDouble<HTimingArcType::ResistanceRise>(arc);
      return r;
    }
    else
      return Utils::GetDriverTimingPhisics(tree->vGAlgorithmData->design, (sPoint,tree->vGAlgorithmData->design).Pin(), SignalDirection_None).R;

  }
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

void VanGinnekenTreeNode::SetRoot(VanGinnekenTreeNode* node)
{
 root = node;
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

double VanGinnekenTreeNodePathBased::GetSourceRAT()
{
  if (secondSPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
  {
    return (tree->vGAlgorithmData->design.TimingPoints[(secondSPoint,tree->vGAlgorithmData->design).Pin()],tree->vGAlgorithmData->design).RequiredTime();
  }
  else
    if (sPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
      return (tree->vGAlgorithmData->design.TimingPoints[(sPoint,tree->vGAlgorithmData->design).Pin()],tree->vGAlgorithmData->design).RequiredTime();
    return 0;
}

double VanGinnekenTreeNodePathBased::GetR(bool isRequiredArc)
{
  if (secondSPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
  {
    if (isRequiredArc)
    {
      HPin driver = tree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(secondSPoint);
      bool inverted;
      double arcTime;
      HTimingArcType arc = FindRequiredArc(tree->vGAlgorithmData->design, tree->vGAlgorithmData->design.TimingPoints[driver], SignalDirection_Rise, arcTime, inverted);
      return tree->vGAlgorithmData->design.GetDouble<HTimingArcType::ResistanceRise>(arc);
    }
    else
      return Utils::GetDriverTimingPhisics(tree->vGAlgorithmData->design, (secondSPoint,tree->vGAlgorithmData->design).Pin(), SignalDirection_None).R;
  } 
  else
    if (sPoint != tree->vGAlgorithmData->design.SteinerPoints.Null())
    {
      if (isRequiredArc)
      {
        HPin driver = tree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(sPoint);
        bool inverted;
        double arcTime;
        HTimingArcType arc = FindRequiredArc(tree->vGAlgorithmData->design, tree->vGAlgorithmData->design.TimingPoints[driver], SignalDirection_Rise, arcTime, inverted);
        return tree->vGAlgorithmData->design.GetDouble<HTimingArcType::ResistanceRise>(arc);
      }
      else
        return Utils::GetDriverTimingPhisics(tree->vGAlgorithmData->design, (sPoint,tree->vGAlgorithmData->design).Pin(), SignalDirection_None).R;

    }
    else
      ALERT("ERROR in GetR");
  return 0;
}

double VanGinnekenTreeNodePathBased::GetT()
{
  HPin driver = tree->vGAlgorithmData->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(sPoint);
  bool inverted;
  double arcTime;
  HTimingArcType arc = FindRequiredArc(tree->vGAlgorithmData->design, tree->vGAlgorithmData->design.TimingPoints[driver], SignalDirection_Rise, arcTime, inverted);
  return tree->vGAlgorithmData->design.GetDouble<HTimingArcType::TIntrinsicRise>(arc);
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