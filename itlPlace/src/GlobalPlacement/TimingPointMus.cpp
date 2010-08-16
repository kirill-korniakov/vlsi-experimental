#include "TimingPointMus.h"
#include "Utils.h"
#include "STA.h"
#include "Timing.h"
#include "Reporting.h"

#include "float.h"
#include <vector>

using namespace std;

const int ONE_FOR_MUA_AND_ONE_FOR_MUR = 2;
const int TIMING_MODEL = 1;
const double MAX_SLACK = 1000.0;

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

void TimingPointMus::InitMuSOnTSP(HDesign& design, double defaultMu)
{
    HTimingPoint startPointsEnd = design.TimingPoints.FirstInternalPoint();
    for (HTimingPointWrapper sp = design[design.TimingPoints.TopologicalOrderRoot()];
        sp.GoNext() != startPointsEnd; )
    {
        //WRITELINE("InitMuS at %s", reporter->GetCellPinName(design, sp).c_str());
        SetMuS(sp, defaultMu);
    }
}

void TimingPointMus::InitMusInternalAndTEP(HDesign& design, double defaultMu) 
{
    for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(tp); tp.GoNext() )
    {
        //WRITELINE("InitMus at %s", reporter->GetCellPinName(design, tp).c_str());
        InitPointMus(design, tp, defaultMu); 
    }
}

TimingPointMus::TimingPointMus(HDesign& design): MuS(0), MuIn(0), reporter(new MuReporter(design))
{
    size = design._Design.NetList.nPinsLimit;
    ::Grow(&MuS, 0, size);
    ::Grow(&MuIn, 0, size);

    double defaultMu = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.muLR", 0.99);

    minFactor = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.minFactor", 0.01);
    referenceValue = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.referenceValue", 100);
    nIncreased = 0;
    nDecreased = 0;

    InitMuSOnTSP(design, defaultMu);
    InitMusInternalAndTEP(design, defaultMu);
    EnforceFlowProperty(design);
    reporter->Report(design, this, "AFTER INITIAL ASSIGNMENT");
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
    //go from TSP to the LastInternalPoint because we propagate muR to the right
    for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; !::IsNull(tp.GoNext()); )
    {
        //WRITELINE("EnforceRMus of %s", reporter->GetCellPinName(design, tp).c_str());
        EnforceRequiredFlowProperty(design, tp);

        if (tp == design.TimingPoints.LastInternalPoint()) break;
    }

    //go from TED to the FirstInternalPoint because we propagate muA to the left
    for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()];  !::IsNull(tp.GoPrevious()); )
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

void TimingPointMus::ScaleOutMuRByIndex(HDesign& design, HTimingPoint pt, double multiplier, int index)
{
    IterateOutMu(design, pt, ScaleMuR(multiplier, index));
}

void TimingPointMus::ScaleOutMuR(HDesign& design, HTimingPoint pt, double multiplier)
{
    IterateOutMu(design, pt, ScaleMuR(multiplier));
}

void TimingPointMus::ScaleInMuA(HTimingPoint pt, double multiplier)
{
    for (int i = 0; i < GetMuInCount(pt); i++)
    {
        SetInMuA(pt, i, GetInMuA(pt, i) * multiplier);
    }
}

template<class Action>
void TimingPointMus::IterateOutMu(HDesign& design, HTimingPoint pt, Action& todo)
{
    HPin _pin = design.Get<HTimingPoint::Pin, HPin>(pt);

    if (design.Get<HPin::Direction, PinDirection>(_pin) == PinDirection_OUTPUT)
    {//cell output pin, iterate net arcs
        for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(design.Get<HPin::Net, HNet>(_pin));
            sink.MoveNext();)
        {
            todo(*this, design.TimingPoints[sink], 0);
        }
    }
    else
    {//cell input pin, iterate cell arcs
        HCell cell = design.Get<HPin::Cell, HCell>(_pin);

        for (HCell::PinsEnumeratorW pin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); pin.MoveNext(); )
        {
            if (pin.Direction() != PinDirection_OUTPUT || ::IsNull(pin.Net())) 
                continue;

            HTimingPoint point = design.TimingPoints[pin];
            int idx = 0;
            for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(pin.Type());
                arc.MoveNext();)
            {
                if (arc.TimingType() == TimingType_Combinational)
                {
                    if (arc.GetStartPin(pin) == _pin)
                        todo(*this, point, idx);
                    idx++;
                }
            }
        }
    }
}

void TimingPointMus::InitPointMus(HDesign& design, HTimingPoint pt, double defaultMu)
{
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

double TimingPointMus::CalculateLocalFactor(double slack, double theta) 
{
    double factor = 0.0;

    if (slack > MAX_SLACK)
    {
        return minFactor;
    }

    double r = (-slack + referenceValue) / referenceValue;
    factor = pow(r, theta);
    factor = minFactor + (1.0 - minFactor) * factor;	

    return factor;
}

double TimingPointMus::CalculateInjectionFactor(double slack, double theta) 
{
    double factor = 0.0;

    if (slack > MAX_SLACK)
    {
        return minFactor;
    }

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
    for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; !IsNull(tp.GoNext()); )
    {
        double factor = CalculateInjectionFactor(tp.Slack(), theta);
        SetMuS(tp, GetMuS(tp) * factor);
    }
}

