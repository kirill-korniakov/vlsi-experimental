#include "GPBuffering.h"
#include "OptimizationContext.h"
#include "STA.h"
#include "Utils.h"
#include "Timing.h"
#include "VanGinnekenTree.h"

void MoveBinIndexesIntoBorders(AppCtx* context, int& min_col, int& min_row, int& max_col, int& max_row);
void DetermineBordersOfClusterPotential(int& min_col, int& max_col, 
                                        int& min_row, int& max_row,
                                        double x, double y, AppCtx* context);

GPBuffering::GPBuffering(HDesign& hd): NetBufferingAlgorithm(hd)
{

}

double GPBuffering::CalcBufferArea(AppCtx* context, int colIdx, int rowIdx, BufferPositions& bufferPositions)
{
  double width = context->sprData.binGrid.binWidth;
  double height = context->sprData.binGrid.binHeight;

  double binX = context->sprData.binGrid.bins[rowIdx][colIdx].xCoord - width / 2.0;
  double binY = context->sprData.binGrid.bins[rowIdx][colIdx].yCoord - height / 2.0;

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

  bool isXVisit = false, isYVisit = false;

  if (x > binX) 
  {
    if (x > binX2)
    {
      xSize = 0;
    }
    else
    {
      if (x2 < binX2)
      {
        xSize = bufWidth;
        isXVisit = true;
      }
      else
      {
        xSize = binX2 - x;
        isXVisit = true;
      }
    }
  }
  else
  {
    if (x2 < binX)
    {
      xSize = 0;
    }
    else
    {
      if (x2 < binX2)
      {
        xSize = x2 - binX;
        isXVisit = true;
      }
      else
      {
        xSize = width;
        isXVisit = true;
      }
    }
  }

  if (y > binY) 
  {
    if (y > binY2)
      ySize = 0;
    else
    {
      if (y2 < binY2)
      {
        ySize = bufHeight;
        isYVisit = true;
      }
      else
      {
        ySize = binY2 - y;
        isYVisit = true;
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
        isYVisit = true;
      }
      else
      {
        ySize = height;
        isYVisit = true;
      }
    }
  }

  //if (isXVisit && isYVisit)
  //  ALERT("nrowIdx = %d\tcolIdx= %d", rowIdx, colIdx);
  double sizeBufferMultiplier = 1.0 / bufferPositions.GetPosition()->GetTree()->vGAlgorithm->data->sizeBufferMultiplier;
  return (xSize * ySize * sizeBufferMultiplier);
}

int GPBuffering::UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant)
{

  for (TemplateTypes<BufferPositions>::list::iterator pos = vGVariant.GetBufferPosition()->begin(); 
    pos != vGVariant.GetBufferPosition()->end(); ++pos)
  {
    for (int rowIdx = 0; rowIdx < context->sprData.binGrid.nBinRows; rowIdx++)
    {
      for (int colIdx = 0; colIdx < context->sprData.binGrid.nBinCols; colIdx++)
      {
        context->sprData.bufferPotentialOverBins[rowIdx][colIdx] = 0;
        double bsf = CalcBufferArea(context, colIdx, rowIdx, *pos);
        context->sprData.bufferPotentialOverBins[rowIdx][colIdx] = bsf;
      }
    }// loop over affected bins


    for (int rowIdx = 0; rowIdx < context->sprData.binGrid.nBinRows; rowIdx++)
    {
      for (int colIdx = 0; colIdx < context->sprData.binGrid.nBinCols; colIdx++)
      {
        context->sprData.binGrid.bins[rowIdx][colIdx].sumBufPotential += 
          context->sprData.bufferPotentialOverBins[rowIdx][colIdx];
      }
    }
  }
  return vGVariant.GetPositionCount();
}

int GPBuffering::SetBinTableBuffer(AppCtx* context, double HPWL, double LHPWL)
{
  if (!isInitialize) Initialize();

  if (data->design.cfg.ValueOf("AdaptiveSizeBufferMultiplier", false))
    //data->SetSizeBufferMultiplier( min((context->sprData.binGrid.binHeight * context->sprData.binGrid.binWidth / 
    //data->GetSizeBuffer()) / 100.0, 1.0));
    data->sizeBufferMultiplier = ((LHPWL - HPWL) / LHPWL);

  if (data->plotBuffer || data->plotBinGridValue)
    data->design.Plotter.ShowPlacement();

  //ALERT("Buffering type: %d", data->design.cfg.ValueOf("TypePartition", 0));
  //ALERT("data->GetSizeBuffer() = %f",data->GetSizeBuffer());
  ALERT("NewSizeBufferMultiplier = %f", data->sizeBufferMultiplier);

  STA(data->design);
  if (data->design.CriticalPaths.Count() < 0)
    FindCriticalPaths(data->design);

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
  ALERT("#CriticalPaths = %d", data->design.CriticalPaths.Count());
  for(int j = 0; j < data->design.CriticalPaths.Count(); j++)
  {
    for (HCriticalPath::PointsEnumeratorW point = (paths[j],data->design).GetEnumeratorW(); point.MoveNext();)
    {
      HNetWrapper net = data->design[((point.TimingPoint(),data->design).Pin(),data->design).OriginalNet()];
      if (net.Kind() == NetKind_Active)// && (net.PinsCount() <= 3 ))//&& (treeRepository.NoVGTree(net)))//
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

            bufferCount += UpdateBinTable(context, BufferingNen(net, false, context));
          }
        }
      }
    }
  }

  if (data->plotBinGridValue)
    data->design.Plotter.PlotFillBinGrid(context);

  if (data->plotBuffer || data->plotBinGridValue)
  {
    data->design.Plotter.Refresh(HPlotter::WaitTime(data->plotterWaitTime));
    data->design.Plotter.ShowPlacement();
  }

  ALERT("Buffers inserted: %d", bufferCount);

  return bufferCount;
}