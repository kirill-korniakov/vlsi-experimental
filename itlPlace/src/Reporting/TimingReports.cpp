#include "Reporting.h"
#include "Utils.h"
#include "Auxiliary.h"
#include "limits.h"
#include "Timing.h"
#include <math.h>
#include "TableFormatter.h"
#include "TimingPointsEnumerator.h"

int PikoSec(double delay_ns)
{
  return Round(1000.0 * delay_ns);
}

string GetCellPinName(HDesign& design, HPin pin)
{
    HPinWrapper pinW = design[pin];
    HCellWrapper cell = design[pinW.Cell()];
    string cellName = IsNull(cell) ? "PIN" : cell.Name();
    return cellName + "." + pinW.Name();
}

string GetCellPinName(HDesign& design, HTimingPoint pt)
{
    HPin pin = design.Get<HTimingPoint::Pin, HPin>(pt);
    return GetCellPinName(design, pin);
}

void ReportTEPNames(HDesign& design)
{
    if (!design.TimingPoints.IsInitialized()) return;

    ALERT("REPORTING LIST OF TIMING END POINTS:\n");
    
    HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
    for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()]; ep.GoPrevious() != endPointsEnd; )
    {
        WRITELINE("\tTEP: %s", GetCellPinName(design, ep).c_str());
    }
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

