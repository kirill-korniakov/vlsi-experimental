#ifndef __DELAY_PROPAGATION_H__
#define __DELAY_PROPAGATION_H__

template<class memberType>
struct RFPair
{
    memberType fall;
    memberType rise;

    RFPair() {}
    RFPair(const memberType& value): fall(value), rise(value) {}
    RFPair(const memberType& f, const memberType& r): fall(f), rise(r){}

    RFPair& operator=(const memberType& value)
    {
        fall = rise = value;
        return *this;
    }
};

typedef double Float1D;
typedef RFPair<double> Float2D;

typedef bool Bool1D;
typedef RFPair<bool> Bool2D;

typedef HTimingPoint TimingPoint1D;
typedef RFPair<HTimingPoint> TimingPoint2D;

template<int SignalDirectionsNumber>
class DelayPropagation;

template<int sdNum>
void CalcArrivalTimeOnOutputPin(HDesign& design, HPin pin, DelayPropagation<sdNum>& dp);

template<int sdNum>
void CalcRequiredTimeOnInputPins(HDesign& design, HPin pin, DelayPropagation<sdNum>& dp);

template<>
class DelayPropagation<2>
{
    HDesign& design;

public:
    typedef Float2D CapacitanceType;
    typedef Float2D TimeType;
    typedef Bool2D BoolType;
    typedef TimingPoint2D TimingPoint;
    std::vector<TimeType> checkedArrivals;
    std::vector<TimeType> checkedRequireds;

    DelayPropagation(HDesign& hd): design(hd) {}

    CapacitanceType GetObservedC(HSteinerPoint point)
    {
        return CapacitanceType(
            design.GetDouble<HSteinerPoint::FallObservedC>(point),
            design.GetDouble<HSteinerPoint::RiseObservedC>(point));
    }

    TimeType GetArrivalTime(HTimingPoint point)
    {
        return TimeType(
            design.GetDouble<HTimingPoint::FallArrivalTime>(point),
            design.GetDouble<HTimingPoint::RiseArrivalTime>(point));
    }

    TimeType GetRequiredTime(HTimingPoint point)
    {
        return TimeType(
            design.GetDouble<HTimingPoint::FallRequiredTime>(point),
            design.GetDouble<HTimingPoint::RiseRequiredTime>(point));
    }

    BoolType GetSignalInversions(TimingSense arcSense, const TimeType& time)
    {
        switch (arcSense)
        {
        case TimingSense_PositiveUnate:
            return BoolType(false);
        case TimingSense_NegativeUnate:
            return BoolType(true);
        case TimingSense_NonUnate:
            if (time.rise > time.fall)
                return BoolType(true, false);
            else if (time.rise < time.fall)
                return BoolType(false, true);
            return BoolType(false);
        default:
            GLOGERROR(LOGINPLACE, "Unknown timing sense: %d", arcSense);
            return BoolType(false);
        }//switch (arc.TimingSense())
    }

    TimeType GetArcOutputTimeArrival(const HTimingArcTypeWrapper& arc,
        const CapacitanceType& observedC,
        const TimeType& inputTime,
        BoolType& inversed)
    {
        inversed = GetSignalInversions(arc.TimingSense(), inputTime);

        return TimeType(
            arc.TIntrinsicFall() + arc.ResistanceFall() * observedC.fall
            + (inversed.fall ? inputTime.rise : inputTime.fall),
            arc.TIntrinsicRise() + arc.ResistanceRise() * observedC.rise
            + (inversed.rise ? inputTime.fall : inputTime.rise));
    }

    TimeType GetArcOutputTimeRequired(const HTimingArcTypeWrapper& arc,
        const CapacitanceType& observedC,
        const TimeType& inputTime,
        BoolType& inversed)
    {
        TimeType time(
            arc.TIntrinsicFall() + arc.ResistanceFall() * observedC.fall - inputTime.fall,
            arc.TIntrinsicRise() + arc.ResistanceRise() * observedC.rise - inputTime.rise);

        inversed = GetSignalInversions(arc.TimingSense(), time);

        return TimeType (
            inversed.fall ? -time.rise : - time.fall,
            inversed.rise ? -time.fall : - time.rise);
    }

    void AccumulateWorstArrivalInFirst(
        TimeType& wTime, BoolType& wInversed, TimingPoint& wPoint,
        const TimeType& time, const BoolType& inversed, HTimingPoint point)
    {
        if (time.fall > wTime.fall)
        {
            wTime.fall = time.fall;
            wPoint.fall = point;
            wInversed.fall = inversed.fall;
        }
        if (time.rise > wTime.rise)
        {
            wTime.rise = time.rise;
            wPoint.rise = point;
            wInversed.rise = inversed.rise;
        }

        checkedArrivals.push_back(time);
    }

