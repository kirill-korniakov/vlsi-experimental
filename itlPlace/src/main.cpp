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
#include <conio.h>

using namespace libconfig;

int main(int argc, char** argv)
{
  try
  {
    bool isCyclesEnabled = false;
    
    gCfg.LoadConfiguration("itlVLSI.cfg");
    InitializeLogging();

    InitFLUTE();

    HDesign hd;
    hd.Initialize();

    hd.cfg.LoadConfiguration(argc > 1 ? argv[1] : "default.cfg");
    hd.cfg.SetArguments(argc, argv);

    ParseLEF(hd);
    if (hd.cfg.Defined("benchmark.lib"))
      ParseLIB(hd);

    TranslateTechnology(hd);

    ParseDEF(hd);

    if (hd.cfg.ValueOf("DesignFlow.SkipSpecialNets", false))
      Utils::SkipSpecialNets(hd);//WARNING: must be called before InitializeTiming
	  
    InitializeTiming(hd);

    PinPlacement(hd);

    hd.Plotter.Initialize();
    hd.Plotter.ShowPlacement(HPlotter::WAIT_3_SECONDS);

    //REPORT CIRCUIT INFO
    ReportBenchmarkStatistics(hd);
    ReportPlacementArea(hd);
    ReportCellsByMacroFunction(hd);

    //START MACROLOOP OF DESIGN
    for (int i = 0; i < hd.cfg.ValueOf("DesignFlow.nMacroIterations", 0); i++)
    {
      if (hd.cfg.ValueOf("NetWeighting.useNetWeights", false))
        ImportNetWeights(hd, hd.cfg.ValueOf("NetWeighting.netWeightsImportFileName"));

      //RANDOM PLACEMENT
      if (hd.cfg.ValueOf("DesignFlow.RandomPlacement", false))
      {
        RandomPlacement(hd);
        hd.Plotter.ShowPlacement();
        hd.Plotter.SaveMilestoneImage("RP");
      }

      //GLOBAL PLACEMENT
      if (hd.cfg.ValueOf("DesignFlow.GlobalPlacement", false))
      {
        AnalyticalPlacement(hd);
        hd.Plotter.ShowPlacement();
        hd.Plotter.SaveMilestoneImage("GP");
      }
      
      HDPGrid DPGrid(hd);

      //LEGALIZATION
      if (hd.cfg.ValueOf("DesignFlow.Legalization", false))
      {
        WRITELINE("");
        ALERT("STA before legalization:");
        STA(hd);

        Legalization(DPGrid);
        hd.Plotter.ShowPlacement();
        hd.Plotter.SaveMilestoneImage("LEG");
        
        WRITELINE("");
        ALERT("STA after legalization:");
        STA(hd);
      }

      //DETAILED PLACEMENT
      if (hd.cfg.ValueOf("DesignFlow.DetailedPlacement", false))
      {
        WRITELINE("");
        ALERT("STA before detailed placement:");
        STA(hd);

        DetailedPlacement(DPGrid);
        hd.Plotter.ShowPlacement();
        hd.Plotter.SaveMilestoneImage("DP");

        WRITELINE("");
        ALERT("STA after detailed placement:");
        STA(hd);
      }

      //TIMING
      if (hd.cfg.ValueOf("DesignFlow.Timing", false))
      {
        WRITELINE("");
        ALERT("Timing:");
        STA(hd);

        //critical paths finding and printing
        FindCriticalPaths(hd);
        ReportCriticalPathsToLogFile(hd, hd.cfg.ValueOf("CriticalPaths.countLogReportCriticalPaths", 0));
        PlotTopCriticalPathsAndSaveImages(hd, hd.cfg.ValueOf("CriticalPaths.countPlotCriticalPaths", 0));

        if (hd.cfg.ValueOf("NetWeighting.useNetWeights", false))
        {
          if (hd.cfg.ValueOf("DesignFlow.nMacroIterations", 0) > 1)
          {
            PrepareNextLoop(hd, argc, argv, i);
          }
        }
      }

      if (hd.cfg.ValueOf("DesignFlow.DrawCongestionMap", false))
      {          
        int nHorTiles  = hd.cfg.ValueOf("CongestionMap.nHorTiles", 20);
        int nVertTiles = hd.cfg.ValueOf("CongestionMap.nVertTiles", 20);
        int nMaxLines  = hd.cfg.ValueOf("CongestionMap.nMaxLines", 50);
        int nMaxPins   = hd.cfg.ValueOf("CongestionMap.nMaxPins", 20);
        int nMaxCLines = hd.cfg.ValueOf("CongestionMap.nMaxCLines", 10);

        TileGrid tileGrid(nHorTiles, nVertTiles, DPGrid);   

        tileGrid.DrawCongestionMap(hd, nMaxLines);
        ALERT("press any key to continue...");
        _getch();
        
        tileGrid.DrawPinDensity(hd, nMaxPins);
        ALERT("press any key to continue...");
        _getch();
        
        tileGrid.DrawCriticalCongestionMap(hd, nMaxCLines);
        ALERT("press any key to continue...");
        _getch();
      }

      if (hd.cfg.ValueOf("DesignFlow.PrintToRoutersFormats", false))
      {
        PrintToFastRouterFormat(DPGrid, hd.cfg.ValueOf("PrintingToRoutersFormats.ISPDFileName", "bench.fr"));
        PrintToBoxRouterFormat(DPGrid, hd.cfg.ValueOf("PrintingToRoutersFormats.LabyrinthFileName", "bench.br"));
      }
    }
    
    //BUFFERING
    if (hd.cfg.ValueOf("DesignFlow.Buffering", false))
    {
      //FIXME: delete this block
      for (HNets::NetsEnumeratorW nIter = hd.Nets.GetNetsEnumeratorW(); nIter.MoveNext(); ) 
      {
        if (nIter.Name() == "overflw__BufferedPart_0" ||
          nIter.Name() == "overflw__BufferedPart_1")
        {
          ReportNetPinsCoordinates(hd, nIter);
          ReportNetTiming(hd, nIter);
          hd.Plotter.ShowPlacement();
          hd.Plotter.PlotNetSteinerTree(nIter, Color_Black);
          hd.Plotter.PlotText(hd.Nets.GetString<HNet::Name>(nIter));
          hd.Plotter.Refresh(HPlotter::WAIT_10_SECONDS);
        }
      }
      TestBuffering(hd);
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