string FormatArcName(HDesign& design, HPinType endPin, HTimingArcType arc)
{
  HPinType startPin = design.TimingArcTypes.GetStartPinType(arc, endPin);
  TimingType tt = design[arc].TimingType();

  if (tt == TimingType_SetupFalling || tt == TimingType_SetupRising)
    return string("(setup) ") + design[endPin].Name();
  else if (tt == TimingType_HoldFalling || tt == TimingType_HoldRising)
    return string("(hold) ") + design[endPin].Name();
  else
    return design[startPin].Name() + "->" + design[endPin].Name();
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

void PrintTimingReport(HDesign& design, int nPaths)
{
  ASSERT(design.CanDoTiming());

  if (nPaths == 0)
      return;

  WRITELINE("");
  ALERT("Critical paths printing started...");

  int reported = Utils::IterateMostCriticalPaths(design, nPaths, PrintPath);
  if (reported > 0) WRITELINE("");

  ALERT("Reported %d paths of %d", reported, design.CriticalPaths.Count());
  ALERT("Critical paths printing finished!");
}

void ReportNegativeSlacks(HDesign& design)
{
  ALERT("Reporting: circuit negative slacks");
  ALERT("    TNS: %f", Utils::TNS(design));
  ALERT("    WNS: %f", Utils::WNS(design));
  ALERT("  AVGNS: %f", Utils::AverageNS(design));
  //ReportVectorOfSlacks(design);
}

void ReportVectorOfSlacks(HDesign& design)
{
    int nCols = 2;
    int idx = 0;
    int colTimingPointName = idx++;
    int colSlacks = idx++;

    TableFormatter tf("TEP Slacks", nCols);
    //columns
    tf.SetColumnAlign(colTimingPointName, TableFormatter::Align_Right);
    tf.SetColumnAlign(colSlacks, TableFormatter::Align_Right);
    //separator
    tf.NewBorderRow();
    tf.SetCell(colTimingPointName, "-", TableFormatter::Align_Fill);
    tf.SetCell(colSlacks, "-", TableFormatter::Align_Fill);
    //header row
    tf.NewHeaderRow();
    tf.SetCell(colTimingPointName, "Cell.Pin", TableFormatter::Align_Right);
    tf.SetCell(colSlacks, "Slack", TableFormatter::Align_Right);
    //separator
    tf.NewBorderRow();
    tf.SetCell(colTimingPointName, "-", TableFormatter::Align_Fill);
    tf.SetCell(colSlacks, "-", TableFormatter::Align_Fill);

    HTimingPoint endPointsEnd = design.TimingPoints.LastInternalPoint();
    for (HTimingPointWrapper ep = design[design.TimingPoints.TopologicalOrderRoot()];
        ep.GoPrevious() != endPointsEnd; )
    {
        tf.NewRow();
        tf.SetCell(colTimingPointName, GetCellPinName(design, ep));
        tf.SetCell(colSlacks, ep.Slack());
    }
    //separator
    tf.NewBorderRow();
    tf.SetCell(colTimingPointName, "-", TableFormatter::Align_Fill);
    tf.SetCell(colSlacks, "-", TableFormatter::Align_Fill);

    WRITELINE("");

    tf.Print();

    WRITELINE("");
    WRITELINE("");
}

void ReportTNSWNSSequence(HDesign& hd, string &tnsStr, string &wnsStr)
{
    char tmpFloat[32];
    sprintf(tmpFloat, "%f\t", Utils::TNS(hd));
    tnsStr += tmpFloat;
    WRITELINE(("%s", tnsStr.c_str()));

    sprintf(tmpFloat, "%f\t", Utils::WNS(hd));
    wnsStr += tmpFloat;
    WRITELINE(("%s", wnsStr.c_str()));
}

void ReportTopologicalOrder(HDesign& design)
{
    WRITELINE("");
    ALERT("Printing Topological Order");

    for(TopologicalOrder::AllPointsEnumerator pt = design; pt.MoveNext(); )
    {
        WRITELINE("  PTO %s", GetCellPinName(design, pt).c_str());
    }
}

struct MacroComparer
{
  bool operator() (HMacroTypeWrapper& left, HMacroTypeWrapper& right)
  {
    return left.Name() < right.Name();
  }

  bool operator() (const HMacroTypeWrapper& left, const HMacroTypeWrapper& right) const
  {
    return left.Name() < right.Name();
  }
};

void ReportMacrotypesShort(HDesign& design)
{
  int numCols = 0;
  int colName = numCols++;
  int colIn = numCols++;
  int colOut = numCols++;
  int colX = numCols++;
  int colY = numCols++;
  int colXs = numCols++;
  int colYs = numCols++;

  std::vector<HMacroTypeWrapper> macros;
  macros.reserve(design.MacroTypes.Count());

  for(HMacroTypes::EnumeratorW macro = design.MacroTypes.GetEnumeratorW(); macro.MoveNext();)
    macros.push_back(macro);

//  struct MacroComparer
//  {
//    bool operator() (HMacroTypeWrapper& left, HMacroTypeWrapper& right)
//    {
//      return left.Name() < right.Name();
//    }
//  };

  std::sort(macros.begin(), macros.end(), MacroComparer());

  HSiteWrapper site = design.Sites.NullW();
  HPlacementRows::EnumeratorW rIter = design.PlacementRows.GetEnumeratorW();
  if (rIter.MoveNext())
    site = rIter.Site();
  
  double sx = site.Width();
  double sy = site.Height();
  if (sx == 0.0) sx = 1.0;
  if (sy == 0.0) sy = 1.0;

  //create header
  TableFormatter tf("Macro Types Summary", numCols);
  //columns
  tf.SetColumnAlign(colName, TableFormatter::Align_Left);
  //header row
  tf.NewHeaderRow();
  tf.SetCell(colName, "Macro Name");
  tf.SetCell(colIn, "Inputs Count");
  tf.SetCell(colOut, "Outputs Count");
  tf.SetCell(colX, "Width (nm)");
  tf.SetColumnPrecision(colX, 0);
  tf.SetCell(colY, "Height (nm)");
  tf.SetColumnPrecision(colY, 0);
  tf.SetCell(colXs, "Width (sites)");
  tf.SetColumnPrecision(colXs, 2);
  tf.SetCell(colYs, "Height (sites)");
  tf.SetColumnPrecision(colYs, 2);
  tf.NewBorderRow();

  for(std::vector<HMacroTypeWrapper>::iterator m = macros.begin(); m != macros.end(); ++m)
  {
    tf.NewRow();
    tf.SetCell(colName, m->Name());
    tf.SetCell(colX, m->SizeX());
    tf.SetCell(colY, m->SizeY());
    tf.SetCell(colXs, m->SizeX()/sx);
    tf.SetCell(colYs, m->SizeY()/sy);

    int numInputs = 0;
    int numOutputs = 0;
    
    for(HMacroType::PinsEnumeratorW pin = m->GetEnumeratorW(); pin.MoveNext(); )
    {
      if (pin.Direction() == PinDirection_OUTPUT) numOutputs++;
      if (pin.Direction() == PinDirection_INPUT) numInputs++;
    }
    tf.SetCell(colIn, numInputs);
    tf.SetCell(colOut, numOutputs);
  }

  tf.Print();
}

void ReportLibraryPhisics(HDesign& design, KSignalDirection sd)
{
  ALERT("Library phisics report:");
  int numCols = 0;
  int colName = numCols++;
  int colC = numCols++;
  int colR = numCols++;
  int colT = numCols++;
  int colArcType = numCols++;

  //create header
  TableFormatter tf("Library Phisics Summary", numCols);
  //columns
  tf.SetColumnAlign(colName, TableFormatter::Align_Left);
  tf.SetColumnAlign(colArcType, TableFormatter::Align_Left);
  //header row
  tf.NewHeaderRow();
  tf.SetCell(colName, "Arc/Pin name");
  tf.SetCell(colR, "R (kohm)");
  tf.SetCell(colC, "C (pf)");
  tf.SetCell(colT, "T (ns)");
  tf.SetCell(colArcType, "Arc type");
  tf.NewBorderRow();

  std::vector<HMacroTypeWrapper> macros;
  macros.reserve(design.MacroTypes.Count());

  for(HMacroTypes::EnumeratorW macro = design.MacroTypes.GetEnumeratorW(); macro.MoveNext();)
    macros.push_back(macro);

//  struct MacroComparer
//  {
//    bool operator() (HMacroTypeWrapper& left, HMacroTypeWrapper& right)
//    {
//      return left.Name() < right.Name();
//    }

//    bool operator() (const HMacroTypeWrapper& left, const HMacroTypeWrapper& right) const
//    {
//      return left.Name() < right.Name();
//    }
//  };

  std::sort(macros.begin(), macros.end(), MacroComparer());
  
  //fill table
 
  tf.NewBorderRow();

  WRITELINE("Wire resistance (kohm/nm): r = %g", design.RoutingLayers.Physics.RPerDist);
  WRITELINE("Wire capacitance (pF/nm):  c = %g", design.RoutingLayers.Physics.LinearC);

  tf.Print();
}
