#include "VanGinnekenAlgorithm.h"
#include <math.h>
#include "Utils.h"
#include "STA.h"
#include "OptimizationContext.h"

void MoveBinIndexesIntoBorders(AppCtx* context, int& min_col, int& min_row, int& max_col, int& max_row);
void DetermineBordersOfClusterPotential(int& min_col, int& max_col, 
                                        int& min_row, int& max_row,
                                        double x, double y, AppCtx* context);

HVGAlgorithm::HVGAlgorithm(HDesign& hd)
{
  data = new VGAlgorithmData(hd, this);
  isInitialize = false;
}

void HVGAlgorithm::Initialize(bool isAgainInitialize)
{
  if (isInitialize && !isAgainInitialize) 
  {
    return;
  }  

  isInitialize = true;

  data->Initialize();

  if (data->GetTypePartition() == 0)
    data->vGTree = new VGTreeUniformDistribution(this, data->GetPartitionCount());
  if (data->GetTypePartition() == 1)
    data->vGTree = new VGTreeDynamicDistribution(this, data->GetPartitionCount());
  if (data->GetTypePartition() == 2)
  {
    data->vGTree = new VGTreeLegalDynamicDistribution(this, data->GetPartitionCount(), 
      data->GetSizeBuffer());
  }

  if (data->GetTypeBufferingAlgorithm() == 0)
    createVGListAlgorithm = new ClassicCreateVGListAlgorithm(this);
  if (data->GetTypeBufferingAlgorithm() == 1)
    createVGListAlgorithm = new LineBypassAtCreateVGListAlgorithm(this);
  if (data->GetTypeBufferingAlgorithm() == 2)
    createVGListAlgorithm = new AdaptiveBypassAtCreateVGListAlgorithm(this);

  if (data->GetTypeModificationVanGinnekenList() == 0)
    modificationVanGinnekenList = new StandartModificationVanGinnekenList(this);
  if (data->GetTypeModificationVanGinnekenList() == 1)
    modificationVanGinnekenList = new ModificationVanGinnekenListAccountingBorder(this);
  if (data->GetTypeModificationVanGinnekenList() == 2)
    modificationVanGinnekenList = new ExhaustiveSearch(this);

  if (data->GetTypeBuferAddition() == 0)
    additionNewElement = new StandartAdditionNewElement(this);
  if (data->GetTypeBuferAddition() == 1)
    additionNewElement = new LegalAdditionNewElement(this);
}

int HVGAlgorithm::BufferingCriticalPath()
{
  if (!isInitialize)   Initialize();


  std::vector<HCriticalPath> paths(data->design.CriticalPaths.Count());
  int idx = 0;

  for(HCriticalPaths::Enumerator i = data->design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;
  //design.Plotter.PlotSteinerForest(Color_Black);

  std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(data->design));

  int bufferCount = 0;
  ALERT("CriticalPaths count = %d", data->design.CriticalPaths.Count());
  for(int j = 0; j < data->design.CriticalPaths.Count(); j++)
  {
    for (HCriticalPath::PointsEnumeratorW point = (paths[j],data->design).GetEnumeratorW(); point.MoveNext();)
    {
      HNetWrapper net = data->design[((point.TimingPoint(),data->design).Pin(),data->design).OriginalNet()];
      if (net.Kind() == NetKind_Active)// && (net.PinsCount() <= 3 ))//&& (treeRepository.NoVGTree(net)))//
      {

        if (!data->GetNetVisit()[::ToID(net)])
        {
          if ((data->GetCountPinInBufferingNet() == 0) || (
            ((net.PinsCount() <= data->GetCountPinInBufferingNet()) 
            && (!data->GetIsTypeLimitationOnCountPinInBufferingNetEquality()) ) ||
            ((net.PinsCount() == data->GetCountPinInBufferingNet()) 
            && (data->GetIsTypeLimitationOnCountPinInBufferingNetEquality()))))
          {

            if ((net.Name() != data->GetNameBufferingNet()) && (data->GetNameBufferingNet() != ""))
              continue;
            //{
            //string na = net.Name();
            //printf("start name = %s\n", na.c_str());
            bufferCount += BufferingNen(net).GetPositionCount();
            //printf("find name = %s\n", na.c_str());
            //}
          }
        }

      }
    }
    //ALERT("CriticalPaths id = %d", j);
  }
  ALERT("Buffers inserted: %d", bufferCount);

  return bufferCount;
}

VGVariantsListElement HVGAlgorithm::BufferingNen(HNet& net, bool isRealBuffering)
{
  if (!isInitialize)   Initialize();

  if (data->GetPrintNetInfo())
  {
    ALERT("\t%s\t%d\t", data->design.Nets.GetString<HNet::Name>(net).c_str(), 
      data->design.Nets.GetInt<HNet::PinsCount>(net));
  }
  if (data->GetTypeBuferAddition() != 1)
    data->GetNetVisit()[::ToID(net)] = true;

  data->vGTree->UpdateTree(data->design.SteinerPoints[(net, data->design).Source()]);
  //  treeRepository.CreateVGTree(net);
  //printf("TreeSize = %d\n",vGTree->GetTreeSize());
  if ((data->GetPlotSteinerPoint()) || (data->GetPlotNets()))
  {
    data->design.Plotter.ShowNetSteinerTree(net, Color_Black, true, 
      HPlotter::WaitTime(data->GetPlotterWaitTime()));
  }

  if (data->GetPlotVGTree())
  {
    data->design.Plotter.ShowVGTree(net, &data->vGTree->GetSource(), 
      Color_Black, true, HPlotter::WaitTime(data->GetPlotterWaitTime()));
  }

  VGVariantsListElement best = Algorithm(data->vGTree);
  int bufCount = best.GetPositionCount();
  best.SortBufferPosition();
  //ALERT("net: %s\tbufcount = %d",(net, design).Name().c_str(), bufCount );
  if (bufCount > 0)
  {
    TemplateTypes<NewBuffer>::list newBuffer;
    HNet* newNet = new HNet[bufCount + 1];
    if (isRealBuffering)
    {
      additionNewElement->InsertsBuffer(newBuffer, &best);
      newBuffer.sort();
      //ALERT("name = %s", (net, design).Name().c_str());
      additionNewElement->CreateNets(net, newBuffer, newNet, data->vGTree->GetSource().GetLeft());
    }
  }

  if ((data->GetPlotSteinerPoint()) || (data->GetPlotVGTree()) || (data->GetPlotNets()))
  {
    data->design.Plotter.ShowPlacement();
  }

  return best;
}

