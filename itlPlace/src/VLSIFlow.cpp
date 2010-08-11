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
#include "Auxiliary.h"
#include "PlacementQualityAnalyzer.h"
#include "VanGinnekenAlgorithm.h"
#include "HippocratePlacement.h"
#include "LRSizing.h"

void FlowMetricsTableAddBorder(TableFormatter& fmt, HDesign& design)
{
	fmt.NewBorderRow();
	fmt.SetCell(0, "-", fmt.NumOfColumns(), TableFormatter::Align_Fill);
}

void InitFlowMetricsTable(TableFormatter& fmt, HDesign& design)
{
    fmt.NewHeaderRow();
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

    FlowMetricsTableAddBorder(fmt, design);
}

void WriteFlowMetrics(TableFormatter& fmt, HDesign& design, const char* stageName, const string& tag)
{
    fmt.NewRow();
    fmt.SetCell(0, stageName);
    fmt.SetCell(1, tag);
    fmt.SetCell(2, Logger::GetUptime());
    fmt.SetCell(3, design.Cells.CellsCount());
    fmt.SetCell(4, Utils::CalculateHPWL(design, true));
    fmt.SetCell(5, Utils::CalculateTWL(design));
    if (design.CanDoTiming())
    {
        fmt.SetCell(6, Utils::TNS(design));
        fmt.SetCell(7, Utils::WNS(design));
    }

    design.Plotter.ShowPlacement();
    design.Plotter.SaveMilestoneImage(tag.c_str());
}

bool DoRandomPlacementIfRequired(HDesign& hd, const char* cfgOptName)
{
    if (hd.cfg.ValueOf(cfgOptName, false))
    {
        RandomPlacement(hd);

        WRITELINE("");
        if (hd.CanDoTiming()) ALERT("STA after random placement:");
        STA(hd);
        return true;
    }
    return false;
}

bool DoGlobalPlacementIfRequired(HDesign& hd, const char* cfgOptName)
{
    //GLOBAL PLACEMENT
    if (hd.cfg.ValueOf(cfgOptName, false))
    {
        GlobalPlacement(hd, hd.cfg.ValueOf("params.objective"));

        WRITELINE("");
        if (hd.CanDoTiming()) ALERT("STA after global placement:");
        STA(hd);
        return true;
    }
    return false;
}

bool DoLRTimingDrivenPlacementIfRequired(HDesign& hd, const char* cfgOptName)
{
    //LR-TDP
    if (hd.cfg.ValueOf(cfgOptName, false))
    {
        GlobalPlacement(hd, "LR");

        WRITELINE("");
        if (hd.CanDoTiming()) ALERT("STA after LR placement:");
        STA(hd);
        //ALERT("No wire STA:");
        //NoWireDelaySTA(hd);
        return true;
    }
    return false;
}

bool DoLegalizationIfRequired(HDPGrid& grid, const char* cfgOptName)
{
    //LEGALIZATION
    if (grid.Design().cfg.ValueOf(cfgOptName, false))
    {
        Legalization(grid);

        WRITELINE("");
        if (grid.Design().CanDoTiming()) ALERT("STA after legalization:");
        STA(grid.Design());
        //ALERT("No wire STA:");
        //NoWireDelaySTA(grid.Design());
        return true;
    }
    return false;
}

bool DoDetailedPlacementIfRequired(HDPGrid& grid, const char* cfgOptName)
{
    if (grid.Design().cfg.ValueOf(cfgOptName, false))
    {
        DetailedPlacement(grid);

        WRITELINE("");
        if (grid.Design().CanDoTiming()) ALERT("STA after detailed placement:");
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
        hd.Plotter.PlotMostCriticalPaths(hd, hd.cfg.ValueOf("CriticalPaths.countPlotCriticalPaths", 0));
    }
}

