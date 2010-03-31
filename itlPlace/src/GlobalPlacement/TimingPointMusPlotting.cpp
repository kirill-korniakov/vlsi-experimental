#include "TimingPointMus.h"
#include "Utils.h"
#include "STA.h"
#include "Timing.h"

MuReporter::MuReporter(HDesign& design)
{
  design.Plotter.InitializeHistogramWindow();
  waitTime = design.cfg.ValueOf("GlobalPlacement.plotWait", 1);
}

void MuReporter::Report(HDesign& design, TimingPointMus* mus)
{
  this->mus = mus;

  //ReportMus(design);

  design.Plotter.ClearHistogram();
  PlotMusInTopologicalOrder(design);
  //PlotMusInCriticalPathOrder(design);
  design.Plotter.RefreshHistogram((HPlotter::WaitTime)waitTime);
}

void MuReporter::ReportMus(HDesign& design)
{
  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoNext()); )
  {
    WRITELINE("MuS %f", mus->GetMuS(pt));
    int count = mus->GetMuInCount(pt);
    for (int i = 0; i < count; i++)
      WRITELINE("MuInA %f    MuInR %f", mus->GetMuInA(pt, i), mus->GetMuInR(pt, i));
  }
}

void MuReporter::PlotMusInTopologicalOrder(HDesign& design)
{
  int nTimingPoints = 0;
  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoNext()); )
    nTimingPoints++;

  design.Plotter.PlotMuLevel(1.0);
  int i = 0;
  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoNext()); i++)
  {
    design.Plotter.PlotMu(i, nTimingPoints, mus->GetInMuA(pt), Color_Red);
  }
}

void MuReporter::PlotPathMus(HDesign& design, HCriticalPath path, int pathIdx)
{
  static int x = 0;
  if (pathIdx == 1)
    x = 0;

  HCriticalPath::PointsEnumeratorW cpoint = (path,design).GetEnumeratorW();
  double sum = 0.0;
  while (cpoint.MoveNext())
  {
    HTimingPoint pt = cpoint.TimingPoint();
    sum += mus->GetInMuA(pt) + mus->GetInMuR(pt);
  }

  if (pathIdx % 2 == 0)
    design.Plotter.PlotMu(sum, x, Color_Red);
  else
    design.Plotter.PlotMu(sum, x, Color_Orange);
  x += 2;
}

void MuReporter::PlotMusInCriticalPathOrder(HDesign& design)
{
  STA(design);
  FindCriticalPaths(design);
  
  Utils::IterateMostCriticalPaths(design, Utils::ALL_PATHS, 
      Utils::CriticalPathHandler(this, &MuReporter::PlotPathMus));
}