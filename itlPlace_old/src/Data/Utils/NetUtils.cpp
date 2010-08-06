#include "NetUtils.h"
#include "TimingPointUtils.h"
#include "TimingUtils.h"
#include "Reporting.h"

namespace Utils
{
  double CalcNetHPWL(HDesign& hd, HNet net)
  {
    LNetList& netlist = hd._Design.NetList;
    IDType pinsStart = netlist.netPinStart[::ToID(net)];
    IDType pinsEnd = netlist.netPinEnd[::ToID(net)];

    if(pinsEnd == pinsStart + 2)
      return fabs(hd.GetDouble<HPin::Y>(::__ConstructPtr<HPin>(netlist.netPins[pinsStart]))
        - hd.GetDouble<HPin::Y>(::__ConstructPtr<HPin>(netlist.netPins[pinsStart + 1])))
        + fabs(hd.GetDouble<HPin::X>(::__ConstructPtr<HPin>(netlist.netPins[pinsStart]))
        - hd.GetDouble<HPin::X>(::__ConstructPtr<HPin>(netlist.netPins[pinsStart + 1])));

    double top = hd.GetDouble<HPin::Y>(::__ConstructPtr<HPin>(netlist.netPins[pinsStart]));
    double bottom = top;
    double left = hd.GetDouble<HPin::X>(::__ConstructPtr<HPin>(netlist.netPins[pinsStart]));
    double right = left;

    for (IDType idx = pinsStart + 1; idx < pinsEnd; idx++)
    {
      double x = hd.GetDouble<HPin::X>(::__ConstructPtr<HPin>(netlist.netPins[idx]));
      if (left > x)
        left = x;
      else if (right < x)
        right = x;

      double y = hd.GetDouble<HPin::Y>(::__ConstructPtr<HPin>(netlist.netPins[idx]));
      if (top < y)
        top = y;
      else
        if (bottom > y)
          bottom = y;
    }
    
    return right - left + top - bottom;
  }

  double CalculateHPWL(HDesign& hd, bool updateCachedValues)
  {
    double hpwl = 0.0;
    int nNetsEnd = hd._Design.NetList.nNetsEnd;

    if (updateCachedValues)
    {
      for (HNets::NetsEnumerator nIter = hd.Nets.GetFullEnumerator(); nIter.MoveNext(); )
        if (hd.Get<HNet::Kind, NetKind>(nIter) == NetKind_Active)
          hd.Set<HNet::HPWL>(nIter, CalcNetHPWL(hd, nIter));
        else
          hd.Set<HNet::HPWL>(nIter, 0.0);
      for (int i = 1; i < nNetsEnd; i++)
        hpwl += hd._Design.NetList.netHPWL[i];
    }
    else
    {
      for (HNets::NetsEnumerator nIter = hd.Nets.GetFullEnumerator(); nIter.MoveNext(); )
        if (hd.Get<HNet::Kind, NetKind>(nIter) == NetKind_Active)
          hpwl += CalcNetHPWL(hd, nIter);
    }
    
    return hpwl;
  }

  double CalculateTWL(HDesign& hd)
  {
    double twl = 0.0;
    for (HNets::NetsEnumerator nIter = hd.Nets.GetFullEnumerator(); nIter.MoveNext(); )
      if (hd.Get<HNet::Kind, NetKind>(nIter) == NetKind_Active)
        twl += hd.GetDouble<HWire::Length>(hd.Wires[nIter]);
    return twl;
  }

  double CalculateWeightedHPWL(HDesign& hd, bool updateCachedValues)
  {
    double hpwl = 0.0;
    int nNetsEnd = hd._Design.NetList.nNetsEnd;

    if (updateCachedValues)
    {
      for (HNets::NetsEnumerator nIter = hd.Nets.GetFullEnumerator(); nIter.MoveNext(); )
        if (hd.Get<HNet::Kind, NetKind>(nIter) == NetKind_Active)
          hd.Set<HNet::HPWL>(nIter, CalcNetHPWL(hd, nIter));
        else
          hd.Set<HNet::HPWL>(nIter, 0.0);
      for (int i = 1; i < nNetsEnd; i++)
        hpwl += hd._Design.NetList.netWeight[i] * hd._Design.NetList.netHPWL[i];
    }
    else
    {
      HNets::NetsEnumeratorW netW = hd.Nets.GetFullEnumeratorW();
      for (HNets::NetsEnumerator nIter = hd.Nets.GetFullEnumerator(); nIter.MoveNext(), netW.MoveNext(); )
      {
        if (hd.Get<HNet::Kind, NetKind>(nIter) == NetKind_Active)
          hpwl += netW.Weight() * CalcNetHPWL(hd, nIter);
      }
    }

    return hpwl;
  }

