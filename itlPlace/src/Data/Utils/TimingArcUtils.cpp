#include "TimingArcUtils.h"

namespace Utils
{
  TimingSense GetTimingSense(const char* sense)
  {
    if (sense == 0)
      return TimingSense_Undefined;
    else if (strcmp(sense, "positive_unate") == 0)
      return TimingSense_PositiveUnate;
    else if (strcmp(sense, "negative_unate") == 0)
      return TimingSense_NegativeUnate;
    else if (strcmp(sense, "non_unate") == 0)
      return TimingSense_NonUnate;
    else
      return TimingSense_Default;
  }

  const char* GetTimingSenseStr(TimingSense sense)
  {
    switch (sense)
    {
    case TimingSense_Undefined:
      return "undefined";
    case TimingSense_PositiveUnate:
      return "positive_unate";
    case TimingSense_NegativeUnate:
      return "negative_unate";
    case TimingSense_NonUnate:
      return "non_unate";
    default:
      GLOGERROR(LOGINPLACE, "Unknown timing sense: %d", sense);
      return "unknown";
    }
  }

  TimingType GetTimingType(const char* typeStr)
  {
    if (typeStr == 0)
      return TimingType_Default;
    else if (strcmp(typeStr, "hold_rising") == 0)
      return TimingType_HoldRising;
    else if (strcmp(typeStr, "setup_rising") == 0)
      return TimingType_SetupRising;
    else if (strcmp(typeStr, "hold_falling") == 0)
      return TimingType_HoldFalling;
    else if (strcmp(typeStr, "setup_falling") == 0)
      return TimingType_SetupFalling;
    else if (strcmp(typeStr, "falling_edge") == 0)
      return TimingType_FallingEdge;
    else if (strcmp(typeStr, "rising_edge") == 0)
      return TimingType_RisingEdge;
    else if (strcmp(typeStr, "preset") == 0)
      return TimingType_Preset;
    else if (strcmp(typeStr, "clear") == 0)
      return TimingType_Clear;
    else if (strcmp(typeStr, "recovery_rising") == 0)
      return TimingType_RecoveryRising;
    else if (strcmp(typeStr, "recovery_falling") == 0)
      return TimingType_RecoveryFalling;
    else if (strcmp(typeStr, "removal_rising") == 0)
      return TimingType_RemovalRising;
    else if (strcmp(typeStr, "removal_falling") == 0)
      return TimingType_RemovalFalling;
    else if (strcmp(typeStr, "three_state_enable") == 0)
      return TimingType_3StateEnable;
    else if (strcmp(typeStr, "three_state_disable") == 0)
      return TimingType_3StateDisable;
    else if (strcmp(typeStr, "nochange_high") == 0)
      return TimingType_NochangeHigh;
    else if (strcmp(typeStr, "nochange_low") == 0)
      return TimingType_NochangeLow;
    else
      return TimingType_Default;
  }

  const char* GetTimingTypeStr(TimingType ttype)
  {
    switch (ttype)
    {
    case TimingType_Combinational:
      return "default";
    case TimingType_HoldRising:
      return "hold _rising";
    case TimingType_SetupRising:
      return "setup_rising";
    case TimingType_HoldFalling:
      return "hold_falling";
    case TimingType_SetupFalling:
      return "setup_falling";
    case TimingType_FallingEdge:
      return "falling_edge";
    case TimingType_RisingEdge:
      return "rising_edge";
    case TimingType_Preset:
      return "preset";
    case TimingType_Clear:
      return "clear";
    case TimingType_RecoveryRising:
      return "recovery_rising";
    case TimingType_RecoveryFalling:
      return "recovery_falling";
    case TimingType_RemovalRising:
      return "removal_rising";
    case TimingType_RemovalFalling:
      return "removal_falling";
    case TimingType_3StateEnable:
      return "three_state_enable";
    case TimingType_3StateDisable:
      return "three_state_disable";
    case TimingType_NochangeHigh:
      return "nochange_high";
    case TimingType_NochangeLow:
      return "nochange_low";
    default:
      GLOGERROR(LOGINPLACE, "Unknown timing type: %d", ttype);
      return "unknown";
    }
  }

  HTimingArcType FindTimingArc(HDesign& hd, HPinType startPin, HPinType endPin)
  {
    for (HPinType::ArcsEnumeratorW arc = hd[endPin].GetArcsEnumeratorW(); arc.MoveNext(); )
    {
      if (arc.GetStartPinType(endPin) != startPin)
        continue;

      if (arc.TimingType() == TimingType_Combinational || arc.TimingType() == TimingType_RisingEdge)
        return arc;
    }
    return hd.TimingArcTypes.Null();
  }
}