void TimingPointMus::UpdateLocalMuA(HDesign& design, double theta)
{
    for (HTimingPointWrapper tp = design[design.TimingPoints.FirstInternalPoint()]; !IsNull(tp.GoNext()); )
    {
        size_t nMuIn = GetMuInCount(tp);

        if (nMuIn == 1) continue; //we don't have to estimate criticality of a single arc

        HPin pin = tp.Pin();
        ASSERT(IsOutputPin(design, pin));

        //obtain input slacks
        DelayPropagation<TIMING_MODEL> dp(design);
        CalcArrivalTimeOnOutputPin(design, pin, dp);
        ASSERT(nMuIn == dp.checkedArrivals.size());

        //calculate local slacks
        vector<double> localSlacks;
        double reqTime = tp.RequiredTime();
        for (size_t i = 0; i < nMuIn; i++)
        {
            localSlacks.push_back(reqTime - dp.checkedArrivals[i]);
        }

        //find minimum slack
        double minSlack = DBL_MAX;
        for (size_t i = 0; i < nMuIn; i++)
        {
            if (minSlack > localSlacks[i])
                minSlack = localSlacks[i];
        }

        //shift slacks
        for (size_t i = 0; i < nMuIn; i++)
        {
            localSlacks[i] -= minSlack;
        }

        //in a loop scale the input muA according to their criticality
        for (size_t i = 0; i < nMuIn; i++)
        {
            double factor = CalculateLocalFactor(localSlacks[i], theta);
            SetInMuA(tp, i, factor * GetInMuA(tp, i));
        }

        if (tp == design.TimingPoints.LastInternalPoint()) break;
    }
}

void TimingPointMus::UpdateLocalMuR(HDesign& design, double theta)
{
    for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; !IsNull(tp.GoNext()); )
    {
        size_t nMuOut = GetMuOutCount(design, tp);
        vector<double> localSlacks;
        double arrTime = tp.ArrivalTime();

        if (nMuOut == 1) continue; //we don't have to estimate criticality of a single arc

        //obtain output slacks
        HPin pin = tp.Pin();
        GetCellPinName(design, tp);

        if (IsInputPin(design, pin))
        {//this is a cell input pin
            //collect required times
            DelayPropagation<TIMING_MODEL> dp(design);
            CollectRequireds<TIMING_MODEL> collector(design, dp, tp);
            IterateOutMu(design, tp, collector);
            ASSERT(nMuOut == dp.checkedRequireds.size());

            //calculate local slacks
            for (size_t i = 0; i < nMuOut; i++)
            {
                localSlacks.push_back(dp.checkedRequireds[i] - arrTime);
            }
        }
        else
        {//this is a net source
            //collect required times
            HNet net = design.Get<HPin::Net, HNet>(pin);
            HPin sourcePin = design.Get<HNet::Source, HPin>(net);
            ASSERT(sourcePin == pin);

            vector<double> requiredTimes;
            for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(net); sink.MoveNext(); )
            {
                HTimingPointWrapper tp = design[design.TimingPoints[sink]];
                HSteinerPointWrapper stP = design[design.SteinerPoints[sink]];
                requiredTimes.push_back(tp.RequiredTime() - stP.PathDelay());
            }
            ASSERT(nMuOut == requiredTimes.size());

            //calculate local slacks
            for (size_t i = 0; i < nMuOut; i++)
            {
                localSlacks.push_back(requiredTimes[i] - arrTime);
            }
        }

        //find minimum slack
        double minSlack = DBL_MAX;
        for (size_t i = 0; i < nMuOut; i++)
        {
            if (minSlack > localSlacks[i])
                minSlack = localSlacks[i];
        }

        //shift slacks
        for (size_t i = 0; i < nMuOut; i++)
        {
            localSlacks[i] -= minSlack;
        }

        //in a loop scale the input muA according to their criticality
        for (size_t i = 0; i < nMuOut; i++)
        {
            double factor = CalculateLocalFactor(localSlacks[i], theta);
            ScaleOutMuRByIndex(design, tp, factor, i);
        }

        if (tp == design.TimingPoints.LastInternalPoint()) break;
    }
}

void TimingPointMus::UpdateMus(HDesign& design)
{
    ALERT("Mu Updating");
    
    nIncreased = 0;
    nDecreased = 0;

    STA(design, false);
    
    double theta = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.theta", 2.0);

    UpdateMuS(design, theta);
    //reporter->Report(design, this, "UpdateMuS");    
    UpdateLocalMuA(design, theta);
    //reporter->Report(design, this, "UpdateLocalMuA");
    UpdateLocalMuR(design, theta);
    //reporter->Report(design, this, "UpdateLocalMuR");

    EnforceFlowProperty(design);
    reporter->Report(design, this, "UPDATED MUS");

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