#include "VanGinnekenAlgorithm.h"
#include <math.h>
#include "Utils.h"
#include "STA.h"
#include "OptimizationContext.h"

void MoveBinIndexesIntoBorders(AppCtx* context, int& min_col, int& min_row, int& max_col, int& max_row);

void HVGAlgorithm::LoadBuffers()
{
  if (design.cfg.ValueOf("UseOnlyDefaultBuffer", false))
  {
    BufferInfo buf = BufferInfo::Create(design);    
    Buffers.push_back(buf);
    ALERT("Buffer type: %s\t input pin: %s\t output pin: %s", (design, buf.Type()).Name().c_str(),
      (design, buf.InPin()).Name().c_str(), (design, buf.OutPin()).Name().c_str());
  }
  else
  {
    string sBufferList = design.cfg.ValueOf("BufferList", "");
    unsigned int n = design.cfg.ValueOf("BufferListLength", 0);
    string* bufferList = NULL;
    if (n > 0)
    {
      bufferList = new string [n];     
      for (unsigned int i = 0, j = 0, t = 0; (i < sBufferList.length()) && (j < n); i++, t++)
      {
        if(sBufferList[i] != ',')
          bufferList[j].push_back(sBufferList[i]);
        else
        {
          t = -1;
          //bufferList[j].push_back(0);
          j++;
        }
      }
      //bufferList[n - 1].push_back(0);
    }

    Buffers = BufferInfo::CreateVector(design, bufferList);
    for (unsigned int i = 0; i < Buffers.size(); i++)
      ALERT("Buffer type: %s\t input pin: %s\t output pin: %s", (design, Buffers[i].Type()).Name().c_str(),
      (design, Buffers[i].InPin()).Name().c_str(), (design, Buffers[i].OutPin()).Name().c_str());
  }
}

HVGAlgorithm::HVGAlgorithm(HDesign& hd): design(hd), WirePhisics(hd.RoutingLayers.Physics)
{
  isInitialize = false;
}

void HVGAlgorithm::Initialize(bool isAgainInitialize)
{
  if (isInitialize && !isAgainInitialize) 
  {
    return;
  }
  isInitialize = true;
  LoadBuffers();
  plotSteinerPoint = design.cfg.ValueOf("PlotSteinerPoint", false);
  plotVGTree = design.cfg.ValueOf("PlotVGTree", false);
  plotNets = design.cfg.ValueOf("PlotNets", false);
  printNetInfo = design.cfg.ValueOf("PrintNetInfo", false);
  printVariantsList = design.cfg.ValueOf("PrintVGVariantsList", false);
  partitionCount = design.cfg.ValueOf("Interval", 1);
  plotterWaitTime = design.cfg.ValueOf("PlotterWaitTime", 1);
  isInsertInSourceAndSink = design.cfg.ValueOf("IsInsertInSourceAndSink", true);
  int TypePartition = design.cfg.ValueOf("TypePartition", 0);

  netVisit = new bool [design.Nets.Count() * 2];

  for (int i = 0; i < design.Nets.Count() * 2; i++)
  {
    netVisit[i] = false;
  }


  if (TypePartition == 0)
    vGTree = new VGTreeUniformDistribution(design, partitionCount);
  if (TypePartition == 1)
    vGTree = new VGTreeDynamicDistribution(design, partitionCount);
  if (TypePartition == 2)
  {

    vGTree = new VGTreeLegalDynamicDistribution(design, partitionCount);
  }

}

