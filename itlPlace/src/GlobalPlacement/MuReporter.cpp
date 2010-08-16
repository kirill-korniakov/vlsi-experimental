#include "MuReporter.h"
#include "TimingPointMus.h"
#include "Utils.h"
#include "STA.h"
#include "Timing.h"
#include "TableFormatter.h"
#include "Reporting.h"

#include <vector>


MuReporter::MuReporter(HDesign& design)
{
    if (design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.Reporter.plotMus", false))
        design.Plotter.InitializeHistogramWindow();
    scaling = 0.01;  //TODO: choose proper scale
    waitTime = design.cfg.ValueOf("GlobalPlacement.Plotting.plotWait", 1);

    plotX = 0;
}

void MuReporter::Report(HDesign& design, TimingPointMus* mus, string msg)
{
    this->mus = mus;

    WRITELINE("");
    bool verbose = design.cfg.ValueOf("GlobalPlacement.LagrangianRelaxation.Reporter.verbose", false);
    if (verbose)
    {
        if (msg != "")
        {
            WRITELINE(msg.c_str());
        }
        //ReportMus(design);
        ReportMuHistogram(design);
    }
    WRITELINE("");

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

void MuReporter::ReportMus(HDesign& design)
{
    WRITELINE("            Cell.Pin | MuS   | MuInA : MuInR |");

    for (HTimingPointWrapper pt = design[design.TimingPoints.TopologicalOrderRoot()]; !::IsNull(pt.GoNext()); )
    {
        if (pt == design.TimingPoints.FirstInternalPoint())
            WRITELINE("------------------------------------");

        WRITE("%20s | ", GetCellPinName(design, pt).c_str());
        WRITE("%.3f | ", mus->GetMuS(pt));
        int count = mus->GetMuInCount(pt);
        for (int i = 0; i < count; i++)
            WRITE("%.3f : %.3f | ", mus->GetInMuA(pt, i), mus->GetInMuR(pt, i));
        WRITELINE("");

        if (pt == design.TimingPoints.LastInternalPoint())
            WRITELINE("------------------------------------");
    }
}

void MuReporter::ReportMuHistogram(HDesign& design)
{
    int nBins = 10;
    std::vector<double> histogramValues;
    histogramValues.assign(nBins, 0);

    int count = 0;
    for (HTimingPointWrapper tp = design[design.TimingPoints.TopologicalOrderRoot()]; !::IsNull(tp.GoNext()); )
    {
        double muS = mus->GetMuS(tp);
        int idx = (int)(100.0*muS/nBins);
        histogramValues[idx]++;
        count++;
    }

    size_t nCols = nBins;

    TableFormatter tf("MuS Histogram", nCols);
    //columns
    for (int i = 0; i < nBins; i++)
    {
        tf.SetColumnAlign(i, TableFormatter::Align_Right);
        tf.SetColumnPrecision(i, 2);
    }
    
    //separator
    tf.NewBorderRow();
    for (int i = 0; i < nBins; i++)
    {
        tf.SetCell(i, "-", TableFormatter::Align_Fill);
    }
    
    //header row
    tf.NewHeaderRow();
    for (int i = 0; i < nBins; i++)
    {
        tf.SetCell(i, ((double)i / nBins), TableFormatter::Align_Right);
    }   

    //separator
    tf.NewBorderRow();
    for (int i = 0; i < nBins; i++)
    {
        tf.SetCell(i, "-", TableFormatter::Align_Fill);
    }

    tf.NewRow();
    for (int i = 0; i < nBins; i++)
    {
        tf.SetCell(i, histogramValues[i]/count*100, TableFormatter::Align_Right);
    }

    //separator
    tf.NewBorderRow();
    for (int i = 0; i < nBins; i++)
    {
        tf.SetCell(i, "-", TableFormatter::Align_Fill);
    }

    WRITELINE("");

    tf.Print();

    WRITELINE("");
    WRITELINE("");
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