  double CalculateHPWLDiff(HDesign& hd, const HNet* nets, int nNets, bool updateCachedValues)
  {
    double dhpwl = 0.0;
    for (int i = 0; i < nNets; i++)
    {
      double netWL = CalcNetHPWL(hd, nets[i]);
      dhpwl += netWL - hd.GetDouble<HNet::HPWL>(nets[i]);
      if (updateCachedValues)
        hd.Set<HNet::HPWL>(nets[i], netWL);
    }
    return dhpwl;
  }

  double CalculateHPWLDiff(HDesign& hd, const HCell* cells, int nCells, bool updateCachedValues)
  {
    double dhpwl = 0.0;
    std::vector<HNet> nets;
    nets.reserve(nCells * 2);

    for (int i = 0; i < nCells; i++)
    {
      for (HCell::PinsEnumerator pe = hd.Get<HCell::Pins, HCell::PinsEnumerator>(cells[i]); pe.MoveNext(); )
      {
        HNet current_net = hd.Get<HPin::Net, HNet>(pe);
        if (!::IsNull(current_net))
          if (find(nets.begin(), nets.end(), current_net) == nets.end())
          {
            nets.push_back(current_net);
            double netWL = CalcNetHPWL(hd, current_net);
            dhpwl += netWL - hd.GetDouble<HNet::HPWL>(current_net);
            if (updateCachedValues)
              hd.Set<HNet::HPWL>(current_net, netWL);
          }
      }
    }

    return dhpwl;
  }

  static void UnbindNet(HDesign& hd, HNet net, NetKind unbind_kind)
  {
    if (hd.Get<HNet::Kind, NetKind>(net) == NetKind_Active)
    {
      hd.Set<HNet::Kind>(net, unbind_kind);
      for (HNet::PinsEnumeratorW pin = hd.Get<HNet::Pins, HNet::PinsEnumeratorW>(net); pin.MoveNext(); )
      {
        if (pin.Net() == net)
        {
          pin.SetNet(hd.Nets.Null());
        }
      }
    }
  }

  void SkipNet(HDesign& hd, HNet net)
  {
    UnbindNet(hd, net, NetKind_Skipped);
  }

  void RemoveNet(HDesign& hd, HNet net)
  {
    UnbindNet(hd, net, NetKind_Removed);
  }

  static bool IsClockNet(HNetWrapper& net, HDesign& hd)
  {
    for (HNet::PinsEnumeratorW pin = net.GetPinsEnumeratorW(); pin.MoveNext(); )
    {
      //MacroType mtype = hd.Get<HMacroType::Type, MacroType>(hd.Get<HCell::MacroType, HMacroType>(pin.Cell()));
      //if (mtype >= MacroType_FirstSpecial && mtype <= MacroType_LastSpecial)
      //  return true;
      PinFunction func = hd.Get<HPinType::Function, PinFunction>(pin.Type());
      if (func == PinFunction_Clock)
        return true;
    }
    return false;
  }

  static bool IsPureClearPresetNet(HNetWrapper& net, HDesign& hd)
  {
    for (HNet::SinksEnumeratorW pin = net.GetSinksEnumeratorW(); pin.MoveNext(); )
    {
      PinFunction func = hd.Get<HPinType::Function, PinFunction>(pin.Type());
      if (func != PinFunction_Clear && func != PinFunction_Preset)
        return false;
    }
    return true;
  }

  void SkipSpecialNets(HDesign& hd)
  {
    int nSkipped = 0;
    ALERT("NETS SKIPPING STARTED...");
    for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
    {
      if (IsClockNet(net, hd) || IsPureClearPresetNet(net, hd))
      {
        if (hd.cfg.ValueOf("Timing.reportSkippedNets", false))
          ReportNetPins(hd, net);
        SkipNet(hd, net);
        ALERT("\tNet \"%s\" skipped", net.Name().c_str());
        nSkipped++;
      }
    }
    ALERT("\tSkipped %d nets", nSkipped);
    ALERT("NETS SKIPPING FINISHED...");
  }