    void SetArrivalTime(HTimingPoint point, const TimeType& time)
    {
        design.Set<HTimingPoint::FallArrivalTime>(point, time.fall);
        design.Set<HTimingPoint::RiseArrivalTime>(point, time.rise);
    }

    void SetRequiredTime(HTimingPoint point, const TimeType& time)
    {
        design.Set<HTimingPoint::FallRequiredTime>(point, time.fall);
        design.Set<HTimingPoint::RiseRequiredTime>(point, time.rise);
    }

    void SetArrivalAncestor(HTimingPoint point, const TimingPoint& anc)
    {
        design.Set<HTimingPoint::FallArrivalAncestor>(point, anc.fall);
        design.Set<HTimingPoint::RiseArrivalAncestor>(point, anc.rise);
    }

    void SetArrivalInversion(HTimingPoint point, const BoolType& inv)
    {
        design.Set<HTimingPoint::IsFallArrivalInversed>(point, inv.fall);
        design.Set<HTimingPoint::IsRiseArrivalInversed>(point, inv.rise);
    }

    void AccumulateWorstRequiredOnPin(HTimingPoint targetPoint,
        const TimeType& time, const BoolType& inversed, HTimingPoint point)
    {
        if (time.fall < design.GetDouble<HTimingPoint::FallRequiredTime>(targetPoint))
        {
            design.Set<HTimingPoint::FallRequiredTime>(targetPoint, time.fall);
            design.Set<HTimingPoint::FallRequiredAncestor>(targetPoint, point);
            design.Set<HTimingPoint::IsFallRequiredInversed>(targetPoint, inversed.fall);
        }
        if (time.rise < design.GetDouble<HTimingPoint::RiseRequiredTime>(targetPoint))
        {
            design.Set<HTimingPoint::RiseRequiredTime>(targetPoint, time.rise);
            design.Set<HTimingPoint::RiseRequiredAncestor>(targetPoint, point);
            design.Set<HTimingPoint::IsRiseRequiredInversed>(targetPoint, inversed.rise);
        }

        checkedRequireds.push_back(time);
    }

    TimeType GetSinkArrivalTime(HTimingPoint source, HSteinerPoint sink)
    {
        return TimeType(
            design.GetDouble<HTimingPoint::FallArrivalTime>(source) + design.GetDouble<HSteinerPoint::FallPathDelay>(sink),
            design.GetDouble<HTimingPoint::RiseArrivalTime>(source) + design.GetDouble<HSteinerPoint::RisePathDelay>(sink));
    }

    TimeType GetSourceRequiredTime(HTimingPoint sink)
    {
        HSteinerPoint point = design.SteinerPoints[design.Get<HTimingPoint::Pin, HPin>(sink)];
        return TimeType(
            design.GetDouble<HTimingPoint::FallRequiredTime>(sink) - design.GetDouble<HSteinerPoint::FallPathDelay>(point),
            design.GetDouble<HTimingPoint::RiseRequiredTime>(sink) - design.GetDouble<HSteinerPoint::RisePathDelay>(point));
    }

    void AccumulateSourceRequiredTime(HTimingPoint source, HTimingPoint sink, const TimeType& time)
    {
        if (design.GetDouble<HTimingPoint::FallRequiredTime>(source) > time.fall)
        {
            design.Set<HTimingPoint::FallRequiredTime>(source, time.fall);
            design.Set<HTimingPoint::FallRequiredAncestor>(source, sink);
        }
        if (design.GetDouble<HTimingPoint::RiseRequiredTime>(source) > time.rise)
        {
            design.Set<HTimingPoint::RiseRequiredTime>(source, time.rise);
            design.Set<HTimingPoint::RiseRequiredAncestor>(source, sink);
        }
    }

    TimeType GetEdgeArcOutputTime(const HTimingArcTypeWrapper& arc, const HTimingPointWrapper& sp, HSteinerPoint spst)
    {
        return TimeType(
            sp.FallArrivalTime() + arc.TIntrinsicFall()
            + arc.ResistanceFall() * design.GetDouble<HSteinerPoint::FallObservedC>(spst),
            sp.RiseArrivalTime() + arc.TIntrinsicRise()
            + arc.ResistanceRise() * design.GetDouble<HSteinerPoint::RiseObservedC>(spst));
    }

    TimeType GetSetupArcOutputTime(const HTimingArcTypeWrapper& arc, const HTimingPointWrapper& ep)
    {
        return TimeType(
            ep.FallRequiredTime() - arc.TIntrinsicFall(),
            ep.RiseRequiredTime() - arc.TIntrinsicRise());
    }
};

template<>
class DelayPropagation<1>
{
    HDesign& design;

public:
    typedef Float1D CapacitanceType;
    typedef Float1D TimeType;
    typedef Bool1D BoolType;
    typedef TimingPoint1D TimingPoint;
    std::vector<TimeType> checkedArrivals;
    std::vector<TimeType> checkedRequireds;

