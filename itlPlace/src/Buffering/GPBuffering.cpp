#include "GPBuffering.h"
#include "OptimizationContext.h"
#include "STA.h"
#include "Utils.h"
#include "Timing.h"
#include "VanGinnekenTree.h"
#include "Reporting.h"

void MoveBinIndexesIntoBorders(AppCtx* context, int& min_col, int& min_row, int& max_col, int& max_row);
void DetermineBordersOfClusterPotential(int& min_col, int& max_col, 
                                        int& min_row, int& max_row,
                                        double x, double y, AppCtx* context);

GPBuffering::GPBuffering(HDesign& hd): PathBasedBuffering(hd)
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

    double dx = bufferPositions.GetPosition()->x - binGrid.bins[rowIdx][colIdx].xCoord;
    double dy = bufferPositions.GetPosition()->y - binGrid.bins[rowIdx][colIdx].yCoord;

    double x = bufferPositions.GetPosition()->x;
    double y = bufferPositions.GetPosition()->y;
    double bufWidth = bufferPositions.GetPosition()->GetTree()->vGAlgorithmData->design[bufferPositions.GetBufferInfo()->Type()].SizeX();
    double bufHeight = bufferPositions.GetPosition()->GetTree()->vGAlgorithmData->design[bufferPositions.GetBufferInfo()->Type()].SizeY();
    double x2 = bufferPositions.GetPosition()->x + bufferPositions.GetPosition()->GetTree()->vGAlgorithmData->design[bufferPositions.GetBufferInfo()->Type()].SizeX();
    double y2 = bufferPositions.GetPosition()->y + bufferPositions.GetPosition()->GetTree()->vGAlgorithmData->design[bufferPositions.GetBufferInfo()->Type()].SizeY();

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

    double sizeBufferMultiplier = 1.0 / bufferPositions.GetPosition()->GetTree()->vGAlgorithmData->sizeBufferMultiplier;
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

int GPBuffering::SetBinTablePathBasedBuffer(AppCtx* context, double HPWL, double LHPWL)
{
    if (!isInitialize) Initialize();

    if (data->design.cfg.ValueOf("AdaptiveSizeBufferMultiplier", false))
        //data->SetSizeBufferMultiplier( min((context->sprData.binGrid.binHeight * context->sprData.binGrid.binWidth / 
        //data->GetSizeBuffer()) / 100.0, 1.0));
        data->sizeBufferMultiplier = ((LHPWL - HPWL) / LHPWL);

    //ALERT("Buffering type: %d", data->design.cfg.ValueOf("TypePartition", 0));
    //ALERT("data->GetSizeBuffer() = %f",data->GetSizeBuffer());
    ALERT("NewSizeBufferMultiplier = %f", data->sizeBufferMultiplier);

    STA(data->design);
    if (data->design.CriticalPaths.Count() < 0)
        FindCriticalPaths(data->design);

    if (data->plotBuffer || data->plotBinGridValue)
        data->design.Plotter.ShowPlacement();

    int totalBufferCount = 0;
    ALERT("CriticalPaths count = %d", data->design.CriticalPaths.Count());
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


    for (int i = 0; i < context->sprData.binGrid.nBinRows; ++i)
    {
        for (int j = 0; j < context->sprData.binGrid.nBinCols; ++j)
        {
            context->sprData.binGrid.bins[i][j].sumBufPotential = 0.0;
            context->sprData.bufferPotentialOverBins[i][j] = 0.0;
        }
    }

    ALERT("#CriticalPaths = %d", data->design.CriticalPaths.Count());int countCP = 0;
    while ((!isBufferingFinish) && IsLimitationCountCriticalPathExecute(totalIndex) && (ind < data->design.CriticalPaths.Count()))
    {
        bufferCount = 0;
        if (IsAppropriateNumberOfPins(data, paths[ind]))
        {

            bool isBufferingNet = true;
            if (!data->isNetContainPrimaryPin) 
                if (data->design.Pins.GetBool<HPin::IsPrimary>(data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(data->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(data->design.CriticalPaths.Get<HCriticalPath::StartPoint, HCriticalPathPoint>(paths[ind])))) ||
                    data->design.Pins.GetBool<HPin::IsPrimary>(data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(data->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(data->design.CriticalPaths.Get<HCriticalPath::EndPoint, HCriticalPathPoint>(paths[ind])))))
                    isBufferingNet = false;

            if (data->IslimitationsCountRepeatNet())
            {
                int countRepeatNet = 0;
                int curRepeatNet = 0;
                for (HCriticalPath::PointsEnumeratorW point = (paths[ind],data->design).GetEnumeratorW(); point.MoveNext();)
                {
                    HNetWrapper net = data->design[((point.TimingPoint(),data->design).Pin(),data->design).OriginalNet()];
                    curRepeatNet = Utils::FindRepeat(net.Name(), data->textIdentifierBufferedNet);
                    if (curRepeatNet > countRepeatNet) 
                        countRepeatNet = curRepeatNet;
                    if (countRepeatNet >= data->maxCountRepeatNet)
                        isBufferingNet = false;
                }
            }

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
            if (data->nameBufferingNet != "")
            {
                HPinWrapper firstPin = data->design[data->design[data->design[data->design[paths[ind]].StartPoint()].TimingPoint()].Pin()];
                HPinWrapper lastPin = data->design[data->design[data->design[data->design[paths[ind]].EndPoint()].TimingPoint()].Pin()];

                HCellWrapper firstCell = data->design[firstPin.Cell()];
                HCellWrapper lastCell = data->design[lastPin.Cell()];

                string pathName = 
                    string("From    ") + 
                    (firstPin.IsPrimary() ?  string("PIN") : firstCell.Name()) +
                    firstPin.Name().c_str() +
                    string("    To    ") +
                    (lastPin.IsPrimary() ?  string("PIN") : lastCell.Name()) +
                    lastPin.Name();

                if (pathName != data->nameBufferingNet)
                {
                    ind++;
                    continue; 
                }
            }

            if (data->printCriticalPathsInfo)
            {
                ALERT("Critical Paths number = %d", ind);
                PrintPath(data->design, paths[ind], ::ToID(paths[ind])); 
            }

            bufferCount = UpdateBinTable(context, BufferingCriticalPath(paths[ind], false));
            totalBufferCount += bufferCount;
            ind++;
        }
        totalIndex++;
    }

    if (data->plotBinGridValue)
        data->design.Plotter.PlotFillBinGrid(context);

    if (data->plotBuffer || data->plotBinGridValue)
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
                    if ((data->countPinInBufferingInterconnection == 0) || (
                        ((net.PinsCount() <= data->countPinInBufferingInterconnection) 
                        && (!data->isExactPinCountRequired) ) ||
                        ((net.PinsCount() == data->countPinInBufferingInterconnection) 
                        && (data->isExactPinCountRequired))))
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

                        bufferCount += UpdateBinTable(context, BufferingNet(net, false));
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