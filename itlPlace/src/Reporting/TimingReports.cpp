#include "Reporting.h"
#include "Utils.h"
#include "Auxiliary.h"
#include "limits.h"
#include "Timing.h"
#include <math.h>

void ReportNegativeSlacks(HDesign& design)
{
  ALERT("Reporting: circuit negative slacks");
  ALERTFORMAT(("  TNS: %f", Utils::TNS(design)));
  ALERTFORMAT(("  WNS: %f", Utils::WNS(design)));
}

void PrintNetInfo(HDesign& hd, HNet net)
{
  HNetWrapper nw = hd[net];
  for (HNet::PinsEnumeratorW pin = nw.GetPinsEnumeratorW(); pin.MoveNext(); )
  {
    HCellWrapper cell = hd[pin.Cell()];
    HSteinerPointWrapper st = hd[hd.SteinerPoints[pin]];
    HTimingPointWrapper tp = hd[hd.TimingPoints[pin]];
    WRITELINE("  %8s %3s %4d %.10f %.10f %.10f %.10f %.10f %.10f",
      pin.IsPrimary() ? "PIN" : cell.Name().c_str(),
      pin.Name().c_str(),
      ::ToID(pin),
      st.FallPathDelay(),
      st.RisePathDelay(),
      tp.FallArrivalTime(),
      tp.RiseArrivalTime(),
      tp.FallRequiredTime(),
      tp.RiseRequiredTime()
      );
  }
}

void PrintPointInfo(HDesign& hd, HCriticalPathPointWrapper& j, int pointNumber)
{
  HTimingPointWrapper tp = hd[j.TimingPoint()];
  HPinWrapper pin = hd[tp.Pin()];
  HNetWrapper net = hd[pin.Net()];
  HCellWrapper cell = hd[pin.Cell()];
  HMacroTypeWrapper mt = hd[cell.Type()];
  HPinTypeWrapper pt = hd[pin.Type()];
  HSteinerPointWrapper sp = hd[hd.SteinerPoints[pin]];

  WRITE("Point #%d   %s    ",
    pointNumber,
    j.SignalDirection() == SignalDirection_Fall ? "Fall" : "Rise");

  if(pin.IsPrimary())
    WRITE("PIN   "); 
  else
    WRITE("%s (%s)   ", cell.Name().c_str(), mt.Name().c_str());

  WRITELINE("%s (%s)    [%.2f , %.2f])",
    pin.Name().c_str(),
    Utils::GetPinDirectionStr(pin.Direction()),
    pin.X(),
    pin.Y());

  WRITELINE("  C = %.10f ( fall = %.10f , rise = %.10f )",
    pt.Capacitance(),
    pt.FallCapacitance(),
    pt.RiseCapacitance());
  WRITELINE("");
  WRITELINE("      \tArrival      \tRequired       \tNegative Slack");
  WRITELINE("  Fall\t%.10f\t%.10f\t%.10f",
    tp.FallArrivalTime(),
    tp.FallRequiredTime(),
    tp.FallNegativeSlack());
  WRITELINE("  Rise\t%.10f\t%.10f\t%.10f",
    tp.RiseArrivalTime(),
    tp.RiseRequiredTime(),
    tp.RiseNegativeSlack());
  WRITELINE("");

  {
    HTimingPointWrapper a = hd[tp.FallArrivalAncestor()];
    HPinWrapper p = hd[a.Pin()];
    HCellWrapper c = hd[p.Cell()];
    WRITELINE("  Fall signal comes from  %s %s: %.10f",
      p.IsPrimary() ? "PIN" : c.Name().c_str(),
      p.Name().c_str(),
      a.FallArrivalTime()
      );
  }
  {
    HTimingPointWrapper a = hd[tp.RiseArrivalAncestor()];
    HPinWrapper p = hd[a.Pin()];
    HCellWrapper c = hd[p.Cell()];
    WRITELINE("  Rise signal comes from  %s %s: %.10f",
      p.IsPrimary() ? "PIN" : c.Name().c_str(),
      p.Name().c_str(),
      a.RiseArrivalTime()
      );
  }
  {
    HTimingPointWrapper a = hd[tp.FallRequiredAncestor()];
    HPinWrapper p = hd[a.Pin()];
    HCellWrapper c = hd[p.Cell()];
    WRITELINE("  Fall signal goes to     %s %s: %.10f",
      p.IsPrimary() ? "PIN" : c.Name().c_str(),
      p.Name().c_str(),
      a.FallRequiredTime()
      );
  }
  {
    HTimingPointWrapper a = hd[tp.RiseRequiredAncestor()];
    HPinWrapper p = hd[a.Pin()];
    HCellWrapper c = hd[p.Cell()];
    WRITELINE("  Rise signal goes to     %s %s: %.10f",
      p.IsPrimary() ? "PIN" : c.Name().c_str(),
      p.Name().c_str(),
      a.RiseRequiredTime()
      );
  }
  WRITELINE("\n");
}