double CalcBufferArea(AppCtx* context, int colIdx, int rowIdx, BufferPositions& bufferPositions)
{
  double width = context->sprData.binGrid.binWidth;
  double height = context->sprData.binGrid.binHeight;

  double binX = context->sprData.binGrid.bins[colIdx][rowIdx].xCoord - width / 2.0;
  double binY = context->sprData.binGrid.bins[colIdx][rowIdx].yCoord - height / 2.0;

  double binX2 = binX + context->sprData.binGrid.binWidth;
  double binY2 = binY + context->sprData.binGrid.binHeight;

  BinGrid& binGrid = context->sprData.binGrid;

  double dx = bufferPositions.GetPosition()->GetX() - binGrid.bins[rowIdx][colIdx].xCoord;
  double dy = bufferPositions.GetPosition()->GetY() - binGrid.bins[rowIdx][colIdx].yCoord;

  double x = bufferPositions.GetPosition()->GetX();
  double y = bufferPositions.GetPosition()->GetY();
  double bufWidth = bufferPositions.GetPosition()->GetTree()->vGAlgorithm->data->design[bufferPositions.GetBufferInfo()->Type()].SizeX();
  double bufHeight = bufferPositions.GetPosition()->GetTree()->vGAlgorithm->data->design[bufferPositions.GetBufferInfo()->Type()].SizeY();
  double x2 = bufferPositions.GetPosition()->GetX() + bufferPositions.GetPosition()->GetTree()->vGAlgorithm->data->design[bufferPositions.GetBufferInfo()->Type()].SizeX();
  double y2 = bufferPositions.GetPosition()->GetY() + bufferPositions.GetPosition()->GetTree()->vGAlgorithm->data->design[bufferPositions.GetBufferInfo()->Type()].SizeY();
  
  double xSize = 0;
  double ySize = 0;

  if (x > binX) 
  {
    if (x > binX2)
      xSize = 0;
    {
      if (x2 < binX2)
      {
        xSize = bufWidth;
      }
      else
      {
        xSize = binX2 - x;
      }
    }
  }
  else
  {
    if (x2 < binX)
      xSize = 0;
    else
    {
      if (x2 < binX2)
      {
        xSize = x2 - binX;
      }
      else
      {
        xSize = width;
      }
    }
  }

  if (y > binY) 
  {
    if (y > binY2)
      ySize = 0;
    {
      if (y2 < binY2)
      {
        ySize = bufHeight;
      }
      else
      {
        ySize = binY2 - y;
      }
    }
  }
  else
  {
    if (y2 < binY)
      ySize = 0;
    else
    {
      if (y2 < binY2)
      {
        ySize = y2 - binY;
      }
      else
      {
        ySize = height;
      }
    }
  }

  double sizeBufferMultiplier = bufferPositions.GetPosition()->GetTree()->vGAlgorithm->data->GetSizeBufferMultiplier();
  return int (fabs(xSize * ySize * sizeBufferMultiplier));
}

int HVGAlgorithm::UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant)
{

  for (TemplateTypes<BufferPositions>::list::iterator pos = vGVariant.GetBufferPosition()->begin(); 
    pos != vGVariant.GetBufferPosition()->end(); ++pos)
  {
    double currBufferTotalPotential = 0;
    int min_row, min_col, max_row, max_col; // area affected by cluster potential
    DetermineBordersOfClusterPotential(min_col, max_col, min_row, max_row, pos->GetPosition()->GetX(), pos->GetPosition()->GetY(), context);

    for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
    {
      for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
      {
        double bsf = CalcBufferArea(context, colIdx, rowIdx, *pos);
        context->sprData.bufferPotentialOverBins[rowIdx][colIdx] = bsf;
        currBufferTotalPotential = bsf;
      }
    }// loop over affected bins


    for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
    {
      for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
      {
        context->sprData.binGrid.bins[rowIdx][colIdx].sumBufPotential += 
         context->sprData.bufferPotentialOverBins[rowIdx][colIdx];
      }
    }
  }
  return vGVariant.GetPositionCount();
}

int HVGAlgorithm::SetBinTableBuffer(AppCtx* context)
{
  if (!isInitialize)   Initialize();
  STA(data->design);
  ALERT("Buffering type: %d", data->design.cfg.ValueOf("TypePartition", 0));
  ALERT("context->sprData.binGrid.binWidth = %f",context->sprData.binGrid.binWidth);
  ALERT("context->sprData.binGrid.binHeight = %f",context->sprData.binGrid.binHeight);
  if (data->design.cfg.ValueOf("AdaptiveSizeBufferMultiplier", false))
    data->SetSizeBufferMultiplier( min((context->sprData.binGrid.binHeight * context->sprData.binGrid.binWidth / 
      data->GetSizeBuffer()) / 100.0, 1.0));
  ALERT("data->GetSizeBuffer() = %f",data->GetSizeBuffer());
  ALERT("NewSizeBufferMultiplier = %f", data->GetSizeBufferMultiplier());
  std::vector<HCriticalPath> paths(data->design.CriticalPaths.Count());
  int idx = 0;
  for(HCriticalPaths::Enumerator i = data->design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;

  std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(data->design));

  for (int i = 0; i < context->sprData.binGrid.nBinRows; ++i)
  {
    for (int j = 0; j < context->sprData.binGrid.nBinCols; ++j)
    {
      context->sprData.binGrid.bins[i][j].sumBufPotential = 0.0;
      context->sprData.bufferPotentialOverBins[i][j] = 0.0;
    }
  }

  int bufferCount = 0;
  ALERT("CriticalPaths count = %d", data->design.CriticalPaths.Count());
  for(int j = 0; j < data->design.CriticalPaths.Count(); j++)
  {
    for (HCriticalPath::PointsEnumeratorW point = (paths[j],data->design).GetEnumeratorW(); point.MoveNext();)
    {
      HNetWrapper net = data->design[((point.TimingPoint(),data->design).Pin(),data->design).OriginalNet()];
      if (net.Kind() == NetKind_Active) 
      {        
        if (!data->GetNetVisit()[::ToID(net)])
          bufferCount += UpdateBinTable(context, BufferingNen(net, false));
      }
    }
    //ALERT("CriticalPaths id = %d", j);
  }

  ALERT("Buffers inserted: %d", bufferCount);
  STA(data->design);
  data->design.Plotter.ShowFillBinGrid(context);
  return bufferCount;
}

VGVariantsListElement HVGAlgorithm::Algorithm(VanGinnekenTree* vGTree)
{

  double t = 0;
  VGVariantsListElement best;
  double tMax = -INFINITY;

  TemplateTypes<VGVariantsListElement>::list* vGList = createVGListAlgorithm->CreateVGList(vGTree);

  if (data->GetPrintVariantsList())
  { 
    ALERT("Result:");
    Utils::PrintVariants(vGList);
    ALERT("R = %.2f",vGTree->GetR());
  }
  for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
  {
    double a, b, c;
    a = i->GetRAT();
    b = i->GetC();
    c =  vGTree->GetR();
    t = i->GetRAT() - i->GetC() * vGTree->GetR();

    if (t > tMax)
    {
      tMax = t;
      best = (*i);
    }
  }
  if (data->GetPrintNetInfo())
  {
    ALERT("best pos = %d\ttmax = %.20f\n", best.GetPositionCount(), tMax);
  }
  //PrintVariantsNode(&best, 0);
  delete vGList;
  if (data->GetPrintVariantsList())
  {    
    ALERT("best variants:");
    PrintVariantsNode(&best, 0);
  }
  return best;
}
//HVGAlgorithm

//AbstractCreateVGListAlgorithm

AbstractCreateVGListAlgorithm::AbstractCreateVGListAlgorithm(HVGAlgorithm* vGA)
{
  vGAlgorithm = vGA;
}

//CreateVGList

//ClassicCreateVGListAlgorithm

ClassicCreateVGListAlgorithm::ClassicCreateVGListAlgorithm(HVGAlgorithm* vGA): 
AbstractCreateVGListAlgorithm(vGA)
{

}

