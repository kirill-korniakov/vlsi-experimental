#include "VLSIFlow.h"
#include "GlobalPlacement.h"
#include "STA.h"
#include "HDPGrid.h"
#include "Legalization.h"
#include "DetailedPlacement.h"
#include "Timing.h"
#include "Reporting.h"
#include "TileGrid.h"
#include "ExportToRouters.h"
#include "FGR.h"
#include "Utils.h"
#include "Parser.h"
#include "Buffering.h"

void InitFlowMetricsTable(TableFormatter& fmt, HDesign& design)
{
  fmt.NewRow();
  if (design.CanDoTiming())
  {
    fmt.SetCell(7, "WNS(ns)");
    fmt.SetCell(6, "TNS(ns)");
    fmt.SetColumnPrecision(7, 4);
    fmt.SetColumnPrecision(6, 4);
  }
  fmt.SetCell(0, "StageName", TableFormatter::Align_Left);
  fmt.SetCell(1, "Tag");
  fmt.SetCell(2, "Time(sec)");
  fmt.SetCell(3, "Cells");
  fmt.SetCell(4, "HPWL(nm)");
  fmt.SetCell(5, "TWL(nm)");

  fmt.SetColumnAlign(0, TableFormatter::Align_Left);
  fmt.SetColumnAlign(1, TableFormatter::Align_Left);
  fmt.SetColumnPrecision(4, 0);
  fmt.SetColumnPrecision(5, 0);
  fmt.SetColumnPrecision(2, 3);

  fmt.NewRow();
  fmt.SetCell(0, "-",fmt.NumOfColumns(), TableFormatter::Align_Fill);
}

template<class T>
void WriteFlowMetrics(TableFormatter& fmt, HDesign& design, const char* stageName, T tag)
{
  fmt.NewRow();
  fmt.SetCell(0, stageName);
  fmt.SetCell(1, tag);
  fmt.SetCell(2, ::GetUptime());
  fmt.SetCell(3, design.Cells.CellsCount());
  fmt.SetCell(4, Utils::CalculateHPWL(design, true));
  fmt.SetCell(5, Utils::CalculateTWL(design));
  if (design.CanDoTiming())
  {
    fmt.SetCell(6, Utils::TNS(design));
    fmt.SetCell(7, Utils::WNS(design));
  }
}

bool DoRandomPlacementIfRequired(HDesign& hd)
{
  if (hd.cfg.ValueOf("DesignFlow.RandomPlacement", false))
  {
    RandomPlacement(hd);
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("RP");

    WRITELINE("");
    ALERT("STA after random placement:");
    STA(hd);
    return true;
  }
  return false;
}

bool DoGlobalPlacementIfRequired(HDesign& hd)
{
  //GLOBAL PLACEMENT
  if (hd.cfg.ValueOf("DesignFlow.GlobalPlacement", false))
  {
    GlobalPlacement(hd, hd.cfg.ValueOf("params.objective"));
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("GP");

    WRITELINE("");
    ALERT("STA after global placement:");
    STA(hd);
    return true;
  }
  return false;
}

bool DoLRTimingDrivenPlacementIfRequired(HDesign& hd)
{
  //LR-TDP
  if (hd.cfg.ValueOf("DesignFlow.LR", false))
  {
    GlobalPlacement(hd, "LR");
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("LR");

    WRITELINE("");
    ALERT("STA after LR placement:");
    STA(hd);
    return true;
  }
  return false;
}

bool DoLegalizationIfRequired(HDPGrid& grid)
{
  //LEGALIZATION
  if (grid.Design().cfg.ValueOf("DesignFlow.Legalization", false))
  {
    Legalization(grid);
    grid.Design().Plotter.ShowPlacement();
    grid.Design().Plotter.SaveMilestoneImage("LEG");
    
    WRITELINE("");
    ALERT("STA after legalization:");
    STA(grid.Design());
    return true;
  }
  return false;
}

bool DoDetailedPlacementIfRequired(HDPGrid& grid)
{
  if (grid.Design().cfg.ValueOf("DesignFlow.DetailedPlacement", false))
  {
    DetailedPlacement(grid);
    grid.Design().Plotter.ShowPlacement();
    grid.Design().Plotter.SaveMilestoneImage("DP");

    WRITELINE("");
    ALERT("STA after detailed placement:");
    STA(grid.Design());
    return true;
  }
  return false;
}

