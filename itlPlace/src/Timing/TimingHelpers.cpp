#include "TimingHelpers.h"
#include "DelayCalculation.h"
#include "AdaptiveRoute.h"

double GetNetMaxDelay(HDesign& hd, HNet net, HNet originalNet, BufferInfo& binf)
{
  double maxDelay = 0.0;
  AdaptiveRoute(hd, net);
  CalculateNetDelays<LayersModel_Lumped, SignalModel_Universal>(hd, net);
  for(HNet::SinksEnumeratorW sink = hd.Get<HNet::Sinks, HNet::SinksEnumeratorW>(net); sink.MoveNext(); )
  {
    double delay = hd.GetDouble<HSteinerPoint::PathDelay>(hd.SteinerPoints[sink]);
    if (sink.OriginalNet() != originalNet)
    {
      for (HCell::PinsEnumeratorW pin = hd.Get<HCell::Pins, HCell::PinsEnumeratorW>(sink.Cell()); pin.MoveNext(); )
        if (!IsNull(pin.Net()) && pin.Direction() == PinDirection_OUTPUT)
        {
          double d = GetNetMaxDelay(hd, pin.Net(), originalNet, binf);
          double observedC = hd.GetDouble<HSteinerPoint::ObservedC>(hd.SteinerPoints[pin]);
          delay += binf.Tb() + binf.Rb() * observedC + d;
          break;
        }
    }
    if (delay > maxDelay)
      maxDelay = delay;
  }
  return maxDelay;
}

double TimingHelper::GetBufferedNetMaxDelay(HNet net, NetInfo& ninf, BufferInfo& binf)
{
  double d = GetNetMaxDelay(hd, hd.Get<HPin::Net, HNet>(hd.Get<HNet::Source, HPin>(net)), net, binf);
  double observedC = hd.GetDouble<HSteinerPoint::ObservedC>(hd.SteinerPoints[hd.Get<HNet::Source, HPin>(net)]);
  return ninf.Rd() * observedC + d;
}

void TimingHelper::PrintNetPinsDetails(HNet net, BufferInfo& binf)
{
  WRITELINE("Net: %s", hd.GetString<HNet::Name>(net).c_str());

  AdaptiveRoute(hd, net);
  CalculateNetDelays<LayersModel_Lumped, SignalModel_Universal>(hd, net);

  NetInfo ni = NetInfo::Create(hd, net, binf);
  if (ni.Rd() == 0.0)
  {
    WRITELINE("DriverResistance: %.10f", binf.Rb());
    WRITELINE("BufferDelay     : %.10f", binf.Tb());
  }
  else
  {
    WRITELINE("DriverResistance: %.10f", ni.Rd());
  }

  WRITELINE("CellName PinName X Y ObservedC PathDelay");
  for(HNet::PinsEnumeratorW pin = hd.Get<HNet::Pins, HNet::PinsEnumeratorW>(net); pin.MoveNext(); )
  {
    HSteinerPointWrapper st = hd[hd.SteinerPoints[pin]];
    WRITELINE("%s %s %10.2f %10.2f %12.10f %12.10f",
      hd.GetString<HCell::Name>(pin.Cell()).c_str(),
      pin.Name().c_str(),
      pin.X(),
      pin.Y(),
      st.ObservedC(),
      st.PathDelay());
  }
}