TemplateTypes<VGVariantsListElement>::list* 
ClassicCreateVGListAlgorithm::CreateVGList(VanGinnekenTree* tree)
{
  return CreateVGList(tree->GetSource().GetLeft());
}

TemplateTypes<VGVariantsListElement>::list* 
ClassicCreateVGListAlgorithm::CreateVGList( VanGinnekenTreeNode* node)
{
  if (node->isSink())
  {    
    return vGAlgorithm->modificationVanGinnekenList->CreateNewVGList(node);
  }
  else
    if (node->isBranchPoint())
    {
      TemplateTypes<VGVariantsListElement>::list* leftVGList = CreateVGList(node->GetLeft());

      TemplateTypes<VGVariantsListElement>::list* RightVGList;
      if (node->HasRight())
      {
        RightVGList = CreateVGList(node->GetRight());        
        if (vGAlgorithm->data->GetPrintVariantsList())
          ALERT("Branch Point id: %d", node->GetIndex());
        return vGAlgorithm->modificationVanGinnekenList->MergeList(leftVGList, RightVGList);
      }
      if (vGAlgorithm->data->GetPrintVariantsList())
      { 
        ALERT("BranchPoint id: %d", node->GetIndex());
        PrintVariants(leftVGList);
      }
      return leftVGList;
    }
    else
    {
      TemplateTypes<VGVariantsListElement>::list* newList = CreateVGList(node->GetLeft());
      vGAlgorithm->modificationVanGinnekenList->UpdateValue(newList, 
        vGAlgorithm->modificationVanGinnekenList->GetLength(node, node->GetLeft()));
      vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(newList);
      if (node->isCandidateAndRealPoint())
      {
        if (vGAlgorithm->data->GetIsInsertInSourceAndSink())
          vGAlgorithm->modificationVanGinnekenList->AddBuffer(newList, node);
      }
      else
        vGAlgorithm->modificationVanGinnekenList->AddBuffer(newList, node);

      if (vGAlgorithm->data->GetPrintVariantsList())
      { 
        ALERT("point id: %d", node->GetIndex());
        ALERT("Length line %d and %d: %.2f",node->GetIndex(), node->GetLeft()->GetIndex(), 
          vGAlgorithm->modificationVanGinnekenList->GetLength(node, node->GetLeft()));
        PrintVariants(newList);
      }
      return newList;
    }

}
//ClassicCreateVGListAlgorithm

//LineBypassAtCreateVGListAlgorithm

LineBypassAtCreateVGListAlgorithm::LineBypassAtCreateVGListAlgorithm(HVGAlgorithm* vGA): 
AbstractCreateVGListAlgorithm(vGA)
{

}

TemplateTypes<VGVariantsListElement>::list* 
LineBypassAtCreateVGListAlgorithm::CreateVGList(VanGinnekenTree* tree)
{
  TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack stackList;
  int lastIndexTree = tree->GetTreeSize() - 1;

  TemplateTypes<VGVariantsListElement>::list* newList = NULL;
  TemplateTypes<VGVariantsListElement>::list* currentList = NULL;
  TemplateTypes<VGVariantsListElement>::list* leftList = NULL;
  TemplateTypes<VGVariantsListElement>::list* rightList = NULL;


  for (int i = lastIndexTree; i > 0; i--)
  {
    if (tree->vGTree[i].isSink())
    {
      newList = vGAlgorithm->modificationVanGinnekenList->CreateNewVGList(&tree->vGTree[i]);
      stackList.push(newList);
      newList = NULL;
    }
    else
      if (tree->vGTree[i].isBranchPoint())
      {
        leftList = stackList.top();
        stackList.pop();

        if (tree->vGTree[i].HasRight())
        {

          rightList = stackList.top();
          stackList.pop();
          if (vGAlgorithm->data->GetPrintVariantsList())
            ALERT("Branch Point id: %d", tree->vGTree[i].GetIndex());          
          currentList = vGAlgorithm->modificationVanGinnekenList->MergeList(leftList, rightList);
          stackList.push(currentList);
          leftList = NULL;
          rightList = NULL;
          currentList = NULL;
        }
        else
        {
          if (vGAlgorithm->data->GetPrintVariantsList())
          { 
            ALERT("BranchPoint id: %d", tree->vGTree[i].GetIndex());
            PrintVariants(leftList);
          }
          stackList.push(leftList);
          leftList = NULL;
        }
      }
      else
      {
        currentList = stackList.top();
        stackList.pop();

        vGAlgorithm->modificationVanGinnekenList->UpdateValue(currentList, 
          vGAlgorithm->modificationVanGinnekenList->GetLength(&(tree->vGTree[i]), 
          (tree->vGTree[i].GetLeft())));
        vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(currentList);
        if (tree->vGTree[i].isCandidateAndRealPoint())
        {
          if (vGAlgorithm->data->GetIsInsertInSourceAndSink())
            vGAlgorithm->modificationVanGinnekenList->AddBuffer(currentList, &tree->vGTree[i]);
        }
        else
          vGAlgorithm->modificationVanGinnekenList->AddBuffer(currentList, &tree->vGTree[i]);

        if (vGAlgorithm->data->GetPrintVariantsList())
        { 
          ALERT("point id: %d", tree->vGTree[i].GetIndex());
          ALERT("Length line %d and %d: %.2f",tree->vGTree[i].GetIndex(), 
            tree->vGTree[i].GetLeft()->GetIndex(), vGAlgorithm->modificationVanGinnekenList->GetLength(&tree->vGTree[i], tree->vGTree[i].GetLeft()));
          PrintVariants(currentList);
        }
        stackList.push(currentList);
        currentList = NULL;
      }
  }
  if (stackList.size() > 1)
    ALERT("Error: length stackList > 1");

  newList = stackList.top();
  stackList.pop();
  return newList;
}

//LineBypassAtCreateVGListAlgorithm

//AdaptiveBypassAtCreateVGListAlgorithm

AdaptiveBypassAtCreateVGListAlgorithm::AdaptiveBypassAtCreateVGListAlgorithm(HVGAlgorithm* vGA): 
AbstractCreateVGListAlgorithm(vGA)
{

}