void DoSTAIfCan(HDesign& hd)
{
  if (hd.CanDoTiming())
  {
    WRITELINE("");
    ALERT("Running STA:");
    STA(hd);
    FindCriticalPaths(hd);
    PrintTimingReport(hd, hd.cfg.ValueOf("CriticalPaths.countLogReportCriticalPaths", 0));
    PlotMostCriticalPaths(hd, hd.cfg.ValueOf("CriticalPaths.countPlotCriticalPaths", 0));
  }
}

void PlotCongestionMapIfRequired(HDPGrid& grid)
{
  if (grid.Design().cfg.ValueOf("DesignFlow.DrawCongestionMap", false))
    PlotCongestionMaps(grid);
}

void RunFGRRoutingIfRequired(HDPGrid& grid)
{
  if (grid.Design().cfg.ValueOf("DesignFlow.FGRRouting", false))
  {
    if (grid.Design().cfg.ValueOf("FGRRouting.PrintToRoutersFormats", false))
    {
      PrintToFastRouterFormat(grid, grid.Design().cfg.ValueOf("FGRRouting.ISPDFileName",
                                                     "bench.fr"));
      
      PrintToBoxRouterFormat(grid, grid.Design().cfg.ValueOf("FGRRouting.LabyrinthFileName",
                                                    "bench.br"));
    }
    fgr::FGRRouting(grid);
  }
}

void UpdateNetWeightsIfRequired(HDesign& hd, int iteration)
{
  if (hd.cfg.ValueOf("NetWeighting.useNetWeights", false))
    if (hd.cfg.ValueOf("DesignFlow.nMacroIterations", 0) > 1)
        PrepareNextNetWeightingLoop(hd, iteration);
}

void RunFlow(HDesign& hd, TableFormatter& flowMetrics)
{
  //START MACROLOOP OF DESIGN
  for (int i = 0; i < hd.cfg.ValueOf("DesignFlow.nMacroIterations", 1); i++)
  {
    if (hd.cfg.ValueOf("NetWeighting.useNetWeights", false))
      ImportNetWeights(hd, hd.cfg.ValueOf("NetWeighting.netWeightsImportFileName"));

    if (DoRandomPlacementIfRequired(hd))
      WriteFlowMetrics(flowMetrics, hd, "RandomPlacement", i + 1);
    if (DoGlobalPlacementIfRequired(hd))
      WriteFlowMetrics(flowMetrics, hd, "GlobalPlacement", i + 1);
    if (DoLRTimingDrivenPlacementIfRequired(hd))
      WriteFlowMetrics(flowMetrics, hd, "LRPlacement", i + 1);

    HDPGrid DPGrid(hd);

    if (DoLegalizationIfRequired(DPGrid))
      WriteFlowMetrics(flowMetrics, hd, "Legalization", i + 1);
    if (DoDetailedPlacementIfRequired(DPGrid))
      WriteFlowMetrics(flowMetrics, hd, "DetailedPlacement", i + 1);

    DoSTAIfCan(hd);

    PlotCongestionMapIfRequired(DPGrid);
    RunFGRRoutingIfRequired(DPGrid);

    if (hd.CanDoTiming() && Utils::TNS(hd) == 0 && Utils::WNS(hd) == 0)
    {
      ALERT("We have satisfied the timing constraints");
      break;// Exit the main loop of the placement
    }

    UpdateNetWeightsIfRequired(hd, i);
  }
  //BUFFERING
  if (hd.cfg.ValueOf("DesignFlow.Buffering", false))
  {
    ExportDEF(hd, "bb_" + hd.Circuit.Name() + ".def");

    if (hd.cfg.ValueOf("Buffering.DoIterative", false))
      InsertRepeaters2(hd, hd.cfg.ValueOf("Buffering.Iterations", 40), hd.cfg.ValueOf("Buffering.Percent", 0.70));
    else
      InsertRepeaters2(hd, hd.cfg.ValueOf("Buffering.Percent", 0.70));
    STA(hd);
    //ALERTFORMAT(("TWLbl=%.6f",Utils::CalculateTWL(hd)));
    WriteFlowMetrics(flowMetrics, hd, "Buffering", "");

    HDPGrid DPGrid(hd);
    if (DoLegalizationIfRequired(DPGrid))
      WriteFlowMetrics(flowMetrics, hd, "Legalization", "buf");
    //STA(hd);
    DoSTAIfCan(hd);
    ALERTFORMAT(("TWLal=%.6f",Utils::CalculateTWL(hd)));
  }

  //EXPORT
  hd.Plotter.ShowPlacement();
  hd.Plotter.SaveMilestoneImage("FINAL");
  ExportDEF(hd, hd.cfg.ValueOf("benchmark.exportDEFFileName"));

  WRITELINE("");
}