#include "TimingPointMus.h"
#include "Utils.h"
#include "STA.h"
#include "Timing.h"

const int ONE_FOR_MUA_AND_ONE_FOR_MUR = 2;

int CountLeftArcs(HDesign& design, HNet net)
{
    HPin source = design[net].Source();
    HPinType sourceType = design.Get<HPin::Type, HPinType>(source);

    int i = 0;
    for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
        if (arc.TimingType() == TimingType_Combinational)
            i++;

    return i;
}

TimingPointMus::TimingPointMus(HDesign& design): MuS(0), MuIn(0)
{
    size = design._Design.NetList.nPinsLimit;
    ::Grow(&MuS, 0, size);
    ::Grow(&MuIn, 0, size);

    //init internal mus
    for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext() )
    {
        //WRITELINE("InitMus of %s", reporter->GetCellPinName(design, tp).c_str());
        InitPointMus(design, tp); 
        
        if (tp == design.TimingPoints.LastInternalPoint()) break;
    }
            
    //init mus on timing end points
    HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
    for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
        ep.GoPrevious() != endPointsEnd; )
    {
        //WRITELINE("InitInMus of %s", reporter->GetCellPinName(design, ep).c_str());
        double defaultMu = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.muLR", 0.99);
        MuIn[::ToID(ep)].resize(ONE_FOR_MUA_AND_ONE_FOR_MUR, defaultMu);
    }

    EnforceFlowProperty(design);

    reporter = new MuReporter(design);
}

TimingPointMus::~TimingPointMus()
{
    ::Grow(&MuS, size, 0);
    ::Grow(&MuIn, size, 0);
}

void TimingPointMus::EnforceArrivalFlowProperty(HDesign& design, HTimingPoint pt)
{
    double leftSum = SumInMuA(pt);
    double rightSum = GetMuS(pt) + SumOutMuA(design, pt);

    ScaleInMuA(pt, rightSum / leftSum);
}

void TimingPointMus::EnforceRequiredFlowProperty(HDesign& design, HTimingPoint pt)
{
    double leftSum = SumInMuR(pt) + GetMuS(pt);
    double rightSum =  SumOutMuR(design, pt);

    ScaleOutMuR(design, pt, leftSum / rightSum);
}

void TimingPointMus::EnforceFlowProperty(HDesign& design)
{
    for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext() )
    {
        //WRITELINE("EnforceRMus of %s", reporter->GetCellPinName(design, tp).c_str());
        EnforceRequiredFlowProperty(design, tp);

        if (tp == design.TimingPoints.LastInternalPoint()) break;
    }

    for (HTimingPointWrapper tp = design[design.TimingPoints.LastInternalPoint()];  !::IsNull(tp); tp.GoPrevious() )
    {
        //WRITELINE("EnforceAMus of %s", reporter->GetCellPinName(design, tp).c_str());
        EnforceArrivalFlowProperty(design, tp);

        if (tp == design.TimingPoints.FirstInternalPoint()) break;
    }
}

double TimingPointMus::SumInMuA(HTimingPoint pt)
{
    double sum = 0.0;
    for (int i = 0; i < GetMuInCount(pt); i++)
    {
        sum += GetInMuA(pt, i);
    }
    return sum;
}

double TimingPointMus::SumInMuR(HTimingPoint pt )
{
    double sum = 0.0;
    for (int i = 0; i < GetMuInCount(pt); i++)
    {
        sum += GetInMuR(pt, i);
    }
    return sum;
}

void TimingPointMus::GetMuIn(HDesign& design, HTimingPoint pt, std::vector<double>& inMus)
{
    inMus.clear();
    for (int i = 0; i < GetMuInCount(pt); i++)
    {
        inMus.push_back(GetInMuA(pt, i) + GetInMuR(pt, i));
    }
}

void TimingPointMus::GetMuOut(HDesign& design, HTimingPoint pt, std::vector<double>& muOutVector)
{
    GetMuOutVector muOut(muOutVector);
    IterateOutMu(design, pt, muOut);
}

