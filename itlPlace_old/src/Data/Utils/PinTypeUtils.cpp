#include "PinTypeUtils.h"

using namespace Utils;

PinDirection Utils::GetPinDirection(const char* dir)
{//INPUT OUTPUT INOUT
  if(dir[0] == 'O')
    return PinDirection_OUTPUT;
  else
    if(dir[2] == 'O')
      return PinDirection_INOUT;
    else
      return PinDirection_INPUT;
}

const char* Utils::GetPinDirectionStr(PinDirection dir)
{
  switch (dir)
  {
  case PinDirection_OUTPUT:
    return "OUTPUT";
  case PinDirection_INPUT:
    return "INPUT";
  case PinDirection_INOUT:
    return "INOUT";
  default:
    return "BOGUS";
  };
  return "BOGUS";
}

HPinType Utils::FindMacroTypePinByName(HDesign& design, HMacroType macro, const string& name)
{
  for (HMacroType::PinsEnumerator pin = design.Get<HMacroType::PinTypesEnumerator, HMacroType::PinsEnumerator>(macro);
    pin.MoveNext(); )
  {
    if (design.GetString<HPinType::Name>(pin) == name)
      return pin;
  }
  return design.PinTypes.Null();
}

const char * Utils::GetPinFunctionStr(PinFunction pf)
{
  switch (pf)
  {
  case PinFunction_Clock  : 
    return "PinFunction_Clock";
  case PinFunction_Ground : 
    return "PinFunction_Ground";
  case PinFunction_Power  : 
    return "PinFunction_Power";
  case PinFunction_Analog : 
    return "PinFunction_Analog";
  case PinFunction_Signal : 
    return "PinFunction_Signal";// no special function (default)
  case PinFunction_Clear  : 
    return "PinFunction_Clear";
  case PinFunction_Preset : 
    return "PinFunction_Preset";
  case PinFunction_3State : 
    return "PinFunction_3State";
  default:
    return "Unknown";
  }

}

PinFunction Utils::GetPinFunction(const char * name)
{
  PinFunction pf;
  if (strcmp(name, "PinFunction_Clock") == 0)
    pf = PinFunction_Clock;
  else
  if (strcmp(name, "PinFunction_Ground") == 0)
    pf = PinFunction_Ground;
  else
  if (strcmp(name, "PinFunction_Power") == 0)
    pf = PinFunction_Power;
  else
  if (strcmp(name, "PinFunction_Analog") == 0)
    pf = PinFunction_Analog;
  else
  if (strcmp(name, "PinFunction_Signal") == 0)
    pf = PinFunction_Signal;
  else
  if (strcmp(name, "PinFunction_Clear") == 0)
    pf = PinFunction_Clear;
  else
  if (strcmp(name, "PinFunction_Preset") == 0)
    pf = PinFunction_Preset;
  else
  if (strcmp(name, "PinFunction_3State") == 0)
    pf = PinFunction_3State;
  else
  if (strcmp(name, "PinFunction_Clock") == 0)
    pf = PinFunction_Clock;
  else pf = PinFunction_Default;


  return pf;

}