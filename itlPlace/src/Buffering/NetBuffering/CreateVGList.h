#ifndef __CreateVGList_H__
#define __CreateVGList_H__

#include "Utils.h"

class NetBufferingAlgorithm;
class VanGinnekenTree;
using namespace Utils;

class AbstractCreateVGListAlgorithm
{
public:
  AbstractCreateVGListAlgorithm(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree) = 0;

protected:
  NetBufferingAlgorithm* vGAlgorithm;
};

class ClassicCreateVGListAlgorithm: public AbstractCreateVGListAlgorithm
{
public:
  ClassicCreateVGListAlgorithm(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTreeNode* node);
};

class LineBypassAtCreateVGListAlgorithm: public AbstractCreateVGListAlgorithm
{
public:
  LineBypassAtCreateVGListAlgorithm(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);  
};

class CalculateVGCandidatePoint
{
public:
  CalculateVGCandidatePoint(NetBufferingAlgorithm* vGA);
  virtual void CalculateCandidatePoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
  TemplateTypes<VGVariantsListElement>::list* currentList, int i);

protected:
  NetBufferingAlgorithm* vGAlgorithm;
};

class AdaptiveBypassAtCalculateVGCandidatePoint: public CalculateVGCandidatePoint
{
public:
  AdaptiveBypassAtCalculateVGCandidatePoint(NetBufferingAlgorithm* vGA);

	virtual void CalculateCandidatePoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
  TemplateTypes<VGVariantsListElement>::list* currentList, int i);
};

class CalculateVGBranchPoint
{
public:
CalculateVGBranchPoint(NetBufferingAlgorithm* vGA);
  virtual void CalculateBranchPoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
    TemplateTypes<VGVariantsListElement>::list* currentList,
    TemplateTypes<VGVariantsListElement>::list* leftList,
    TemplateTypes<VGVariantsListElement>::list* rightList, int i);
  virtual void UpdateInBranchPoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
    TemplateTypes<VGVariantsListElement>::list* currentList,
    TemplateTypes<VGVariantsListElement>::list* leftList, int i);
protected:
  NetBufferingAlgorithm* vGAlgorithm;
};

class PathBasedCalculateVGBranchPoint: public CalculateVGBranchPoint
{
public:
  PathBasedCalculateVGBranchPoint(NetBufferingAlgorithm* vGA): CalculateVGBranchPoint(vGA)
  {
  };
  virtual void UpdateInBranchPoint(VanGinnekenTree* tree, TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack& stackList,
    TemplateTypes<VGVariantsListElement>::list* currentList,
    TemplateTypes<VGVariantsListElement>::list* leftList, int i);
};

#endif