void PrintNetArc(HDesign& hd, HCriticalPathPointWrapper& pt1, HCriticalPathPointWrapper& pt2)
{
  HTimingPointWrapper tp1 = hd[pt1.TimingPoint()];
  HPinWrapper pin1 = hd[tp1.Pin()];
  HCellWrapper cell1 = hd[pin1.Cell()];
  //HSteinerPointWrapper sp1 = hd[hd.SteinerPoints[pin1]];

  HTimingPointWrapper tp2 = hd[pt2.TimingPoint()];
  HPinWrapper pin2 = hd[tp2.Pin()];
  HCellWrapper cell2 = hd[pin2.Cell()];
  HSteinerPointWrapper sp2 = hd[hd.SteinerPoints[pin2]];

  HNetWrapper net = hd[pin1.Net()];

  WRITELINE("NetArc   %s   {%s %s  ->  %s %s}",
    net.Name().c_str(),
    pin1.IsPrimary() ? "PIN" : cell1.Name().c_str(),
    pin1.Name().c_str(),
    pin2.IsPrimary() ? "PIN" : cell2.Name().c_str(),
    pin2.Name().c_str());
  WRITELINE("  PinsInNet = %d", net.PinsCount());
  WRITELINE("  HPWL      = %.2f", Utils::CalcNetHPWL(hd, net));
  if (pt1.SignalDirection() == SignalDirection_Fall)
    WRITELINE("  FallDelay = %.10f", sp2.FallPathDelay());
  else
    WRITELINE("  RiseDelay = %.10f", sp2.RisePathDelay());

  //PrintNetInfo(hd, net);
  WRITELINE("\n");
}

void PrintCellArc(HDesign& hd, HCriticalPathPointWrapper& pt1, HCriticalPathPointWrapper& pt2)
{
  HTimingPointWrapper tp1 = hd[pt1.TimingPoint()];
  HPinWrapper pin1 = hd[tp1.Pin()];
  HNetWrapper net1 = hd[pin1.Net()];
  HSteinerPointWrapper sp1 = hd[hd.SteinerPoints[pin1]];

  HTimingPointWrapper tp2 = hd[pt2.TimingPoint()];
  HPinWrapper pin2 = hd[tp2.Pin()];
  HNetWrapper net2 = hd[pin2.Net()];
  HSteinerPointWrapper sp2 = hd[hd.SteinerPoints[pin2]];

  HCellWrapper cell = hd[pin1.Cell()];

  WRITELINE("CellArc  %s  {%s ->  %s}",
    cell.Name().c_str(),
    //::ToID(cell),
    pin1.Name().c_str(),
    //::ToID(pin1),
    pin2.Name().c_str()
    //::ToID(pin2)
    );

  HTimingArcType arrival_arc;
  HTimingArcType required_arc;
  bool is_arrival_inverted;
  bool is_required_inverted;
  double arrival_arc_time;
  double required_arc_time;

  if (pt2.SignalDirection() == SignalDirection_Rise)
  {
    GetArrivalRisingArc(hd, tp1, tp2, &arrival_arc, &arrival_arc_time, &is_arrival_inverted);
    GetRequiredRisingArc(hd, tp1, tp2, &required_arc, &required_arc_time, &is_required_inverted);
  }
  else
  {
    GetArrivalFallingArc(hd, tp1, tp2, &arrival_arc, &arrival_arc_time, &is_arrival_inverted);
    GetRequiredFallingArc(hd, tp1, tp2, &required_arc, &required_arc_time, &is_required_inverted);
  }

  HTimingArcTypeWrapper aarc = hd[arrival_arc];
  HTimingArcTypeWrapper rarc = hd[required_arc];

  if (aarc.GetStartPin(pin2) == pin1 && aarc.GetEndPin(pin1) == pin2)
  {
    if (rarc == aarc)
      WRITELINE("  Timing Arc:");
    else
      WRITELINE("  Arrival Timing Arc:");

    WRITELINE("    TimingType     = %s", Utils::GetTimingTypeStr(aarc.TimingType()));
    WRITELINE("    TimingSense    = %s", Utils::GetTimingSenseStr(aarc.TimingSense()));
    WRITELINE("    SignalInversed = %s", is_arrival_inverted ? "True" : "False");
    WRITELINE("    ArcDelay(%s) = %.10f", pt2.SignalDirection() == SignalDirection_Fall ? "fall" : "rise", arrival_arc_time);
    WRITELINE("");
    WRITELINE("        \tTIntrinsic  \tResistance  \tObservedC   \tDelay");
    WRITELINE("    Fall\t%.10f\t%.10f\t%.10f\t%.10f",
      aarc.TIntrinsicFall(),
      aarc.ResistanceFall(),
      sp2.FallObservedC(),
      aarc.TIntrinsicFall() + aarc.ResistanceFall() * sp2.FallObservedC());
    WRITELINE("    Rise\t%.10f\t%.10f\t%.10f\t%.10f",
      aarc.TIntrinsicRise(),
      aarc.ResistanceRise(),
      sp2.RiseObservedC(),
      aarc.TIntrinsicRise() + aarc.ResistanceRise() * sp2.RiseObservedC());
  }

  if (rarc.GetStartPin(pin2) == pin1 && rarc.GetEndPin(pin1) == pin2 && aarc != rarc)
  {
    WRITELINE("\n  Required Timing Arc:");

    WRITELINE("    TimingType     = %s", Utils::GetTimingTypeStr(rarc.TimingType()));
    WRITELINE("    TimingSense    = %s", Utils::GetTimingSenseStr(rarc.TimingSense()));
    WRITELINE("    SignalInversed = %s", is_required_inverted ? "True" : "False");
    WRITELINE("    ArcDelay(%s) = %.10f", pt2.SignalDirection() == SignalDirection_Fall ? "fall" : "rise", required_arc_time);
    WRITELINE("");
    WRITELINE("        \tTIntrinsic  \tResistance  \tObservedC   \tDelay");
    WRITELINE("    Fall\t%.10f\t%.10f\t%.10f\t%.10f",
      rarc.TIntrinsicFall(),
      rarc.ResistanceFall(),
      sp2.FallObservedC(),
      rarc.TIntrinsicFall() + rarc.ResistanceFall() * sp2.FallObservedC());
    WRITELINE("    Rise\t%.10f\t%.10f\t%.10f\t%.10f",
      rarc.TIntrinsicRise(),
      rarc.ResistanceRise(),
      sp2.RiseObservedC(),
      rarc.TIntrinsicRise() + rarc.ResistanceRise() * sp2.RiseObservedC());
  }

  WRITELINE("\n");
}

