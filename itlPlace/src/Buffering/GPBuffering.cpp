#include "GPBuffering.h"
#include "OptimizationContext.h"
#include "STA.h"
#include "Timing.h"
#include "Reporting.h"

void MoveBinIndexesIntoBorders(AppCtx* context, int& min_col, int& min_row, int& max_col, int& max_row);
void DetermineBordersOfClusterPotential(int& min_col, int& max_col, 
                                        int& min_row, int& max_row,
                                        double x, double y, AppCtx* context);

GPBuffering::GPBuffering(HDesign& hd): PathBasedBuffering(hd)
{
    removeBuffer = new RemoveBuffer(this->data);
}

void GPBuffering::DoBuffering(AppCtx& context, double HPWL, double LHPWL)
{
    WRITELINE("");
    ALERT("NEW BUFFERING STARTED");
    ConfigContext ctx = data->design.cfg.OpenContext("GlobalPlacement.New_Buffering");

    if (!isInitialize) 
        Initialize();

    Init2(&context, HPWL, LHPWL);

    if (data->design.cfg.ValueOf("TypeNetListBuffering", 0) == BUFFERING_ALL_CRITICAL_PATH)
        FillBinTable(&context, paths);
    else
        FillBinTablePathBased(&context, paths);

    Plot(&context);

    ctx.Close();
    ALERT("NEW BUFFERING FINISHED");
    WRITELINE("");
}

double GPBuffering::CalcBufferArea(AppCtx* context, int binCol, int binRow, BufferPositions& bufferPosition)
{
    HDesign& design = bufferPosition.GetPosition()->GetTree()->vGAlgorithmData->design;
    BinGrid& binGrid = context->sprData.binGrid;

    double width = binGrid.binWidth;
    double height = binGrid.binHeight;

    double binX = binGrid.bins[binRow][binCol].xCoord - width / 2.0;
    double binY = binGrid.bins[binRow][binCol].yCoord - height / 2.0;

    double binX2 = binX + binGrid.binWidth;
    double binY2 = binY + binGrid.binHeight;

    double dx = bufferPosition.GetPosition()->x - binGrid.bins[binRow][binCol].xCoord;
    double dy = bufferPosition.GetPosition()->y - binGrid.bins[binRow][binCol].yCoord;

    double x = bufferPosition.GetPosition()->x;
    double y = bufferPosition.GetPosition()->y;

    HMacroType type = bufferPosition.GetBufferInfo()->Type();    
    double bufWidth = design[type].SizeX();
    double bufHeight = design[type].SizeY();
    double x2 = bufferPosition.GetPosition()->x + design[type].SizeX();
    double y2 = bufferPosition.GetPosition()->y + design[type].SizeY();

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

    double sizeBufferMultiplier = bufferPosition.GetPosition()->GetTree()->vGAlgorithmData->sizeBufferMultiplier;
    return (xSize * ySize * sizeBufferMultiplier);
}

int GPBuffering::UpdateBinTable(AppCtx* context, VGVariantsListElement& vGVariant)
{
    for (TemplateTypes<BufferPositions>::list::iterator pos = vGVariant.GetBufferPosition()->begin(); 
        pos != vGVariant.GetBufferPosition()->end(); ++pos)
    {
        for (int binRow = 0; binRow < context->sprData.binGrid.nBinRows; binRow++)
        {
            for (int binCol = 0; binCol < context->sprData.binGrid.nBinCols; binCol++)
            {
                context->sprData.bufferPotentialOverBins[binRow][binCol] = 0;
                double bsf = CalcBufferArea(context, binCol, binRow, *pos);
                context->sprData.bufferPotentialOverBins[binRow][binCol] = bsf;
            }
        }// loop over affected bins

        for (int binRow = 0; binRow < context->sprData.binGrid.nBinRows; binRow++)
        {
            for (int binCol = 0; binCol < context->sprData.binGrid.nBinCols; binCol++)
            {
                context->sprData.binGrid.bins[binRow][binCol].sumBufPotential += 
                    context->sprData.bufferPotentialOverBins[binRow][binCol];
            }
        }
    }
    return vGVariant.GetPositionCount();
}

