#include "Reporting.h"
#include "Utils.h"
#include "Auxiliary.h"
#include "limits.h"
#include "Timing.h"
#include <math.h>
#include "TableFormatter.h"

int PikoSec(double delay_ns)
{
  return Round(1000.0 * delay_ns);
}

string FormatArcName(HDesign& design, HPin endPin, HTimingArcType arc)
{
  if (design[endPin].IsPrimary())
    return "PIN";
  HPin startPin = design.TimingArcTypes.GetStartPin(arc, endPin);
  HCell cell = design.Get<HPin::Cell, HCell>(endPin);
  string typeName = design.GetString<HMacroType::Name>(design.Get<HCell::MacroType, HMacroType>(cell));
  TimingType tt = design[arc].TimingType();

  if (tt == TimingType_SetupFalling || tt == TimingType_SetupRising)
    return string("(setup) ") + typeName + "%" + design[endPin].Name();
  else
    return typeName + "%"+ design.GetString<HPin::Name>(startPin) + "->" + design.GetString<HPin::Name>(endPin);
}


void PrintPath(HDesign& design, HCriticalPath path, int pathNumber)
{
  //columns numbers
  int sigDirections = design.cfg.ValueOf("Timing.SignalDirectionsUsed", 2);

  int ColAAT = 0;
  int ColRAT = ColAAT + 1;
  int ColGate = ColRAT + 1;
  int ColRC = ColGate + 1;
  int ColPhase = sigDirections == 2 ? ColRC + 1 : -1;
  int ColCap = ColRC + sigDirections;
  int ColFanout = ColCap + 1;
  int ColArcName = ColFanout + 1;
  int ColCellName = ColArcName + 1;
  int ColSep1 = ColCellName + 1;
  int ColNetName = ColSep1 + 1;
  int NCols = ColNetName + 1;

  //create header
  TableFormatter tf(NCols);
  //columns
  tf.SetColumnAlign(ColArcName, TableFormatter::Align_Left);
  tf.SetColumnAlign(ColCellName, TableFormatter::Align_Left);
  tf.SetColumnAlign(ColSep1, TableFormatter::Align_Fill);
  tf.SetColumnAlign(ColNetName, TableFormatter::Align_Left);
  //first header row
  tf.NewHeaderRow();
  tf.SetCell(ColGate, "Delay[ps]", 2, TableFormatter::Align_Left);
  tf.SetCell(ColCap, "Cap", TableFormatter::Align_Left);
  tf.SetCell(ColFanout, "Fan-", TableFormatter::Align_Left);
  //second header row
  tf.NewHeaderRow();
  tf.SetCell(ColAAT, "AAT", TableFormatter::Align_Left);
  tf.SetCell(ColRAT, "RAT", TableFormatter::Align_Left);
  tf.SetCell(ColGate, "Gate", TableFormatter::Align_Left);
  tf.SetCell(ColRC, "RC", TableFormatter::Align_Left);
  tf.SetCell(ColCap, "(fF)", TableFormatter::Align_Left);
  tf.SetCell(ColFanout, "out", TableFormatter::Align_Left);
  tf.SetCell(ColArcName, "Info", TableFormatter::Align_Left, 1000);
  //separator
  tf.NewBorderRow();
  tf.SetCell(ColAAT, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColRAT, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColGate, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColRC, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColCap, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColFanout, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColPhase, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColArcName, "-", TableFormatter::Align_Fill);

  //print arc (cell + net pairs)
  HCriticalPath::PointsEnumeratorW cpoint = (path,design).GetEnumeratorW();
  HCriticalPathPointWrapper previousSink = design.CriticalPathPoints.NullW();
  while (cpoint.MoveNext())
  {
    HCriticalPathPointWrapper driver = cpoint;
    if (!cpoint.MoveNext())
      break;//wtf?

    HPinWrapper driverPin = design[(driver.TimingPoint(),design).Pin()];
    HSteinerPointWrapper stPointDriver = design[design.SteinerPoints[driverPin]];
    HSteinerPointWrapper stPointSink = design[design.SteinerPoints[(cpoint.TimingPoint(),design).Pin()]];
    HNetWrapper net = design[driverPin.Net()];

    tf.NewRow();
    tf.SetCell(ColFanout, net.SinksCount());
    tf.SetCell(ColSep1, '-');
    tf.SetCell(ColNetName, net.Name());
    tf.SetCell(ColCellName, driverPin.IsPrimaryInput() ? driverPin.Name() : (driverPin.Cell(),design).Name());

    if (driver.SignalDirection() == SignalDirection_Rise)
    {
      tf.SetCell(ColAAT, PikoSec(design[cpoint.TimingPoint()].RiseArrivalTime()));
      tf.SetCell(ColRAT, PikoSec(design[cpoint.TimingPoint()].RiseRequiredTime()));
      tf.SetCell(ColPhase, 'R');
      tf.SetCell(ColRC, PikoSec(stPointSink.RisePathDelay()));
      tf.SetCell(ColCap, Round(1000.0 * stPointDriver.RiseObservedC()));
    }
    else if (driver.SignalDirection() == SignalDirection_Fall)
    {
      tf.SetCell(ColAAT, PikoSec(design[cpoint.TimingPoint()].FallArrivalTime()));
      tf.SetCell(ColRAT, PikoSec(design[cpoint.TimingPoint()].FallRequiredTime()));
      tf.SetCell(ColPhase, 'F');
      tf.SetCell(ColRC, PikoSec(stPointSink.FallPathDelay()));
      tf.SetCell(ColCap, Round(1000.0 * stPointDriver.FallObservedC()));
    }
    else if (driver.SignalDirection() == SignalDirection_None)
    {
      tf.SetCell(ColAAT, PikoSec(design[cpoint.TimingPoint()].ArrivalTime()));
      tf.SetCell(ColRAT, PikoSec(design[cpoint.TimingPoint()].RequiredTime()));
      tf.SetCell(ColRC, PikoSec(stPointSink.PathDelay()));
      tf.SetCell(ColCap, Round(1000.0 * stPointDriver.ObservedC()));
    }
    else
    {
      LOGERROR("Unsupported signal direction.");
    }

    double arcTime;
    bool inversed;
    HTimingArcType arc = design.TimingArcTypes.Null();
    if(design[path].ExtractionType() == PathExtractionType_Arrival || ::IsNull(previousSink))
      arc = FindArrivalArc(design, driver.TimingPoint(), driver.SignalDirection(), arcTime, inversed);
    else if(design[path].ExtractionType() == PathExtractionType_Required)
      arc = FindRequiredArc(design, previousSink.TimingPoint(), previousSink.SignalDirection(), arcTime, inversed);

    tf.SetCell(ColGate, PikoSec(arcTime));
    tf.SetCell(ColArcName, FormatArcName(design, driverPin, arc));

    previousSink = cpoint;
  }

  //check for setup arc
  HPin finalPin = design.Get<HTimingPoint::Pin, HPin>(previousSink.TimingPoint());
  for (HPinType::ArcsEnumeratorW arc = design.Get<HPinType::ArcTypesEnumerator, HPinType::ArcsEnumeratorW>(
    design.Get<HPin::Type, HPinType>(finalPin));
    arc.MoveNext(); )
  {
    if (arc.TimingType() == TimingType_SetupRising || arc.TimingType() == TimingType_SetupFalling)
    {
      tf.NewRow();
      if (previousSink.SignalDirection() == SignalDirection_Fall)
        tf.SetCell(ColGate, PikoSec(arc.TIntrinsicFall()));
      else if (previousSink.SignalDirection() == SignalDirection_Rise || previousSink.SignalDirection() == SignalDirection_None)
        tf.SetCell(ColGate, PikoSec(arc.TIntrinsicRise()));
      else
      {
        LOGERROR("Unsupported signal direction.");
      }
      tf.SetCell(ColArcName, FormatArcName(design, finalPin, arc));
      tf.SetCell(ColCellName, design[design[finalPin].Cell()].Name());
      break;
    }
  }

  //separator
  tf.NewRow();
  tf.SetCell(ColAAT, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColRAT, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColGate, "-", TableFormatter::Align_Fill);
  tf.SetCell(ColArcName, "-", TableFormatter::Align_Fill);

  //slack
  tf.NewRow();
  tf.SetCell(ColAAT, PikoSec(design[path].Criticality()));
  tf.SetCell(ColArcName, "(slack)", 10);


  //printing
  Logger::Global.WriteIgnoringHTML("================================================================================");
  WRITELINE("");
  HPinWrapper firstPin = design[design[design[design[path].StartPoint()].TimingPoint()].Pin()];
  HPinWrapper lastPin = design[design[design[design[path].EndPoint()].TimingPoint()].Pin()];

  HCellWrapper firstCell = design[firstPin.Cell()];
  HCellWrapper lastCell = design[lastPin.Cell()];

  tf.Caption = Aux::Format("Path #%d %10s %s %s %8s %s %s",
    pathNumber, 
    "From   ",
    firstPin.IsPrimary() ? "PIN" : firstCell.Name().c_str(),
    firstPin.Name().c_str(),
    "To   ",
    lastPin.IsPrimary() ? "PIN" : lastCell.Name().c_str(),
    lastPin.Name().c_str());

  WRITELINE("");

  tf.Print();

  WRITELINE("");
  WRITELINE("");
}

