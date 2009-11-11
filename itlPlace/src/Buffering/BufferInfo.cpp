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

  Utils::DriverPhisics dph = Utils::GetDriverAveragePhisics(hd, outPin, SignalModel);
  Utils::SinkPhisics sph = Utils::GetSinkCapacitance(hd, inPin, SignalModel);

  BufferInfo result = BufferInfo();
  result.m_BufType = mType;
  result.m_C = sph;
  result.m_R = dph.R;
  result.m_T = dph.T;

  result.m_Lbuf = Utils::LBuf(dph, sph, hd.RoutingLayers.Physics);
  result.m_Dbuf = Utils::DBuf(dph, sph, hd.RoutingLayers.Physics);
  
  return result;
}