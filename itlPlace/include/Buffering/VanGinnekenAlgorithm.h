#ifndef __VanGinnekenAlgorithm_H__
#define __VanGinnekenAlgorithm_H__

#include "VanGinnekenData.h"
#include "HCell.h"
#include "HPin.h"
#include "HPinType.h"
#include "HNet.h"

struct AppCtx;
class HWirePhysicalParams;

#define	INFINITY	20000000000.0
#define MAXBUFFERTYPE 64

using namespace Utils;

class HVGAlgorithm;

class NewBuffer
{
public:
  HCell cell;
  HPin source;
  HPin sink;
  BufferPositions Positions;
  bool operator > (NewBuffer& element)
  {
    if(Positions > element.Positions)
      return true;
    return false;
  }

  bool operator < (NewBuffer& element)
  {
        if(Positions < element.Positions)
      return true;
    return false;
  }
};

class AbstractCreateVGListAlgorithm
{
protected:
HVGAlgorithm* vGAlgorithm;
public:
  AbstractCreateVGListAlgorithm(HVGAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree) = 0;
};

class ClassicCreateVGListAlgorithm: public AbstractCreateVGListAlgorithm
{
public:
  ClassicCreateVGListAlgorithm(HVGAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTreeNode* node);
};

class LineBypassAtCreateVGListAlgorithm: public AbstractCreateVGListAlgorithm
{
public:
  LineBypassAtCreateVGListAlgorithm(HVGAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);
};

class AdaptiveBypassAtCreateVGListAlgorithm: public AbstractCreateVGListAlgorithm
{
public:
  AdaptiveBypassAtCreateVGListAlgorithm(HVGAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);
};

class AbstractModificationVanGinnekenList
{
protected:
  HVGAlgorithm* vGAlgorithm;
public:
  AbstractModificationVanGinnekenList(HVGAlgorithm* vGA);
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
  StandartModificationVanGinnekenList(HVGAlgorithm* vGA);
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
  ModificationVanGinnekenListAccountingBorder(HVGAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
  virtual void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node);
};

class ExhaustiveSearch: public StandartModificationVanGinnekenList
{
public:
  ExhaustiveSearch(HVGAlgorithm* vGA);
  virtual TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
  virtual void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node);
  virtual void SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList);
  virtual void InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element);
};

class AbstractAdditionNewElement
{
protected:
  HVGAlgorithm* vGAlgorithm;
public:
  AbstractAdditionNewElement(HVGAlgorithm* vGA);
  virtual void InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best) = 0;
  virtual void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position) = 0;
  virtual void AddSinks2Net(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnumW, TemplateTypes<NewBuffer>::list& newBuffer) = 0;
  virtual NewBuffer* FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer) = 0;
  virtual void PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer) = 0;
  virtual void CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node) = 0;
};

class StandartAdditionNewElement: public AbstractAdditionNewElement
{
public:
  StandartAdditionNewElement(HVGAlgorithm* vGA);
  virtual void InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best);
  virtual void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position);
  virtual void AddSinks2Net(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnumW, TemplateTypes<NewBuffer>::list& newBuffer);
  virtual NewBuffer* FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer);
  virtual void PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer);
  virtual void CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node);
};

class LegalAdditionNewElement: public StandartAdditionNewElement
{
public:
  LegalAdditionNewElement(HVGAlgorithm* vGA);
  virtual void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position);
};

class HVGAlgorithm
{
protected:
  bool isInitialize;
  virtual VGVariantsListElement Algorithm(VanGinnekenTree* vGTree);
  virtual int UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant);

public:


  VGAlgorithmData* data;
  AbstractCreateVGListAlgorithm* createVGListAlgorithm;
  AbstractModificationVanGinnekenList* modificationVanGinnekenList;
  AbstractAdditionNewElement* additionNewElement;

  HVGAlgorithm(HDesign& hd);
  virtual void Initialize(bool isAgainInitialize = false);

  virtual int BufferingCriticalPath();
  virtual VGVariantsListElement BufferingNen(HNet& net, bool isRealBuffering = true, AppCtx* context = NULL);
  virtual int SetBinTableBuffer(AppCtx* context);
};

#endif //__VanGinnekenAlgorithm_H__