int HVGAlgorithm::BufferingCriticalPath()
{
  if (!isInitialize)   Initialize();
  ALERT("Buffering type: %d", design.cfg.ValueOf("TypePartition", 0));

  std::vector<HCriticalPath> paths(design.CriticalPaths.Count());
  int idx = 0;

  for(HCriticalPaths::Enumerator i = design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;
  //design.Plotter.PlotSteinerForest(Color_Black);

  std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(design));

  int bufferCount = 0;
  ALERT("CriticalPaths count = %d", design.CriticalPaths.Count());
  for(int j = 0; j < design.CriticalPaths.Count(); j++)
  {
    for (HCriticalPath::PointsEnumeratorW point = (paths[j],design).GetEnumeratorW(); point.MoveNext(); )
    {
      HNetWrapper net = design[((point.TimingPoint(),design).Pin(),design).OriginalNet()];
      if (net.Kind() == NetKind_Active) //&& (net.PinsCount() == 2 ))//&& (treeRepository.NoVGTree(net)))//
      {


        if (!netVisit[::ToID(net)])
        {
          //if ((net.Kind() == NetKind_Active) && (net.Name() == "busak_n"))  
          //{
          //string na = net.Name();
          //printf("start name = %s\n", na.c_str());
          bufferCount += BufferingNen(net).GetPositionCount();
          //printf("find name = %s\n", na.c_str());
          //}
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
  //VanGinnekenTree* vGTree = //(design, partitionCount, design.SteinerPoints[(net, design).Source()]);

  netVisit[::ToID(net)] = true;

  vGTree->UpdateTree(design.SteinerPoints[(net, design).Source()]);
  //  treeRepository.CreateVGTree(net);
  if (printNetInfo)
  {
    ALERT("\t%s\t%d\t", design.Nets.GetString<HNet::Name>(net).c_str(), design.Nets.GetInt<HNet::PinsCount>(net));
  }
  if ((plotSteinerPoint) || (plotNets))
  {
    design.Plotter.ShowNetSteinerTree(net, Color_Black, true, HPlotter::WaitTime(plotterWaitTime));
  }

  if (plotVGTree)
  {
    design.Plotter.ShowVGTree(net, &vGTree->GetSource(), Color_Black, true, HPlotter::WaitTime(plotterWaitTime));
  }

  VGVariantsListElement best = Algorithm(vGTree);
  int bufCount = best.GetPositionCount();
  //ALERT("net: %s\tbufcount = %d",(net, design).Name().c_str(), bufCount );
  if (bufCount > 0)
  {
    TemplateTypes<NewBuffer>::list newBuffer;
    HNet* newNet = new HNet[bufCount + 1];
    if (isRealBuffering)
    {
      InsertsBuffer(newBuffer, &best);
      newBuffer.sort();
      //ALERT("name = %s", (net, design).Name().c_str());
      CreateNets(net, newBuffer, newNet, vGTree->GetSource().GetLeft());
    }
  }

  if ((plotSteinerPoint) || (plotVGTree) || (plotNets))
  {
    design.Plotter.ShowPlacement();
  }

  return best;
}

void DetermineBordersOfBufferPotential(int& min_col, int& max_col, 
                                       int& min_row, int& max_row,
                                       BufferPositions& bufferPositions, AppCtx* context)
{
  BinGrid& binGrid = context->spreadingData.binGrid;

  min_col = (Aux::cool_dtoi((bufferPositions.GetPosition()->GetX() - context->hd->Circuit.PlacementMinX() - 
    context->spreadingData.potentialRadiusX) / binGrid.binWidth));
  max_col = (Aux::cool_dtoi((bufferPositions.GetPosition()->GetX() - context->hd->Circuit.PlacementMinX() + 
    context->spreadingData.potentialRadiusX) / binGrid.binWidth));
  min_row = (Aux::cool_dtoi((bufferPositions.GetPosition()->GetY() - context->hd->Circuit.PlacementMinY() - 
    context->spreadingData.potentialRadiusY) / binGrid.binHeight));    
  max_row = (Aux::cool_dtoi((bufferPositions.GetPosition()->GetY() - context->hd->Circuit.PlacementMinY() + 
    context->spreadingData.potentialRadiusY) / binGrid.binHeight));

  MoveBinIndexesIntoBorders(context, min_col, min_row, max_col, max_row);
}

double CalcBellShapedFunction(AppCtx* context, int colIdx, int rowIdx, BufferPositions& bufferPositions)
{
  double potentialRadiusX = context->spreadingData.potentialRadiusX;
  double potentialRadiusY = context->spreadingData.potentialRadiusY;
  BinGrid& binGrid = context->spreadingData.binGrid;
  double invPSX = context->spreadingData.invPSX;
  double invPSY = context->spreadingData.invPSY;

  double potX = 0;
  double potY = 0;

  double dx = bufferPositions.GetPosition()->GetX() - binGrid.bins[rowIdx][colIdx].xCoord;
  double dy = bufferPositions.GetPosition()->GetY() - binGrid.bins[rowIdx][colIdx].yCoord;

  double fabsdx = fabs(dx);
  double fabsdy = fabs(dy);

  if (fabsdx > potentialRadiusX || fabsdy > potentialRadiusY)
  {//bin out of potential
    return 0;
  }

  //calculate solution-potential
  if (fabsdx <= potentialRadiusX/2)
  {
    potX = 1 - 2 * dx * dx * invPSX;
  }
  else
  {
    potX = 2 * (fabsdx - potentialRadiusX) * (fabsdx - potentialRadiusX) * invPSX;
  }

  //calculate y-potential
  if (fabsdy <= potentialRadiusY/2)
  {
    potY = 1 - 2 * dy * dy * invPSY;
  }
  else
  {
    potY = 2 * (fabsdy - potentialRadiusY) * (fabsdy - potentialRadiusY) * invPSY;
  }

  return potX * potY;
}

int HVGAlgorithm::UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant)
{
  for (TemplateTypes<BufferPositions>::list::iterator pos = vGVariant.GetBufferPosition()->begin(); pos != vGVariant.GetBufferPosition()->end(); ++pos)
  {
    int min_row, min_col, max_row, max_col; // area affected by cluster potential
    DetermineBordersOfBufferPotential(min_col, max_col, min_row, max_row, *pos, context);

    for (int rowIdx = min_row; rowIdx <= max_row; ++rowIdx)
    {
      for (int colIdx = min_col; colIdx <= max_col; ++colIdx)
      {
        double bsf = CalcBellShapedFunction(context, colIdx, rowIdx, *pos);

        context->spreadingData.binGrid.bins[rowIdx][colIdx].sumBufPotential = bsf;
      }
    }// loop over affected bins
  }
  return vGVariant.GetPositionCount();
}

int HVGAlgorithm::SetBinTableBuffer(AppCtx* context)
{
  if (!isInitialize)   Initialize();
  STA(design);
  ALERT("Buffering type: %d", design.cfg.ValueOf("TypePartition", 0));
  std::vector<HCriticalPath> paths(design.CriticalPaths.Count());
  int idx = 0;
  for(HCriticalPaths::Enumerator i = design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;

  std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(design));

  for (int i = 0; i < context->spreadingData.binGrid.nBinRows; ++i)
  {
    for (int j = 0; j < context->spreadingData.binGrid.nBinCols; ++j)
    {
      context->spreadingData.binGrid.bins[i][j].sumBufPotential = 0.0;
    }
  }

  int bufferCount = 0;
  ALERT("CriticalPaths count = %d", design.CriticalPaths.Count());
  for(int j = 0; j < design.CriticalPaths.Count(); j++)
  {
    for (HCriticalPath::PointsEnumeratorW point = (paths[j],design).GetEnumeratorW(); point.MoveNext(); )
    {
      HNetWrapper net = design[((point.TimingPoint(),design).Pin(),design).OriginalNet()];
      if (net.Kind() == NetKind_Active) 
      {        
        bufferCount += UpdateBinTable(context, BufferingNen(net, false));
      }
    }
    //ALERT("CriticalPaths id = %d", j);
  }
  ALERT("Buffers inserted: %d", bufferCount);
  STA(design);
  return bufferCount;
}

VGVariantsListElement HVGAlgorithm::Algorithm(VanGinnekenTree* vGTree)
{

  double t = 0;
  VGVariantsListElement best;
  double tMax = -INFINITY;

  TemplateTypes<VGVariantsListElement>::list* vGList = CreateVGList(vGTree->GetSource().GetLeft());
  if (printVariantsList)
  { 
    ALERT("Result:");
    PrintVariants(vGList);
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
  if (printNetInfo)
  {
    ALERT("best pos = %d\ttmax = %.2f\n", best.GetPositionCount(), tMax);
  }
  delete vGList;
  if (printVariantsList)
  {    
    ALERT("best variants:");
    PrintVariantsNode(&best, 0);
  }
  return best;
}

TemplateTypes<VGVariantsListElement>::list* HVGAlgorithm::CreateVGList( VanGinnekenTreeNode* node)
{
  if (node->isSink())
  {
    TemplateTypes<VGVariantsListElement>::list* newList = new TemplateTypes<VGVariantsListElement>::list();
    VGVariantsListElement element;
    element.SetC(node->GetC());
    element.SetRAT(node->GetRAT());
    newList->push_back(element);
    if (printVariantsList)
    { 
      ALERT("sink id: %d", node->GetIndex());
      PrintVariants(newList);
    }
    return newList;
  }
  else
    if (node->isBranchPoint())
    {
      TemplateTypes<VGVariantsListElement>::list* leftVGList = CreateVGList(node->GetLeft());

      TemplateTypes<VGVariantsListElement>::list* RightVGList;
      if (node->HasRight())
      {
        RightVGList = CreateVGList(node->GetRight());
        if (printVariantsList)
          ALERT("Branch Point id: %d", node->GetIndex());

        return MergeList(leftVGList, RightVGList);
      }
      if (printVariantsList)
      { 
        ALERT("BranchPoint id: %d", node->GetIndex());
        PrintVariants(leftVGList);
      }
      return leftVGList;
    }
    else
    {
      TemplateTypes<VGVariantsListElement>::list* newList = CreateVGList(node->GetLeft());
      UpdateValue(newList, GetLength(node, node->GetLeft()));
      SortVGVariantsListElement(newList);
      if (node->isSink() || node->isSource())
      {
        if (isInsertInSourceAndSink)
          AddBuffer(newList, node);
      }
      else
        AddBuffer(newList, node);

      if (printVariantsList)
      { 
        ALERT("point id: %d", node->GetIndex());
        ALERT("Length line %d and %d: %.2f",node->GetIndex(), node->GetLeft()->GetIndex(), GetLength(node, node->GetLeft()));
        PrintVariants(newList);
      }
      return newList;
    }

}

TemplateTypes<VGVariantsListElement>::list* HVGAlgorithm::MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList)
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
      for (TemplateTypes<BufferPositions>::list::iterator pos = right->GetBufferPosition()->begin(); pos != right->GetBufferPosition()->end(); ++pos)
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
  delete leftVGList;
  delete RightVGList;
  if (printVariantsList)
    PrintVariants(result);
  return result;
}

void HVGAlgorithm::UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge)
{
  for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
  {
    i->SetRAT(i->GetRAT() - WirePhisics.RPerDist * lengthEdge * i->GetC() - 0.5 * WirePhisics.RPerDist * lengthEdge * lengthEdge * WirePhisics.LinearC);
    i->SetC(i->GetC() + WirePhisics.LinearC * lengthEdge);
  }
}

void HVGAlgorithm::AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node)
{
  for(unsigned int j = 0; j < Buffers.size(); j++)
  {
    double tMax = -INFINITY;
    double t;

    TemplateTypes<VGVariantsListElement>::list::iterator iOpt;
    for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
    {
      t = i->GetRAT() - Buffers[j].Tb() - Buffers[j].Rb() * i->GetC();
      if (t > tMax)
      {
        tMax = t;
        iOpt = i;
      }
    }
    VGVariantsListElement newElement;
    newElement.SetBufferPosition(*iOpt->GetBufferPosition());
    BufferPositions bp(node, &Buffers[j], 0);
    newElement.SetBufferPosition(bp);
    newElement.SetC(Buffers[j].Cb());
    newElement.SetRAT(tMax);

    InsertVGVariantsListElement(vGList, newElement);
  }
}

