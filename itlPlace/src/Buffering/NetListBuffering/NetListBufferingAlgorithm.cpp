#include "NetListBufferingAlgorithm.h"
#include "NetBufferingAlgorithm.h"
#include "Utils.h"
#include "Timing.h"
#include "STA.h"
#include "VanGinnekenTree.h"
#include <math.h>
#include "Reporting.h"

bool BufferingAllCriticalPath::IsAppropriateNumberOfPins(VGAlgorithmData* data, HNet interconnection)
{
  return (data->countPinInBufferingInterconnection == 0) || (
    ((data->design[interconnection].PinsCount() <= data->countPinInBufferingInterconnection) 
    && (!data->isExactPinCountRequired) ) ||
    ((data->design[interconnection].PinsCount() == data->countPinInBufferingInterconnection) 
    && (data->isExactPinCountRequired)));
}

bool BufferingAllCriticalPath::IsAppropriateNumberOfPins(VGAlgorithmData* data, HCriticalPath interconnection)
{
  return (data->countPinInBufferingInterconnection == 0) || (
    ((data->design[interconnection].PointsCount() <= data->countPinInBufferingInterconnection) 
    && (!data->isExactPinCountRequired) ) ||
    ((data->design[interconnection].PointsCount() == data->countPinInBufferingInterconnection) 
    && (data->isExactPinCountRequired)));
}

int BufferingAllCriticalPath::BufferingCriticalPaths()
{
  if (!isInitialize)   Initialize();
  if (data->design.CriticalPaths.Count() < 0)
    FindCriticalPaths(data->design);

  if (data->plotBuffer)
    data->design.Plotter.ShowPlacement();

  std::vector<HCriticalPath> paths(data->design.CriticalPaths.Count());
  int idx = 0;

  for(HCriticalPaths::Enumerator i = data->design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;

  std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(data->design));
  int bufferCount = 0;
  ALERT("CriticalPaths count = %d", data->design.CriticalPaths.Count());
  for(int j = 0; j < data->design.CriticalPaths.Count(); j++)
  {
    for (HCriticalPath::PointsEnumeratorW point = (paths[j],data->design).GetEnumeratorW(); point.MoveNext();)
    {
      HNetWrapper net = data->design[((point.TimingPoint(),data->design).Pin(),data->design).OriginalNet()];
      if (net.Kind() == NetKind_Active)
      {
        if (!data->netVisit[::ToID(net)])
        {
          if (IsAppropriateNumberOfPins(data, net))
          {
            bool isBufferingNet = true;
            if (!data->isNetContainPrimaryPin)            
              for (HNet::PinsEnumeratorW pew = net.GetPinsEnumeratorW(); pew.MoveNext();)              
                if (pew.IsPrimary())
                  isBufferingNet = false;

            if ((net.Name() != data->nameBufferingNet) && (data->nameBufferingNet != ""))
              continue;  
            if (!isBufferingNet) 
              continue;  
            if (!data->IsBuffering())
              continue;              

            bufferCount += BufferingNet(net).GetPositionCount();
          }
        }
      }
    }
  }
  if (data->plotBuffer)
  {
    data->design.Plotter.Refresh(HPlotter::WaitTime(data->plotterWaitTime));
    data->design.Plotter.ShowPlacement();
  }
  ALERT("Buffers inserted: %d", bufferCount);
  ALERT("Percent area compose buffers = %f", data->PercentAreaComposeBuffers());
  return bufferCount;
}

