#include "DetailedPlacement.h"
#include "Legalization.h"
#include "Utils.h"

#define NCELLS_TO_REPORT 20

void DetailedPlacement(HDPGrid& grid)
{
  ConfigContext ctx = grid.Design().cfg.OpenContext("DetailedPlacement");

  WRITELINE("");
  ALERT("DETAILED PLACEMENT STARTED\n");

  double wlBeforeDP = Utils::CalculateHPWL(grid.Design(), true);
  double wlBeforeIteration = wlBeforeDP;
  double wlAfterIteration  = wlBeforeDP;
  
  ALERTFORMAT(("%5s %5s %25s %11s %11s", "Iter", "Stage", "WireLength", "Ratio", "DPRatio"));
  ALERTFORMAT(("%5d %5s %25f %11s %11s\n", 1, "-", wlBeforeDP, "-", "-"));

  int iteration = 0;
  double stoppingCriteriaValue = grid.Design().cfg.ValueOf(".StoppingCriteria", 0.001);
  do
  {
    iteration++;
    wlBeforeIteration = wlAfterIteration;

    //GLOBAL SWAP
    GlobalSwap(grid);
    if (!CheckGridBoundary(grid, NCELLS_TO_REPORT, true)
      || !CheckGridConsistency(grid, NCELLS_TO_REPORT, 1e-6, true)
      || !CheckOverlaps(grid, NCELLS_TO_REPORT, true)
      )
      Legalization(grid);
    grid.Design().Plotter.Refresh();

    double wlAfterGlobalSwap = Utils::CalculateHPWL(grid.Design(), true);
    ALERTFORMAT(("%5d %5s %25f %10f%% %10f%%",
      iteration,
      "GS", 
      wlAfterGlobalSwap,
      (wlBeforeIteration - wlAfterGlobalSwap) / wlBeforeIteration * 100.0,
      (wlBeforeIteration - wlAfterGlobalSwap) / wlBeforeDP * 100.0));

    //VERTICAL SEARCH
    VerticalSearch(grid);
    if (!CheckGridBoundary(grid, NCELLS_TO_REPORT, true)
      || !CheckGridConsistency(grid, NCELLS_TO_REPORT, 1e-6, true)
      || !CheckOverlaps(grid, NCELLS_TO_REPORT, true)
      )
      Legalization(grid);
    grid.Design().Plotter.Refresh();

    double wlAfterVerticalSearch = Utils::CalculateHPWL(grid.Design(), true);
    ALERTFORMAT(("%5d %5s %25f %10f%% %10f%%",
      iteration,
      "VS",
      wlAfterVerticalSearch,
      (wlAfterGlobalSwap - wlAfterVerticalSearch) / wlBeforeIteration * 100.0,
      (wlAfterGlobalSwap - wlAfterVerticalSearch) / wlBeforeDP * 100.0));

    //HORIZONTAL SEARCH
    HorizontalSearch(grid);
    if (!CheckGridBoundary(grid, NCELLS_TO_REPORT, true)
      || !CheckGridConsistency(grid, NCELLS_TO_REPORT, 1e-6, true)
      || !CheckOverlaps(grid, NCELLS_TO_REPORT, true)
      )
      Legalization(grid);
    grid.Design().Plotter.Refresh();

    double wlAfterHorizontalSearch = Utils::CalculateHPWL(grid.Design(), true);
    ALERTFORMAT(("%5d %5s %25f %10f%% %10f%%",
      iteration,
      "HS",
      wlAfterHorizontalSearch,
      (wlAfterVerticalSearch - wlAfterHorizontalSearch) / wlBeforeIteration * 100.0,
      (wlAfterVerticalSearch - wlAfterHorizontalSearch) / wlBeforeDP * 100.0));

    wlAfterIteration = wlAfterHorizontalSearch;
    ALERTFORMAT(("%5d %5s %25f %10f%% %10f%%\n",
      iteration,
      "",
      wlAfterIteration,
      (wlBeforeIteration - wlAfterIteration) / wlBeforeIteration * 100.0,
      (wlBeforeIteration - wlAfterIteration) / wlBeforeDP * 100.0));
  } while ((1.0 - wlAfterIteration / wlBeforeIteration) > stoppingCriteriaValue);

  ALERTFORMAT(("HPWL before detailed placement: %f", wlBeforeDP));
  ALERTFORMAT(("HPWL after  detailed placement: %f", wlAfterIteration));
  ALERTFORMAT(("Improvement after detailed placement: %f%%", 100.0 * (1.0 - wlAfterIteration / wlBeforeDP) ));
  
  ALERT("DETAILED PLACEMENT FINISHED");
}