void PlotPath(HDesign& design, HCriticalPath path, int pathNumber)
{
  ASSERT(design.CanDoTiming());
  char fileName[32];
  char signature[256];
  sprintf(fileName, "critical_path_%d", pathNumber);
  sprintf(signature, "critical path #%d %f", pathNumber, design.GetDouble<HCriticalPath::Criticality>(path));

  design.Plotter.Clear();
  design.Plotter.PlotPlacement();
  design.Plotter.PlotCriticalPath(path);
  design.Plotter.PlotText(signature);
  design.Plotter.Refresh("CriticalPaths.plotWait");
  design.Plotter.SaveImage(fileName,
    design.cfg.ValueOf("plotter.pixDirectory", ".\\") + design.Circuit.Name() + "_critical_path\\");
}

void PrintTimingReport(HDesign& design, int nPaths)
{
  ASSERT(design.CanDoTiming());

  WRITELINE("");
  ALERT("Critical paths printing started...");

  int reported = Utils::IterateMostCriticalPaths(design, nPaths, PrintPath);
  if (reported > 0) WRITELINE("");

  ALERT("Reported %d paths of %d", reported, design.CriticalPaths.Count());
  ALERT("Critical paths printing finished!");
}

void PlotMostCriticalPaths(HDesign& design, int n)
{
  Utils::IterateMostCriticalPaths(design, n, PlotPath);
}

