#include "HDesign.h"
#include "Parser.h"
#include "Auxiliary.h"
#include "FLUTERoute.h"
#include "GlobalPlacement.h"
#include "Legalization.h"
#include "DetailedPlacement.h"
#include "Reporting.h"
#include "Timing.h"
#include "STA.h"
#include "Utils.h"
#include "Buffering.h"
#include "TileGrid.h"
#include "FGR.h"
#include "ExportToRouters.h"
#include "SensitivityGuidedNetWeighting.h"

using namespace libconfig;

void InitializeDesign(HDesign& design, int argc, char** argv)
{
  design.Initialize();

  design.cfg.LoadConfiguration(argc > 1 ? argv[1] : "default.cfg");
  design.cfg.SetArguments(argc, argv);

  ParseLEF(design);
  if (design.cfg.Defined("benchmark.lib"))
    ParseLIB(design);

  TranslateTechnology(design);

  ParseDEF(design);

  if (design.cfg.ValueOf("DesignFlow.SkipSpecialNets", false))
    Utils::SkipSpecialNets(design);//WARNING: must be called before InitializeTiming

  design.SteinerPoints.Initialize();//initialize routing
  InitializeTiming(design);

  design.Plotter.Initialize();
}

void DoRandomPlacementIfRequired(HDesign& hd)
{
  if (hd.cfg.ValueOf("DesignFlow.RandomPlacement", false))
  {
    RandomPlacement(hd);
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("RP");
  }
}

void DoGlobalPlacementIfRequired(HDesign& hd)
{
  //GLOBAL PLACEMENT
  if (hd.cfg.ValueOf("DesignFlow.GlobalPlacement", false))
  {
    GlobalPlacement(hd, hd.cfg.ValueOf("params.objective"));
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("GP");
  }
}

void DoLRTimingDrivenPlacementIfRequired(HDesign& hd)
{
  //LR-TDP
  if (hd.cfg.ValueOf("DesignFlow.LR", false))
  {
    GlobalPlacement(hd, "LR");
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("LR");
  }
}

void DoLegalizationIfRequired(HDPGrid& grid)
{
  //LEGALIZATION
  if (grid.Design().cfg.ValueOf("DesignFlow.Legalization", false))
  {
    //WRITELINE("");
    //ALERT("STA before legalization:");
    //STA(grid.Design());

    Legalization(grid);
    grid.Design().Plotter.ShowPlacement();
    grid.Design().Plotter.SaveMilestoneImage("LEG");
    
    WRITELINE("");
    ALERT("STA after legalization:");
    STA(grid.Design());
  }
}

void DoDetailedPlacementIfRequired(HDPGrid& grid)
{
  if (grid.Design().cfg.ValueOf("DesignFlow.DetailedPlacement", false))
  {
    WRITELINE("");
    ALERT("STA before detailed placement:");
    STA(grid.Design());

    DetailedPlacement(grid);
    grid.Design().Plotter.ShowPlacement();
    grid.Design().Plotter.SaveMilestoneImage("DP");

    WRITELINE("");
    ALERT("STA after detailed placement:");
    STA(grid.Design());
  }
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

int main(int argc, char** argv)
{
  try
  {
    //global initializations
    gCfg.LoadConfiguration("itlVLSI.cfg");
    InitializeLogging();
    PrintRevisionNumber();
    InitFLUTE();//initialize routing

    //benchmark initialization
    HDesign hd;
    InitializeDesign(hd, argc, argv);

    PinPlacement(hd);
    hd.Plotter.ShowPlacement(HPlotter::WAIT_3_SECONDS);

    //REPORT CIRCUIT INFO
    ReportBenchmarkStatistics(hd);
    ReportPlacementArea(hd);
    ReportCellsByMacroFunction(hd);

    //START MACROLOOP OF DESIGN
    for (int i = 0; i < hd.cfg.ValueOf("DesignFlow.nMacroIterations", 1); i++)
    {
      if (hd.cfg.ValueOf("NetWeighting.useNetWeights", false))
        ImportNetWeights(hd, hd.cfg.ValueOf("NetWeighting.netWeightsImportFileName"));

      DoRandomPlacementIfRequired(hd);
      DoGlobalPlacementIfRequired(hd);
      DoLRTimingDrivenPlacementIfRequired(hd);

      HDPGrid DPGrid(hd);

      DoLegalizationIfRequired(DPGrid);
      DoDetailedPlacementIfRequired(DPGrid);

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
      InsertRepeaters2(hd);
      STA(hd);

      HDPGrid DPGrid(hd);
      DoLegalizationIfRequired(DPGrid);
      STA(hd);
    }

    //EXPORT
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("FINAL");
    ExportDEF(hd, hd.cfg.ValueOf("benchmark.exportDEFFileName"));

    WRITELINE("");
    ALERT("Thats All!");

    return 0;
  }
  catch(ParseException& ex)
  {
    printf("Configuration error on line %d: %s\n", ex.getLine(), ex.getError());
  }
  catch(SettingNotFoundException nfex)
  {
    printf("Configuration setting not found: %s\n", nfex.getPath());
  }
  catch(ConfigException& cex)
  {
    printf("Configuration exception!\n%s\n", cex.what());
  }

  return 1;
}