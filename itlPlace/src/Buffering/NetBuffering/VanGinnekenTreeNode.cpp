#include "VanGinnekenTreeNode.h"
#include "HBaseDesignItem.h"
#include "VanGinnekenTree.h"
#include "NetBufferingAlgorithm.h"

VanGinnekenTreeNode::VanGinnekenTreeNode()
{
  x = 0;
  y = 0;
  type = 3;
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