bool IsOneNet(HDesign& hd, HCriticalPathPointWrapper& first, HCriticalPathPointWrapper& second)
{
  return hd.Get<HPin::Net, HNet>(hd.Get<HTimingPoint::Pin, HPin>(first.TimingPoint()))
    == hd.Get<HPin::Net, HNet>(hd.Get<HTimingPoint::Pin, HPin>(second.TimingPoint()));
}

bool IsOneCell(HDesign& hd, HCriticalPathPointWrapper& first, HCriticalPathPointWrapper& second)
{
  return hd.Get<HPin::Cell, HCell>(hd.Get<HTimingPoint::Pin, HPin>(first.TimingPoint()))
    == hd.Get<HPin::Cell, HCell>(hd.Get<HTimingPoint::Pin, HPin>(second.TimingPoint()));
}

void ReportOnePath(HDesign& hd, const HCriticalPathWrapper& path, int pathNumber)
{
  WRITELINE("================================================================================");
  WRITELINE("");
  HPinWrapper firstPin = hd[hd[hd[path.StartPoint()].TimingPoint()].Pin()];
  HPinWrapper lastPin = hd[hd[hd[path.EndPoint()].TimingPoint()].Pin()];

  HCellWrapper firstCell = hd[firstPin.Cell()];
  HCellWrapper lastCell = hd[lastPin.Cell()];

  WRITELINE("Path #%d %10s %s %s %8s %s %s", 
    pathNumber, 
    "From   ",
    firstPin.IsPrimary() ? "PIN" : firstCell.Name().c_str(),
    firstPin.Name().c_str(),
    "To   ",
    lastPin.IsPrimary() ? "PIN" : lastCell.Name().c_str(),
    lastPin.Name().c_str());

  WRITELINE("#Points = %d       Slack = %.10f", path.PointsCount(), path.Criticality());
  WRITELINE("");

  int pointNumber=1;
  HCriticalPathPointWrapper prev = hd[hd.CriticalPathPoints.Null()];
  for(HCriticalPath::PointsEnumeratorW j = path.GetEnumeratorW(); j.MoveNext(); ++pointNumber)
  {
    if (!::IsNull(prev))
      if (IsOneNet(hd, prev, j))
        PrintNetArc(hd, prev, j);
      else if (IsOneCell(hd, prev, j))
        PrintCellArc(hd, prev, j);
      else
      {
        LOGERROR("Critical path points is disconnected.");
      }
      prev = j;

      PrintPointInfo(hd, j, pointNumber);
  }
}

