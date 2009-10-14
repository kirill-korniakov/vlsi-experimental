#include "MacroTypeUtils.h"
#include <math.h>

namespace Utils
{
  HMacroType FindMacroTypeByName(const HDesign& hd, const string name)
  {
    for(HMacroTypes::Enumerator i = hd.MacroTypes.GetEnumerator(); i.MoveNext(); )
      if(hd.GetString<HMacroType::Name>(i) == name)
        return i;
    return hd.MacroTypes.Null();
  }

  bool StartsWith(const string& str, const char* prefix)
  {
    for(int i = 0; i < (int)str.length() && prefix[i] != 0; i++)
      if(str[i] != prefix[i])
        return false;
    return true;
  }

  void TryDetectMacroTypeByName(HMacroTypeWrapper& self)
  {
    string name = self.Name();
    if (StartsWith(name, "DFF") || StartsWith(name, "SDFF"))
      self.SetType(MacroType_REFF);
    else if (StartsWith(name, "LAT") || StartsWith(name, "TLAT") || StartsWith(name, "DL"))
      self.SetType(MacroType_HLSL);
    else if (StartsWith(name, "LOGIC0"))
      self.SetType(MacroType_LOGIC0);
    else if (StartsWith(name, "LOGIC1"))
      self.SetType(MacroType_LOGIC1);
    else if (StartsWith(name, "ANTENNA"))
      self.SetType(MacroType_ANTENNA);
    else if (StartsWith(name, "FILL"))
      self.SetType(MacroType_FILL);
    else if (StartsWith(name, "CLKBUF"))
      self.SetType(MacroType_CLKBUF);
    else if (StartsWith(name, "CLK"))
      self.SetType(MacroType_CLKOther);
    else if (StartsWith(name, "AND"))
      self.SetType(MacroType_AND);
    else if (StartsWith(name, "AOI"))
      self.SetType(MacroType_AOI);
    else if (StartsWith(name, "OAI"))
      self.SetType(MacroType_OAI);
    else if (StartsWith(name, "NAND"))
      self.SetType(MacroType_NAND);
    else if (StartsWith(name, "NOR"))
      self.SetType(MacroType_NOR);
    else if (StartsWith(name, "OR"))
      self.SetType(MacroType_OR);
    else if (StartsWith(name, "XNOR"))
      self.SetType(MacroType_XNOR);
    else if (StartsWith(name, "XOR"))
      self.SetType(MacroType_XOR);
    else if (StartsWith(name, "TBUF"))
      self.SetType(MacroType_TBUF);
    else if (StartsWith(name, "TINV"))
      self.SetType(MacroType_TINV);
    else if (StartsWith(name, "BUF"))
      self.SetType(MacroType_BUF);
    else if (StartsWith(name, "INV"))
      self.SetType(MacroType_INV);
    else
      self.SetType(MacroType_Default);
  }


  const char* GetMacroFunctionName(MacroType mtype)
  {
    switch (mtype)
    {
    case MacroType_UnknownSpecial:
      return "UnknownSpecial";
    case MacroType_CLKBUF:
      return "CLKBUF";
    case MacroType_CLKOther:
      return "CLKGATE";
    case MacroType_FILL:
      return "FILL";
    case MacroType_UnknownSequential:
      return "UnknownSequential";
    case MacroType_ANTENNA:
      return "ANTENNA";
    case MacroType_LOGIC0:
      return "LOGIC0";
    case MacroType_LOGIC1:
      return "LOGIC1";
    case MacroType_FEFF:
      return "FEFF";
    case MacroType_REFF:
      return "REFF";
    case MacroType_HLSL:
      return "HLSL";
    case MacroType_LLSL:
      return "LLSL";
    case MacroType_UnknownCombinational:
      return "UnknownCombinational";
    case MacroType_AND:
      return "AND";
    case MacroType_AOI:
      return "AOI";
    case MacroType_OAI:
      return "OAI";
    case MacroType_NAND:
      return "NAND";
    case MacroType_NOR:
      return "NOR";
    case MacroType_OR:
      return "OR";
    case MacroType_XNOR:
      return "XNOR";
    case MacroType_XOR:
      return "XOR";
    case MacroType_TBUF:
      return "TBUF";
    case MacroType_TINV:
      return "TINV";
    case MacroType_INV:
      return "INV";
    case MacroType_BUF:
      return "BUF";
    default:
      return "Unknown";
    }
  }