VGVariantsListElement PathBasedBuffering::BufferingCriticalPath(HCriticalPath criticalPath, bool isRealBuffering)
{
  if (!isInitialize)   Initialize();

  if (data->printNetInfo)
  {    
    ALERT("\t%d\t%d\t", ::ToID(criticalPath), data->design.CriticalPaths.GetInt<HCriticalPath::PointsCount>(criticalPath));
  }

  if (data->typeBufferAddition != LEGAL_ADDITION)
    data->netVisit[::ToID(criticalPath)] = true;
  HCriticalPath::PointsEnumeratorW pointsEnumeratorW = (criticalPath,data->design).GetEnumeratorW();
  pointsEnumeratorW.MoveNext();
  //ALERT("critical Path point count = %d", data->design.CriticalPaths.GetInt<HCriticalPath::PointsCount>(criticalPath));
  data->vGTree->updateVanGinnekenTree->UpdateTree<HCriticalPath::PointsEnumeratorW>(pointsEnumeratorW);


  if ((data->plotSteinerPoint) || (data->plotNets))
  {
    data->design.Plotter.ShowCriticalPathSteinerTree(criticalPath, Color_Black, true, 
    HPlotter::WaitTime(data->plotterWaitTime));
  }

  /*if (data->plotVGTree)
  {
  data->design.Plotter.ShowVGTree(net, &data->vGTree->GetSource(), 
  Color_Black, true, HPlotter::WaitTime(data->plotterWaitTime));
  }*/

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
      HNet curNet = best.GetBufferPosition()->begin()->GetPosition()->GetNet();
      int newNetCount = 0;
      for (TemplateTypes<BufferPositions>::list::iterator buf = best.GetBufferPosition()->begin(); 
        buf != best.GetBufferPosition()->end(); ++buf)
      {
        if (buf->GetPosition()->GetNet() == curNet)
        {
          newNetCount++;
        }
        else
        {
          newNetCount += 2;
          curNet = buf->GetPosition()->GetNet();
        }
      }
      newNetCount++;
      HNet* newNet = new HNet[newNetCount + 1];
      additionNewElement->InsertsBuffer(newBuffer, &best);
      newBuffer.sort();
      /*for (TemplateTypes<NewBuffer>::list::iterator i = newBuffer.begin(); i != newBuffer.end(); i++)
      {
        HSteinerPointWrapper spw1 = data->design[data->design.SteinerPoints[i->sink]];
        HSteinerPointWrapper spw2 = data->design[data->design.SteinerPoints[i->source]];
        HNetWrapper nw1 = data->design[i->Positions.GetPosition()->GetNet()];     

        ALERT("node=%d\sink=%d sinkSP=%d source%d sourceSP%d net=%d name=%s ", i->Positions.GetPosition()->index,::ToID(i->sink),::ToID(spw1),::ToID(i->source), ::ToID(spw2), ::ToID(nw1),  nw1.Name().c_str());
      }*/

      /*TemplateTypes<NewBuffer>::list newBuffer2;
      HNetWrapper nw = data->design[newBuffer.begin()->Positions.GetPosition()->GetNet()];
      newBuffer2.push_back(*newBuffer.begin());
      for (TemplateTypes<NewBuffer>::list::iterator i = newBuffer.begin(); i != newBuffer.end(); i++)
      {
        HNetWrapper nw1 = data->design[i->Positions.GetPosition()->GetNet()];
        for (TemplateTypes<NewBuffer>::list::iterator j = i; j != newBuffer.end(); j++)
        {
          HNetWrapper nw2 = data->design[j->Positions.GetPosition()->GetNet()];
          if (nw1 == nw2)
          {
            bool f = false;
            for (TemplateTypes<NewBuffer>::list::iterator t = newBuffer2.begin(); t != newBuffer2.end(); t++)
            {
              if (t->Positions.GetPosition()->index == j->Positions.GetPosition()->index)
              {
                f = true;
                break;
              }
            }
            if (!f)
            {
              newBuffer2.push_back(*j);
            }
          }
        }
      }*/
      additionNewElement->currentCriticalPath = criticalPath;
      additionNewElement->CreateNets(data->design.Pins.Get<HPin::Net, HNet>(
        data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(
        data->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(
        data->design.CriticalPaths.Get<HCriticalPath::StartPoint, HCriticalPathPoint>(criticalPath)))), newBuffer, newNet, data->vGTree->GetSource()->GetLeft(), newNetCount);
      delete [] newNet;
    }
  }

  if ((data->plotSteinerPoint) || (data->plotVGTree) || (data->plotNets))
  {
  data->design.Plotter.ShowPlacement();
  }

  return best;
}