bool IsPlaced(HDesign& hd, double eps = 0.001)
{
  HCells::PlaceableCellsEnumeratorW cell = hd.Cells.GetPlaceableCellsEnumeratorW();
  cell.MoveNext(); //get first cell
  double startCellX = cell.X();
  double startCellY = cell.Y();

  while (cell.MoveNext())
  {
    //if coords of current cell are not equal to coord of start cell
    if ((fabs(cell.X() - startCellX) >= eps) || (fabs(cell.X() - startCellX) >= eps))
      return true; //not all elements are placed to the same place
  }

  return false; //all elements are placed to the same place
}

bool DoHippocratePlacementIfRequired(HDesign& hd, const char* cfgOptName)
{
    HDPGrid grid(hd);	
    if (grid.Design().cfg.ValueOf(cfgOptName, false))
    {
        grid.FindCellsPositions();
        HippocratePlacementInit();

        bool LogEveryIteration;
        if(hd.cfg.ValueOf("HippocratePlacement.LogEveryIteration", false))
            LogEveryIteration=true;
        else LogEveryIteration=false;

        TimingHPWLWatcher thpwlWatcher(hd, LogEveryIteration);
        StatisticsAnalyser stat;

        ALERT("STA before Legalization Hippocrate placement:");     
        STA(hd,true);

        if (!CheckGridBoundary(grid, 2, true)|| !CheckGridConsistency(grid, 2, 1e-6, true)|| !CheckOverlaps(grid,2, true))
        {
            ALERT("NOT LEGALIZED before hippocrate!!");
            Legalization(grid);
        }else ALERT("Legalized before Hippocrate");

        ALERT("STA before Hippocrate placement:");     
        STA(hd,true);
        double oldTNS=Utils::TNS(hd);
        double oldWNS=Utils::WNS(hd);
        ALERT("HPWL before Hippocrate placement: %f", Utils::CalculateHPWL(hd, true));

        DoHippocratePlacement(grid, hd, stat, thpwlWatcher);

        if (!CheckGridBoundary(grid, 2, true)|| !CheckGridConsistency(grid, 2, 1e-6, true)|| !CheckOverlaps(grid, 2, true))
        {
            ALERT("Not Legalized after!");
        }
        else ALERT("!!!Legalized after!");
        hd.Plotter.ShowPlacement();

        ALERT("STA after Hippocrate placement:");     
        STA(hd,true);
        ALERT("HPWL after Hippocrate placement: %f", Utils::CalculateHPWL(hd, true));

        HippocratePlacementFinalize();

        stat.doReport();

        //DetailedPlacement(grid);

        //WRITELINE("");
        //if (grid.Design().CanDoTiming()) ALERT("STA after detailed placement:");
        //STA(grid.Design());
        return true;
    }
    return false;
}

bool DoLRSizingIfRequired(HDPGrid& grid, const char* cfgOptName)
{
		if (grid.Design().cfg.ValueOf(cfgOptName, false))
		{
				//DetailedPlacement(grid);

				WRITELINE("LRSizing started");
				//if (grid.Design().CanDoTiming()) ALERT("STA after detailed placement:");
				//STA(grid.Design());
				//LambdaMatrix lm (grid,grid.Design());
				LRSizer sizer(grid.Design());
				
				sizer.doLRSizing();
				//DoLRSizing(grid, grid.Design());
				WRITELINE("LRSizing finished");
				return true;
		}
		return false;
}


