#include "VanGinnekenAlgorithm.h"
#include <math.h>
#include "Utils.h"


void VGAlgorithm::LoadBuffers()
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

VGAlgorithm::VGAlgorithm(HDesign& hd): design(hd), WirePhisics(hd.RoutingLayers.Physics)
{
  LoadBuffers();
  PlotSteinerPoint = design.cfg.ValueOf("PlotSteinerPoint", false);
  PlotVGTree = design.cfg.ValueOf("PlotVGTree", false);
  PlotNets = design.cfg.ValueOf("PlotNets", false);
  PrintNetInfo = design.cfg.ValueOf("PrintNetInfo", false);
  partitionCount = design.cfg.ValueOf("Interval", 1);
  vGTree = new VanGinnekenTree(design, partitionCount);
}

int VGAlgorithm::BufferingPlacement()
{
  ALERT("Buffering type: %d", design.cfg.ValueOf("TypePartition", 0));
  std::vector<HCriticalPath> paths(design.CriticalPaths.Count());
  int idx = 0;
  for(HCriticalPaths::Enumerator i = design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;
  design.Plotter.PlotSteinerForest(Color_Black);

  std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(design));

  int bufferCount = 0;
  ALERT("CriticalPaths count = %d", design.CriticalPaths.Count());
  for(int j = 0; j < design.CriticalPaths.Count(); j++)
  {
    for (HCriticalPath::PointsEnumeratorW point = (paths[j],design).GetEnumeratorW(); point.MoveNext(); )
    {
      HNetWrapper net = design[((point.TimingPoint(),design).Pin(),design).OriginalNet()];
      if (net.Kind() == NetKind_Active) //&& (treeRepository.NoVGTree(net)))//  && (net.PinsCount() >= 50 ))//&& (treeRepository.NoVGTree(net)))//
      {

        //if ((net.Kind() == NetKind_Active) && (net.Name() == "n_7347"))  
        
          bufferCount += BufferingNen(net);
      }
    }
    //ALERT("CriticalPaths id = %d", j);
  }
  ALERT("Buffers inserted: %d", bufferCount);
  
  return bufferCount;
}

int VGAlgorithm::BufferingNen(HNet& net)
{
  
  //VanGinnekenTree* vGTree = //(design, partitionCount, design.SteinerPoints[(net, design).Source()]);
  
  vGTree->UpdateTree(design.SteinerPoints[(net, design).Source()]);
//  treeRepository.CreateVGTree(net);
  if (PrintNetInfo)
  {
    ALERT("\t%s\t%d\t", design.Nets.GetString<HNet::Name>(net).c_str(), design.Nets.GetInt<HNet::PinsCount>(net));
  }
  if ((PlotSteinerPoint) || (PlotNets))
  {
    design.Plotter.PlotText(design.Nets.GetString<HNet::Name>(net));
    design.Plotter.PlotNetSteinerTree(net, Color_Black);  
    design.Plotter.Refresh();
  }
  if (PlotVGTree)
  {
    design.Plotter.PlotText(design.Nets.GetString<HNet::Name>(net));
    design.Plotter.PlotVGTree(&vGTree->GetSource(), Color_Black); 
    design.Plotter.Refresh();
  }
  VGVariantsListElement best = Algorithm(vGTree);
  int bufCount = best.GetPositionCount();
  //ALERT("net: %s\tbufcount = %d",(net, design).Name().c_str(), bufCount );
  if (bufCount > 0)
  {
    TemplateTypes<NewBuffer>::list newBuffer;
    HNet* newNet = new HNet[bufCount + 1];
    InsertsBuffer(newBuffer, &best);
    newBuffer.sort();
    //ALERT("name = %s", (net, design).Name().c_str());
    CreateNets(net, newBuffer, newNet, vGTree->GetSource().GetLeft());
  }
  if ((PlotSteinerPoint) || (PlotVGTree) || (PlotNets))
  {
    design.Plotter.ShowPlacement();
  }
  
  return bufCount;
}

VGVariantsListElement VGAlgorithm::Algorithm(VanGinnekenTree* vGTree)
{

  double t = 0;
  VGVariantsListElement best;
  double tMax = -INFINITY;

  TemplateTypes<VGVariantsListElement>::list* vGList = CreateVGList(vGTree->GetSource().GetLeft());
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
  if (PrintNetInfo)
  {
    printf("best pos = %d\ttmax = %f\n", best.GetPositionCount(), tMax);
  }
  delete vGList;
  return best;
}

TemplateTypes<VGVariantsListElement>::list* VGAlgorithm::CreateVGList( VanGinnekenTreeNode* node)
{
  if (node->isSink())
  {
    TemplateTypes<VGVariantsListElement>::list* newList = new TemplateTypes<VGVariantsListElement>::list();
    VGVariantsListElement element;
    element.SetC(node->GetC());
    element.SetRAT(node->GetRAT());
    newList->push_back(element);
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
        return MergeList(leftVGList, RightVGList);
      }
      return leftVGList;
    }
    else
    {
      TemplateTypes<VGVariantsListElement>::list* newList = CreateVGList(node->GetLeft());
      UpdateValue(newList, GetLength(node, node->GetLeft()));
      SortVGVariantsListElement(newList);
      AddBuffer(newList, node);      
      return newList;
    }

}