  MacroType GetMacroFunctionType(const char* name)
  {
    MacroType mt ;

    if ((strcmp(name, "UnknownSpecial") == 0) || 
      (strcmp(name, "MacroType_UnknownSpecial") == 0))
      mt = MacroType_UnknownSpecial;
    else
    if ((strcmp(name, "CLKBUF") == 0) || 
      (strcmp(name, "MacroType_CLKBUF") == 0))
      mt = MacroType_CLKBUF;
    else
    if ((strcmp(name, "CLKGATE") == 0) || 
      (strcmp(name, "MacroType_CLKOther") == 0))
      mt = MacroType_CLKOther;
    else
    if ((strcmp(name, "FILL") == 0) || 
      (strcmp(name, "MacroType_FILL") == 0))
      mt = MacroType_FILL;
    else
    if ((strcmp(name, "UnknownSequential") == 0) || 
      (strcmp(name, "MacroType_UnknownSequential") == 0))
      mt = MacroType_UnknownSequential;
    else
    if ((strcmp(name, "ANTENNA") == 0) || 
      (strcmp(name, "MacroType_ANTENNA") == 0))
      mt = MacroType_ANTENNA;
    else
    if ((strcmp(name, "LOGIC0") == 0) || 
      (strcmp(name, "MacroType_LOGIC0") == 0))
      mt = MacroType_LOGIC0;
    else
    if ((strcmp(name, "LOGIC1") == 0) || 
      (strcmp(name, "MacroType_LOGIC1") == 0))
      mt = MacroType_LOGIC1;
    else
    if ((strcmp(name, "FEFF") == 0) || 
      (strcmp(name, "MacroType_FEFF") == 0))
      mt = MacroType_FEFF;
    else
    if ((strcmp(name, "REFF") == 0) || 
      (strcmp(name, "MacroType_REFF") == 0))
      mt = MacroType_REFF;
    else
    if ((strcmp(name, "HLSL") == 0) || 
      (strcmp(name, "MacroType_HLSL") == 0))
      mt = MacroType_HLSL;
    else
    if ((strcmp(name, "LLSL") == 0) || 
      (strcmp(name, "MacroType_LLSL") == 0))
      mt = MacroType_LLSL;
    else
    if ((strcmp(name, "UnknownCombinational") == 0) || 
      (strcmp(name, "MacroType_UnknownCombinational") == 0))
      mt = MacroType_UnknownCombinational;
    else
    if ((strcmp(name, "AND") == 0) || 
      (strcmp(name, "MacroType_AND") == 0))
      mt = MacroType_AND;
    else
    if ((strcmp(name, "AOI") == 0) || 
      (strcmp(name, "MacroType_AOI") == 0))
      mt = MacroType_AOI;
    else
    if ((strcmp(name, "OAI") == 0) || 
      (strcmp(name, "MacroType_OAI") == 0))
      mt = MacroType_OAI;
    else
    if ((strcmp(name, "NAND") == 0) || 
      (strcmp(name, "MacroType_NAND") == 0))
      mt = MacroType_NAND;
    else
    if ((strcmp(name, "NOR") == 0) || 
      (strcmp(name, "MacroType_NOR") == 0))
      mt = MacroType_NOR;
    else
    if ((strcmp(name, "OR") == 0) || 
      (strcmp(name, "MacroType_OR") == 0))
      mt = MacroType_OR;
    else
    if ((strcmp(name, "XNOR") == 0) || 
      (strcmp(name, "MacroType_XNOR") == 0))
      mt = MacroType_XNOR;
    else
    if ((strcmp(name, "XOR") == 0) || 
      (strcmp(name, "MacroType_XOR") == 0))
      mt = MacroType_XOR;
    else
    if ((strcmp(name, "TBUF") == 0) || 
      (strcmp(name, "MacroType_TBUF") == 0))
      mt = MacroType_TBUF;
    else
    if ((strcmp(name, "TINV") == 0) || 
      (strcmp(name, "MacroType_TINV") == 0))
      mt = MacroType_TINV;
    else
    if ((strcmp(name, "INV") == 0) || 
      (strcmp(name, "MacroType_INV") == 0))
      mt = MacroType_INV;
    else
    if ((strcmp(name, "BUF") == 0) || 
      (strcmp(name, "MacroType_BUF") == 0))
      mt = MacroType_BUF;
    else
      mt = MacroType_Default;

  return mt;
  }

  void CalcElementLD(HDesign& design, HMacroType elementType, double* L, double* D)
  {
    *L = *D = 0;

    if (::IsNull(elementType))
      return;

    double T;
    double R;
    double C;    

    CalcElementTRC(design, elementType, &T, &R, &C);

    *L = R / design.RoutingLayers.Physics.RPerDist + C / design.RoutingLayers.Physics.CPerDist;
    *D = sqrt(2.0 * (T + R * C) / design.RoutingLayers.Physics.RPerDist / design.RoutingLayers.Physics.CPerDist);

    return;
  }

  void CalcElementTRC(HDesign& design, HMacroType elementType, double* T, double* R, double* C)
  {
    *T = *R = 0.0;

    if (::IsNull(elementType))
      return;

    for (HMacroType::PinsEnumeratorW pinType = design.Get<HMacroType::PinTypesEnumerator, 
         HMacroType::PinsEnumeratorW>(elementType); pinType.MoveNext(); )
    {
      if (pinType.Direction() == PinDirection_INPUT)
        *C = pinType.Capacitance();

      if (pinType.Direction() == PinDirection_OUTPUT)
      {
        int nArcs = 0;

        for (HPinType::ArcsEnumeratorW arc = pinType.GetArcsEnumeratorW(); arc.MoveNext(); )
        {
          TimingType tt = arc.TimingType();
          if (arc.TimingType() == TimingType_Combinational ||
            arc.TimingType() == TimingType_RisingEdge) //NOTE:FIXME: this is workaround of issue 141
          {
            *T += arc.TIntrinsicFall() + arc.TIntrinsicRise();
            *R += arc.ResistanceFall() + arc.ResistanceRise();
            nArcs += 2;
          }
        }

        *T /= nArcs;
        *R /= nArcs;

        return;
      }
    }
  }

  void CalcBufferLD(HDesign& design, const char* name, double* L, double* D)
  {
    if (name == 0)
      name = design.cfg.ValueOf("GlobalPlacement.bufferName", "INVX1");
    CalcElementLD(design, FindMacroTypeByName(design, name), L, D);
  }
}