double TimingPointMus::SumOutMuA(HDesign& design, HTimingPoint pt)
{
    AccumulateMu acc(true);
    IterateOutMu(design, pt, acc);
    return acc.sum;
}

double TimingPointMus::SumOutMuR(HDesign& design, HTimingPoint pt)
{
    AccumulateMu acc(false);
    IterateOutMu(design, pt, acc);
    return acc.sum;
}

void TimingPointMus::ScaleOutMuR(HDesign& design, HTimingPoint pt, double multiplier)
{
    IterateOutMu(design, pt, ScaleMuR(multiplier));
}

void TimingPointMus::ScaleInMuA(HTimingPoint pt, double multiplier)
{
    for (int i = 0; i < GetMuInCount(pt); i++)
    {
        SetMuInA(pt, i, GetInMuA(pt, i) * multiplier);
    }
}

template<class Action>
void TimingPointMus::IterateOutMu(HDesign& design, HTimingPoint pt, Action& todo)
{
    HPin pin = design.Get<HTimingPoint::Pin, HPin>(pt);

    if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT)
    {//cell output pin, iterate net arcs
        for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(design.Get<HPin::Net, HNet>(pin));
            sink.MoveNext();)
        {
            todo(*this, design.TimingPoints[sink], 0);
        }
    }
    else
    {//cell input pin, iterate cell arcs
        HCell cell = design.Get<HPin::Cell, HCell>(pin);

        for (HCell::PinsEnumeratorW p = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); p.MoveNext(); )
        {
            if (p.Direction() != PinDirection_OUTPUT || ::IsNull(p.Net())) 
                continue;

            HTimingPoint point = design.TimingPoints[p];
            int idx = 0;
            for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(p.Type());
                arc.MoveNext();)
            {
                if (arc.TimingType() == TimingType_Combinational)
                {
                    if (arc.GetStartPin(p) == pin)
                        todo(*this, point, idx);
                    idx++;
                }
            }
        }
    }
}

void TimingPointMus::InitPointMus(HDesign& design, HTimingPoint pt)
{
    double defaultMu = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.muLR", 0.99);

    SetMuS(pt, defaultMu);

    HPin pin = design.Get<HTimingPoint::Pin, HPin>(pt);
    if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
    {
        MuIn[::ToID(pt)].resize(ONE_FOR_MUA_AND_ONE_FOR_MUR, defaultMu);
    }
    else
    {
        if (!design.GetBool<HPin::IsPrimary>(pin))
            MuIn[::ToID(pt)].resize(ONE_FOR_MUA_AND_ONE_FOR_MUR * 
            CountLeftArcs(design, design.Get<HPin::Net, HNet>(pin)), defaultMu);
    }
}

int nIncreased = 0;
int nDecreased = 0;

double TimingPointMus::CalculateFactor(double slack, double theta) 
{
    double minFactor = 0.01;
    double referenceValue = 100;
    double factor = 0.0;

    double r = (-slack + referenceValue) / referenceValue;

    if (r < 1.0)
    {
        factor = pow(r, theta);
        nDecreased++;
    }
    else
    {
        factor = 2.0 - 1.0 / pow(r, theta);
        nIncreased++;
    }
    factor = minFactor + (1.0 - minFactor) * factor;	
    
    return factor;
}

void TimingPointMus::UpdateMuS(HDesign& design, double theta)
{
    for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; 
        tp.GoNext() != design.TimingPoints.LastInternalPoint(); )
    {
        double factor = CalculateFactor(tp.Slack(), theta);
        SetMuS(tp, GetMuS(tp) * factor);
        //HPinWrapper pin = design[tp.Pin()];
        //WRITELINE("%10s %f %f", pin.Name().c_str(), factor, GetMuS(tp));
    }
}

void TimingPointMus::UpdateMuAOnTEP(HDesign& design, double theta)
{
    HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
    for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
        ep.GoPrevious() != endPointsEnd; )
    {
        //WRITELINE("UpdateMuAOnPO of %s", reporter->GetCellPinName(design, ep).c_str());
        double factor = CalculateFactor(ep.Slack(), theta);
        SetMuInA(ep, 0, GetInMuA(ep, 0) * factor);
    }
}