void ReportCriticalPathsToLogFile(HDesign& hd, int nPaths)
{
  WRITELINE("");
  ALERT("Critical paths printing started...");

  if (nPaths == -1)
    nPaths = INT_MAX; //we will see all critical paths

  int countCP = hd.CriticalPaths.Count();
  std::vector<CriticalPathsCriticality> vec(countCP);
  int idx = 0;
  for(HCriticalPaths::EnumeratorW i = hd.CriticalPaths.GetEnumeratorW();
    i.MoveNext(); idx++)
  {
    vec[idx].criticality = i.Criticality();
    vec[idx].path = i;
  }

  std::sort(vec.begin(), vec.end(), NameSortCPC());

  int pathNumber=1;

  for(int k = 0; k < nPaths && k < countCP; ++k)
    ReportOnePath(hd, hd[vec[k].path], k + 1);

  if (min(nPaths, countCP))
    WRITELINE("");
  ALERTFORMAT(("Reported %d paths of %d", min(nPaths, countCP), countCP));
  ALERT("Critical paths printing finished!");  
}

void PlotCriticalPathsAndSaveImages(HDesign& design)
{
  int pathNumber=0;
  char charNumber[100];
  charNumber[0] = 0;
  char buf[100];
  string strCriticality(buf);
  for(HCriticalPaths::Enumerator i = design.CriticalPaths.GetEnumerator();
    i.MoveNext(); ++pathNumber)
  {
    design.Plotter.Clear();
    sprintf(charNumber, "%d", pathNumber);
    string strNumber(charNumber);
    design.Plotter.PlotPlacement();
    design.Plotter.PlotCriticalPath(i, "critical_path_" + strNumber );
    buf[0] = 0;
    HCriticalPathWrapper criticalPathW = design[i];
    double criticality = criticalPathW.Criticality();
    sprintf(buf, "%f", criticality);
    strCriticality = buf;
    design.Plotter.PlotText("critical path " + strNumber + " " + strCriticality);
    design.Plotter.Refresh( ((HPlotter::WaitTime)design.cfg.lookforDefValue("CriticalPaths.plotWait", 1)));
    string picturesDefaultDirectory = design.cfg.lookfor("plotter.pixDirectory");
    design.Plotter.SaveImage(".\\pix\\critical_path_" + strNumber, 
      picturesDefaultDirectory + design.Circuit.Name() + "_critical_path\\");
  }
}

void PlotTopCriticalPathsAndSaveImages(HDesign& design, int n)
{
  if (n == -1)
    n = INT_MAX; //we will see all critical paths

  int countCP = design.CriticalPaths.Count();
  std::vector<CriticalPathsCriticality> vec(countCP);
  int idx = 0;
  for(HCriticalPaths::EnumeratorW i = design.CriticalPaths.GetEnumeratorW();
    i.MoveNext(); idx++)
  {
    vec[idx].criticality = i.Criticality();
    vec[idx].path = i;
  }

  std::sort(vec.begin(), vec.end(), NameSortCPC());

  char charNumber[100];
  charNumber[0] = 0;
  char buf[100];
  string strCriticality(buf);
  for(int k = 0; k < n && k < countCP; ++k)
  {
    sprintf(charNumber, "%d", k);
    string strNumber(charNumber);
    design.Plotter.Clear();
    design.Plotter.PlotPlacement();
    design.Plotter.PlotCriticalPath(vec[k].path, "critical_path_" + strNumber);
    buf[0] = 0;
    HCriticalPathWrapper criticalPathW = design[vec[k].path];
    double criticality = criticalPathW.Criticality();
    sprintf(buf, "%f", criticality);
    strCriticality = buf;
    design.Plotter.PlotText("critical path " + strNumber + " " + strCriticality);
    design.Plotter.Refresh( ((HPlotter::WaitTime)design.cfg.lookforDefValue("CriticalPaths.plotWait", 1)));
    string picturesDefaultDirectory = design.cfg.lookfor("plotter.pixDirectory");
    design.Plotter.SaveImage("critical_path_" + strNumber, 
      picturesDefaultDirectory + design.Circuit.Name() + "_critical_path\\");
  }
}