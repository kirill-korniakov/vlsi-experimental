#include "DetailedPlacement.h"
#include "Legalization.h"
#include "Utils.h"

#define NCELLS_TO_REPORT 20

void LegalityControl(HDPGrid& grid)
{
  if (!CheckGridBoundary(grid, NCELLS_TO_REPORT, true)
    || !CheckGridConsistency(grid, NCELLS_TO_REPORT, 1e-6, true)
    || !CheckOverlaps(grid, NCELLS_TO_REPORT, true)
    )
    Legalization(grid);
}

void DetailedPlacement(HDPGrid& grid)
{
  ConfigContext ctx = grid.Design().cfg.OpenContext("DetailedPlacement");

  WRITELINE("");
  ALERT("DETAILED PLACEMENT STARTED\n");

  LegalityControl(grid);

  double wlBeforeDP = Utils::CalculateHPWL(grid.Design(), true);
  double wlBeforeIteration = wlBeforeDP;
  double wlAfterIteration  = wlBeforeDP;

  bool isGS = grid.Design().cfg.ValueOf(".GlobalSwap.active", true);
  bool isVS = grid.Design().cfg.ValueOf(".VerticalSearch.active", true);
  bool isHS = grid.Design().cfg.ValueOf(".HorizontalSearch.active", true);

  double wlAfterGlobalSwap = 0.0;
  double wlAfterVerticalSearch = 0.0;
  double wlAfterHorizontalSearch = 0.0;
  
  ALERT("%5s %5s %25s %11s %11s", "Iter", "Stage", "WireLength", "Ratio", "DPRatio");
  ALERT("%5d %5s %25f %11s %11s\n", 1, "-", wlBeforeDP, "-", "-");

  int iteration = 0;
  double stoppingCriteriaValue = grid.Design().cfg.ValueOf(".StoppingCriteria", 0.001);
  do
  {
    iteration++;
    wlBeforeIteration = wlAfterIteration;

    //GLOBAL SWAP
    if (isGS)
    {
      GlobalSwap(grid);
      LegalityControl(grid);
      grid.Design().Plotter.Refresh();

      wlAfterGlobalSwap = Utils::CalculateHPWL(grid.Design(), true);
      ALERT("%5d %5s %25f %10f%% %10f%%",
        iteration,
        "GS", 
        wlAfterGlobalSwap,
        (wlBeforeIteration - wlAfterGlobalSwap) / wlBeforeIteration * 100.0,
        (wlBeforeIteration - wlAfterGlobalSwap) / wlBeforeDP * 100.0);
    }
    else
      wlAfterGlobalSwap = wlBeforeIteration;

    //VERTICAL SEARCH
    if (isVS)
    {
      VerticalSearch(grid);
      LegalityControl(grid);
      grid.Design().Plotter.Refresh();

      wlAfterVerticalSearch = Utils::CalculateHPWL(grid.Design(), true);
      ALERT("%5d %5s %25f %10f%% %10f%%",
        iteration,
        "VS",
        wlAfterVerticalSearch,
        (wlAfterGlobalSwap - wlAfterVerticalSearch) / wlBeforeIteration * 100.0,
        (wlAfterGlobalSwap - wlAfterVerticalSearch) / wlBeforeDP * 100.0);
    }
    else
      wlAfterVerticalSearch = wlAfterGlobalSwap;

    //HORIZONTAL SEARCH
    if (isHS)
    {
      HorizontalSearch(grid);
      LegalityControl(grid);
      grid.Design().Plotter.Refresh();

      wlAfterHorizontalSearch = Utils::CalculateHPWL(grid.Design(), true);
      ALERT("%5d %5s %25f %10f%% %10f%%",
        iteration,
        "HS",
        wlAfterHorizontalSearch,
        (wlAfterVerticalSearch - wlAfterHorizontalSearch) / wlBeforeIteration * 100.0,
        (wlAfterVerticalSearch - wlAfterHorizontalSearch) / wlBeforeDP * 100.0);
    }

    if (isHS)
      wlAfterIteration = wlAfterHorizontalSearch;
    else if (isVS)
      wlAfterIteration = wlAfterVerticalSearch;
    else
      wlAfterIteration = wlAfterGlobalSwap;

    //wlAfterIteration = wlAfterHorizontalSearch;
    ALERT("%5d %5s %25f %10f%% %10f%%\n",
      iteration,
      "",
      wlAfterIteration,
      (wlBeforeIteration - wlAfterIteration) / wlBeforeIteration * 100.0,
      (wlBeforeIteration - wlAfterIteration) / wlBeforeDP * 100.0);
  } while ((1.0 - wlAfterIteration / wlBeforeIteration) > stoppingCriteriaValue);

  ALERT("HPWL before detailed placement: %f", wlBeforeDP);
  ALERT("HPWL after  detailed placement: %f", wlAfterIteration);
  ALERT("Improvement after detailed placement: %f%%", 100.0 * (1.0 - wlAfterIteration / wlBeforeDP));
  
  ALERT("DETAILED PLACEMENT FINISHED");
}