  int CalcOutputPinsCount(HNetWrapper& net, HDesign& hd)
  {
    int count = 0;
    for(HNet::PinsEnumeratorW pin = net.GetPinsEnumeratorW(); pin.MoveNext(); )
      if (pin.Direction() != PinDirection_INPUT)
        count++;
    return count;
  }

  void SkipBrokenNets(HDesign& hd)
  {
    int nSkipped = 0;
    ALERT("BROKEN NETS SKIPPING STARTED...");
    for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
    {
      bool skip = false;

      HPinTypeWrapper ptype = (hd,(net.Source(),hd).Type());
      if(::IsNull(ptype))
        skip = (net.Source(),hd).Direction() != PinDirection_OUTPUT;
      else
        skip = ptype.Direction() != PinDirection_OUTPUT;

      if (skip || net.PinsCount() < 2 || CalcOutputPinsCount(net,hd) > 1)
      {
        if (hd.cfg.ValueOf("Timing.reportSkippedNets", false))
          ReportNetPins(hd, net);
        SkipNet(hd, net);
        ALERT("\tNet \"%s\" skipped", net.Name().c_str());
        nSkipped++;
      }
    }
    ALERT("\tSkipped %d nets", nSkipped);
    ALERT("BROKEN NETS SKIPPING FINISHED...");
  }

  static void RemoveRepeatersTree(HDesign& design, HNet originalNet, HNet netToRemove, HPin bufferInput)
  {
    HPin source = design.Get<HNet::Source, HPin>(netToRemove);
    if (design.Get<HPin::OriginalNet, HNet>(source) != originalNet)
    {
      Utils::DeletePointInList(design, design.TimingPoints[bufferInput]);
      Utils::DeletePointInList(design, design.TimingPoints[source]);
      design.Set<HCell::PlacementStatus>(design.Get<HPin::Cell, HCell>(source), PlacementStatus_Fictive);
    }

    for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(netToRemove); sink.MoveNext(); )
      if (sink.OriginalNet() != originalNet)
      {
        ASSERT(MacroType_BUF == (design.Get<HMacroType::Type, MacroType>(design.Get<HCell::MacroType, HMacroType>(sink.Cell()))));
        for (HCell::PinsEnumeratorW cpin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(sink.Cell()); cpin.MoveNext(); )
          if (cpin.Direction() == PinDirection_OUTPUT && cpin.Net() != design.Nets.Null())
            RemoveRepeatersTree(design, originalNet, cpin.Net(), sink);
      }

    Utils::RemoveRouting(design, netToRemove);
    Utils::RemoveNet(design, netToRemove);
  }

  void RestoreBufferedNet(HDesign& design, HNet oldNet)
  {
    if (design.Get<HNet::Kind, NetKind>(oldNet) != NetKind_Buffered) return;
    RemoveRepeatersTree(design, oldNet, design.Get<HPin::Net, HNet>(design.Get<HNet::Source, HPin>(oldNet)), design.Pins.Null());
    design.Set<HNet::Kind>(oldNet, NetKind_Active);

    for (HNet::PinsEnumeratorW pin = design.Get<HNet::Pins, HNet::PinsEnumeratorW>(oldNet); pin.MoveNext(); )
    {
      ASSERT(::IsNull(pin.Net()));
      ASSERT(pin.OriginalNet() == oldNet);
      pin.SetNet(oldNet);
    }
  }

  void CalculateLNets(HDesign& hd)
  {
    const SignalDirection sigDir = SignalDirection_Average;

    for (HNets::NetsEnumeratorW net = hd.Nets.GetFullEnumeratorW(); net.MoveNext(); )
      net.SetLNet(Utils::LNet(
        Utils::GetDriverWorstPhisics(hd, net.Source(), sigDir),
        GetNetLoad(hd, net, sigDir),
        hd.RoutingLayers.Physics));
  }

  void RemoveRouting(HDesign& aDesign, HNet& aNet)
  {
    HWireWrapper wire = aDesign[aDesign.Wires[aNet]];
    if (wire.RoutingType() != RoutingType_Unrouted)
      aDesign.SteinerPoints.RemoveSteinerTree(wire.Root());
  }
}