TemplateTypes<VGVariantsListElement>::list* VGAlgorithm::MergeList(TemplateTypes<VGVariantsListElement>::list* leftVGList, TemplateTypes<VGVariantsListElement>::list* RightVGList)
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
  return result;
}

void VGAlgorithm::UpdateValue(TemplateTypes<VGVariantsListElement>::list* vGList, double lengthEdge)
{
  for (TemplateTypes<VGVariantsListElement>::list::iterator i = vGList->begin(); i != vGList->end(); ++i)
  {
    i->SetRAT(i->GetRAT() - WirePhisics.RPerDist * lengthEdge * i->GetC() - 0.5 * WirePhisics.RPerDist * lengthEdge * lengthEdge * WirePhisics.LinearC);
    i->SetC(i->GetC() + WirePhisics.LinearC * lengthEdge);
  }
}

void VGAlgorithm::AddBuffer(TemplateTypes<VGVariantsListElement>::list* vGList, VanGinnekenTreeNode* node)
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

void VGAlgorithm::SortVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList)
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


void VGAlgorithm::InsertVGVariantsListElement(TemplateTypes<VGVariantsListElement>::list* vGList, VGVariantsListElement& element)
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

double VGAlgorithm::GetLength(VanGinnekenTreeNode* node1, VanGinnekenTreeNode* node2)
{

  return fabs(node1->GetX() - node2->GetX()) + fabs(node1->GetY() - node2->GetY());
}

void VGAlgorithm::InsertsBuffer(TemplateTypes<NewBuffer>::list& newBuffer, VGVariantsListElement* best)
{

  for (TemplateTypes<BufferPositions>::list::iterator pos = best->GetBufferPosition()->begin(); pos != best->GetBufferPosition()->end(); ++pos)
  {

    InsertBuffer(newBuffer, *pos);

  }


}

void  VGAlgorithm::InsertBuffer(TemplateTypes<NewBuffer>::list& newBuffer, BufferPositions& position)
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

void VGAlgorithm::AddSinks2Net(HNet& subNet, VanGinnekenTreeNode* node, HNetWrapper::PinsEnumeratorW& subNetPinEnum, TemplateTypes<NewBuffer>::list& newBuffer)
{

  NewBuffer* bufferNumber = FindBufferNumberByIndex(node, newBuffer);

  //insert sink pin (buffer input) to the subnet
  if (bufferNumber != NULL)
  {
    
      HPin bufferInput = bufferNumber->sink;

      subNetPinEnum.MoveNext();

      design.Nets.AssignPin(subNet, subNetPinEnum, bufferInput);

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
    AddSinks2Net(subNet, node->GetLeft(), subNetPinEnum, newBuffer);

  }

  //insert pins from right subtree
  if (node->HasRight())
  {
    AddSinks2Net(subNet, node->GetRight(), subNetPinEnum, newBuffer);
  }

  //insert sink pin to the final subnets
  if (node->isSink())
  {		
    subNetPinEnum.MoveNext();
    design.Nets.AssignPin(subNet, subNetPinEnum, design.SteinerPoints.Get<HSteinerPoint::Pin, HPin>(node->GetSteinerPoint()));
    return;
  }
}


NewBuffer* VGAlgorithm::FindBufferNumberByIndex(VanGinnekenTreeNode* node, TemplateTypes<NewBuffer>::list& newBuffer)
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
void VGAlgorithm::PinsCountCalculation(VanGinnekenTreeNode* node, int& nPins, TemplateTypes<NewBuffer>::list& newBuffer)
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


void VGAlgorithm::CreateNets(HNet& net, TemplateTypes<NewBuffer>::list& newBuffer, HNet* newNet, VanGinnekenTreeNode* node)
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
  
  if (PlotNets)
  {
    design.Plotter.PlotText(design.Nets.GetString<HNet::Name>(subNet));
    design.Plotter.PlotNetSteinerTree(subNet, Color_Red);  
    design.Plotter.Refresh();
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

    if (PlotNets)
    {
      design.Plotter.PlotText(design.Nets.GetString<HNet::Name>(subNet));
      design.Plotter.PlotNetSteinerTree(subNet, Color_Red);  
      design.Plotter.Refresh();
    }
  }
  HNetWrapper netw = design[net];
  int ttt = (netw.PinsCount() + int(newBuffer.size()) * 2);
  if ((netw.PinsCount() + newBuffer.size() * 2) != newPinCount)
    ALERT("ERRORR pin count");
  design.Nets.Set<HNet::Kind, NetKind>(net, NetKind_Buffered);
}