TemplateTypes<VGVariantsListElement>::list* 
AdaptiveBypassAtCreateVGListAlgorithm::CreateVGList(VanGinnekenTree* tree)
{
  TemplateTypes<TemplateTypes<VGVariantsListElement>::list*>::stack stackList;
  int lastIndexTree = tree->GetTreeSize() - 1;

  TemplateTypes<VGVariantsListElement>::list* newList = NULL;
  TemplateTypes<VGVariantsListElement>::list* currentList = NULL;
  TemplateTypes<VGVariantsListElement>::list* leftList = NULL;
  TemplateTypes<VGVariantsListElement>::list* rightList = NULL;


  for (int i = lastIndexTree; i > 0; i--)
  {
    if (tree->vGTree[i].isSink())
    {
      newList = vGAlgorithm->modificationVanGinnekenList->CreateNewVGList(&tree->vGTree[i]);

      stackList.push(newList);
      newList = NULL;
    }
    else
      if (tree->vGTree[i].isBranchPoint())
      {
        leftList = stackList.top();
        stackList.pop();

        if (tree->vGTree[i].HasRight())
        {

          rightList = stackList.top();
          stackList.pop();
          if (vGAlgorithm->data->GetPrintVariantsList())
            ALERT("Branch Point id: %d", tree->vGTree[i].GetIndex());          
          currentList = vGAlgorithm->modificationVanGinnekenList->MergeList(leftList, rightList);
          stackList.push(currentList);
          leftList = NULL;
          rightList = NULL;
          currentList = NULL;
        }
        else
        {
          if (vGAlgorithm->data->GetPrintVariantsList())
          { 
            ALERT("BranchPoint id: %d", tree->vGTree[i].GetIndex());
            PrintVariants(leftList);
          }
          stackList.push(leftList);
          leftList = NULL;
        }
      }
      else
      {
        currentList = stackList.top();
        stackList.pop();
        if (tree->vGTree[i].isInternal() || tree->vGTree[i].isCandidateAndRealPoint())
        {
          vGAlgorithm->modificationVanGinnekenList->UpdateValue(currentList, 
            vGAlgorithm->modificationVanGinnekenList->GetLength(&(tree->vGTree[i]), 
            (tree->vGTree[i].GetLeft())));
          vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(currentList);
        }
        else
        {
          TemplateTypes<VGVariantsListElement>::list copyList(*currentList);

          int currentMaxIndex = vGAlgorithm->data->GetMaxIndex();

          vGAlgorithm->modificationVanGinnekenList->UpdateValue(&copyList, 
            vGAlgorithm->modificationVanGinnekenList->GetLength(&(tree->vGTree[i]), 
            (tree->vGTree[i].GetLeft())));
          vGAlgorithm->modificationVanGinnekenList->SortVGVariantsListElement(&copyList);

          if (tree->vGTree[i].isCandidateAndRealPoint())
          {
            if (vGAlgorithm->data->GetIsInsertInSourceAndSink())
              vGAlgorithm->modificationVanGinnekenList->AddBuffer(&copyList, &tree->vGTree[i]);
          }
          else
            vGAlgorithm->modificationVanGinnekenList->AddBuffer(&copyList, &tree->vGTree[i]);
          if (currentMaxIndex != vGAlgorithm->data->GetMaxIndex())
          {
            if (copyList.begin()->GetIndex() > currentMaxIndex)
            {
              currentList->push_front(*copyList.begin());
            }
            else
            {

              TemplateTypes<VGVariantsListElement>::list::iterator j = currentList->begin();
              for (TemplateTypes<VGVariantsListElement>::list::iterator k = copyList.begin(); 
                k != copyList.end(); k++)
              {
                if ((*k) == (*j))
                  j++;
                else
                  vGAlgorithm->modificationVanGinnekenList->InsertVGVariantsListElement(currentList, *k);
              }
            }
          }
        }

        if (vGAlgorithm->data->GetPrintVariantsList())
        { 
          ALERT("point id: %d", tree->vGTree[i].GetIndex());
          ALERT("Length line %d and %d: %.2f",tree->vGTree[i].GetIndex(), 
            tree->vGTree[i].GetLeft()->GetIndex(), vGAlgorithm->modificationVanGinnekenList->GetLength(&tree->vGTree[i], tree->vGTree[i].GetLeft()));
          PrintVariants(currentList);
        }
        stackList.push(currentList);
        currentList = NULL;
      }
  }
  if (stackList.size() > 1)
    ALERT("Error: length stackList > 1");

  newList = stackList.top();
  stackList.pop();
  return newList;
}

//AdaptiveBypassAtCreateVGListAlgorithm

//AbstractModificationVanGinnekenList

AbstractModificationVanGinnekenList::AbstractModificationVanGinnekenList(HVGAlgorithm* vGA)
{
  vGAlgorithm = vGA;
}

//AbstractModificationVanGinnekenList

//StandartModificationVanGinnekenList

StandartModificationVanGinnekenList::StandartModificationVanGinnekenList(HVGAlgorithm* vGA): 
AbstractModificationVanGinnekenList(vGA)
{

}

TemplateTypes<VGVariantsListElement>::list* 
StandartModificationVanGinnekenList::MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, 
                                               TemplateTypes<VGVariantsListElement>::list* RightVGList)
{
  TemplateTypes<VGVariantsListElement>::list* result = new TemplateTypes<VGVariantsListElement>::list();

  TemplateTypes<VGVariantsListElement>::list::iterator left = leftVGList->begin(); 
  TemplateTypes<VGVariantsListElement>::list::iterator right = RightVGList->begin();
  {
    while ((left != leftVGList->end()) && (right != RightVGList->end()))
    {
      VGVariantsListElement newElement;

      newElement.SetC(left->GetC() + right->GetC());
      newElement.SetBufferPosition(*left->GetBufferPosition());
      for (TemplateTypes<BufferPositions>::list::iterator pos = right->GetBufferPosition()->begin(); 
        pos != right->GetBufferPosition()->end(); ++pos)
      {
        newElement.SetBufferPosition(*pos);
      }
      if (left->GetRAT() < right->GetRAT())
      {
        newElement.SetRAT(left->GetRAT());
        left++;
      }
      else      
      {
        newElement.SetRAT(right->GetRAT());
        right++;
      }
      result->push_back(newElement);

    }
  }
  if (vGAlgorithm->data->GetPrintVariantsList())
  {
    ALERT("left:\n");
    PrintVariants(leftVGList);
    ALERT("right:\n");
    PrintVariants(RightVGList);
    ALERT("merge:\n");
    PrintVariants(result);
  }

  delete leftVGList;
  delete RightVGList;

  return result;
}

void StandartModificationVanGinnekenList::UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                                      double lengthEdge)
{
  for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
  {    
    i->SetRAT(i->GetRAT() - vGAlgorithm->data->GetWirePhisics().RPerDist * lengthEdge * i->GetC() - 0.5 * vGAlgorithm->data->GetWirePhisics().RPerDist * lengthEdge * lengthEdge * vGAlgorithm->data->GetWirePhisics().LinearC);
    i->SetC(i->GetC() + vGAlgorithm->data->GetWirePhisics().LinearC * lengthEdge);
  }
}

void StandartModificationVanGinnekenList::AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                                    VanGinnekenTreeNode* node)
{  
  for(unsigned int j = 0; j < vGAlgorithm->data->Buffers.size(); j++)
  {
    double tMax = -INFINITY;
    double t;

    TemplateTypes<VGVariantsListElement>::list::iterator iOpt;
    for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
    {
      t = i->GetRAT() - vGAlgorithm->data->Buffers[j].Tb() - vGAlgorithm->data->Buffers[j].Rb() * 
        i->GetC();
      if (t > tMax)
      {
        tMax = t;
        iOpt = i;
      }
    }
    VGVariantsListElement newElement;
    newElement.SetBufferPosition(*iOpt->GetBufferPosition());
    BufferPositions bp(node, &vGAlgorithm->data->Buffers[j], 0);
    newElement.SetBufferPosition(bp);
    newElement.SetC(vGAlgorithm->data->Buffers[j].Cb());
    newElement.SetRAT(tMax);    
    vGAlgorithm->data->SetMaxIndex(vGAlgorithm->data->GetMaxIndex() + 1);
    newElement.SetIndex(vGAlgorithm->data->GetMaxIndex());
    InsertVGVariantsListElement(vGList, newElement);
  }
}

