#include "VLSIFlow.h"
#include "FLUTERoute.h"
#include "Parser.h"
#include "Utils.h"
#include "Timing.h"
#include "GlobalPlacement.h"
#include "Reporting.h"
#include "Auxiliary.h"
#include "Logging.h"
#include "LRSizing.h"

using namespace libconfig;

void RunFlowWrapper(HDesign& hd, TableFormatter& flowMetrics)
{
    try //TODO: here was __try
    {
        RunFlow(hd, flowMetrics);
    }
    catch(...) // and here __finally, need to think about it more
    {
        ALERT("We've caught exception! Please investigate this!");
    }

    flowMetrics.Print();
    WRITELINE();
    ALERT("Thats All!");
}

void InitializeDesign(HDesign& design, int argc, char** argv)
{
    design.Initialize();

    design.cfg.LoadConfiguration(argc > 1 ? argv[1] : "./cfg/default.cfg");
    design.cfg.SetArguments(argc, argv);

    ParseLEF(design);
    if (design.cfg.Defined("benchmark.lib"))
        ParseLIB(design);

    TranslateTechnology(design);

    ParseDEF(design);

    Logger::RenameDuplicatedFiles(Aux::CreateCoolFileName(
        "log\\",
        design.cfg.ValueOf("params.techname", "IWLS") + string("_") + design.Circuit.Name(),
        "log"));

    if (design.cfg.ValueOf("DesignFlow.SkipSpecialNets", false))
        Utils::SkipSpecialNets(design);//WARNING: must be called before InitializeTiming
    if (design.cfg.ValueOf("DesignFlow.SkipBrokenNets", false))
        Utils::SkipBrokenNets(design);//WARNING: must be called before InitializeTiming

    //initialize routing
    design.SteinerPoints.Initialize();
    design.Wires.Initialize();
    InitializeTiming(design);
    design.Plotter->Initialize();
}

int main(int argc, char** argv)
{
    try
    {
        //global initializations
        ::gCfg.LoadConfiguration("./cfg/itlVLSI.cfg");
        Logger::InitializeLogging(::gCfg);
        Logger::Global.PrintRevisionNumber();

        //initialize routing
        InitFLUTE();

        //benchmark initialization
        HDesign design;
        InitializeDesign(design, argc, argv);

        PinPlacement(design);//TODO: move from global placement to separate files.
        design.Plotter->ShowPlacement(HPlotter::WAIT_3_SECONDS);

        //REPORT CIRCUIT INFO
        ReportBenchmarkStatistics(design);
        ReportPlacementArea(design);
        ReportCellsByMacroFunction(design);
        //ReportNetsInfo(design);

        TableFormatter flowMetrics("Placement Flow Stages Table");
        InitFlowMetricsTable(flowMetrics, design);

        RunFlowWrapper(design, flowMetrics);

        return 0;
    }
    catch(ParseException& ex)
    {
        GLOGCRITICAL(LOGINPLACE, "Configuration error on line %d: %s\n", ex.getLine(), ex.getError());
    }
    catch(SettingNotFoundException& nfex)
    {
        GLOGCRITICAL(LOGINPLACE, "Configuration setting not found: %s\n", nfex.getPath());
    }
    catch(SettingTypeException& stex)
    {
        GLOGCRITICAL(LOGINPLACE, "Configuration satting has invalid type: %s\n", stex.getPath());
    }
    catch(SettingException& sex)
    {
        GLOGCRITICAL(LOGINPLACE, "Configuration setting error: %s\n", sex.getPath());
    }
    catch(ConfigException& cex)
    {
        GLOGCRITICAL(LOGINPLACE, "Configuration exception!\n%s\n", cex.what());
    }

    return 1;
}