void GPBuffering::Init2(AppCtx* context, double HPWL, double LHPWL)
{
    if (data->design.cfg.ValueOf("AdaptiveSizeBufferMultiplier", false))
        //data->SetSizeBufferMultiplier( min((context->sprData.binGrid.binHeight * context->sprData.binGrid.binWidth / 
        //data->GetSizeBuffer()) / 100.0, 1.0));
        data->sizeBufferMultiplier = 1.0 - ((LHPWL - HPWL) / LHPWL);

    //ALERT("Buffering type: %d", data->design.cfg.ValueOf("TypePartition", 0));
    //ALERT("data->GetSizeBuffer() = %f",data->GetSizeBuffer());
    ALERT("NewSizeBufferMultiplier = %f", data->sizeBufferMultiplier);

    STA(data->design);
    if (data->design.CriticalPaths.Count() < 0)
        FindCriticalPaths(data->design);
    ALERT("CriticalPaths count = %d", data->design.CriticalPaths.Count());

    paths.clear();
    paths.resize(data->design.CriticalPaths.Count());
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
}

void GPBuffering::FillBinTablePathBased(AppCtx* context, std::vector<HCriticalPath>& paths)
{
    int countCP = 0;

    int totalBufferCount = 0;
    bool isBufferingFinish = false;
    int totalIndex = 0;
    int ind = 0;
    int bufferCount = 0;
    double curTNS = 0;
    double curWNS = 0;
    double minTNS = INFINITY;
    double minWNS = INFINITY;  

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

            bufferCount = UpdateBinTable(context, BufferingCriticalPath(paths[ind]));
            totalBufferCount += bufferCount;

            if (bufferCount == 0)
            {
                ind++;
            }
            else
            {
                STA(data->design, data->printTimingAfterBufferingCriticalPaths, data->reRoutingSteinerTree);

                curTNS = Utils::TNS(data->design);
                curWNS = Utils::WNS(data->design);

                if (curTNS < minTNS)
                    minTNS = curTNS;
                if (curWNS < minWNS)
                    minWNS = curWNS;

                if (countCP > data->numberBufferedAtOnceCriticalPaths)
                {
                    FindCriticalPaths(data->design);
                    countCP = 0;
                    ind = 0;
                    paths.resize(data->design.CriticalPaths.Count());

                    int idx = 0;
                    for(HCriticalPaths::Enumerator i = data->design.CriticalPaths.GetEnumerator(); i.MoveNext();)
                        paths[idx++] = i;
                    std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(data->design));
                }
                else
                {
                    ind++;   
                    countCP++;
                }

            }
        }
        totalIndex++;
    }

    ALERT("Buffers inserted: %d", totalBufferCount);
    ALERT("Minimal TNS: %f", minTNS);
    ALERT("Minimal WNS: %f", minWNS);
    ALERT("Percent area compose buffers = %f", data->PercentAreaComposeBuffers());

    ALERT("REMOVE NEW BUFFERING STARTED");
    removeBuffer->RemoveNewBuffering();
    ALERT("REMOVE NEW BUFFERING FINISHED");
}

void GPBuffering::FillBinTable(AppCtx* context, std::vector<HCriticalPath>& paths)
{
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
                    //FIXME: create a function with a meaningful name and boolean return value
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
    ALERT("Buffers inserted: %d", bufferCount);
}

void GPBuffering::Plot(AppCtx* context)
{
    if (data->plotBuffer || data->plotBinGridValue)
        data->design.Plotter.ShowPlacement();

    if (data->plotBinGridValue)
        data->design.Plotter.PlotFillBinGrid(context);

    if (data->plotBuffer || data->plotBinGridValue)
    {
        data->design.Plotter.Refresh(HPlotter::WaitTime(data->plotterWaitTime));
        data->design.Plotter.ShowPlacement();
    }     
}