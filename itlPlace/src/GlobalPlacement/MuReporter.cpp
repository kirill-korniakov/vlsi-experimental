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

void MuReporter::Report(HDesign& design, TimingPointMus* mus, string msg)
{
    this->mus = mus;

    bool verbose = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.Reporter.verbose", false);
    if (verbose)
    {
        if (msg != "")
        {
            WRITELINE(msg.c_str());
        }
        ReportMus(design);
    }

    design.Plotter.ClearHistogram();
    string order = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.Reporter.order", "CriticalPath");
    if (order == "CriticalPath")
        PlotMusInCriticalPathOrder(design);
    else if (order == "Topological")
        PlotMusInTopologicalOrder(design);
    else
        GLOGWARNING(LOGINPLACE, "Unknown LagrangianRelaxation.Reporter.order = %s", order.c_str());                    
    design.Plotter.PlotMuLevel(1.0, scaling); //TODO: probably better to get initial mu
    design.Plotter.RefreshHistogram((HPlotter::WaitTime)waitTime);
}

string MuReporter::GetCellPinName(HDesign& design, HTimingPointWrapper pt)
{
    HPinWrapper pin = design[pt.Pin()];
    HCellWrapper cell = design[pin.Cell()];
    return cell.Name() + "." + pin.Name();
}

void MuReporter::ReportMus(HDesign& design)
{
    WRITELINE("            Cell.Pin | MuS   | MuInA : MuInR |");

    for (HTimingPointWrapper pt = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(pt.GoNext()); )
    {
        WRITE("%20s | ", GetCellPinName(design, pt).c_str());
        WRITE("%.3f | ", mus->GetMuS(pt));
        int count = mus->GetMuInCount(pt);
        for (int i = 0; i < count; i++)
            WRITE("%.3f : %.3f | ", mus->GetInMuA(pt, i), mus->GetInMuR(pt, i));
        WRITELINE("");
    }
}

void MuReporter::PlotMusInTopologicalOrder(HDesign& design)
{
    int nTimingPoints = 0;
    for (HTimingPointWrapper pt = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(pt.GoNext()); )
        nTimingPoints++;

    int i = 0;
    for (HTimingPointWrapper pt = design[design.TimingPoints.FirstInternalPoint()]; !::IsNull(pt.GoNext()); i++)
    {
        design.Plotter.PlotMu(i, nTimingPoints, mus->SumInMuA(pt), scaling, Color_Red);
    }
}

void MuReporter::PlotPathMus(HDesign& design, HCriticalPath path, int pathIdx)
{
    HCriticalPath::PointsEnumeratorW cpoint = (path,design).GetEnumeratorW();
    double sum = 0.0;
    int nTP = 0;

    while (cpoint.MoveNext()) //we start from second timing point
    {
        HTimingPoint pt = cpoint.TimingPoint();
        sum += mus->SumInMuA(pt) + mus->SumInMuR(pt);
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