void StandartModificationVanGinnekenList::SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList)
{
  TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin();
  TemplateTypes<VGVariantsListElement>::list::iterator j = i;

  while (i != vGList->end())
  {
    j++;
    if (j == vGList->end())
      break;

    if ((i->GetRAT() >= j->GetRAT()) && (i->GetC() <= j->GetC()))
    {
      //i - ый лучше
      TemplateTypes<VGVariantsListElement>::list::iterator j1 = j;
      j++;
      vGList->erase(j1);
    }
    else
      if ((i->GetRAT() < j->GetRAT()) && (i->GetC() > j->GetC()))
      {
        //j - ый лучше
        TemplateTypes<VGVariantsListElement>::list::iterator i1 = i;
        ++i;
        vGList->erase(i1);
      }
      i = j;   
  }
}

void StandartModificationVanGinnekenList::InsertVGVariantsListElement(
  TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element)
{
  if (vGList->size() == 0)
  {
    vGList->push_front(element);
    return;
  }
  bool isIns = false;
  bool stop = false;
  if ((vGList->front().GetRAT() > element.GetRAT()) && (vGList->front().GetC() > element.GetC()))
  {
    vGList->push_front(element);
  }
  else
  {
    TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->end();
    --i;
    for (; ((!stop) && (!isIns)); )
    {
      if ((i->GetRAT() >= element.GetRAT()) && (i->GetC() <= element.GetC()))
      {
        isIns = true;
      }
      else
        if ((i->GetRAT() < element.GetRAT()) && (i->GetC() > element.GetC()))
        {
          (*i) = element;
          isIns = true;
        }
        else
          if ((i->GetRAT() < element.GetRAT()) && (i->GetC() < element.GetC()))
          {
            i++;
            if (i != vGList->end())
              vGList->insert(i, element);
            else
              vGList->push_back(element);
            isIns = true;
          }
          if (i == vGList->begin())
            stop = true;
          else
            --i;
    }
  }
}

double StandartModificationVanGinnekenList::GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2)
{
  return fabs(node1->GetX() - node2->GetX()) + fabs(node1->GetY() - node2->GetY());
}

TemplateTypes<VGVariantsListElement>::list* 
StandartModificationVanGinnekenList::CreateNewVGList(VanGinnekenTreeNode* node)
{
  TemplateTypes<VGVariantsListElement>::list* newList = new TemplateTypes<VGVariantsListElement>::list();
  VGVariantsListElement element;
  element.SetC(node->GetC());
  element.SetRAT(node->GetRAT());  
  vGAlgorithm->data->SetMaxIndex(vGAlgorithm->data->GetMaxIndex() + 1);
  element.SetIndex(vGAlgorithm->data->GetMaxIndex());
  newList->push_back(element);

  if (vGAlgorithm->data->GetPrintVariantsList())
  { 
    ALERT("sink id: %d", node->GetIndex());
    PrintVariants(newList);
  }
  return newList;
}
//StandartModificationVanGinnekenList

//ModificationVanGinnekenListAccountingBorder

ModificationVanGinnekenListAccountingBorder::ModificationVanGinnekenListAccountingBorder(HVGAlgorithm* vGA): 
StandartModificationVanGinnekenList(vGA)
{

}
TemplateTypes<VGVariantsListElement>::list* 
ModificationVanGinnekenListAccountingBorder::MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, 
                                                       TemplateTypes<VGVariantsListElement>::list* RightVGList)
{
  TemplateTypes<VGVariantsListElement>::list* result = new TemplateTypes<VGVariantsListElement>::list();

  for (TemplateTypes<VGVariantsListElement>::list::iterator left = leftVGList->begin(); left != leftVGList->end(); left++)
    for (TemplateTypes<VGVariantsListElement>::list::iterator right = RightVGList->begin(); right != RightVGList->end(); right++)
    {
      if (((left->GetPositionCount() + right->GetPositionCount()) > vGAlgorithm->data->GetMaxBufferCount()) && (vGAlgorithm->data->GetMaxBufferCount() > 0))
        continue;

      VGVariantsListElement newElement;

      newElement.SetC(left->GetC() + right->GetC());
      newElement.SetBufferPosition(*left->GetBufferPosition());
      for (TemplateTypes<BufferPositions>::list::iterator pos = right->GetBufferPosition()->begin(); 
        pos != right->GetBufferPosition()->end(); ++pos)
      {
        newElement.SetBufferPosition(*pos);
      }
      if (left->GetRAT() < right->GetRAT())
      {
        newElement.SetRAT(left->GetRAT());
      }
      else      
      {
        newElement.SetRAT(right->GetRAT());
      }

      InsertVGVariantsListElement(result, newElement);
    }  

    if (vGAlgorithm->data->GetPrintVariantsList())
    {
      ALERT("left:\n");
      PrintVariants(leftVGList);
      ALERT("right:\n");
      PrintVariants(RightVGList);
      ALERT("merge:\n");
      PrintVariants(result);
    }

    delete leftVGList;
    delete RightVGList;

    return result;
}

void ModificationVanGinnekenListAccountingBorder::AddBuffer(
  TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node)
{  
  for(unsigned int j = 0; j < vGAlgorithm->data->Buffers.size(); j++)
  {
    double tMax = -INFINITY;
    double t;

    TemplateTypes<VGVariantsListElement>::list::iterator iOpt;
    bool findOpt = false;
    for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
    {
      if ((i->GetPositionCount() >= vGAlgorithm->data->GetMaxBufferCount()) && (vGAlgorithm->data->GetMaxBufferCount() > 0))
        continue;

      t = i->GetRAT() - vGAlgorithm->data->Buffers[j].Tb() - vGAlgorithm->data->Buffers[j].Rb() * 
        i->GetC();
      if (t > tMax)
      {
        tMax = t;
        iOpt = i;
        findOpt = true;
      }
    }
    if (findOpt)
    {
      VGVariantsListElement newElement;
      newElement.SetBufferPosition(*iOpt->GetBufferPosition());
      BufferPositions bp(node, &vGAlgorithm->data->Buffers[j], 0);
      newElement.SetBufferPosition(bp);
      newElement.SetC(vGAlgorithm->data->Buffers[j].Cb());
      newElement.SetRAT(tMax);    
      vGAlgorithm->data->SetMaxIndex(vGAlgorithm->data->GetMaxIndex() + 1);
      newElement.SetIndex(vGAlgorithm->data->GetMaxIndex());
      InsertVGVariantsListElement(vGList, newElement);
    }
  }
}

//ModificationVanGinnekenListAccountingBorder

