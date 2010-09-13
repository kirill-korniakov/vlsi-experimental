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

class AdaptiveBypassAtCreateVGListAlgorithm: public AbstractCreateVGListAlgorithm
{
public:
  AdaptiveBypassAtCreateVGListAlgorithm(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);
};

#endif