void PlotCongestionMapIfRequired(HDPGrid& grid)
{
    if (grid.Design().cfg.ValueOf("DesignFlow.DrawCongestionMap", false))
        grid.Design().Plotter.PlotCongestionMaps(grid);
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

    if ((hd.cfg.ValueOf("Logger.PrintStartInformations", false))/* && IsPlaced(hd)*/)
    {
        WRITELINE("");
        ALERT(Color_White, "INITIAL STATE");
        DoSTAIfCan(hd);
        WriteFlowMetrics(flowMetrics, hd, "InitialState", "INIT");
    }

    if (hd.cfg.ValueOf("DesignFlow.nMacroIterations", 0) > 0)
    {
        ConfigContext ctx = hd.cfg.OpenContext("MacroLoop");
        PlacementQualityAnalyzer QA = PlacementQualityAnalyzer(hd, hd.cfg.ValueOf(".QAcriteria",
            PlacementQualityAnalyzer::GetMetric(PlacementQualityAnalyzer::MetricTNSleg)));

        for (int i = 0; i < hd.cfg.ValueOf("DesignFlow.nMacroIterations", 0); i++)
        {
            if (hd.cfg.ValueOf("NetWeighting.useNetWeights", false) && i > 0)
                ImportNetWeights(hd, hd.cfg.ValueOf("NetWeighting.netWeightsImportFileName"));

            if (DoRandomPlacementIfRequired(hd, "DesignFlow.LoopRandomPlacement"))
                WriteFlowMetrics(flowMetrics, hd, "RandomPlacement", Aux::Format("RND%d", i + 1));
            if (DoGlobalPlacementIfRequired(hd, "DesignFlow.LoopGlobalPlacement"))
                WriteFlowMetrics(flowMetrics, hd, "GlobalPlacement", Aux::Format("GP%d", i + 1));
            if (DoLRTimingDrivenPlacementIfRequired(hd, "DesignFlow.LoopLR"))
                WriteFlowMetrics(flowMetrics, hd, "LRPlacement", Aux::Format("LR%d", i + 1));

            HDPGrid DPGrid(hd);

            if (DoLegalizationIfRequired(DPGrid, "DesignFlow.LoopLegalization"))
                WriteFlowMetrics(flowMetrics, hd, "Legalization",  Aux::Format("LEG%d", i + 1));
            if (DoDetailedPlacementIfRequired(DPGrid, "DesignFlow.LoopDetailedPlacement"))
                WriteFlowMetrics(flowMetrics, hd, "DetailedPlacement", Aux::Format("DP%d", i + 1));

            DoSTAIfCan(hd);

            if (hd.CanDoTiming() && Utils::TNS(hd) == 0 && Utils::WNS(hd) == 0)
            {
                ALERT("We have satisfied the timing constraints");
                break;// Exit the main loop of the placement
            }

            QA.AnalyzeQuality(i);//i must be zero based!

            if (i < hd.cfg.ValueOf("DesignFlow.nMacroIterations", 0))
                UpdateNetWeightsIfRequired(hd, i);
        }

        QA.RestoreBestAchievedPlacement();
        QA.Report();
    }

    //PLACEMENT
    if (DoRandomPlacementIfRequired(hd, "DesignFlow.RandomPlacement"))
        WriteFlowMetrics(flowMetrics, hd, "RandomPlacement", "RND");
    if (DoGlobalPlacementIfRequired(hd, "DesignFlow.GlobalPlacement"))
        WriteFlowMetrics(flowMetrics, hd, "GlobalPlacement", "GP");
    if (DoLRTimingDrivenPlacementIfRequired(hd, "DesignFlow.LR"))
        WriteFlowMetrics(flowMetrics, hd, "LRPlacement", "LR");

    HDPGrid DPGrid(hd);

    if (DoLegalizationIfRequired(DPGrid, "DesignFlow.Legalization"))
        WriteFlowMetrics(flowMetrics, hd, "Legalization",  "LEG");
    if (DoDetailedPlacementIfRequired(DPGrid, "DesignFlow.DetailedPlacement"))
        WriteFlowMetrics(flowMetrics, hd, "DetailedPlacement", "DP");
    if (DoHippocratePlacementIfRequired(hd, "DesignFlow.HippocratePlacement"))
        WriteFlowMetrics(flowMetrics, hd, "HippocratePlacement", "HP");
		
		if (DoLRSizingIfRequired(DPGrid, "DesignFlow.LRSizing"))
				/*WriteFlowMetrics(flowMetrics, hd, "HippocratePlacement", "HP")*/;


    //DoSTAIfCan(hd);

    PlotCongestionMapIfRequired(DPGrid);
    RunFGRRoutingIfRequired(DPGrid);

    //BUFFERING
    if (hd.cfg.ValueOf("DesignFlow.Buffering", false))
    {
        ConfigContext ctx = hd.cfg.OpenContext("Buffering");
        //ExportDEF(hd, "bb_" + hd.Circuit.Name() + ".def");

        if (hd.cfg.ValueOf("Buffering.DoIterative", false))
            InsertRepeaters2(hd, hd.cfg.ValueOf("Buffering.Iterations", 40), hd.cfg.ValueOf("Buffering.Percent", 0.70));
        else
            InsertRepeaters2(hd, hd.cfg.ValueOf("Buffering.Percent", 0.70));
        STA(hd);

        WriteFlowMetrics(flowMetrics, hd, "Buffering", "BUF");

        if (DoRandomPlacementIfRequired(hd, "DesignFlow.BufRandomPlacement"))
            WriteFlowMetrics(flowMetrics, hd, "RandomPlacement", "RNDB");
        if (DoGlobalPlacementIfRequired(hd, "DesignFlow.BufGlobalPlacement"))
            WriteFlowMetrics(flowMetrics, hd, "GlobalPlacement", "GPB");
        if (DoLRTimingDrivenPlacementIfRequired(hd, "DesignFlow.BufLR"))
            WriteFlowMetrics(flowMetrics, hd, "LRPlacement", "LRB");

        HDPGrid DPGrid(hd);
        if (DoLegalizationIfRequired(DPGrid, "DesignFlow.BufLegalization"))
            WriteFlowMetrics(flowMetrics, hd, "Legalization", "LEGB");

        if (DoDetailedPlacementIfRequired(DPGrid, "DesignFlow.BufDetailedPlacement"))
            WriteFlowMetrics(flowMetrics, hd, "DetailedPlacement", "DPB");

        DoSTAIfCan(hd);
    }

    //New_BUFFERING
    if (hd.cfg.ValueOf("DesignFlow.New_Buffering", false))
    {
        ALERT("NEW BUFFERING STARTED");
        ConfigContext ctx = hd.cfg.OpenContext("New_Buffering");
        HVGAlgorithm buf(hd);
        buf.Initialize();
        buf.BufferingCriticalPath();
        //hd.VGAlgorithm.Initialize();
        //hd.VGAlgorithm.BufferingCriticalPath();
        STA(hd);
        ALERT("NEW BUFFERING FINISHED");
        WriteFlowMetrics(flowMetrics, hd, "New_Buffering", "NBUF");

        if (DoRandomPlacementIfRequired(hd, "DesignFlow.BufRandomPlacement"))
            WriteFlowMetrics(flowMetrics, hd, "RandomPlacement", "RNDB");
        if (DoGlobalPlacementIfRequired(hd, "DesignFlow.BufGlobalPlacement"))
            WriteFlowMetrics(flowMetrics, hd, "GlobalPlacement", "GPB");
        if (DoLRTimingDrivenPlacementIfRequired(hd, "DesignFlow.BufLR"))
            WriteFlowMetrics(flowMetrics, hd, "LRPlacement", "LRB");

        HDPGrid DPGrid(hd);
        if (DoLegalizationIfRequired(DPGrid, "DesignFlow.BufLegalization"))
            WriteFlowMetrics(flowMetrics, hd, "Legalization", "LEGB");

        if (DoDetailedPlacementIfRequired(DPGrid, "DesignFlow.BufDetailedPlacement"))
            WriteFlowMetrics(flowMetrics, hd, "DetailedPlacement", "DPB");

        DoSTAIfCan(hd);
    }

    FlowMetricsTableAddBorder(flowMetrics, hd);

    //EXPORT
    hd.Plotter.ShowPlacement();
    hd.Plotter.SaveMilestoneImage("FINAL", false);
    ExportDEF(hd, hd.cfg.ValueOf("benchmark.exportDEFFileName"));

    WRITELINE("");
}