//ExhaustiveSearch
ExhaustiveSearch::ExhaustiveSearch(HVGAlgorithm* vGA):StandartModificationVanGinnekenList(vGA)
{

}
TemplateTypes<VGVariantsListElement>::list* ExhaustiveSearch::MergeList(
  TemplateTypes<VGVariantsListElement>::list* leftVGList, 
  TemplateTypes<VGVariantsListElement>::list* RightVGList)
{
  TemplateTypes<VGVariantsListElement>::list* result = new TemplateTypes<VGVariantsListElement>::list();

  for (TemplateTypes<VGVariantsListElement>::list::iterator left = leftVGList->begin(); left != leftVGList->end(); left++)
    for (TemplateTypes<VGVariantsListElement>::list::iterator right = RightVGList->begin(); right != RightVGList->end(); right++)
    {
      if (( (left->GetPositionCount() + right->GetPositionCount()) > vGAlgorithm->data->GetMaxBufferCount()) && (vGAlgorithm->data->GetMaxBufferCount() > 0))
        continue;

      VGVariantsListElement newElement;

      newElement.SetC(left->GetC() + right->GetC());
      newElement.SetBufferPosition(*left->GetBufferPosition());
      for (TemplateTypes<BufferPositions>::list::iterator pos = right->GetBufferPosition()->begin(); 
        pos != right->GetBufferPosition()->end(); ++pos)
      {
        newElement.SetBufferPosition(*pos);
      }
      if (left->GetRAT() < right->GetRAT())
      {
        newElement.SetRAT(left->GetRAT());
      }
      else      
      {
        newElement.SetRAT(right->GetRAT());
      }
      InsertVGVariantsListElement(result, newElement);
    }

    if (vGAlgorithm->data->GetPrintVariantsList())
    {
      ALERT("left:\n");
      PrintVariants(leftVGList);
      ALERT("right:\n");
      PrintVariants(RightVGList);
      ALERT("merge:\n");
      PrintVariants(result);
    }

    delete leftVGList;
    delete RightVGList;

    return result;
}

void ExhaustiveSearch::AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                 VanGinnekenTreeNode* node)
{  
  for(unsigned int j = 0; j < vGAlgorithm->data->Buffers.size(); j++)
  {
    double tMax;
    for (TemplateTypes<VGVariantsListElement>::list::iterator iOpt = vGList->begin(); iOpt != vGList->end(); ++iOpt)
    {
      if ((iOpt->GetPositionCount() >= vGAlgorithm->data->GetMaxBufferCount()) && (vGAlgorithm->data->GetMaxBufferCount() > 0))
        continue;

      tMax = iOpt->GetRAT() - vGAlgorithm->data->Buffers[j].Tb() - vGAlgorithm->data->Buffers[j].Rb() * 
        iOpt->GetC();

      VGVariantsListElement newElement;
      newElement.SetBufferPosition(*iOpt->GetBufferPosition());
      BufferPositions bp(node, &vGAlgorithm->data->Buffers[j], 0);
      newElement.SetBufferPosition(bp);
      newElement.SetC(vGAlgorithm->data->Buffers[j].Cb());
      newElement.SetRAT(tMax);    
      vGAlgorithm->data->SetMaxIndex(vGAlgorithm->data->GetMaxIndex() + 1);
      newElement.SetIndex(vGAlgorithm->data->GetMaxIndex());
      InsertVGVariantsListElement(vGList, newElement);
    }
  }
}

void ExhaustiveSearch::SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList)
{
  /*TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin();
  TemplateTypes<VGVariantsListElement>::list::iterator j = i;

  while (i != vGList->end())
  {
  j++;
  if (j == vGList->end())
  break;

  if ((i->GetRAT() >= j->GetRAT()) && (i->GetC() <= j->GetC()))
  {
  //i - ый лучше
  TemplateTypes<VGVariantsListElement>::list::iterator j1 = j;
  j++;
  vGList->erase(j1);
  }
  else
  if ((i->GetRAT() < j->GetRAT()) && (i->GetC() > j->GetC()))
  {
  //j - ый лучше
  TemplateTypes<VGVariantsListElement>::list::iterator i1 = i;
  ++i;
  vGList->erase(i1);
  }
  i = j;   
  }*/
  //vGList->sort();

}

void ExhaustiveSearch::InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, 
                                                   VGVariantsListElement& element)
{
  bool isIns = false;
  bool stop = false;
  if (vGList->size() == 0)
  {
    vGList->push_front(element);
    return;
  }

  if ((vGList->front().GetRAT() > element.GetRAT()) && (vGList->front().GetC() > element.GetC()))
  {
    vGList->push_front(element);
  }
  else
  {
    TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->end();
    --i;
    for (; ((!stop) && (!isIns)); )
    {
      if ((i->GetRAT() < element.GetRAT()) && (i->GetC() < element.GetC()))
      {
        i++;
        if (i != vGList->end())
          vGList->insert(i, element);
        else
          vGList->push_back(element);
        isIns = true;
      }
      if (i == vGList->begin())
        stop = true;
      else
        --i;
    }
  }
  if (!isIns)
    vGList->push_front(element);
}

//ExhaustiveSearch

//AbstractAdditionNewElement

AbstractAdditionNewElement::AbstractAdditionNewElement(HVGAlgorithm* vGA)
{
  vGAlgorithm = vGA;
}

//AbstractAdditionNewElement

//StandartAdditionNewElement

StandartAdditionNewElement::StandartAdditionNewElement(HVGAlgorithm* vGA): AbstractAdditionNewElement(vGA)
{

}

void StandartAdditionNewElement::InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, 
                                               VGVariantsListElement* best)
{
  for (TemplateTypes<BufferPositions>::list::iterator pos = best->GetBufferPosition()->begin(); 
    pos != best->GetBufferPosition()->end(); ++pos)
  {
    InsertBuffer(newBuffer, *pos);
  }
}

void  StandartAdditionNewElement::InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer, 
                                               BufferPositions& position)
{
  if (position.GetBufferInfo() == 0) return;

  HCellWrapper buffer = vGAlgorithm->data->design[vGAlgorithm->data->design.Cells.AllocateCell()];
  buffer.SetType(position.GetBufferInfo()->Type());
  buffer.SetPlacementStatus(PlacementStatus_Movable);
  buffer.SetHeight(vGAlgorithm->data->design.GetDouble<HMacroType::SizeY>(position.GetBufferInfo()->Type()));
  buffer.SetWidth(vGAlgorithm->data->design.GetDouble<HMacroType::SizeX>(position.GetBufferInfo()->Type()));

  /*double drift = vGAlgorithm->data->design.Cells.GetDouble<HCell::Height>(
    vGAlgorithm->data->design.Pins.Get<HPin::Cell, HCell>(
    vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(
    position.GetPosition()->GetTree()->GetSource().GetSteinerPoint()))) / 2.0;

  HSteinerPoint sp = position.GetPosition()->GetTree()->GetSource().GetSteinerPoint();
  HPinWrapper pw = vGAlgorithm->data->design[vGAlgorithm->data->design[sp].Pin()];
  //vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(position.GetPosition()->GetTree()->GetSource().GetSteinerPoint())
  HCellWrapper cell = vGAlgorithm->data->design[pw.Cell()];
  //vGAlgorithm->data->design.Pins.Get<HPin::Cell, HCell>(vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(position.GetPosition()->GetTree()->GetSource().GetSteinerPoint()))
  double x = cell.X();
  double y = cell.Y();
  int ojpjv=0;

  //cell.Height()

  //vGAlgorithm->data->design.Cells.GetDouble<HCell::Height>(vGAlgorithm->data->design.Pins.Get<HPin::Cell, HCell>(vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(position.GetPosition()->GetTree()->GetSource().GetSteinerPoint())))

  //(*(VGTreeLegalDynamicDistribution*){*}((*((position).position)).tree)).pGrid;

  //position.GetPosition()->GetTree()->

  //vtbl;*/
//  position.GetPosition()->GetTree()->pGrid.SetCell();

  char bufferName[32];
  sprintf(bufferName, "buf_%d", ::ToID(buffer));//TODO: create unique name
  buffer.SetName(bufferName);
  if ((vGAlgorithm->data->GetTypePartition() == 2) && position.GetPosition()->isCandidate() && 
    !position.GetPosition()->isCandidateAndRealPoint() && !position.GetPosition()->isInternal())
    buffer.SetX(position.GetPosition()->GetX());
  else
    buffer.SetX(position.GetPosition()->GetX() - buffer.Width() * 0.5);

  if ((vGAlgorithm->data->GetTypePartition() == 2) && position.GetPosition()->isCandidate() && 
    !position.GetPosition()->isCandidateAndRealPoint() && !position.GetPosition()->isInternal())
    buffer.SetY(position.GetPosition()->GetY());
  else
    buffer.SetY(position.GetPosition()->GetY() - buffer.Height() * 0.5);

  buffer.SetOrientation(Orientation_N);
  vGAlgorithm->data->design.Pins.AllocatePins(buffer);
  position.GetPosition()->GetTree()->pGrid.SetCell(buffer);
  NewBuffer buf;
  buf.Positions = position;
  buf.cell = buffer;
  buf.sink = Utils::FindCellPinByName(vGAlgorithm->data->design, buffer, (position.GetBufferInfo()->InPin(), 
    vGAlgorithm->data->design).Name());
  buf.source = Utils::FindCellPinByName(vGAlgorithm->data->design, buffer, 
    (position.GetBufferInfo()->OutPin(), vGAlgorithm->data->design).Name());
  newBuffer.push_back(buf);
}

