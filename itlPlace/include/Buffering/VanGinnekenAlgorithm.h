#ifndef __VanGinnekenAlgorithm_H__
#define __VanGinnekenAlgorithm_H__

#include "VanGinnekenData.h"
//#include "OptimizationContext.h"
#include "HCell.h"
#include "HPin.h"
//#include "HWirePhysicalParams.h"
#include "HPinType.h"
#include "HNet.h"

class AppCtx;
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

  VanGinnekenTree* vGTree;

  bool* netVisit;

  void LoadBuffers();

  VGVariantsListElement Algorithm(VanGinnekenTree* vGTree);
  TemplateTypes<VGVariantsListElement>::list* CreateVGList(VanGinnekenTreeNode* node);
  TemplateTypes<VGVariantsListElement>::list* MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList);
  void UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge);
  void AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node);
  void SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList);
  void InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element);
  double GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2);
  void InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best);
  void InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer,BufferPositions& position);

  void PrintVariantsNode(VGVariantsListElement* vGE, int i);
  void PrintVariants(TemplateTypes<VGVariantsListElement>::list* vGList);

  void AddSinks2Net(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnumW, TemplateTypes<NewBuffer>::list& newBuffer);

  NewBuffer* FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer);
  void PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer);
  void CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node);

  int UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant);

public:
  HVGAlgorithm(HDesign& hd);
  void Initialize(bool isAgainInitialize = false);

  int BufferingCriticalPath();
  VGVariantsListElement BufferingNen(HNet& net, bool isRealBuffering = true);
  int SetBinTableBuffer(AppCtx* context);

};

#endif //__VanGinnekenAlgorithm_H__