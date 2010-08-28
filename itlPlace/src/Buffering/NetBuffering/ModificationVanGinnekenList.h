#ifndef __ModificationVanGinnekenList_H__
#define __ModificationVanGinnekenList_H__

#include "Utils.h"

class NetBufferingAlgorithm;
using namespace Utils;

class AbstractModificationVanGinnekenList
{
protected:
  NetBufferingAlgorithm* vGAlgorithm;
public:
  AbstractModificationVanGinnekenList(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList) = 0;
  virtual void UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge) = 0;
  virtual void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node) = 0;
  virtual void SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList) = 0;
  virtual void InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element) = 0;
  virtual double GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2) = 0;
  virtual TemplateTypes<VGVariantsListElement>::list* CreateNewVGList(VanGinnekenTreeNode* node) = 0;
};

class StandartModificationVanGinnekenList: public AbstractModificationVanGinnekenList
{
public:
  StandartModificationVanGinnekenList(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
  virtual void UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge);
  virtual void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node);
  virtual void SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList);
  virtual void InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element);
  virtual double GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateNewVGList(VanGinnekenTreeNode* node);
};

class ModificationVanGinnekenListAccountingBorder: public StandartModificationVanGinnekenList
{
public:
  ModificationVanGinnekenListAccountingBorder(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
};

class ExhaustiveSearch: public StandartModificationVanGinnekenList
{
public:
  ExhaustiveSearch(NetBufferingAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
  virtual void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node);
  virtual void SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList);
  virtual void InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element);
};

#endif