void StandartAdditionNewElement::AddSinks2Net(HNet& subNet, VanGinnekenTreeNode* node, 
                                              HNetWrapper::PinsEnumeratorW& subNetPinEnumW, 
                                              TemplateTypes<NewBuffer>::list& newBuffer)
{
  NewBuffer* bufferNumber = FindBufferNumberByIndex(node, newBuffer);

  //insert sink pin (buffer input) to the subnet
  if (bufferNumber != NULL)
  {

    HPin bufferInput = bufferNumber->sink;

    subNetPinEnumW.MoveNext();

    vGAlgorithm->data->design.Nets.AssignPin(subNet, subNetPinEnumW, bufferInput);

    {//update topological order
      HTimingPoint source = vGAlgorithm->data->design.TimingPoints[
        vGAlgorithm->data->design.Get<HNet::Source, HPin>(subNet)];
        //TODO: fix next line
        HPin bufferOutput = Utils::FindCellPinByName(vGAlgorithm->data->design, bufferNumber->cell, 
          vGAlgorithm->data->design.cfg.ValueOf("Buffering.DefaultBuffer.OutputPin", "Y"));
        Utils::InsertNextPoint(vGAlgorithm->data->design, vGAlgorithm->data->design.TimingPoints[bufferInput], 
          source);
        Utils::InsertNextPoint(vGAlgorithm->data->design, vGAlgorithm->data->design.TimingPoints[bufferOutput], 
          vGAlgorithm->data->design.TimingPoints[bufferInput]);
        //ERROR_ASSERT(Utils::VerifyTimingCalculationOrder(design));
    }
    return;
  }

  //insert pins from left subtree
  if (node->HasLeft())
  {
    AddSinks2Net(subNet, node->GetLeft(), subNetPinEnumW, newBuffer);
  }

  //insert pins from right subtree
  if (node->HasRight())
  {
    AddSinks2Net(subNet, node->GetRight(), subNetPinEnumW, newBuffer);
  }

  //insert sink pin to the final subnets
  if (node->isSink())
  {		
    subNetPinEnumW.MoveNext();
    vGAlgorithm->data->design.Nets.AssignPin(subNet, subNetPinEnumW, 
      vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(node->GetSteinerPoint()));
    return;
  }
}


NewBuffer* StandartAdditionNewElement::FindBufferNumberByIndex(VanGinnekenTreeNode* node, 
                                                               TemplateTypes<NewBuffer>::list& newBuffer)
{
  int i = 0;

  for (TemplateTypes<NewBuffer>::list::iterator pos = newBuffer.begin(); pos != newBuffer.end(); ++pos)
  {
    i++;
    if(pos->Positions.GetPosition() == node)
      return &*pos;
  }
  return NULL;
}

void StandartAdditionNewElement::PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, 
                                                      TemplateTypes<NewBuffer>::list& newBuffer)
{
  if ((FindBufferNumberByIndex(node, newBuffer) != NULL))
  {
    nPins++;
    return;
  }
  if (node->HasLeft())
  {
    PinsCountCalculation(node->GetLeft(), nPins, newBuffer);
  }
  if (node->HasRight())
  {
    PinsCountCalculation(node->GetRight(), nPins, newBuffer);
  }
  if (node->isSink())
  {		
    nPins++;
    return;
  }

  return;
}


void StandartAdditionNewElement::CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, 
                                            HNet* newNet, VanGinnekenTreeNode* node)
{
  int newPinCount = 0;
  int nNewNetPin = 0;
  int nPins = 0;
  char cellIdx[10];

  HNet subNet = vGAlgorithm->data->design.Nets.AllocateNet(false);
  newNet[0] = subNet;
  vGAlgorithm->data->design.Nets.Set<HNet::Kind, NetKind>(subNet, NetKind_Active);
  sprintf(cellIdx, "%d", 0);
  vGAlgorithm->data->design.Nets.Set<HNet::Name>(subNet, vGAlgorithm->data->design[net].Name() + 
    "__BufferedPart_" + string(cellIdx));

  //allocate pins
  PinsCountCalculation(node, nPins, newBuffer);
  nPins++;
  nNewNetPin += nPins;
  vGAlgorithm->data->design.Nets.AllocatePins(subNet, nPins);  
  newPinCount += nPins;
  //init source
  vGAlgorithm->data->design.Nets.Set<HNet::Source, HPin>(subNet, vGAlgorithm->data->design[net].Source());

  //add other pins
  AddSinks2Net(subNet, node, vGAlgorithm->data->design[subNet].GetSinksEnumeratorW(), newBuffer);

  if (vGAlgorithm->data->GetPlotNets())
  {
    vGAlgorithm->data->design.Plotter.ShowNetSteinerTree(subNet, Color_Red, true, 
      HPlotter::WaitTime(vGAlgorithm->data->GetPlotterWaitTime()));
  }

  int ind = 0;
  for (TemplateTypes<NewBuffer>::list::iterator j = newBuffer.begin(); j != newBuffer.end(); ++j, ind++)
  {
    //allocate new net
    subNet = vGAlgorithm->data->design.Nets.AllocateNet(false);
    newNet[ind] = subNet;
    vGAlgorithm->data->design.Nets.Set<HNet::Kind, NetKind>(subNet, NetKind_Active);
    sprintf(cellIdx, "%d", j);
    vGAlgorithm->data->design.Nets.Set<HNet::Name>(subNet, vGAlgorithm->data->design[net].Name() + 
      "__BufferedPart_" + string(cellIdx));

    //allocate pins
    nPins = 0;
    NewBuffer& nodeStart = *j;
    PinsCountCalculation(nodeStart.Positions.GetPosition()->GetLeft(), nPins, newBuffer);
    nPins++;
    nNewNetPin += nPins;
    newPinCount += nPins;
    vGAlgorithm->data->design.Nets.AllocatePins(subNet, nPins);
    vGAlgorithm->data->design.Nets.Set<HNet::Source, HPin>(subNet, j->source);

    //add other pins
    NewBuffer& nodeStart2 = *j;
    AddSinks2Net(subNet, nodeStart2.Positions.GetPosition()->GetLeft(), 
      vGAlgorithm->data->design[subNet].GetSinksEnumeratorW(), newBuffer);

    if (vGAlgorithm->data->GetPlotNets())
    {
      vGAlgorithm->data->design.Plotter.ShowNetSteinerTree(subNet, Color_Red, false, 
        HPlotter::WaitTime(vGAlgorithm->data->GetPlotterWaitTime()));
    }
  }
  HNetWrapper netw = vGAlgorithm->data->design[net];
  int ttt = (netw.PinsCount() + int(newBuffer.size()) * 2);
  if ((netw.PinsCount() + newBuffer.size() * 2) != newPinCount)
    ALERT("ERRORR pin count");
  vGAlgorithm->data->design.Nets.Set<HNet::Kind, NetKind>(net, NetKind_Buffered);
}

