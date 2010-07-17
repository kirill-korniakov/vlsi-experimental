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

class HVGAlgorithm
{
protected:
  HDesign& design;
  HWirePhysicalParams& WirePhisics;
  TemplateTypes<BufferInfo>::vector Buffers;

  int partitionCount;
  bool plotVGTree;
  bool plotNets;
  bool printNetInfo;
  bool plotSteinerPoint;
  bool printVariantsList;
  int plotterWaitTime;
  bool isInitialize;
  bool isInsertInSourceAndSink;
  int typeBufferingAlgorithm;
  double sizeBuffer;
  int maxIndex;

  VanGinnekenTree* vGTree;

  bool* netVisit;

  void LoadBuffers();

  virtual VGVariantsListElement Algorithm(VanGinnekenTree* vGTree);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTreeNode* node);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);
  virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList2(VanGinnekenTree* tree);
  virtual TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
  virtual void UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge);
  virtual void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node);
  virtual void SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList);
  virtual void InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element);
  virtual double GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2);
  virtual void InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best);
  virtual void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position);

  virtual void PrintVariantsNode(VGVariantsListElement* vGE, int i);
  virtual void PrintVariants(TemplateTypes<VGVariantsListElement>::list* vGList);

  virtual void AddSinks2Net(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnumW, TemplateTypes<NewBuffer>::list& newBuffer);

  virtual NewBuffer* FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer);
  virtual void PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer);
  virtual void CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node);

  virtual int UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant);

public:
  HVGAlgorithm(HDesign& hd);
  virtual void Initialize(bool isAgainInitialize = false);

  virtual int BufferingCriticalPath();
  virtual VGVariantsListElement BufferingNen(HNet& net, bool isRealBuffering = true);
  virtual int SetBinTableBuffer(AppCtx* context);

};

class HVGA2: public HVGAlgorithm
{
public:
    HVGA2(HDesign& hd);
    virtual TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTree* tree);

};

#endif //__VanGinnekenAlgorithm_H__