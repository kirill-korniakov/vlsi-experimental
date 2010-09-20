#include "NetBufferingAlgorithm.h"
#include "Utils.h"
#include "Timing.h"
#include "STA.h"
#include "VanGinnekenTree.h"

#include <math.h>

NetBufferingAlgorithm::NetBufferingAlgorithm()
{
  isInitialize = false;
  data = NULL;
}

NetBufferingAlgorithm::NetBufferingAlgorithm(HDesign& hd)
{
  data = new VGAlgorithmData(hd, this);
  isInitialize = false;
}
NetBufferingAlgorithm::~NetBufferingAlgorithm()
{
  delete createVGListAlgorithm;
  delete modificationVanGinnekenList;
  delete additionNewElement;
  delete data;
}

void NetBufferingAlgorithm::Initialize(bool isAgainInitialize)
{
  if (isInitialize && !isAgainInitialize)
    return;  

  if (data == NULL)
      ALERT("ERROR Buffering not initialize!!!");

  isInitialize = true;
  data->Initialize();

  data->vGTree = new VanGinnekenTree(data);

/*  if (data->typePartition == LINEAR)
    data->vGTree = new VGTreeUniformDistribution(data);
  if (data->typePartition == DYNAMIC)
    data->vGTree = new VGTreeDynamicDistribution(data);
  if (data->typePartition == LEGAL_POSITIONS_ONLY)    
    data->vGTree = new VGTreeLegalDynamicDistribution(data, data->sizeBuffer);    
*/
  if (data->typeBufferingAlgorithm == CLASSIC_CREATE_VG_LIST)
    createVGListAlgorithm = new ClassicCreateVGListAlgorithm(this);
  if (data->typeBufferingAlgorithm == LINE_BYPASS_CREATE_VG_LIST)
    createVGListAlgorithm = new LineBypassAtCreateVGListAlgorithm(this);
  if (data->typeBufferingAlgorithm == ADAPTIVE_BYPASS_CREATE_VG_LIST)
    createVGListAlgorithm = new AdaptiveBypassAtCreateVGListAlgorithm(this);

  if (data->typeModificationVanGinnekenList == STANDART_MODIFICATION_VG_LIST)
    modificationVanGinnekenList = new StandartModificationVanGinnekenList(this);
  if (data->typeModificationVanGinnekenList == ACCOUNTING_BORDER_MODIFICATION_VG_LIST)
    modificationVanGinnekenList = new ModificationVanGinnekenListAccountingBorder(this);
  if (data->typeModificationVanGinnekenList == EXHAUSTIVE_SEARCH)
    modificationVanGinnekenList = new ExhaustiveSearch(this);

  if (data->typeBufferAddition == STANDART_ADDITION)
    additionNewElement = new StandartAdditionNewElement(this);
  if (data->typeBufferAddition == LEGAL_ADDITION)
    additionNewElement = new LegalAdditionNewElement(this);
  
  if (data->typeNetListBuffering == BUFFERING_ALL_CRITICAL_PATH)
    branchPoint = new CalculateVGBranchPoint(this);
  if (data->typeNetListBuffering == PATH_BASED)
    branchPoint = new PathBasedCalculateVGBranchPoint(this);
}

VGVariantsListElement NetBufferingAlgorithm::BufferingNet(HNet& net, bool isRealBuffering)
{
  if (!isInitialize)   Initialize();

  if (data->printNetInfo)
  {
    ALERT("\t%s\t%d\t", data->design.Nets.GetString<HNet::Name>(net).c_str(), 
      data->design.Nets.GetInt<HNet::PinsCount>(net));
  }
  if (data->typeBufferAddition != LEGAL_ADDITION)
    data->netVisit[::ToID(net)] = true;

  data->vGTree->updateVanGinnekenTree->UpdateTree<HSteinerPoint>(data->design.SteinerPoints[(net, data->design).Source()]);

  if ((data->plotSteinerPoint) || (data->plotNets))
  {
    data->design.Plotter.ShowNetSteinerTree(net, Color_Black, true, 
      HPlotter::WaitTime(data->plotterWaitTime));
  }

  if (data->plotVGTree)
  {
    data->design.Plotter.ShowVGTree(net, data->vGTree->GetSource(), 
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
        data->design.Plotter.DrawFilledRectangle2(pos->GetPosition()->x, pos->GetPosition()->y, data->design[pos->GetBufferInfo()->Type()].SizeX(), data->design[pos->GetBufferInfo()->Type()].SizeY(), Color_Red);
      }
    }
    if (isRealBuffering)
    {
      HNet* newNet = new HNet[bufCount + 1];
      additionNewElement->InsertsBuffer(newBuffer, &best);
      newBuffer.sort();
      additionNewElement->CreateNets(net, newBuffer, newNet, data->vGTree->GetSource()->GetLeft(), bufCount);
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