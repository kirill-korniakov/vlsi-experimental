#include "NetListBufferingAlgorithm.h"
#include "NetBufferingAlgorithm.h"
#include "Utils.h"
#include "Timing.h"
#include "STA.h"
#include "VanGinnekenTree.h"
#include <math.h>

BufferingAllCriticalPath::BufferingAllCriticalPath(HDesign& hd): NetBufferingAlgorithm(hd)
{
}

int BufferingAllCriticalPath::BufferingCriticalPath()
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
          if ((data->countPinInBufferingNet == 0) || (
            ((net.PinsCount() <= data->countPinInBufferingNet) 
            && (!data->isTypeLimitationOnCountPinInBufferingNetEquality) ) ||
            ((net.PinsCount() == data->countPinInBufferingNet) 
            && (data->isTypeLimitationOnCountPinInBufferingNetEquality))))
          {
            bool isBufferingNet = true;
            if (!data->isBufferingNetContainPrimaryPin)            
              for (HNet::PinsEnumeratorW pew = net.GetPinsEnumeratorW(); pew.MoveNext();)              
                if (pew.IsPrimary())
                  isBufferingNet = false;

            if ((net.Name() != data->nameBufferingNet) && (data->nameBufferingNet != ""))
              continue;  
            if (!isBufferingNet) 
              continue;  
            if (!data->IsBuffering())
              continue;              

            bufferCount += BufferingNen(net).GetPositionCount();
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