void HVGAlgorithm::SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList)
{

  TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin();
  TemplateTypes<VGVariantsListElement>::list::iterator j = i;
  //j++;

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


void HVGAlgorithm::InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element)
{
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

double HVGAlgorithm::GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2)
{

  return fabs(node1->GetX() - node2->GetX()) + fabs(node1->GetY() - node2->GetY());
}

void HVGAlgorithm::InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best)
{

  for (TemplateTypes<BufferPositions>::list::iterator pos = best->GetBufferPosition()->begin(); pos != best->GetBufferPosition()->end(); ++pos)
  {

    InsertBuffer(newBuffer, *pos);

  }


}

void  HVGAlgorithm::InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer, BufferPositions& position)
{
  if (position.GetBufferInfo() == 0) return;

  HCellWrapper buffer = design[design.Cells.AllocateCell()];
  buffer.SetType(position.GetBufferInfo()->Type());
  buffer.SetPlacementStatus(PlacementStatus_Movable);
  buffer.SetHeight(design.GetDouble<HMacroType::SizeY>(position.GetBufferInfo()->Type()));
  buffer.SetWidth(design.GetDouble<HMacroType::SizeX>(position.GetBufferInfo()->Type()));

  char bufferName[32];
  sprintf(bufferName, "buf_%d", ::ToID(buffer));//TODO: create unique name
  buffer.SetName(bufferName);
  buffer.SetX(position.GetPosition()->GetX() - buffer.Width() * 0.5);
  buffer.SetY(position.GetPosition()->GetY() - buffer.Height() * 0.5);
  buffer.SetOrientation(Orientation_N);
  design.Pins.AllocatePins(buffer);
  NewBuffer buf;
  buf.Positions = position;
  buf.cell = buffer;
  buf.sink = Utils::FindCellPinByName(design, buffer, (position.GetBufferInfo()->InPin(),design).Name());
  buf.source = Utils::FindCellPinByName(design, buffer, (position.GetBufferInfo()->OutPin(),design).Name());
  newBuffer.push_back(buf);


}