int PathBasedBuffering::BufferingNetlist()
{

  if (!isInitialize)   Initialize();
  if (data->design.CriticalPaths.Count() < 0)
    FindCriticalPaths(data->design);

  if (data->plotBuffer)
    data->design.Plotter.ShowPlacement();
  
  int totalBufferCount = 0;
  ALERT("CriticalPaths count = %d", data->design.CriticalPaths.Count());
  //for(int j = 0; j < data->design.CriticalPaths.Count(); j++)
  bool isBufferingFinish = false;
  int totalIndex = 0;
  int ind = 0;
  int bufferCount = 0;
  double curTNS = 0;
  double curWNS = 0;
  double minTNS = INFINITY;
  double minWNS = INFINITY;

  std::vector<HCriticalPath> paths(data->design.CriticalPaths.Count());
  int idx = 0;
  for(HCriticalPaths::Enumerator i = data->design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;
  std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(data->design));

  while ((!isBufferingFinish) && (totalIndex < data->design.CriticalPaths.Count() * 2) && (ind < data->design.CriticalPaths.Count()))
  {
    bufferCount = 0;
    if (IsAppropriateNumberOfPins(data, paths[ind]))
    {

      bool isBufferingNet = true;
      if (!data->isNetContainPrimaryPin) 
        if (data->design.Pins.GetBool<HPin::IsPrimary>(data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(data->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(data->design.CriticalPaths.Get<HCriticalPath::StartPoint, HCriticalPathPoint>(paths[ind])))) ||
          data->design.Pins.GetBool<HPin::IsPrimary>(data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(data->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(data->design.CriticalPaths.Get<HCriticalPath::EndPoint, HCriticalPathPoint>(paths[ind])))))
          isBufferingNet = false;

      /*for (HCriticalPath::PointsEnumeratorW point = (paths[ind],data->design).GetEnumeratorW(); point.MoveNext();)
      {
        HNetWrapper net = data->design[((point.TimingPoint(),data->design).Pin(),data->design).OriginalNet()];
        if (net.Name().find(data->textIdentifierBufferedNet) != -1)
          isBufferingNet = false;
      }*/

      if (!isBufferingNet) 
      {
        ind++;
        continue; 
      }
      if (!data->IsBuffering())
      {
        ind++;
        continue; 
      }

      if (data->printCriticalPathsInfo)
        PrintPath(data->design, paths[ind], ::ToID(paths[ind]));       
      bufferCount = BufferingCriticalPath(paths[ind]).GetPositionCount();
      
      totalBufferCount += bufferCount;

      if (bufferCount == 0)
      {
        ind++;
      }
      else
      {
        STA(data->design, data->printTimingAfterBufferingCriticalPaths);
        FindCriticalPaths(data->design);
        curTNS = Utils::TNS(data->design);
        curWNS = Utils::WNS(data->design);
        if (curTNS < minTNS)
          minTNS = curTNS;
        //else
        //break;
        if (curWNS < minWNS)
          minWNS = curWNS;
        //else
        //break;
        paths.resize(data->design.CriticalPaths.Count());
        int idx = 0;
        for(HCriticalPaths::Enumerator i = data->design.CriticalPaths.GetEnumerator(); i.MoveNext();)
          paths[idx++] = i;
        std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(data->design));
        ind = 0;
      }
    }
    totalIndex++;
  }

  if (data->plotBuffer)
  {
    data->design.Plotter.Refresh(HPlotter::WaitTime(data->plotterWaitTime));
    data->design.Plotter.ShowPlacement();
  }
  ALERT("Buffers inserted: %d", totalBufferCount);
  ALERT("Minimal TNS: %f", minTNS);
  ALERT("Minimal WNS: %f", minWNS);
  ALERT("Percent area compose buffers = %f", data->PercentAreaComposeBuffers());
  return totalBufferCount;
}