//StandartAdditionNewElement

//LegalAdditionNewElement

LegalAdditionNewElement::LegalAdditionNewElement(HVGAlgorithm* vGA): StandartAdditionNewElement(vGA)
{

}
void LegalAdditionNewElement::InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer, 
                                           BufferPositions& position)
{
  if (position.GetBufferInfo() == 0) return;

  HCellWrapper buffer = vGAlgorithm->data->design[vGAlgorithm->data->design.Cells.AllocateCell()];
  buffer.SetType(position.GetBufferInfo()->Type());
  buffer.SetPlacementStatus(PlacementStatus_Movable);
  buffer.SetHeight(vGAlgorithm->data->design.GetDouble<HMacroType::SizeY>(position.GetBufferInfo()->Type()));
  buffer.SetWidth(vGAlgorithm->data->design.GetDouble<HMacroType::SizeX>(position.GetBufferInfo()->Type()));

  /*double drift = vGAlgorithm->data->design.Cells.GetDouble<HCell::Height>(
  vGAlgorithm->data->design.Pins.Get<HPin::Cell, HCell>(
  vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(
  position.GetPosition()->GetTree()->GetSource().GetSteinerPoint()))) / 2.0;

  HSteinerPoint sp = position.GetPosition()->GetTree()->GetSource().GetSteinerPoint();
  HPinWrapper pw = vGAlgorithm->data->design[vGAlgorithm->data->design[sp].Pin()];
  //vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(position.GetPosition()->GetTree()->GetSource().GetSteinerPoint())
  HCellWrapper cell = vGAlgorithm->data->design[pw.Cell()];
  //vGAlgorithm->data->design.Pins.Get<HPin::Cell, HCell>(vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(position.GetPosition()->GetTree()->GetSource().GetSteinerPoint()))
  double x = cell.X();
  double y = cell.Y();
  int ojpjv=0;

  //cell.Height()

  //vGAlgorithm->data->design.Cells.GetDouble<HCell::Height>(vGAlgorithm->data->design.Pins.Get<HPin::Cell, HCell>(vGAlgorithm->data->design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(position.GetPosition()->GetTree()->GetSource().GetSteinerPoint())))

  //(*(VGTreeLegalDynamicDistribution*){*}((*((position).position)).tree)).pGrid;

  //position.GetPosition()->GetTree()->

  //vtbl;*/
//  position.GetPosition()->GetTree()->pGrid.SetCell();

  char bufferName[32];
  sprintf(bufferName, "buf_%d", ::ToID(buffer));//TODO: create unique name
  buffer.SetName(bufferName);

  double x = 0;
  if ((vGAlgorithm->data->GetTypePartition() == 2) && position.GetPosition()->isCandidate() && 
    !position.GetPosition()->isCandidateAndRealPoint() && !position.GetPosition()->isInternal())
    x = position.GetPosition()->GetX();
  else
    x = position.GetPosition()->GetX() - buffer.Width() * 0.5;
  int column = position.GetPosition()->GetTree()->pGrid.GetColumn(x);
  
  double y = 0;
  if ((vGAlgorithm->data->GetTypePartition() == 2) && position.GetPosition()->isCandidate() && 
    !position.GetPosition()->isCandidateAndRealPoint() && !position.GetPosition()->isInternal())
    y = position.GetPosition()->GetY();
  else
    y = position.GetPosition()->GetY() - buffer.Height() * 0.5;
  int row = position.GetPosition()->GetTree()->pGrid.GetRow(y);

  double newX = position.GetPosition()->GetTree()->pGrid.GetNode(row, column)->GetX();
  double newY = position.GetPosition()->GetTree()->pGrid.GetNode(row, column)->GetY();
  buffer.SetX(newX);
  buffer.SetY(newY);

  buffer.SetOrientation(Orientation_N);
  vGAlgorithm->data->design.Pins.AllocatePins(buffer);

  int interseptCount = 0;
  double x1 = newX, y1 = newY, 
    x2 = newX + buffer.Width(), y2 =  newY,
    x3 = newX, y3 = newY + buffer.Height(),
    x4 = newX + buffer.Width(), y4 = newY + buffer.Height();
    double  bufferWidth =  buffer.Width();

  int rowBegin = position.GetPosition()->GetTree()->pGrid.GetRow(y1);
  int rowEnd = position.GetPosition()->GetTree()->pGrid.GetRow(y3);
  int columnBegin = position.GetPosition()->GetTree()->pGrid.GetColumn(x1);
  int columnEnd = position.GetPosition()->GetTree()->pGrid.GetColumn(x4);
  HCell* intersept = new HCell [abs(rowEnd - rowBegin) * abs(columnEnd - columnBegin)];
  position.GetPosition()->GetTree()->pGrid.CellInZone(newX, newY, buffer.Width(), buffer.Height(), interseptCount, intersept);
  double leftShift = 0;
  double leftRight = 0;
  for (int i = 0; i < interseptCount; i++)
  {
    HCellWrapper cw = vGAlgorithm->data->design[intersept[i]];
    position.GetPosition()->GetTree()->pGrid.ExtractCell(cw);
    double cwx = cw.X(), cwy = cw.Y();
    double cwWidth = cw.Width();
    if (cw.X() < (newX + buffer.Width() / 2.0))
    {
      if (cw.X() > newX) 
      cw.SetX(newX - cw.Width());
    }
    else
    {
      cw.SetX(newX + buffer.Width());
    }
    position.GetPosition()->GetTree()->pGrid.SetCell(cw);
  }
  if (interseptCount > 0)
    delete [] intersept;

  position.GetPosition()->GetTree()->pGrid.SetCell(buffer);
  NewBuffer buf;
  buf.Positions = position;
  buf.cell = buffer;
  buf.sink = Utils::FindCellPinByName(vGAlgorithm->data->design, buffer, (position.GetBufferInfo()->InPin(), 
    vGAlgorithm->data->design).Name());
  buf.source = Utils::FindCellPinByName(vGAlgorithm->data->design, buffer, 
    (position.GetBufferInfo()->OutPin(), vGAlgorithm->data->design).Name());
  newBuffer.push_back(buf);
}

//LegalAdditionNewElement