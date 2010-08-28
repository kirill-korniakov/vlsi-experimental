#include "NetBufferingAlgorithm.h"
#include "Utils.h"
#include "Timing.h"
#include "STA.h"
#include "VanGinnekenTree.h"
#include <math.h>

NetBufferingAlgorithm::NetBufferingAlgorithm(HDesign& hd)
{
  data = new VGAlgorithmData(hd, this);
  isInitialize = false;
}

void NetBufferingAlgorithm::Initialize(bool isAgainInitialize)
{
  if (isInitialize && !isAgainInitialize)
    return;  

  isInitialize = true;
  data->Initialize();

  if (data->typePartition == 0)
    data->vGTree = new VGTreeUniformDistribution(this, data->partitionCount);
  if (data->typePartition == 1)
    data->vGTree = new VGTreeDynamicDistribution(this, data->partitionCount);
  if (data->typePartition == 2)    
    data->vGTree = new VGTreeLegalDynamicDistribution(this, data->partitionCount, 
    data->sizeBuffer);    

  if (data->typeBufferingAlgorithm == 0)
    createVGListAlgorithm = new ClassicCreateVGListAlgorithm(this);
  if (data->typeBufferingAlgorithm == 1)
    createVGListAlgorithm = new LineBypassAtCreateVGListAlgorithm(this);
  if (data->typeBufferingAlgorithm == 2)
    createVGListAlgorithm = new AdaptiveBypassAtCreateVGListAlgorithm(this);

  if (data->typeModificationVanGinnekenList == 0)
    modificationVanGinnekenList = new StandartModificationVanGinnekenList(this);
  if (data->typeModificationVanGinnekenList == 1)
    modificationVanGinnekenList = new ModificationVanGinnekenListAccountingBorder(this);
  if (data->typeModificationVanGinnekenList == 2)
    modificationVanGinnekenList = new ExhaustiveSearch(this);

  if (data->typeBuferAddition == 0)
    additionNewElement = new StandartAdditionNewElement(this);
  if (data->typeBuferAddition == 1)
    additionNewElement = new LegalAdditionNewElement(this);
}

VGVariantsListElement NetBufferingAlgorithm::BufferingNen(HNet& net, bool isRealBuffering, AppCtx* context)
{
  if (!isInitialize)   Initialize();

  if (data->printNetInfo)
  {
    ALERT("\t%s\t%d\t", data->design.Nets.GetString<HNet::Name>(net).c_str(), 
      data->design.Nets.GetInt<HNet::PinsCount>(net));
  }
  if (data->typeBuferAddition != 1)
    data->netVisit[::ToID(net)] = true;

  data->vGTree->UpdateTree(data->design.SteinerPoints[(net, data->design).Source()]);

  if ((data->plotSteinerPoint) || (data->plotNets))
  {
    data->design.Plotter.ShowNetSteinerTree(net, Color_Black, true, 
      HPlotter::WaitTime(data->plotterWaitTime));
  }

  if (data->plotVGTree)
  {
    data->design.Plotter.ShowVGTree(net, &data->vGTree->GetSource(), 
      Color_Black, true, HPlotter::WaitTime(data->plotterWaitTime));
  }

  VGVariantsListElement best = Algorithm(data->vGTree);
  int bufCount = best.GetPositionCount();
  best.SortBufferPosition();
  if (bufCount > 0)
  {
    TemplateTypes<NewBuffer>::list newBuffer;
    for (TemplateTypes<BufferPositions>::list::iterator pos = best.GetBufferPosition()->begin(); 
      pos != best.GetBufferPosition()->end(); ++pos)
    {
      data->AddAreaBuffer(data->design[pos->GetBufferInfo()->Type()].SizeX() * data->design[pos->GetBufferInfo()->Type()].SizeY());
      if (data->plotBuffer)
      {
        data->design.Plotter.DrawFilledRectangle2(pos->GetPosition()->GetX(), pos->GetPosition()->GetY(), data->design[pos->GetBufferInfo()->Type()].SizeX(), data->design[pos->GetBufferInfo()->Type()].SizeY(), Color_Red);
      }
    }
    if (isRealBuffering)
    {
      HNet* newNet = new HNet[bufCount + 1];
      additionNewElement->InsertsBuffer(newBuffer, &best);
      newBuffer.sort();
      additionNewElement->CreateNets(net, newBuffer, newNet, data->vGTree->GetSource().GetLeft());
      delete [] newNet;
    }
  }

  if ((data->plotSteinerPoint) || (data->plotVGTree) || (data->plotNets))
  {
    data->design.Plotter.ShowPlacement();
  }

  return best;
}


VGVariantsListElement NetBufferingAlgorithm::Algorithm(VanGinnekenTree* vGTree)
{
  double t = 0;
  VGVariantsListElement best;
  double tMax = -INFINITY;

  TemplateTypes<VGVariantsListElement>::list* vGList = createVGListAlgorithm->CreateVGList(vGTree);

  if (data->printVariantsList)
  { 
    ALERT("Result:");
    Utils::PrintVariants(vGList);
    ALERT("R = %.2f",vGTree->GetR());
  }
  for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
  {
    t = i->GetRAT() - i->GetC() * vGTree->GetR();

    if (t > tMax)
    {
      tMax = t;
      best = (*i);
    }
  }
  if (data->printNetInfo)
  {
    ALERT("best pos = %d\ttmax = %.20f\n", best.GetPositionCount(), tMax);
  }
  delete vGList;
  if (data->printVariantsList)
  {    
    ALERT("best variants:");
    PrintVariantsNode(&best, 0);
  }
  return best;
}