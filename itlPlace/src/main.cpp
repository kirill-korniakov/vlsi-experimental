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
//#include <conio.h>
#include "FGR.h"
#include "BufferInfo.h"
#include "NetInfo.h"
#include "TimingHelpers.h"

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

        {
          ConfigContext ctx = hd.cfg.OpenContext("Buffering");

          BufferInfo buf = BufferInfo::Create(hd);
          VanGinneken vg(hd);

          WRITELINE("Buffering Parameters:");
          WRITELINE("Rb: %.20f kOhm", buf.Rb());
          WRITELINE("Cb: %.20f pF", buf.Cb());
          WRITELINE("Tb: %.20f ns", buf.Tb());
          WRITELINE("r:  %.20f kOhm/nm", hd.RoutingLayers.Physics.RPerDist);
          WRITELINE("c:  %.20f pf/nm", hd.RoutingLayers.Physics.LinearC);
          WRITELINE("Lbuf: %.20f", buf.Lbuf());
          WRITELINE("Dbuf: %.20f", buf.Dbuf());

          int buffers = 0;
          int vgbuffers = 0;

          string h1 = "fanout ",     b1 = "%6d ";
          string h2 = "vgbufs ",     b2 = "%6d ";
          string h3 = "kopt ",       b3 = "%4d ";
          string h4 = "koptd ",      b4 = "%5.3f ";
          string h5 = "   knaked ",  b5 = "%9.5f ";
          string h6 = "   T(kopt) ", b6 = "%10.8f ";
          string h7 = "  T(koptd) ", b7 = "%10.8f ";
          string hh = "   vgSlack ", bb = "%10.8f ";
          string h8 = "      T(0) ", b8 = "%10.8f ";
          string h9 = "  maxSlack ", b9 = "%10.8f ";
          string f1 = "      HPWL ", c1 = "%10.2f ";
          string f2 = "        WL ", c2 = "%10.2f ";
          string f3 = "        Rd ", c3 = "%10.8f ";
          string f4 = "        Cs ", c4 = "%10.8f ";
          string f5 = "OO ",         c5 = "%2d ";
          string f6 = "OM ",         c6 = "%2d ";
          string f7 = "OE ",         c7 = "%2d ";
          string ff = "      Name",  cc = "%10s";

          string header = h1 + h2 + h3 + h4 + h5 + h6 + h7 + h8 + hh + h9 + f1 + f2 + f3 + f4 + f5 + f6 + f7 + ff;
          string bodyer = b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + bb + b9 + c1 + c2 + c4 + c4 + c5 + c6 + c7 + cc;
          WRITELINE(header.c_str());
          for(HNets::ActiveNetsEnumeratorW net = hd.Nets.GetNetsEnumeratorW(); net.MoveNext(); )
          {
            NetInfo n = NetInfo::Create(hd, net, buf);
            buffers += n.KoptInt();

            vg.__DriverResistance = n.Rd();
            int vgbufs = vg.NetBuffering(net);
            vgbuffers += vgbufs;

            double bufferedDelay = TimingHelper(hd).GetBufferedNetMaxDelay(net, n, buf);

            WRITELINE(bodyer.c_str()
              , n.Fanout() //h1
              , vgbufs //h2
              , n.KoptInt() //h3
              , n.Kopt() //h4
              , n.KoptNaked() //h5
              , n.KoptDelay() //h6
              , n.OptimalDelay() //h7
              , n.OttenDelay() //h8
              , bufferedDelay //hh
              , n.MaxRealDelay() //h9
              , n.HPWL() //f1
              , n.WL() //f2
              , n.Rd() //f3
              , n.Cs() //f4
              , n.IsOttenApplicable() //f5
              , n.IsModifiedOttenApplicable() //f6
              , n.IsOttenApplicableExact() //f7
              , n.Name().c_str() //ff
              );
            //if ((n.IsOttenApplicable() || n.IsModifiedOttenApplicable()))
            //{
              
            //WRITELINE("%3d %3d %.5f %.5f %.5f %.5f %.5f %.5f %d %.5f %d",
            //n.Fanout(), n.KoptInt(), n.Kopt(),
            //n.KoptDelay(), n.OptimalDelay(), n.OttenDelay(), n.MaxRealDelay(), n.L(), n.IsOttenApplicable(), n.X2opt(), nets);
            //}
          }

          WRITELINE("Inserted             %d buffers", buffers);
          WRITELINE("VanGinneken Inserted %d buffers", vgbuffers);
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
    }
    
    //BUFFERING
    if (hd.cfg.ValueOf("DesignFlow.Buffering", false))
    {
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