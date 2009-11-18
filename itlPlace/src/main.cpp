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

using namespace libconfig;

int main(int argc, char** argv)
{
  try
  {
    bool isCyclesEnabled = false;
    
    gCfg.LoadConfiguration("itlVLSI.cfg");
    InitializeLogging();
    PrintRevisionNumber();

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
        GlobalPlacement(hd, hd.cfg.ValueOf("params.objective"));
        hd.Plotter.ShowPlacement();
        hd.Plotter.SaveMilestoneImage("GP");
      }

      //LR-TDP
      if (hd.cfg.ValueOf("DesignFlow.LR", false))
      {
        GlobalPlacement(hd, "LR");
        hd.Plotter.ShowPlacement();
        hd.Plotter.SaveMilestoneImage("LR");
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

        //BufferingAndReport(hd);
        if (0)
        {
          ConfigContext ctx = hd.cfg.OpenContext("Buffering");
          VanGinneken vg(hd);
          BufferInfo buf = BufferInfo::Create(hd);

          HNetWrapper bufferedNet = hd[hd.Nets.Null()];

          double TNS = Utils::TNS(hd);
          double WNS = Utils::WNS(hd);
          ExportDEF(hd, "def_1.def");

          TimingHelper th(hd);

          //double vgSlack = GetBufferedNetMaxDelay(net, netInfo, m_AvailableBuffers[0]);

          for (HNets::ActiveNetsEnumeratorW net = hd.Nets.GetNetsEnumeratorW(); net.MoveNext(); )
          {
            //if (net.Name() == "n_4598")
            {
              NetInfo netInfo = NetInfo::Create(hd, net, buf);
              double before_buffering = th.GetBufferedNetMaxDelay(net, netInfo, buf);
              if (vg.MathBuffering(net) > 0)
              {
                bufferedNet = net;
                //break;
              }
              double after_buffering = th.GetBufferedNetMaxDelay(net, netInfo, buf);
              WRITELINE("before %.10f", before_buffering);
              WRITELINE("after  %.10f", after_buffering);
              break;
            }
          }

          STA(hd);

          double TNS1 = Utils::TNS(hd);
          double WNS1 = Utils::WNS(hd);

          WRITELINE("TNS: %.10f", TNS - TNS1);
          WRITELINE("WNS: %.10f", WNS - WNS1);
          ExportDEF(hd, "def_2.def");
        }
      }

      if (hd.cfg.ValueOf("DesignFlow.DrawCongestionMap", false))
      {          
        PlotCongestionMaps(DPGrid);
      }

      if (hd.cfg.ValueOf("DesignFlow.FGRRouting", false))
      {
        if (hd.cfg.ValueOf("FGRRouting.PrintToRoutersFormats", false))
        {
          PrintToFastRouterFormat(DPGrid, hd.cfg.ValueOf("FGRRouting.ISPDFileName",
                                                         "bench.fr"));
          
          PrintToBoxRouterFormat(DPGrid, hd.cfg.ValueOf("FGRRouting.LabyrinthFileName",
                                                        "bench.br"));
        }
        fgr::FGRRouting(DPGrid);
      }

      if (Utils::TNS(hd) == 0 && Utils::WNS(hd) == 0)
      {
        // Exit the main loop of the placement
        ALERT("We have satisfied the timing constraints");
        break;
      }
    }
    
    //BUFFERING
    if (hd.cfg.ValueOf("DesignFlow.Buffering", false))
    {
      if (hd.cfg.ValueOf("Buffering.BufferingType", "Test") == string("Test"))
        TestBuffering(hd);
      else
        NetlistBuffering(hd);
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