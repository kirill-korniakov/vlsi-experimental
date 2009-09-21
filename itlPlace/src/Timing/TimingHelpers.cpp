#include "TimingHelpers.h"
#include "DelayCalculation.h"

double GetNetMaxDelay(HDesign& hd, HNet net, HNet originalNet, BufferInfo& binf)
{
  double maxDelay = 0.0;
  CalculateNetDelays<LayersModel_Lumped, SignalModel_Universal>(hd, net);
  for(HNet::SinksEnumeratorW sink = hd.Get<HNet::Sinks, HNet::SinksEnumeratorW>(net); sink.MoveNext(); )
  {
    double delay = hd.GetDouble<HSteinerPoint::PathDelay>(hd.SteinerPoints[sink]);
    if (sink.OriginalNet() != originalNet)
    {
      for (HCell::PinsEnumeratorW pin = hd.Get<HCell::Pins, HCell::PinsEnumeratorW>(sink.Cell()); pin.MoveNext(); )
        if (!IsNull(pin.Net()) && pin.Direction() == PinDirection_OUTPUT)
        {
          delay += binf.Tb() + binf.Rb() * hd.GetDouble<HSteinerPoint::ObservedC>(hd.SteinerPoints[pin])
          + GetNetMaxDelay(hd, pin.Net(), originalNet, binf);
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
  return ninf.Rd() * hd.GetDouble<HSteinerPoint::ObservedC>(hd.SteinerPoints[hd.Get<HNet::Source, HPin>(net)])
    + GetNetMaxDelay(hd, hd.Get<HPin::Net, HNet>(hd.Get<HNet::Source, HPin>(net)), net, binf);
}