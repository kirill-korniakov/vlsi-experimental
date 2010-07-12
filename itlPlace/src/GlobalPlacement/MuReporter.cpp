#include "MuReporter.h"
#include "TimingPointMus.h"
#include "Utils.h"
#include "STA.h"
#include "Timing.h"

MuReporter::MuReporter(HDesign& design)
{
  design.Plotter.InitializeHistogramWindow();
  scaling = 0.01;  //TODO: choose proper scale
  waitTime = design.cfg.ValueOf("GlobalPlacement.plotWait", 1);

  plotX = 0;
}

void MuReporter::Report(HDesign& design, TimingPointMus* mus)
{
  this->mus = mus;

  //ReportMus(design);
  design.Plotter.ClearHistogram();
  //PlotMusInTopologicalOrder(design);
  PlotMusInCriticalPathOrder(design);
  design.Plotter.PlotMuLevel(1.0, scaling); //TODO: probably better to get initial mu
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

  int i = 0;
  for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()];
    !::IsNull(pt.GoNext()); i++)
  {
    design.Plotter.PlotMu(i, nTimingPoints, mus->GetInMuA(pt), scaling, Color_Red);
  }
}

void MuReporter::PlotPathMus(HDesign& design, HCriticalPath path, int pathIdx)
{
  HCriticalPath::PointsEnumeratorW cpoint = (path,design).GetEnumeratorW();
  double sum = 0.0;
  int nTP = 0;

  while (cpoint.MoveNext())
  {
    HTimingPoint pt = cpoint.TimingPoint();
    sum += mus->GetInMuA(pt) + mus->GetInMuR(pt);
    nTP++;
  }

  Color color;
  if (pathIdx % 2 == 0)
    color = Color_Red;
  else
    color = Color_Orange;

  design.Plotter.PlotMu(sum, plotX, scaling / nTP, color);
  plotX += 1;
}

void MuReporter::PlotMusInCriticalPathOrder(HDesign& design)
{
  STA(design, false);
  FindCriticalPaths(design);
  
  plotX = 0;
  Utils::IterateMostCriticalPaths(design, Utils::ALL_PATHS, 
      Utils::CriticalPathHandler(this, &MuReporter::PlotPathMus));

  design.Plotter.PlotMu(1.0, plotX, 1.0, Color_Black);
}