void HVGAlgorithm::PrintVariantsNode(VGVariantsListElement* vGE, int i)
{
  char* sBuf = new char [256]; 
  string s;
  s = "(";
  sprintf(sBuf, "%.2f", vGE->GetRAT());
  s += string(sBuf);
  s += ",\t";
  sprintf(sBuf, "%.2f", vGE->GetC());
  s += string(sBuf);
  s += ",\t{";
  for (TemplateTypes<BufferPositions>::list::iterator pos = vGE->GetBufferPosition()->begin(); pos != vGE->GetBufferPosition()->end(); ++pos)
  {
    sprintf(sBuf, "%d", pos->GetPosition()->GetIndex());
    s += string(sBuf);
    s += ",\t";
  }
  s += "})";
  ALERT("variant: %d\t\t%s",i, s.c_str());
  delete [] sBuf;
}

void HVGAlgorithm::PrintVariants(TemplateTypes<VGVariantsListElement>::list* vGList)
{
  int ind = 0;
  for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
  {
    PrintVariantsNode(&(*i), ind);
    ind++;
  } 
}

void HVGAlgorithm::AddSinks2Net(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnumW, TemplateTypes<NewBuffer>::list& newBuffer)
{

  //for (HNetWrapper::PinsEnumeratorW subNetPinEnumW = design[subNet].GetSinksEnumeratorW(); subNetPinEnumW.MoveNext();)
  //  if (::ToID(subNetPinEnumW) == ::ToID(subNetPinEnum))
  //    break;

  NewBuffer* bufferNumber = FindBufferNumberByIndex(node, newBuffer);

  //insert sink pin (buffer input) to the subnet
  if (bufferNumber != NULL)
  {

    HPin bufferInput = bufferNumber->sink;

    subNetPinEnumW.MoveNext();

    design.Nets.AssignPin(subNet, subNetPinEnumW, bufferInput);

    {//update topological order
      HTimingPoint source = design.TimingPoints[design.Get<HNet::Source, HPin>(subNet)];
      //TODO: fix next line
      HPin bufferOutput = Utils::FindCellPinByName(design, bufferNumber->cell, design.cfg.ValueOf("Buffering.DefaultBuffer.OutputPin", "Y"));
      Utils::InsertNextPoint(design, design.TimingPoints[bufferInput], source);
      Utils::InsertNextPoint(design, design.TimingPoints[bufferOutput], design.TimingPoints[bufferInput]);
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
    design.Nets.AssignPin(subNet, subNetPinEnumW, design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(node->GetSteinerPoint()));
    return;
  }
}


NewBuffer* HVGAlgorithm::FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer)
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
void HVGAlgorithm::PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer)
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


