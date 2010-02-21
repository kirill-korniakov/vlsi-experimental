#include "VLSIFlow.h"
#include "FLUTERoute.h"
#include "Parser.h"
#include "Utils.h"
#include "Timing.h"
#include "GlobalPlacement.h"
#include "Reporting.h"

using namespace libconfig;

void RunFlowWrapper(HDesign& hd, TableFormatter& flowMetrics)
{
  __try
  {
    RunFlow(hd, flowMetrics);
  }
  __finally
  {
    flowMetrics.Print();

    WRITELINE("");
    ALERT("Thats All!");
  }
}

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

  //initialize routing
  design.SteinerPoints.Initialize();
  design.Wires.Initialize();

  InitializeTiming(design);

  design.Plotter.Initialize();
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

    PinPlacement(hd);//TODO: move from global placement to separate files.
    hd.Plotter.ShowPlacement(HPlotter::WAIT_3_SECONDS);

    //REPORT CIRCUIT INFO
    ReportBenchmarkStatistics(hd);
    ReportPlacementArea(hd);
    ReportCellsByMacroFunction(hd);

    TableFormatter flowMetrics;
    InitFlowMetricsTable(flowMetrics, hd);

    RunFlowWrapper(hd, flowMetrics);

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