    DelayPropagation(HDesign& hd): design(hd) {}

    CapacitanceType GetObservedC(HSteinerPoint point)
    {
        return design.GetDouble<HSteinerPoint::ObservedC>(point);
    }

    TimeType GetArrivalTime(HTimingPoint point)
    {
        return design.GetDouble<HTimingPoint::ArrivalTime>(point);
    }

    TimeType GetRequiredTime(HTimingPoint point)
    {
        return design.GetDouble<HTimingPoint::RequiredTime>(point);
    }

    BoolType GetSignalInversions(TimingSense arcSense, const TimeType& time)
    {
        switch (arcSense)
        {
        case TimingSense_PositiveUnate:
        case TimingSense_NegativeUnate:
        case TimingSense_NonUnate:
            return BoolType(false);
        default:
            GLOGERROR(LOGINPLACE, "Unknown timing sense: %d", arcSense);
            return BoolType(false);
        }//switch (arc.TimingSense())
    }

    TimeType GetArcOutputTimeArrival(const HTimingArcTypeWrapper& arc,
        const CapacitanceType& observedC,
        const TimeType& inputTime,
        BoolType& inversed)
    {
        inversed = GetSignalInversions(arc.TimingSense(), inputTime);

        return arc.TIntrinsicRise() + arc.ResistanceRise() * observedC + inputTime;
    }

    TimeType GetArcOutputTimeRequired(const HTimingArcTypeWrapper& arc,
        const CapacitanceType& observedC,
        const TimeType& inputTime,
        BoolType& inversed)
    {
        inversed = GetSignalInversions(arc.TimingSense(), inputTime);
        return inputTime - arc.TIntrinsicRise() - arc.ResistanceRise() * observedC;
    }

    void AccumulateWorstArrivalInFirst(
        TimeType& wTime, BoolType& wInversed, TimingPoint& wPoint,
        const TimeType& time, const BoolType& inversed, HTimingPoint point)
    {
        if (time > wTime)
        {
            wTime = time;
            wPoint = point;
            wInversed = inversed;
        }

        checkedArrivals.push_back(time);
    }

    void SetArrivalTime(HTimingPoint point, const TimeType& time)
    {
        design.Set<HTimingPoint::ArrivalTime>(point, time);
    }

    void SetRequiredTime(HTimingPoint point, const TimeType& time)
    {
        design.Set<HTimingPoint::RequiredTime>(point, time);
    }

    void SetArrivalAncestor(HTimingPoint point, const TimingPoint& anc)
    {
        design.Set<HTimingPoint::ArrivalAncestor>(point, anc);
    }

    void SetArrivalInversion(HTimingPoint point, const BoolType& inv)
    {
    }

    void AccumulateWorstRequiredOnPin(HTimingPoint targetPoint,
        const TimeType& time, const BoolType& inversed, HTimingPoint point)
    {
        if (time < design.GetDouble<HTimingPoint::RequiredTime>(targetPoint))
        {
            design.Set<HTimingPoint::RequiredTime>(targetPoint, time);
            design.Set<HTimingPoint::RequiredAncestor>(targetPoint, point);
        }

        checkedRequireds.push_back(time);
    }

    TimeType GetSinkArrivalTime(HTimingPoint source, HSteinerPoint sink)
    {
        return design.GetDouble<HTimingPoint::ArrivalTime>(source) + design.GetDouble<HSteinerPoint::PathDelay>(sink);
    }

    TimeType GetSourceRequiredTime(HTimingPoint sink)
    {
        HSteinerPoint point = design.SteinerPoints[design.Get<HTimingPoint::Pin, HPin>(sink)];
        return design.GetDouble<HTimingPoint::RequiredTime>(sink) - design.GetDouble<HSteinerPoint::PathDelay>(point);
    }

    void AccumulateSourceRequiredTime(HTimingPoint source, HTimingPoint sink, const TimeType& time)
    {
        if (design.GetDouble<HTimingPoint::RequiredTime>(source) > time)
        {
            design.Set<HTimingPoint::RequiredTime>(source, time);
            design.Set<HTimingPoint::RequiredAncestor>(source, sink);
        }
    }

    TimeType GetEdgeArcOutputTime(const HTimingArcTypeWrapper& arc, const HTimingPointWrapper& sp, HSteinerPoint spst)
    {
        return sp.ArrivalTime() + arc.TIntrinsicRise()
            + arc.ResistanceRise() * design.GetDouble<HSteinerPoint::ObservedC>(spst);
    }

    TimeType GetSetupArcOutputTime(const HTimingArcTypeWrapper& arc, const HTimingPointWrapper& ep)
    {
        return ep.RequiredTime() - arc.TIntrinsicRise();
    }
};

#endif