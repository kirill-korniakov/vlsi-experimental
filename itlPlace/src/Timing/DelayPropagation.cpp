#include "Timing.h"
#include <float.h>

void GetArrivalRisingArc(HDesign& hd,
                  HTimingPoint startPoint,
                  HTimingPoint endPoint,
                  HTimingArcType* risingArc,
                  double* arcRiseTime,
                  bool* isInversed)
{
  HPin pin = hd.Get<HTimingPoint::Pin, HPin>(endPoint);
  HPinType ptype = hd.Get<HPin::Type, HPinType>(pin);
  if (hd.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
  {
    HPin start_pin = hd.Get<HTimingPoint::Pin, HPin>(startPoint);
    HSteinerPoint endStPoint = hd.SteinerPoints[pin];
    double riseObservedC = hd.GetDouble<HSteinerPoint::RiseObservedC>(endStPoint);
    double worstRiseTime = -DBL_MAX;
    bool found = false;
    for (HPinType::ArcsEnumeratorW arc = hd.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(ptype);
      arc.MoveNext(); )
    {
      if (arc.TimingType() == TimingType_Combinational
        && arc.GetStartPin(pin) == start_pin)
      {
        found = true;
        double riseTime;
        bool isSignalInversed;

        switch (arc.TimingSense())
        {
        case TimingSense_PositiveUnate:
          {
            riseTime = hd.GetDouble<HTimingPoint::RiseArrivalTime>(startPoint)
              + arc.TIntrinsicRise() 
              + arc.ResistanceRise() * riseObservedC;
            isSignalInversed = false;
          }
          break;
        case TimingSense_NegativeUnate:
          {
            riseTime = hd.GetDouble<HTimingPoint::FallArrivalTime>(startPoint)
              + arc.TIntrinsicRise() 
              + arc.ResistanceRise() * riseObservedC;
            isSignalInversed = true;
          }
          break;
        case TimingSense_NonUnate:
          {
            double pinArrival1 = hd.GetDouble<HTimingPoint::RiseArrivalTime>(startPoint);
            double pinArrival2 = hd.GetDouble<HTimingPoint::FallArrivalTime>(startPoint);
            if (pinArrival1 >= pinArrival2)
            {
              riseTime = pinArrival1
                + arc.TIntrinsicRise() 
                + arc.ResistanceRise() * riseObservedC;
              isSignalInversed = false;
            }
            else
            {
              riseTime = pinArrival2
                + arc.TIntrinsicRise() 
                + arc.ResistanceRise() * riseObservedC;
              isSignalInversed = true;
            }
          }
          break;
        default:
          LOGERRORFORMAT(("Unknown timing sense: %d", arc.TimingSense()));
          break;
        }//switch (arc.TimingSense())
        if (riseTime > worstRiseTime)
        {
          worstRiseTime = riseTime;
          *risingArc = arc;
          *isInversed = isSignalInversed;
        }
      }
    }
    
    if(found)
    {
      *arcRiseTime = hd.GetDouble<HTimingArcType::TIntrinsicRise>(*risingArc)
        + hd.GetDouble<HTimingArcType::ResistanceRise>(*risingArc) * riseObservedC;
    }
    else
    {
      *arcRiseTime = 0;
      *isInversed = false;
      *risingArc = hd.TimingArcTypes.Null();
    }
  }
  else
  {
    *arcRiseTime = 0;
    *isInversed = false;
    *risingArc = hd.TimingArcTypes.Null();
  }
}

void GetArrivalFallingArc(HDesign& hd,
                         HTimingPoint startPoint,
                         HTimingPoint endPoint,
                         HTimingArcType* fallingArc,
                         double* arcFallTime,
                         bool* isInversed)
{
  HPin pin = hd.Get<HTimingPoint::Pin, HPin>(endPoint);
  HPinType ptype = hd.Get<HPin::Type, HPinType>(pin);
  if (hd.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
  {
    HPin start_pin = hd.Get<HTimingPoint::Pin, HPin>(startPoint);
    HSteinerPoint endStPoint = hd.SteinerPoints[pin];
    double fallObservedC = hd.GetDouble<HSteinerPoint::FallObservedC>(endStPoint);
    double worstFallTime = -DBL_MAX;
    bool found = false;
    for (HPinType::ArcsEnumeratorW arc = hd.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(ptype);
      arc.MoveNext(); )
    {
      if (arc.TimingType() == TimingType_Combinational
        && arc.GetStartPin(pin) == start_pin)
      {
        found = true;
        double fallTime;
        bool isSignalInversed;

        switch (arc.TimingSense())
        {
        case TimingSense_PositiveUnate:
          {
            fallTime = hd.GetDouble<HTimingPoint::FallArrivalTime>(startPoint)
              + arc.TIntrinsicFall() 
              + arc.ResistanceFall() * fallObservedC;
            isSignalInversed = false;
          }
          break;
        case TimingSense_NegativeUnate:
          {
            fallTime = hd.GetDouble<HTimingPoint::RiseArrivalTime>(startPoint)
              + arc.TIntrinsicFall() 
              + arc.ResistanceFall() * fallObservedC;
            isSignalInversed = true;
          }
          break;
        case TimingSense_NonUnate:
          {
            double pinArrival2 = hd.GetDouble<HTimingPoint::RiseArrivalTime>(startPoint);
            double pinArrival1 = hd.GetDouble<HTimingPoint::FallArrivalTime>(startPoint);
            if (pinArrival1 >= pinArrival2)
            {
              fallTime = pinArrival1
                + arc.TIntrinsicFall() 
                + arc.ResistanceFall() * fallObservedC;
              isSignalInversed = false;
            }
            else
            {
              fallTime = pinArrival2
                + arc.TIntrinsicFall() 
                + arc.ResistanceFall() * fallObservedC;
              isSignalInversed = true;
            }
          }
          break;
        default:
          LOGERRORFORMAT(("Unknown timing sense: %d", arc.TimingSense()));
          break;
        }//switch (arc.TimingSense())
        if (fallTime > worstFallTime)
        {
          worstFallTime = fallTime;
          *fallingArc = arc;
          *isInversed = isSignalInversed;
        }
      }
    }

    if(found)
    {
      *arcFallTime = hd.GetDouble<HTimingArcType::TIntrinsicFall>(*fallingArc)
        + hd.GetDouble<HTimingArcType::ResistanceFall>(*fallingArc) * fallObservedC;
    }
    else
    {
      *arcFallTime = 0;
      *isInversed = false;
      *fallingArc = hd.TimingArcTypes.Null();
    }
  }
  else
  {
    *arcFallTime = 0;
    *isInversed = false;
    *fallingArc = hd.TimingArcTypes.Null();
  }
}

void GetRequiredFallingArc(HDesign& hd,
                          HTimingPoint startPoint,
                          HTimingPoint endPoint,
                          HTimingArcType* fallingArc,
                          double* arcFallTime,
                          bool* isInversed)
{
  HPin pin = hd.Get<HTimingPoint::Pin, HPin>(endPoint);
  HPinType ptype = hd.Get<HPin::Type, HPinType>(pin);
  if (hd.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
  {
    HPin start_pin = hd.Get<HTimingPoint::Pin, HPin>(startPoint);
    HSteinerPoint endStPoint = hd.SteinerPoints[pin];
    double fallObservedC = hd.GetDouble<HSteinerPoint::FallObservedC>(endStPoint);
    double riseObservedC = hd.GetDouble<HSteinerPoint::RiseObservedC>(endStPoint);
    double worstFallTime = DBL_MAX;
    bool found = false;
    for (HPinType::ArcsEnumeratorW arc = hd.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(ptype);
      arc.MoveNext(); )
    {
      if (arc.TimingType() == TimingType_Combinational
        && arc.GetStartPin(pin) == start_pin)
      {
        found = true;
        double fallTime;
        bool isSignalInversed;

        switch (arc.TimingSense())
        {
        case TimingSense_PositiveUnate:
          {
            fallTime = hd.GetDouble<HTimingPoint::FallRequiredTime>(endPoint)
              - arc.TIntrinsicFall() 
              - arc.ResistanceFall() * fallObservedC;
            isSignalInversed = false;
          }
          break;
        case TimingSense_NegativeUnate:
          {
            fallTime = hd.GetDouble<HTimingPoint::RiseRequiredTime>(endPoint)
              - arc.TIntrinsicRise() 
              - arc.ResistanceRise() * riseObservedC;
            isSignalInversed = true;
          }
          break;
        case TimingSense_NonUnate:
          {
            double fallTime1 = hd.GetDouble<HTimingPoint::FallRequiredTime>(endPoint)
              - arc.TIntrinsicFall() 
              - arc.ResistanceFall() * fallObservedC;
            double riseTime1 = hd.GetDouble<HTimingPoint::RiseRequiredTime>(endPoint)
              - arc.TIntrinsicRise() 
              - arc.ResistanceRise() * riseObservedC;
            if (fallTime1 <= riseTime1)
            {
              fallTime = fallTime1;
              isSignalInversed = false;
            }
            else
            {
              fallTime = riseTime1;
              isSignalInversed = true;
            }
          }
          break;
        default:
          LOGERRORFORMAT(("Unknown timing sense: %d", arc.TimingSense()));
          break;
        }//switch (arc.TimingSense())
        if (fallTime < worstFallTime)
        {
          worstFallTime = fallTime;
          *fallingArc = arc;
          *isInversed = isSignalInversed;
        }
      }
    }

    if(found)
    {
      if(*isInversed)
        *arcFallTime = hd.GetDouble<HTimingArcType::TIntrinsicRise>(*fallingArc)
          + hd.GetDouble<HTimingArcType::ResistanceRise>(*fallingArc) * riseObservedC;
      else
        *arcFallTime = hd.GetDouble<HTimingArcType::TIntrinsicFall>(*fallingArc)
          + hd.GetDouble<HTimingArcType::ResistanceFall>(*fallingArc) * fallObservedC;
    }
    else
    {
      *arcFallTime = 0;
      *isInversed = false;
      *fallingArc = hd.TimingArcTypes.Null();
    }
  }
  else
  {
    *arcFallTime = 0;
    *isInversed = false;
    *fallingArc = hd.TimingArcTypes.Null();
  }
}

void GetRequiredRisingArc(HDesign& hd,
                          HTimingPoint startPoint,
                          HTimingPoint endPoint,
                          HTimingArcType* risingArc,
                          double* arcRiseTime,
                          bool* isInversed)
{
  HPin pin = hd.Get<HTimingPoint::Pin, HPin>(endPoint);
  HPinType ptype = hd.Get<HPin::Type, HPinType>(pin);
  if (hd.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
  {
    HPin start_pin = hd.Get<HTimingPoint::Pin, HPin>(startPoint);
    HSteinerPoint endStPoint = hd.SteinerPoints[pin];
    double fallObservedC = hd.GetDouble<HSteinerPoint::FallObservedC>(endStPoint);
    double riseObservedC = hd.GetDouble<HSteinerPoint::RiseObservedC>(endStPoint);
    double worstRiseTime = DBL_MAX;
    bool found = false;
    for (HPinType::ArcsEnumeratorW arc = hd.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(ptype);
      arc.MoveNext(); )
    {
      if (arc.TimingType() == TimingType_Combinational
        && arc.GetStartPin(pin) == start_pin)
      {
        found = true;
        double riseTime;
        bool isSignalInversed;

        switch (arc.TimingSense())
        {
        case TimingSense_PositiveUnate:
          {
            riseTime = hd.GetDouble<HTimingPoint::RiseRequiredTime>(endPoint)
              - arc.TIntrinsicRise() 
              - arc.ResistanceRise() * riseObservedC;
            isSignalInversed = false;
          }
          break;
        case TimingSense_NegativeUnate:
          {
            riseTime = hd.GetDouble<HTimingPoint::FallRequiredTime>(endPoint)
              - arc.TIntrinsicFall() 
              - arc.ResistanceFall() * fallObservedC;
            isSignalInversed = true;
          }
          break;
        case TimingSense_NonUnate:
          {
            double fallTime1 = hd.GetDouble<HTimingPoint::FallRequiredTime>(endPoint)
              - arc.TIntrinsicFall() 
              - arc.ResistanceFall() * fallObservedC;
            double riseTime1 = hd.GetDouble<HTimingPoint::RiseRequiredTime>(endPoint)
              - arc.TIntrinsicRise() 
              - arc.ResistanceRise() * riseObservedC;
            if (riseTime1 <= fallTime1)
            {
              riseTime = riseTime1;
              isSignalInversed = false;
            }
            else
            {
              riseTime = fallTime1;
              isSignalInversed = true;
            }
          }
          break;
        default:
          LOGERRORFORMAT(("Unknown timing sense: %d", arc.TimingSense()));
          break;
        }//switch (arc.TimingSense())
        if (riseTime < worstRiseTime)
        {
          worstRiseTime = riseTime;
          *risingArc = arc;
          *isInversed = isSignalInversed;
        }
      }
    }

    if(found)
    {
      if(!*isInversed)
        *arcRiseTime = hd.GetDouble<HTimingArcType::TIntrinsicRise>(*risingArc)
          + hd.GetDouble<HTimingArcType::ResistanceRise>(*risingArc) * riseObservedC;
      else
        *arcRiseTime = hd.GetDouble<HTimingArcType::TIntrinsicFall>(*risingArc)
          + hd.GetDouble<HTimingArcType::ResistanceFall>(*risingArc) * fallObservedC;
    }
    else
    {
      *arcRiseTime = 0;
      *isInversed = false;
      *risingArc = hd.TimingArcTypes.Null();
    }
  }
  else
  {
    *arcRiseTime = 0;
    *isInversed = false;
    *risingArc = hd.TimingArcTypes.Null();
  }
}
void SetStartPointsArrivals(HDesign& design, double clock_cycle)
{
  HTimingPoint startPointsEnd = design.TimingPoints.FirstInternalPoint();
  for (HTimingPointWrapper sp = design[design.TimingPoints.TopologicalOrderRoot()];
    sp.GoNext() != startPointsEnd; )
  {
    if (design.GetBool<HPin::IsPrimary>(sp.Pin()))
    {
      sp.SetFallArrivalTime(0.0);
      sp.SetRiseArrivalTime(0.0);
    }
    else 
    {
      switch (design.Get<HMacroType::Type, MacroType>(
                design.Get<HCell::MacroType, HMacroType>(
                  design.Get<HPin::Cell, HCell>(sp.Pin()))))
      {
      case MacroType_REFF:
        sp.SetFallArrivalTime(0.0);
        sp.SetRiseArrivalTime(0.0);
        break;
      case MacroType_HLSL:
        sp.SetFallArrivalTime(0.0);
        sp.SetRiseArrivalTime(0.0);
        break;
      case MacroType_FEFF:
        sp.SetFallArrivalTime(-clock_cycle * 0.5);
        sp.SetRiseArrivalTime(-clock_cycle * 0.5);
        break;
      case MacroType_LLSL:
        sp.SetFallArrivalTime(clock_cycle * 0.5);
        sp.SetRiseArrivalTime(clock_cycle * 0.5);
        break;
      default:
        sp.SetFallArrivalTime(0.0);
        sp.SetRiseArrivalTime(0.0);
        break;
      }

      //apply edge arcs
      HSteinerPoint this_point = design.SteinerPoints[sp.Pin()];
      for (HPinType::ArcsEnumeratorW arc
        = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(
            design.Get<HPin::Type, HPinType>(sp.Pin()));
        arc.MoveNext(); )
      {
        if (arc.TimingType() == TimingType_RisingEdge || arc.TimingType() == TimingType_FallingEdge)
        {
          sp.SetRiseArrivalTime(sp.RiseArrivalTime()
            + arc.TIntrinsicRise()
            + arc.ResistanceRise() * design.GetDouble<HSteinerPoint::RiseObservedC>(this_point));
          sp.SetFallArrivalTime(sp.FallArrivalTime()
            + arc.TIntrinsicFall()
            + arc.ResistanceFall() * design.GetDouble<HSteinerPoint::FallObservedC>(this_point));
        }
      }//for (HPinType::ArcsEnumeratorW arc
    }
  }//for (HTimingPointWrapper sp

  //prefill other points
  for (HTimingPointWrapper p = design[startPointsEnd]; !::IsNull(p); p.GoNext())
  {
    p.SetRiseArrivalTime(-DBL_MAX);
    p.SetFallArrivalTime(-DBL_MAX);
  }
}

void SetEndPointsRequires(HDesign& design, double clock_cycle)
{
  HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
  for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
    ep.GoPrevious() != endPointsEnd; )
  {
    if (design.GetBool<HPin::IsPrimary>(ep.Pin()))
    {
      ep.SetFallRequiredTime(clock_cycle);
      ep.SetRiseRequiredTime(clock_cycle);
    }
    else 
    {
      switch (design.Get<HMacroType::Type, MacroType>(
                design.Get<HCell::MacroType, HMacroType>(
                  design.Get<HPin::Cell, HCell>(ep.Pin()))))
      {
      case MacroType_REFF:
        ep.SetFallRequiredTime(clock_cycle);
        ep.SetRiseRequiredTime(clock_cycle);
        break;
      case MacroType_HLSL:
        ep.SetFallRequiredTime(clock_cycle * 0.5);
        ep.SetRiseRequiredTime(clock_cycle * 0.5);
        break;
      case MacroType_FEFF:
        ep.SetFallRequiredTime(clock_cycle * 0.5);
        ep.SetRiseRequiredTime(clock_cycle * 0.5);
        break;
      case MacroType_LLSL:
        ep.SetFallRequiredTime(clock_cycle);
        ep.SetRiseRequiredTime(clock_cycle);
        break;
      default:
        ep.SetFallRequiredTime(clock_cycle);
        ep.SetRiseRequiredTime(clock_cycle);
        break;
      }

      //apply setup constraints
      for (HPinType::ArcsEnumeratorW arc
        = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(
            design.Get<HPin::Type, HPinType>(ep.Pin()));
        arc.MoveNext(); )
      {
        if (arc.TimingType() == TimingType_SetupRising || arc.TimingType() == TimingType_SetupFalling)
        {
          ep.SetRiseRequiredTime(ep.RiseRequiredTime() + arc.TIntrinsicRise());
          ep.SetFallRequiredTime(ep.FallRequiredTime() + arc.TIntrinsicFall());
        }
      }//for (HPinType::ArcsEnumeratorW arc
    }
  }//for (HTimingPointWrapper ep

  //prefill other points
  for (HTimingPointWrapper p = design[endPointsEnd]; !::IsNull(p); p.GoPrevious())
  {
    p.SetRiseRequiredTime(DBL_MAX);
    p.SetFallRequiredTime(DBL_MAX);
  }
}

void PropagateArrivals(HDesign& design)
{
  for (HTimingPointWrapper pt = design[design.TimingPoints.FirstInternalPoint()];
    !::IsNull(pt); pt.GoNext())
  {
    HPin pin = pt.Pin();
    if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
    {//single arc from net source
      HNet net = design.Get<HPin::Net, HNet>(pin);
      HTimingPoint source = design.TimingPoints[design.Get<HNet::Source, HPin>(net)];
      HSteinerPoint this_point = design.SteinerPoints[pin];
      pt.SetFallArrivalTime(design.GetDouble<HTimingPoint::FallArrivalTime>(source)
        + design.GetDouble<HSteinerPoint::FallPathDelay>(this_point));
      pt.SetRiseArrivalTime(design.GetDouble<HTimingPoint::RiseArrivalTime>(source)
        + design.GetDouble<HSteinerPoint::RisePathDelay>(this_point));
      pt.SetFallArrivalAncestor(source);
      pt.SetRiseArrivalAncestor(source);
    }
    else
    {//here can be several cell's arcs
      ASSERT((design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT));
      HPinType ptype = design.Get<HPin::Type, HPinType>(pin);
      if (design.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
      {
        HSteinerPoint this_point = design.SteinerPoints[pin];
        double fallObservedC = design.GetDouble<HSteinerPoint::FallObservedC>(this_point);
        double riseObservedC = design.GetDouble<HSteinerPoint::RiseObservedC>(this_point);
        double worstFallTime = -DBL_MAX;
        double worstRiseTime = -DBL_MAX;
        HTimingPoint worstFallRelatedPoint = design.TimingPoints.Null();
        HTimingPoint worstRiseRelatedPoint = design.TimingPoints.Null();
        
        for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(ptype);
          arc.MoveNext(); )
        {
          if (arc.TimingType() == TimingType_Combinational)
          {
            HTimingPoint startPoint = design.TimingPoints[arc.GetStartPin(pin)];
            double fallTime = -DBL_MAX;
            double riseTime = -DBL_MAX;

            switch (arc.TimingSense())
            {
            case TimingSense_PositiveUnate:
              {
                fallTime = design.GetDouble<HTimingPoint::FallArrivalTime>(startPoint)
                  + arc.TIntrinsicFall() 
                  + arc.ResistanceFall() * fallObservedC;
                riseTime = design.GetDouble<HTimingPoint::RiseArrivalTime>(startPoint)
                  + arc.TIntrinsicRise() 
                  + arc.ResistanceRise() * riseObservedC;
              }
              break;
            case TimingSense_NegativeUnate:
              {
                fallTime = design.GetDouble<HTimingPoint::RiseArrivalTime>(startPoint)
                  + arc.TIntrinsicFall() 
                  + arc.ResistanceFall() * fallObservedC;
                riseTime = design.GetDouble<HTimingPoint::FallArrivalTime>(startPoint)
                  + arc.TIntrinsicRise() 
                  + arc.ResistanceRise() * riseObservedC;
              }
              break;
            case TimingSense_NonUnate:
              {
                double pinArrival = max(design.GetDouble<HTimingPoint::RiseArrivalTime>(startPoint),
                                        design.GetDouble<HTimingPoint::FallArrivalTime>(startPoint));
                fallTime = pinArrival
                  + arc.TIntrinsicFall() 
                  + arc.ResistanceFall() * fallObservedC;
                riseTime = pinArrival
                  + arc.TIntrinsicRise() 
                  + arc.ResistanceRise() * riseObservedC;
              }
              break;
            default:
              LOGERRORFORMAT(("Unknown timing sense: %d", arc.TimingSense()));
              break;
            }//switch (arc.TimingSense())
            if (fallTime > worstFallTime)
            {
              worstFallTime = fallTime;
              worstFallRelatedPoint = startPoint;
            }
            if (riseTime > worstRiseTime)
            {
              worstRiseTime = riseTime;
              worstRiseRelatedPoint = startPoint;
            }
          }//if (arc.TimingType() == TimingType_Combinational)
        }//for (HPinType::ArcsEnumeratorW arc

        //set results
        pt.SetFallArrivalTime(worstFallTime);
        pt.SetRiseArrivalTime(worstRiseTime);
        pt.SetFallArrivalAncestor(worstFallRelatedPoint);
        pt.SetRiseArrivalAncestor(worstRiseRelatedPoint);
      }//if (design.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
    }//if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
  }//for (HTimingPointWrapper pt
}

void PropagateRequires(HDesign& design)
{
  HTimingPoint firstPoint = design.Get<HTimingPoint::PreviousPoint, HTimingPoint>(design.TimingPoints.FirstInternalPoint());

  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    pt.GoPrevious() != firstPoint; )
  {
    HPin pin = pt.Pin();
    if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
    {//single arc to net source
      HNet net = design.Get<HPin::Net, HNet>(pin);
      HTimingPoint source = design.TimingPoints[design.Get<HNet::Source, HPin>(net)];
      HSteinerPoint this_point = design.SteinerPoints[pin];
      
      double thisFallRequired = pt.FallRequiredTime()
        - design.GetDouble<HSteinerPoint::FallPathDelay>(this_point);
      if (design.GetDouble<HTimingPoint::FallRequiredTime>(source) > thisFallRequired)
      {
        design.Set<HTimingPoint::FallRequiredTime>(source, thisFallRequired);
        design.Set<HTimingPoint::FallRequiredAncestor>(source, (HTimingPoint)pt);
      }
      
      double thisRiseRequired = pt.FallRequiredTime()
        - design.GetDouble<HSteinerPoint::RisePathDelay>(this_point);
      if (design.GetDouble<HTimingPoint::RiseRequiredTime>(source) > thisRiseRequired)
      {
        design.Set<HTimingPoint::RiseRequiredTime>(source, thisRiseRequired);
        design.Set<HTimingPoint::RiseRequiredAncestor>(source, (HTimingPoint)pt);
      }
    }
    else
    {
      ASSERT((design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT));
      HPinType ptype = design.Get<HPin::Type, HPinType>(pin);
      if (design.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
      {
        HSteinerPoint this_point = design.SteinerPoints[pin];
        double fallObservedC = design.GetDouble<HSteinerPoint::FallObservedC>(this_point);
        double riseObservedC = design.GetDouble<HSteinerPoint::RiseObservedC>(this_point);

        for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(ptype);
          arc.MoveNext(); )
        {
          if (arc.TimingType() == TimingType_Combinational)
          {
            HTimingPoint startPoint = design.TimingPoints[arc.GetStartPin(pin)];
            double fallTime = DBL_MAX;
            double riseTime = DBL_MAX;

            switch (arc.TimingSense())
            {
            case TimingSense_PositiveUnate:
              {
                fallTime = pt.FallRequiredTime()
                  - arc.TIntrinsicFall() 
                  - arc.ResistanceFall() * fallObservedC;
                riseTime = pt.RiseRequiredTime()
                  - arc.TIntrinsicRise() 
                  - arc.ResistanceRise() * riseObservedC;
              }
              break;
            case TimingSense_NegativeUnate:
              {
                fallTime = pt.RiseRequiredTime()
                  - arc.TIntrinsicRise() 
                  - arc.ResistanceRise() * riseObservedC;
                riseTime = pt.FallRequiredTime()
                  - arc.TIntrinsicFall() 
                  - arc.ResistanceFall() * fallObservedC;
              }
              break;
            case TimingSense_NonUnate:
              {
                fallTime = pt.FallRequiredTime()
                  - arc.TIntrinsicFall() 
                  - arc.ResistanceFall() * fallObservedC;
                riseTime = pt.RiseRequiredTime()
                  - arc.TIntrinsicRise() 
                  - arc.ResistanceRise() * riseObservedC;
                fallTime = riseTime = min(fallTime, riseTime);
              }
              break;
            default:
              LOGERRORFORMAT(("Unknown timing sense: %d", arc.TimingSense()));
              break;
            }//switch (arc.TimingSense())
            if (fallTime < design.GetDouble<HTimingPoint::FallRequiredTime>(startPoint))
            {
              design.Set<HTimingPoint::FallRequiredTime>(startPoint, fallTime);
              design.Set<HTimingPoint::FallRequiredAncestor>(startPoint, (HTimingPoint)pt);
            }
            if (riseTime < design.GetDouble<HTimingPoint::RiseRequiredTime>(startPoint))
            {
              design.Set<HTimingPoint::RiseRequiredTime>(startPoint, riseTime);
              design.Set<HTimingPoint::RiseRequiredAncestor>(startPoint, (HTimingPoint)pt);
            }
          }//if (arc.TimingType() == TimingType_Combinational)
        }//for (HPinType::ArcsEnumeratorW arc
      }//if (design.GetInt<HPinType::TimingArcsCount>(ptype) > 0)
    }//if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
  }//for (HTimingPointWrapper pt
}

void PropagateDelays(HDesign& design, double clock_cycle)
{
  //WRITELINE("");
  //ALERT("TIMING STARTED (delays propagation phase)...");

  //remove all previously calculated results
  design.TimingPoints.ClearTimingResults();
  //ALERT("Clear finished");

  //initialize arrival times
  SetStartPointsArrivals(design, clock_cycle);
  //ALERT("Arrivals initialized");

  //initialize required times
  SetEndPointsRequires(design, clock_cycle);
  //ALERT("Requires initialized");

  //propagate delays
  PropagateArrivals(design);
  //ALERT("Arrivals propagated");
  PropagateRequires(design);
  //ALERT("Requires propagated");

  //ALERT("TIMING FINISHED");
}

void PropagateDelays(HDesign& design)
{
  PropagateDelays(design, design.cfg.ValueOf("benchmark.target_clock_cycle", 0.0));
}