void TimingPointMus::UpdateMuROnTSP(HDesign& design, double theta)
{
    HTimingPoint startPointsEnd = design.TimingPoints.FirstInternalPoint();
    for (HTimingPointWrapper sp = design[design.TimingPoints.TopologicalOrderRoot()];
        sp.GoNext() != startPointsEnd; )
    {
        HTimingPointWrapper tp = design[sp];
        
        //int sz = GetMuOutCount(design, tp);
        //ALERT("MuOutCount = %d", sz);
    }
}

//void TimingPointMus::UpdateMuAMuROnPrimaries(HDesign& design, double theta)
//{
//    for (HPins::PrimariesEnumeratorW pin = design.Pins.GetEnumeratorW(); pin.MoveNext(); )
//    {
//        HTimingPointWrapper tp = design[design.TimingPoints[pin]];
//        double factor = CalculateFactor(tp.Slack(), theta);
//
//        if (pin.IsPrimaryInput())
//            SetMuInR(tp, 0, GetInMuR(tp, 0) * factor);
//            
//        else
//            SetMuInA(tp, 0, GetInMuA(tp, 0) * factor);
//    }
//}

//void TimingPointMus::UpdateLocalMuAMuR(HDesign& design, double theta)
//{
//    for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; 
//        tp.GoNext() != design.TimingPoints.LastInternalPoint(); )
//    {
//
//        HPin pin = tp.Pin();
//
//        if (pin.Direction == PinDirection::PinDirection_INPUT)
//        {//cell input pin, single input arc (do not update), possible several output arcs (update)
//            double targetSlack = 0.0;
//
//
//        }
//        else
//        {//cell output pin, possible several input and output arcs
//
//        }
//
//
//        double targetSlackA = 0.0;
//        
//
//        tp.ArrivalAncestor()
//        tp.
//
//        if (tp.IsPrimary())
//            continue;
//
//        GetMuInCount(tp)
//
//        double factor = CalculateFactor(tp.Slack(), theta);
//        SetMuS(tp, GetMuS(tp) * factor);
//    }
//
//    for (HPins::PrimariesEnumeratorW pin = design.Pins.GetEnumeratorW(); pin.MoveNext(); )
//    {
//        HTimingPoint tp = design.TimingPoints[pin];
//        double factor = CalculateFactor(tp.Slack(), theta);
//
//        if (pin.IsPrimaryInput())
//            SetMuInA(tp, 0, GetInMuA(tp, 0) * factor);
//        else
//            SetMuInR(tp, 0, GetInMuR(tp, 0) * factor);
//    }
//}

void TimingPointMus::UpdateMus(HDesign& design)
{
    ALERT("Mu Updating");

    STA(design, false);

    double theta = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.theta", 2.0);
    
    reporter->Report(design, this, "BEFORE UPDATING");

    UpdateMuS(design, theta);
    reporter->Report(design, this, "UpdateMuS");
    
    UpdateMuAOnTEP(design, theta);
    reporter->Report(design, this, "UpdateMuAOnTEP");
    
    UpdateMuROnTSP(design, theta);
    reporter->Report(design, this, "UpdateMuROnTSP");
    
    //UpdateLocalMuAMuR(design, theta);
    reporter->Report(design, this, "AFTER UPDATING");

    EnforceFlowProperty(design);

    reporter->Report(design, this, "AFTER ENFORCING");

    ALERT("  Total: %d", nIncreased + nDecreased);
    ALERT("  Increased: %d", nIncreased);
    ALERT("  Decreased: %d", nDecreased);    
}

void TimingPointMus::GetNetMus(HDesign& design, HNet net, 
                               std::vector<double>& cellArcMus, 
                               std::vector<double>& netArcMus)
{
    HPin sourcePin = design[net].Source();
    HTimingPoint point = design.TimingPoints[sourcePin];

    GetMuIn(design, point, cellArcMus);
    GetMuOut(design, point, netArcMus);
}