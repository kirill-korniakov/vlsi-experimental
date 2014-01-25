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

bool BufferingAllCriticalPath::IsAppropriateNumberOfPins(VGAlgorithmData* data, CriticalPathBuffering& interconnection)
{
    return (data->countPinInBufferingInterconnection == 0) || (
        ((interconnection.pointCount <= data->countPinInBufferingInterconnection) 
        && (!data->isExactPinCountRequired) ) ||
        ((interconnection.pointCount == data->countPinInBufferingInterconnection) 
        && (data->isExactPinCountRequired)));
}

int BufferingAllCriticalPath::BufferingCriticalPaths()
{
    if (!isInitialize)   Initialize();
    if (data->design.CriticalPaths.Count() < 0)
        FindCriticalPaths(data->design);

    if (data->plotBuffer)
        data->design.Plotter->ShowPlacement();

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
        data->design.Plotter->Refresh(HPlotter::WaitTime(data->plotterWaitTime));
        data->design.Plotter->ShowPlacement();
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
        ALERT("Critical path id:%d\t%d\t", ::ToID(criticalPath), data->design.CriticalPaths.GetInt<HCriticalPath::PointsCount>(criticalPath));
    }

    if (data->typeBufferAddition != LEGAL_ADDITION)
        data->netVisit[::ToID(criticalPath)] = true;
    HCriticalPath::PointsEnumeratorW pointsEnumeratorW = (criticalPath,data->design).GetEnumeratorW();
    pointsEnumeratorW.MoveNext();
    data->vGTree->updateVanGinnekenTree->UpdateTree<HCriticalPath::PointsEnumeratorW>(pointsEnumeratorW);


    if ((data->plotSteinerPoint) || (data->plotNets))
    {
        data->design.Plotter->ShowCriticalPathSteinerTree(criticalPath, Color_Black, true, 
            HPlotter::WaitTime(data->plotterWaitTime));
    }

    /*if (data->plotVGTree)
    {
    data->design.Plotter->ShowVGTree(net, &data->vGTree->GetSource(), 
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
                data->design.Plotter->DrawFilledRectangle(pos->GetPosition()->x, pos->GetPosition()->y, data->design[pos->GetBufferInfo()->Type()].SizeX(), data->design[pos->GetBufferInfo()->Type()].SizeY(), Color_Red, false);
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


            HNet tempNet = data->design.Pins.Get<HPin::Net, HNet>(
                        data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(
                        data->design.CriticalPathPoints.Get<HCriticalPathPoint::TimingPoint, HTimingPoint>(
                        data->design.CriticalPaths.Get<HCriticalPath::StartPoint, HCriticalPathPoint>(criticalPath))));
            additionNewElement->CreateNets(tempNet, newBuffer, newNet, data->vGTree->GetSource()->GetLeft(), newNetCount);
            delete [] newNet;
        }
    }

    if ((data->plotSteinerPoint) || (data->plotVGTree) || (data->plotNets))
    {
        data->design.Plotter->ShowPlacement();
    }

    return best;
}

bool PathBasedBuffering::IsLimitationCountCriticalPathExecute(int totalIndex)
{
    if (data->limitationCountCriticalPath == 0) return true;
    return (totalIndex < data->design.CriticalPaths.Count() * data->limitationCountCriticalPath);
}

int PathBasedBuffering::BufferingNetlist()
{

    if (!isInitialize)   Initialize();
    if (data->design.CriticalPaths.Count() < 0)
        FindCriticalPaths(data->design);

    if (data->plotBuffer)
        data->design.Plotter->ShowPlacement();

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
    int countCP = 0;
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


            bufferCount = BufferingCriticalPath(paths[ind]).GetPositionCount();
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

    if (data->plotBuffer)
    {
        data->design.Plotter->Refresh(HPlotter::WaitTime(data->plotterWaitTime));
        data->design.Plotter->ShowPlacement();
    }

    ALERT("Buffers inserted: %d", totalBufferCount);
    ALERT("Minimal TNS: %f", minTNS);
    ALERT("Minimal WNS: %f", minWNS);
    ALERT("Percent area compose buffers = %f", data->PercentAreaComposeBuffers());
    return totalBufferCount;
}

VGVariantsListElement PathBasedBuffering::BufferingCriticalPath(CriticalPathBuffering& criticalPath, bool isRealBuffering)
{
    if (!isInitialize)   Initialize();

    if (data->printNetInfo)
    {    
        //        ALERT("Critical path id:%d\t%d\t", ::ToID(criticalPath), data->design.CriticalPaths.GetInt<HCriticalPath::PointsCount>(criticalPath));
    }

    if (data->typeBufferAddition != LEGAL_ADDITION)
        data->netVisit[criticalPath.indexPaht] = true;
    //    HCriticalPath::PointsEnumeratorW pointsEnumeratorW = (criticalPath,data->design).GetEnumeratorW();
    criticalPath.MoveNext();
    data->vGTree->updateVanGinnekenTree->UpdateTree<CriticalPathBuffering>(criticalPath);


    if ((data->plotSteinerPoint) || (data->plotNets))
    {
        // data->design.Plotter->ShowCriticalPathSteinerTree(criticalPath, Color_Black, true, 
        //     HPlotter::WaitTime(data->plotterWaitTime));
    }

    /*if (data->plotVGTree)
    {
    data->design.Plotter->ShowVGTree(net, &data->vGTree->GetSource(), 
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
                data->design.Plotter->DrawFilledRectangle(pos->GetPosition()->x, pos->GetPosition()->y, data->design[pos->GetBufferInfo()->Type()].SizeX(), data->design[pos->GetBufferInfo()->Type()].SizeY(), Color_Red, false);
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


            HNet tempNet = data->design.Pins.Get<HPin::Net, HNet>(
                        data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(criticalPath.TimingPointStart()));
            additionNewElement->CreateNets(tempNet, newBuffer, newNet, data->vGTree->GetSource()->GetLeft(), newNetCount);
            delete [] newNet;
        }
    }

    if ((data->plotSteinerPoint) || (data->plotVGTree) || (data->plotNets))
    {
        data->design.Plotter->ShowPlacement();
    }

    return best;
}

int PathBasedBuffering::BufferingNetlist2()
{

    if (!isInitialize)   Initialize();
    if (data->design.CriticalPaths.Count() < 0)
        FindCriticalPaths(data->design);

    if (data->plotBuffer)
        data->design.Plotter->ShowPlacement();

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

    /*std::vector<HCriticalPath> paths(data->design.CriticalPaths.Count());
    int idx = 0;
    for(HCriticalPaths::Enumerator i = data->design.CriticalPaths.GetEnumerator(); i.MoveNext();)
    paths[idx++] = i;
    std::sort(paths.begin(), paths.end(), Utils::CriticalPathComparator(data->design));
    */
    CriticalPathBufferingList cpb(data->design);


    //    CriticalPathBuffering cpb(0, (paths[0],data->design).PointsCount(), (paths[0],data->design).GetEnumeratorW(), data->design);

    int countCP = 0;
    int ttt = cpb.paths.size();
    while ((!isBufferingFinish) && IsLimitationCountCriticalPathExecute(totalIndex) && (ind < cpb.paths.size()))
    {
        bufferCount = 0;
        if (IsAppropriateNumberOfPins(data, cpb.paths[ind]))
        {

            bool isBufferingNet = true;
            if (!data->isNetContainPrimaryPin) 
                if (data->design.Pins.GetBool<HPin::IsPrimary>(data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(cpb.paths[ind].TimingPointStart())) ||
                    data->design.Pins.GetBool<HPin::IsPrimary>(data->design.TimingPoints.Get<HTimingPoint::Pin, HPin>(cpb.paths[ind].TimingPointStart())))
                    isBufferingNet = false;

            /*if (data->IslimitationsCountRepeatNet())
            {
            int countRepeatNet = 0;
            int curRepeatNet = 0;
            for (HCriticalPath::PointsEnumeratorW point = (cpb.paths[ind],data->design).GetEnumeratorW(); point.MoveNext();)
            {
            HNetWrapper net = data->design[((point.TimingPoint(),data->design).Pin(),data->design).OriginalNet()];
            curRepeatNet = Utils::FindRepeat(net.Name(), data->textIdentifierBufferedNet);
            if (curRepeatNet > countRepeatNet) 
            countRepeatNet = curRepeatNet;
            if (countRepeatNet >= data->maxCountRepeatNet)
            isBufferingNet = false;
            }
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
            if (data->nameBufferingNet != "")
            {
                HPinWrapper firstPin = data->design[data->design[cpb.paths[ind].TimingPointStart()].Pin()];
                HPinWrapper lastPin = data->design[data->design[cpb.paths[ind].TimingPointStart()].Pin()];

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
                //PrintPath(data->design, paths[ind], ::ToID(paths[ind])); 
            }


            bufferCount = BufferingCriticalPath(cpb.paths[ind]).GetPositionCount();
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

                ind++;   
                countCP++;
            }
        }
        totalIndex++;
    }

    for (int i = 0; i < cpb.nets.size(); i++)
    {
        this->BufferingNet(cpb.nets[i]);
    }
    //this->BufferingCriticalPaths();

    if (data->plotBuffer)
    {
        data->design.Plotter->Refresh(HPlotter::WaitTime(data->plotterWaitTime));
        data->design.Plotter->ShowPlacement();
    }

    ALERT("Buffers inserted: %d", totalBufferCount);
    ALERT("Minimal TNS: %f", minTNS);
    ALERT("Minimal WNS: %f", minWNS);
    ALERT("Percent area compose buffers = %f", data->PercentAreaComposeBuffers());
    return totalBufferCount;
}

RemoveBuffer::RemoveBuffer(VGAlgorithmData* oldData)
{
    data = oldData;
}

RemoveBuffer::RemoveBuffer(HDesign& hd)
{
    data = new VGAlgorithmData(hd);
    data->Initialize();
}

RemoveBuffer::~RemoveBuffer()
{
    delete data;
}

void RemoveBuffer::RemoveNewBuffering()
{
    ALERT("Cells before remove = %d", data->design.Cells.PlaceableCellsCount());
    data->design.Plotter->ShowPlacement();
    int bufferCount = 0;
    int startNewPinIndex = data->design._Design.NetList.nPinsEnd;

    for (HCells::PlaceableCellsEnumeratorW pCell = data->design.Cells.GetPlaceableCellsEnumeratorW(); pCell.MoveNext(); )
    {
        for(unsigned int j = 0; j < data->Buffers.size(); j++)
        {
            if (data->design.Cells.Get<HCell::MacroType, HMacroType>(pCell) == data->Buffers[j].Type())
            {
                bufferCount++;
                for (HCell::PinsEnumeratorW pw = pCell.GetPinsEnumeratorW(); pw.MoveNext();)
                    if (::ToID(pw) < startNewPinIndex)
                        startNewPinIndex = ::ToID(pw);
            }
        }
    }

    int textIdentifierBufferedNetCount = 0;
    for (HNets::NetsEnumeratorW nw =  data->design.Nets.GetFullEnumeratorW(); nw.MoveNext(); )
    {
        if (Utils::FindRepeat(nw.Name(), data->textIdentifierBufferedNet) == 0)
            Utils::RestoreBufferedNet(data->design, nw);
        else
            textIdentifierBufferedNetCount++;
    }

    ALERT("Buffer count = %d", bufferCount);
    int newCellEnd = data->design._Design.NetList.nCellsEnd - bufferCount;
    for (int i = newCellEnd; i < data->design._Design.NetList.nCellsEnd; i++)
    {
        data->design._Design.NetList.cellHeight[i] = 0;
        data->design._Design.NetList.cellName[i].clear();
        data->design._Design.NetList.cellOrient[i] = Orientation_Default;
        data->design._Design.NetList.cellPinEndIdx[i] = 0;
        data->design._Design.NetList.cellPinStartIdx[i] = 0;
        data->design._Design.NetList.cellPlStatus[i] = PlacementStatus_Default;
        data->design._Design.NetList.cellType[i] = 0;
        data->design._Design.NetList.cellWidth[i] = 0;
        data->design._Design.NetList.cellX[i] = 0;
        data->design._Design.NetList.cellY[i] = 0;
    }
    data->design._Design.NetList.nCellsEnd -= bufferCount;
    data->design._Design.NetList.cellTypeBounds[MacroType_Last + 1].nFictive = 0;

    for (int k = startNewPinIndex; k < data->design._Design.NetList.nPinsEnd; k++)
    {
        data->design._Design.NetList.pinCellIdx[k] = 0;
        data->design._Design.NetList.pinDirection[k] = PinDirection_Default;
        //data->design._Design.NetList.pinName[k].clear();
        data->design._Design.NetList.pinNetIdx[k] = 0;
        data->design._Design.NetList.pinOffsetX[k] = 0;
        data->design._Design.NetList.pinOffsetY[k] = 0;
        data->design._Design.NetList.pinOriginalNetIdx[k] = 0;
        data->design._Design.NetList.pinType[k] = 0;
    }

    data->design._Design.NetList.nPinsEnd = startNewPinIndex;

    int newNetCount = data->design._Design.NetList.nNetsByKind[NetKind_Removed] + data->design._Design.NetList.nNetsByKind[NetKind_Buffered];
    ASSERT(newNetCount == textIdentifierBufferedNetCount);
    int newNetEnd = data->design._Design.NetList.nNetsEnd - newNetCount;
    if (newNetCount != 0)
    {
        for (int h = data->design._Design.NetList.netPinStart[newNetEnd]; h < data->design._Design.NetList.nNetPinEnd; h++)
            data->design._Design.NetList.netPins[h] = 0;

        data->design._Design.NetList.nNetPinEnd = data->design._Design.NetList.netPinStart[newNetEnd];
    }

    for (int l = newNetEnd; l < data->design._Design.NetList.nNetsEnd; l++)
    {
        data->design._Design.NetList.netHPWL[l] = 0;
        data->design._Design.NetList.netKind[l] = NetKind_Default;
        data->design._Design.NetList.netLnet[l] = 0;
        data->design._Design.NetList.netName[l].clear();
        data->design._Design.NetList.netPinEnd[l] = 0;
        data->design._Design.NetList.netPinStart[l] = 0;
        data->design._Design.NetList.netWeight[l] = 0;
    }

    data->design._Design.NetList.nNetsEnd = newNetEnd;
    data->design._Design.NetList.nNetsByKind[NetKind_Removed] = 0;
    data->design._Design.NetList.nNetsByKind[NetKind_Buffered] = 0;

    ALERT("Cells after remove = %d", data->design.Cells.PlaceableCellsCount());

    data->design.Plotter->ShowPlacement();
    STA(data->design);
    FindCriticalPaths(data->design);
    data->design.Plotter->ShowPlacement();
}
