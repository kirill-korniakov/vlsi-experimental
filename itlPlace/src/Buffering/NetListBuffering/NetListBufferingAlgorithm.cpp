#include "NetListBufferingAlgorithm.h"
#include "NetBufferingAlgorithm.h"
#include "Utils.h"
#include "Timing.h"
#include "STA.h"
#include "VanGinnekenTree.h"
#include <math.h>

bool BufferingAllCriticalPath::IsAppropriateNumberOfPins(VGAlgorithmData* data, HNet net)
{
    return (data->countPinInBufferingNet == 0) || (
        ((data->design[net].PinsCount() <= data->countPinInBufferingNet) 
        && (!data->isExactPinCountRequired) ) ||
        ((data->design[net].PinsCount() == data->countPinInBufferingNet) 
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
  data->vGTree->updateVanGinnekenTree->UpdateTree<HCriticalPath::PointsEnumeratorW>(pointsEnumeratorW);
   

 /* if ((data->plotSteinerPoint) || (data->plotNets))
  {
    data->design.Plotter.ShowNetSteinerTree(net, Color_Black, true, 
      HPlotter::WaitTime(data->plotterWaitTime));
  }*/

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

      TemplateTypes<NewBuffer>::list newBuffer2;
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
      }

      additionNewElement->CreateNets(data->design.Pins.Get<HPin::Net, HNet>(
        data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(
        data->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(
        data->design.CriticalPaths.Get<HCriticalPath::StartPoint, HCriticalPathPoint>(criticalPath)))), newBuffer2, newNet, data->vGTree->GetSource()->GetLeft(), newNetCount);
      delete [] newNet;
    }
  }

  /*if ((data->plotSteinerPoint) || (data->plotVGTree) || (data->plotNets))
  {
    data->design.Plotter.ShowPlacement();
  }*/

  return best;
}

int PathBasedBuffering::BufferingNetlist()
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
    //if (data->design.CriticalPaths.GetInt<HCriticalPath::PointsCount>(paths[j]) == 8)
      bufferCount += BufferingCriticalPath(paths[j]).GetPositionCount();
      ALERT("Buffer insited = %d", bufferCount);
      FindCriticalPaths(data->design);
      STA(data->design);
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