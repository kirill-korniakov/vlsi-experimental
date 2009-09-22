#include "BufferInfo.h"
#include "Utils.h"
#include <math.h>

BufferInfo BufferInfo::Create(HDesign& hd)
{
  return Create(hd,
    hd.cfg.ValueOf("Buffering.DefaultBuffer.Macro", "INVX1"),
    hd.cfg.ValueOf("Buffering.DefaultBuffer.InputPin", "A"),
    hd.cfg.ValueOf("Buffering.DefaultBuffer.OutputPin", "Y"));
}

BufferInfo BufferInfo::Create(HDesign& hd, string macro, string inputPin, string outputPin)
{
  HMacroType mType = Utils::FindMacroTypeByName(hd, macro);
  if (::IsNull(mType))
    return BufferInfo();

  HPinType inPin = Utils::FindMacroTypePinByName(hd, mType, inputPin);
  HPinType outPin = Utils::FindMacroTypePinByName(hd, mType, outputPin);

  if (::IsNull(inPin) || ::IsNull(outPin))
    return BufferInfo();

  ERROR_ASSERT((hd.Get<HPinType::Direction, PinDirection>(inPin) == PinDirection_INPUT));
  ERROR_ASSERT((hd.Get<HPinType::Direction, PinDirection>(outPin) == PinDirection_OUTPUT));

  BufferInfo result = BufferInfo();
  result.m_C = hd.GetDouble<HPinType::Capacitance>(inPin);

  HTimingArcType arc = Utils::FindTimingArc(hd, inPin, outPin);

  if (::IsNull(arc))
  {
    result.m_R = 0.0;
    result.m_T = 0.0;
  }
  else
  {
    result.m_R = 0.5 * (hd.GetDouble<HTimingArcType::ResistanceFall>(arc) + hd.GetDouble<HTimingArcType::ResistanceRise>(arc));
    result.m_T = 0.5 * (hd.GetDouble<HTimingArcType::TIntrinsicFall>(arc) + hd.GetDouble<HTimingArcType::TIntrinsicRise>(arc));
  }

  result.m_Lbuf = result.Rb() / hd.RoutingLayers.Physics.RPerDist + result.Cb() / hd.RoutingLayers.Physics.LinearC;
  result.m_Dbuf = sqrt(2.0 * (result.Tb() + result.Rb() * result.Cb()) / hd.RoutingLayers.Physics.RPerDist / hd.RoutingLayers.Physics.LinearC);
  
  double resistance = 0, capacitance = 0;
  Utils::CalcElementTRC(hd, result.m_BufType, &result.tIntrinsic, &resistance, &capacitance);

  return result;
}