void ReportNegativeSlacks(HDesign& design)
{
  ALERT("Reporting: circuit negative slacks");
  ALERT("  TNS: %f", Utils::TNS(design));
  ALERT("  WNS: %f", Utils::WNS(design));
}
//old report
//void PrintNetInfo(HDesign& hd, HNet net)
//{
//  HNetWrapper nw = hd[net];
//  for (HNet::PinsEnumeratorW pin = nw.GetPinsEnumeratorW(); pin.MoveNext(); )
//  {
//    HCellWrapper cell = hd[pin.Cell()];
//    HSteinerPointWrapper st = hd[hd.SteinerPoints[pin]];
//    HTimingPointWrapper tp = hd[hd.TimingPoints[pin]];
//    WRITELINE("  %8s %3s %4d %.10f %.10f %.10f %.10f %.10f %.10f",
//      pin.IsPrimary() ? "PIN" : cell.Name().c_str(),
//      pin.Name().c_str(),
//      ::ToID(pin),
//      st.FallPathDelay(),
//      st.RisePathDelay(),
//      tp.FallArrivalTime(),
//      tp.RiseArrivalTime(),
//      tp.FallRequiredTime(),
//      tp.RiseRequiredTime()
//      );
//  }
//}
//
//void PrintPointInfo(HDesign& hd, HCriticalPathPointWrapper& j, int pointNumber)
//{
//  HTimingPointWrapper tp = hd[j.TimingPoint()];
//  HPinWrapper pin = hd[tp.Pin()];
//  HNetWrapper net = hd[pin.Net()];
//  HCellWrapper cell = hd[pin.Cell()];
//  HMacroTypeWrapper mt = hd[cell.Type()];
//  HPinTypeWrapper pt = hd[pin.Type()];
//  HSteinerPointWrapper sp = hd[hd.SteinerPoints[pin]];
//
//  WRITE("Point #%d   %s    ",
//    pointNumber,
//    j.SignalDirection() == SignalDirection_Fall ? "Fall" : "Rise");
//
//  if(pin.IsPrimary())
//    WRITE("PIN   "); 
//  else
//    WRITE("%s (%s)   ", cell.Name().c_str(), mt.Name().c_str());
//
//  WRITELINE("%s (%s)    [%.2f , %.2f])",
//    pin.Name().c_str(),
//    Utils::GetPinDirectionStr(pin.Direction()),
//    pin.X(),
//    pin.Y());
//
//  WRITELINE("  C = %.10f ( fall = %.10f , rise = %.10f )",
//    pt.Capacitance(),
//    pt.FallCapacitance(),
//    pt.RiseCapacitance());
//  WRITELINE("");
//  WRITELINE("      \tArrival      \tRequired       \tNegative Slack");
//  WRITELINE("  Fall\t%.10f\t%.10f\t%.10f",
//    tp.FallArrivalTime(),
//    tp.FallRequiredTime(),
//    tp.FallNegativeSlack());
//  WRITELINE("  Rise\t%.10f\t%.10f\t%.10f",
//    tp.RiseArrivalTime(),
//    tp.RiseRequiredTime(),
//    tp.RiseNegativeSlack());
//  WRITELINE("");
//
//  {
//    HTimingPointWrapper a = hd[tp.FallArrivalAncestor()];
//    HPinWrapper p = hd[a.Pin()];
//    HCellWrapper c = hd[p.Cell()];
//    WRITELINE("  Fall signal comes from  %s %s: %.10f",
//      p.IsPrimary() ? "PIN" : c.Name().c_str(),
//      p.Name().c_str(),
//      a.FallArrivalTime()
//      );
//  }
//  {
//    HTimingPointWrapper a = hd[tp.RiseArrivalAncestor()];
//    HPinWrapper p = hd[a.Pin()];
//    HCellWrapper c = hd[p.Cell()];
//    WRITELINE("  Rise signal comes from  %s %s: %.10f",
//      p.IsPrimary() ? "PIN" : c.Name().c_str(),
//      p.Name().c_str(),
//      a.RiseArrivalTime()
//      );
//  }
//  {
//    HTimingPointWrapper a = hd[tp.FallRequiredAncestor()];
//    HPinWrapper p = hd[a.Pin()];
//    HCellWrapper c = hd[p.Cell()];
//    WRITELINE("  Fall signal goes to     %s %s: %.10f",
//      p.IsPrimary() ? "PIN" : c.Name().c_str(),
//      p.Name().c_str(),
//      a.FallRequiredTime()
//      );
//  }
//  {
//    HTimingPointWrapper a = hd[tp.RiseRequiredAncestor()];
//    HPinWrapper p = hd[a.Pin()];
//    HCellWrapper c = hd[p.Cell()];
//    WRITELINE("  Rise signal goes to     %s %s: %.10f",
//      p.IsPrimary() ? "PIN" : c.Name().c_str(),
//      p.Name().c_str(),
//      a.RiseRequiredTime()
//      );
//  }
//  WRITELINE("\n");
//}
//
//void PrintNetArc(HDesign& hd, HCriticalPathPointWrapper& pt1, HCriticalPathPointWrapper& pt2)
//{
//  HTimingPointWrapper tp1 = hd[pt1.TimingPoint()];
//  HPinWrapper pin1 = hd[tp1.Pin()];
//  HCellWrapper cell1 = hd[pin1.Cell()];
//  //HSteinerPointWrapper sp1 = hd[hd.SteinerPoints[pin1]];
//
//  HTimingPointWrapper tp2 = hd[pt2.TimingPoint()];
//  HPinWrapper pin2 = hd[tp2.Pin()];
//  HCellWrapper cell2 = hd[pin2.Cell()];
//  HSteinerPointWrapper sp2 = hd[hd.SteinerPoints[pin2]];
//
//  HNetWrapper net = hd[pin1.Net()];
//
//  WRITELINE("NetArc   %s   {%s %s  ->  %s %s}",
//    net.Name().c_str(),
//    pin1.IsPrimary() ? "PIN" : cell1.Name().c_str(),
//    pin1.Name().c_str(),
//    pin2.IsPrimary() ? "PIN" : cell2.Name().c_str(),
//    pin2.Name().c_str());
//  WRITELINE("  PinsInNet = %d", net.PinsCount());
//  WRITELINE("  HPWL      = %.2f", Utils::CalcNetHPWL(hd, net));
//  if (pt1.SignalDirection() == SignalDirection_Fall)
//    WRITELINE("  FallDelay = %.10f", sp2.FallPathDelay());
//  else
//    WRITELINE("  RiseDelay = %.10f", sp2.RisePathDelay());
//
//  //PrintNetInfo(hd, net);
//  WRITELINE("\n");
//}
//
//void PrintCellArc(HDesign& hd, HCriticalPathPointWrapper& pt1, HCriticalPathPointWrapper& pt2)
//{
//  HTimingPointWrapper tp1 = hd[pt1.TimingPoint()];
//  HPinWrapper pin1 = hd[tp1.Pin()];
//  HNetWrapper net1 = hd[pin1.Net()];
//  HSteinerPointWrapper sp1 = hd[hd.SteinerPoints[pin1]];
//
//  HTimingPointWrapper tp2 = hd[pt2.TimingPoint()];
//  HPinWrapper pin2 = hd[tp2.Pin()];
//  HNetWrapper net2 = hd[pin2.Net()];
//  HSteinerPointWrapper sp2 = hd[hd.SteinerPoints[pin2]];
//
//  HCellWrapper cell = hd[pin1.Cell()];
//
//  WRITELINE("CellArc  %s  {%s ->  %s}",
//    cell.Name().c_str(),
//    //::ToID(cell),
//    pin1.Name().c_str(),
//    //::ToID(pin1),
//    pin2.Name().c_str()
//    //::ToID(pin2)
//    );
//
//  HTimingArcType arrival_arc;
//  HTimingArcType required_arc;
//  bool is_arrival_inverted;
//  bool is_required_inverted;
//  double arrival_arc_time;
//  double required_arc_time;
//
//  if (pt2.SignalDirection() == SignalDirection_Rise)
//  {
//    GetArrivalRisingArc(hd, tp1, tp2, &arrival_arc, &arrival_arc_time, &is_arrival_inverted);
//    GetRequiredRisingArc(hd, tp1, tp2, &required_arc, &required_arc_time, &is_required_inverted);
//  }
//  else
//  {
//    GetArrivalFallingArc(hd, tp1, tp2, &arrival_arc, &arrival_arc_time, &is_arrival_inverted);
//    GetRequiredFallingArc(hd, tp1, tp2, &required_arc, &required_arc_time, &is_required_inverted);
//  }
//
//  HTimingArcTypeWrapper aarc = hd[arrival_arc];
//  HTimingArcTypeWrapper rarc = hd[required_arc];
//
//  if (aarc.GetStartPin(pin2) == pin1 && aarc.GetEndPin(pin1) == pin2)
//  {
//    if (rarc == aarc)
//      WRITELINE("  Timing Arc:");
//    else
//      WRITELINE("  Arrival Timing Arc:");
//
//    WRITELINE("    TimingType     = %s", Utils::GetTimingTypeStr(aarc.TimingType()));
//    WRITELINE("    TimingSense    = %s", Utils::GetTimingSenseStr(aarc.TimingSense()));
//    WRITELINE("    SignalInversed = %s", is_arrival_inverted ? "True" : "False");
//    WRITELINE("    ArcDelay(%s) = %.10f", pt2.SignalDirection() == SignalDirection_Fall ? "fall" : "rise", arrival_arc_time);
//    WRITELINE("");
//    WRITELINE("        \tTIntrinsic  \tResistance  \tObservedC   \tDelay");
//    WRITELINE("    Fall\t%.10f\t%.10f\t%.10f\t%.10f",
//      aarc.TIntrinsicFall(),
//      aarc.ResistanceFall(),
//      sp2.FallObservedC(),
//      aarc.TIntrinsicFall() + aarc.ResistanceFall() * sp2.FallObservedC());
//    WRITELINE("    Rise\t%.10f\t%.10f\t%.10f\t%.10f",
//      aarc.TIntrinsicRise(),
//      aarc.ResistanceRise(),
//      sp2.RiseObservedC(),
//      aarc.TIntrinsicRise() + aarc.ResistanceRise() * sp2.RiseObservedC());
//  }
//
//  if (rarc.GetStartPin(pin2) == pin1 && rarc.GetEndPin(pin1) == pin2 && aarc != rarc)
//  {
//    WRITELINE("\n  Required Timing Arc:");
//
//    WRITELINE("    TimingType     = %s", Utils::GetTimingTypeStr(rarc.TimingType()));
//    WRITELINE("    TimingSense    = %s", Utils::GetTimingSenseStr(rarc.TimingSense()));
//    WRITELINE("    SignalInversed = %s", is_required_inverted ? "True" : "False");
//    WRITELINE("    ArcDelay(%s) = %.10f", pt2.SignalDirection() == SignalDirection_Fall ? "fall" : "rise", required_arc_time);
//    WRITELINE("");
//    WRITELINE("        \tTIntrinsic  \tResistance  \tObservedC   \tDelay");
//    WRITELINE("    Fall\t%.10f\t%.10f\t%.10f\t%.10f",
//      rarc.TIntrinsicFall(),
//      rarc.ResistanceFall(),
//      sp2.FallObservedC(),
//      rarc.TIntrinsicFall() + rarc.ResistanceFall() * sp2.FallObservedC());
//    WRITELINE("    Rise\t%.10f\t%.10f\t%.10f\t%.10f",
//      rarc.TIntrinsicRise(),
//      rarc.ResistanceRise(),
//      sp2.RiseObservedC(),
//      rarc.TIntrinsicRise() + rarc.ResistanceRise() * sp2.RiseObservedC());
//  }
//
//  WRITELINE("\n");
//}
//
//bool IsOneNet(HDesign& hd, HCriticalPathPointWrapper& first, HCriticalPathPointWrapper& second)
//{
//  return hd.Get<HPin::Net, HNet>(hd.Get<HTimingPoint::Pin, HPin>(first.TimingPoint()))
//    == hd.Get<HPin::Net, HNet>(hd.Get<HTimingPoint::Pin, HPin>(second.TimingPoint()));
//}
//
//bool IsOneCell(HDesign& hd, HCriticalPathPointWrapper& first, HCriticalPathPointWrapper& second)
//{
//  return hd.Get<HPin::Cell, HCell>(hd.Get<HTimingPoint::Pin, HPin>(first.TimingPoint()))
//    == hd.Get<HPin::Cell, HCell>(hd.Get<HTimingPoint::Pin, HPin>(second.TimingPoint()));
//}
//
//void ReportOnePath_(HDesign& hd, const HCriticalPathWrapper& path, int pathNumber)
//{
//  WRITELINE("================================================================================");
//  WRITELINE("");
//  HPinWrapper firstPin = hd[hd[hd[path.StartPoint()].TimingPoint()].Pin()];
//  HPinWrapper lastPin = hd[hd[hd[path.EndPoint()].TimingPoint()].Pin()];
//
//  HCellWrapper firstCell = hd[firstPin.Cell()];
//  HCellWrapper lastCell = hd[lastPin.Cell()];
//
//  WRITELINE("Path #%d %10s %s %s %8s %s %s", 
//    pathNumber, 
//    "From   ",
//    firstPin.IsPrimary() ? "PIN" : firstCell.Name().c_str(),
//    firstPin.Name().c_str(),
//    "To   ",
//    lastPin.IsPrimary() ? "PIN" : lastCell.Name().c_str(),
//    lastPin.Name().c_str());
//
//  WRITELINE("#Points = %d       Slack = %.10f", path.PointsCount(), path.Criticality());
//  WRITELINE("");
//
//  int pointNumber=1;
//  HCriticalPathPointWrapper prev = hd[hd.CriticalPathPoints.Null()];
//  for(HCriticalPath::PointsEnumeratorW j = path.GetEnumeratorW(); j.MoveNext(); ++pointNumber)
//  {
//    if (!::IsNull(prev))
//      if (IsOneNet(hd, prev, j))
//        PrintNetArc(hd, prev, j);
//      else if (IsOneCell(hd, prev, j))
//        PrintCellArc(hd, prev, j);
//      else
//      {
//        LOGERROR("Critical path points is disconnected.");
//      }
//      prev = j;
//
//      PrintPointInfo(hd, j, pointNumber);
//  }
//}

