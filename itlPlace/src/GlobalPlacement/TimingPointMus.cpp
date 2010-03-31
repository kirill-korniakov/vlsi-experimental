#include "TimingPointMus.h"
#include "Utils.h"
#include "STA.h"
#include "Timing.h"

int CalcNumberOfLRArc(HDesign& design, HNet net)
{
  HPin source = design[net].Source();
  HPinType sourceType = design.Get<HPin::Type, HPinType>(source);

  int i = 0;
  for (HPinType::ArcsEnumeratorW arc = design[sourceType].GetArcsEnumeratorW(); arc.MoveNext(); )
    if (arc.TimingType() == TimingType_Combinational)
      i++;

  return i;
}

//FIXME: check if we need to find topological order by ourselves (as prestage)
TimingPointMus::TimingPointMus(HDesign& design): MuS(0), MuIn(0)
{
  size = design._Design.NetList.nPinsLimit;
  ::Grow(&MuS, 0, size);
  ::Grow(&MuIn, 0, size);

  reporter = new MuReporter(design);

  for (HTimingPointWrapper sp = design[design.TimingPoints.TopologicalOrderRoot()]; !::IsNull(sp.GoNext()); )
    InitPoint(design, sp);

  EnforceFlowProperty(design);
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

  UpdateInMuA(pt, rightSum / leftSum);
}

void TimingPointMus::EnforceRequiredFlowProperty(HDesign& design, HTimingPoint pt)
{
  double leftSum = SumInMuR(pt) + GetMuS(pt);
  double rightSum =  SumOutMuR(design, pt);

  UpdateOutMuR(design, pt, leftSum / rightSum);
}

void TimingPointMus::EnforceFlowProperty(HDesign& design)
{
  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoNext()); )
  {
    EnforceRequiredFlowProperty(design, pt);
  }

  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoPrevious()); )
  {
    EnforceArrivalFlowProperty(design, pt);
  }

  reporter->Report(design, this);
}

double TimingPointMus::GetInMuA(HTimingPoint pt)
{
  int count = GetMuInCount(pt);
  double sum = 0.0;
  for (int j = 0; j < count; j++)
  {
    double muInA = GetMuInA(pt, j);
    sum = sum + muInA;
  }
  return sum;
}

double TimingPointMus::GetInMuR(HTimingPoint pt)
{
  int count = GetMuInCount(pt);
  double sum = 0.0;
  for (int j = 0; j < count; j++)
  {
    double muInR = GetMuInR(pt, j);
    sum = sum + muInR;
  }
  return sum;
}

void TimingPointMus::GetMuIn(HDesign& design, HTimingPoint pt, std::vector<double>& inMus)
{
  inMus.clear();
  for (int i = 0; i < GetMuInCount(pt); i++)
  {
    inMus.push_back(GetMuInA(pt, i) + GetMuInR(pt, i));
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

void TimingPointMus::UpdateOutMuR(HDesign& design, HTimingPoint pt, double multiplier)
{
  IterateOutMu(design, pt, UpdateMuR(multiplier));
}

template<class Action>
void TimingPointMus::IterateOutMu(HDesign& design, HTimingPoint pt, Action& todo)
{
  HPin pin = design.Get<HTimingPoint::Pin, HPin>(pt);
  if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_OUTPUT)
  {
    for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(design.Get<HPin::Net, HNet>(pin));
      sink.MoveNext();)
    {
      todo(*this, design.TimingPoints[sink], 0);
    }
  }
  else
  {
    HCell cell = design.Get<HPin::Cell, HCell>(pin);
    for(HCell::PinsEnumeratorW p = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); p.MoveNext(); )
    {
      if (p.Direction() != PinDirection_OUTPUT || ::IsNull(p.Net())) continue;
      HTimingPoint point = design.TimingPoints[p];
      int idx = 0;
      for (HPinType::ArcsEnumeratorW arc 
        = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(p.Type());
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

void TimingPointMus::InitPoint(HDesign& design, HTimingPoint pt)
{
  double defaultMu = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.muLR", 0.99);

  SetMuS(pt, defaultMu);
  HPin pin = design.Get<HTimingPoint::Pin, HPin>(pt);

  if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
  {
    MuIn[::ToID(pt)].resize(2, defaultMu);
  }
  else
  {
    if (!design.GetBool<HPin::IsPrimary>(pin))
      MuIn[::ToID(pt)].resize(2 * CalcNumberOfLRArc(design, design.Get<HPin::Net, HNet>(pin)), defaultMu);
  }
}

void TimingPointMus::UpdateMus(HDesign& design)
{
  STA(design);

  double wns = Utils::WNS(design);
  double ns = 0.0;
  double r;
  double gamma = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.gamma", 0.0);
  double theta = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.theta", 2.0);
  double factor;

  for (HTimingPointWrapper sp = design[design.TimingPoints.TopologicalOrderRoot()]; 
       !::IsNull(sp.GoNext()); )
  {
    ns = sp.NegativeSlack();
    r = ns / wns * (1 - gamma);

	if (r < 1.0)
		factor = pow(r, theta);
	else
		factor = 2.0 - 1.0/pow(r, theta);

    for (int i = 0; i < GetMuInCount(sp); i++)
    {
      SetMuInA(sp, i, GetMuInA(sp, i) * factor);
      SetMuInR(sp, i, GetMuInR(sp, i) * factor);
    }
    SetMuS(sp, GetMuS(sp) * factor);
  }

  EnforceFlowProperty(design);
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

double TimingPointMus::SumInMuA(HTimingPoint pt)
{
  double sum = 0.0;
  for (int i = 0; i < GetMuInCount(pt); i++)
  {
    sum += GetMuInA(pt, i);
  }
  return sum;
}

double TimingPointMus::SumInMuR(HTimingPoint pt )
{
  double sum = 0.0;
  for (int i = 0; i < GetMuInCount(pt); i++)
  {
    sum += GetMuInR(pt, i);
  }
  return sum;
}

void TimingPointMus::UpdateInMuA(HTimingPoint pt, double multiplier)
{
  for (int i = 0; i < GetMuInCount(pt); i++)
  {
    SetMuInA(pt, i, GetMuInA(pt, i) * multiplier);
  }
}