void HVGAlgorithm::CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node)
{
  int newPinCount = 0;
  int nNewNetPin = 0;
  int nPins = 0;
  char cellIdx[10];

  HNet subNet = design.Nets.AllocateNet(false);
  newNet[0] = subNet;
  design.Nets.Set<HNet::Kind, NetKind>(subNet, NetKind_Active);
  sprintf(cellIdx, "%d", 0);
  design.Nets.Set<HNet::Name>(subNet, design[net].Name() + "__BufferedPart_" + string(cellIdx));

  //allocate pins
  PinsCountCalculation(node, nPins, newBuffer);
  nPins++;
  nNewNetPin += nPins;
  design.Nets.AllocatePins(subNet, nPins);  
  newPinCount += nPins;
  //init source
  design.Nets.Set<HNet::Source, HPin>(subNet, design[net].Source());

  //add other pins
  AddSinks2Net(subNet, node, design[subNet].GetSinksEnumeratorW(), newBuffer);

  if (plotNets)
  {
    design.Plotter.ShowNetSteinerTree(subNet, Color_Red, true, HPlotter::WaitTime(plotterWaitTime));
  }

  int ind = 0;
  for (TemplateTypes<NewBuffer>::list::iterator j = newBuffer.begin(); j != newBuffer.end(); ++j, ind++)
  {
    //allocate new net
    subNet = design.Nets.AllocateNet(false);
    newNet[ind] = subNet;
    design.Nets.Set<HNet::Kind, NetKind>(subNet, NetKind_Active);
    sprintf(cellIdx, "%d", j);
    design.Nets.Set<HNet::Name>(subNet, design[net].Name() + "__BufferedPart_" + string(cellIdx));

    //allocate pins
    nPins = 0;
    NewBuffer& nodeStart = *j;
    PinsCountCalculation(nodeStart.Positions.GetPosition()->GetLeft(), nPins, newBuffer);
    nPins++;
    nNewNetPin += nPins;
    newPinCount += nPins;
    design.Nets.AllocatePins(subNet, nPins);
    design.Nets.Set<HNet::Source, HPin>(subNet, j->source);

    //add other pins
    NewBuffer& nodeStart2 = *j;
    AddSinks2Net(subNet, nodeStart2.Positions.GetPosition()->GetLeft(), design[subNet].GetSinksEnumeratorW(), newBuffer);

    if (plotNets)
    {
      design.Plotter.ShowNetSteinerTree(subNet, Color_Red, false, HPlotter::WaitTime(plotterWaitTime));
    }
  }
  HNetWrapper netw = design[net];
  int ttt = (netw.PinsCount() + int(newBuffer.size()) * 2);
  if ((netw.PinsCount() + newBuffer.size() * 2) != newPinCount)
    ALERT("ERRORR pin count");
  design.Nets.Set<HNet::Kind, NetKind>(net, NetKind_Buffered);
}
