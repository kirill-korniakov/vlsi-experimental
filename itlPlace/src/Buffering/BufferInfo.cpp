#include "BufferInfo.h"
#include "Utils.h"
#include <math.h>

BufferInfo BufferInfo::Create(HDesign& hd)
{
  return Create(hd,
    hd.cfg.ValueOf("DefaultBuffer.Macro", "INVX1"),
    hd.cfg.ValueOf("DefaultBuffer.InputPin", "A"),
    hd.cfg.ValueOf("DefaultBuffer.OutputPin", "Y"));
}

TemplateTypes<BufferInfo>::vector BufferInfo::CreateVector(HDesign& hd, string* bufferList)
{
  TemplateTypes<BufferInfo>::vector buffervector;

  string macro;
  string inputPinName; 
  string outputPinName;
  if (bufferList == NULL)
  {
    for (HMacroTypes::EnumeratorW macroTypeEW = hd.MacroTypes.GetEnumeratorW(); macroTypeEW.MoveNext();)
      if (macroTypeEW.Type() == MacroType_BUF)
      {
        macro = macroTypeEW.Name();
        for (HMacroType::PinsEnumeratorW pin = macroTypeEW.GetEnumeratorW(); pin.MoveNext();)
        {
          if(pin.Direction() == PinDirection_INPUT) inputPinName = pin.Name();
          if(pin.Direction() == PinDirection_OUTPUT) outputPinName = pin.Name();
        }     
        BufferInfo buf = Create(hd, macro, inputPinName, outputPinName);

        bool isIns = false;

        for (TemplateTypes<BufferInfo>::vector::iterator bufIter = buffervector.begin(); bufIter != buffervector.end(); ++bufIter)
        {
          if ((bufIter->Tb() >  buf.Tb()) && (bufIter->Cb() > buf.Cb()))
          {
            buffervector.insert(bufIter, buf);
            isIns = true;
            break;
          }
        }
        if (!isIns)
          buffervector.push_back(buf);
      }
  }
  else
  {
    string macro;
    string inputPinName; 
    string outputPinName;
    int n = hd.cfg.ValueOf("BufferListLength", 0);
    for (HMacroTypes::EnumeratorW macroTypeEW = hd.MacroTypes.GetEnumeratorW(); macroTypeEW.MoveNext();)
    {
      if (macroTypeEW.Type() == MacroType_BUF)
      {
        macro = macroTypeEW.Name();
        bool isbuf = false;
        for (int i = 0; (i < n) && !isbuf; i++)
          if (macro == bufferList[i])
            isbuf = true;
        if (!isbuf)
          continue;

        for (HMacroType::PinsEnumeratorW pin = hd[macroTypeEW].GetEnumeratorW(); pin.MoveNext();)
        {
          if(pin.Direction() == PinDirection_INPUT) inputPinName = pin.Name();
          if(pin.Direction() == PinDirection_OUTPUT) outputPinName = pin.Name();
        }

        BufferInfo buf = Create(hd, macro, inputPinName, outputPinName);
        bool isIns = false;

        for (TemplateTypes<BufferInfo>::vector::iterator bufIter = buffervector.begin(); bufIter != buffervector.end(); ++bufIter)
        {
          if ((bufIter->Tb() >  buf.Tb()) && (bufIter->Cb() > buf.Cb()))
          {
            buffervector.insert(bufIter, buf);
            isIns = true;
            break;
          }
        }
        if (!isIns)
          buffervector.push_back(buf);
      }
    }
  }


  return buffervector;

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

  ASSERT((hd.Get<HPinType::Direction, PinDirection>(inPin) == PinDirection_INPUT));
  ASSERT((hd.Get<HPinType::Direction, PinDirection>(outPin) == PinDirection_OUTPUT));

  Utils::DriverPhisics dph = Utils::GetDriverAveragePhisics(hd, outPin, SignalModel);
  Utils::SinkPhisics sph = Utils::GetSinkCapacitance(hd, inPin, SignalModel);

  BufferInfo result = BufferInfo();
  result.m_BufType = mType;
  result.m_C = sph;
  result.m_R = dph.R;
  result.m_T = dph.T;
  result.m_InPin = inPin;
  result.m_OutPin = outPin;

  result.m_Lbuf = Utils::LBuf(dph, sph, hd.RoutingLayers.Physics);
  result.m_Dbuf = Utils::DBuf(dph, sph, hd.RoutingLayers.Physics);
  
  return result;
}