#include "DetailedPlacement.h"
#include "Legalization.h"
#include "Utils.h"
#include "TableFormatter.h"

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

  TableFormatter dpt("Detailed Placement");
  const int col_time    = 0;
  const int col_iter    = 1;
  const int col_stage   = 2;
  const int col_HPWL    = 3;
  const int col_Ratio   = 4;
  const int col_DPRatio = 5;
  dpt.SetColumnMinWidth(col_DPRatio, 11); dpt.SetColumnAlign(col_DPRatio, TableFormatter::Align_Right);
  dpt.SetColumnMinWidth(col_Ratio, 11); dpt.SetColumnAlign(col_Ratio, TableFormatter::Align_Right);
  dpt.SetColumnMinWidth(col_HPWL, 25); dpt.SetColumnAlign(col_HPWL, TableFormatter::Align_Right);
  dpt.SetColumnMinWidth(col_stage, 5); dpt.SetColumnAlign(col_stage, TableFormatter::Align_Right);
  dpt.SetColumnMinWidth(col_iter, 5); dpt.SetColumnAlign(col_iter, TableFormatter::Align_Right);
  dpt.SetColumnMinWidth(col_time, 12); dpt.SetColumnAlign(col_time, TableFormatter::Align_Right); dpt.SetColumnPrecision(col_time, 3);

  dpt.NewHeaderRow();
  dpt.SetCell(col_time, "Time");
  dpt.SetCell(col_iter, "Iter");
  dpt.SetCell(col_stage, "Stage");
  dpt.SetCell(col_HPWL, "WireLength");
  dpt.SetCell(col_Ratio, "Ratio");
  dpt.SetCell(col_DPRatio, "DPRatio");
  dpt.FlushLastRow();

  dpt.NewRow();
  dpt.SetCell(col_time, Logger::GetUptime());
  dpt.SetCell(col_iter, 0);
  dpt.SetCell(col_stage, "-");
  dpt.SetCell(col_HPWL, wlBeforeDP);
  dpt.SetCell(col_Ratio, "-");
  dpt.SetCell(col_DPRatio, "-");
  dpt.FlushLastRow();

  ConstraintsController* checker; //for checking criteria

  //----Init checker-------------------
  if (!(grid.Design().cfg.ValueOf("DetailedPlacement.useConstraints", false)))
    checker = new HPWLChecker(grid); //check only hpwl chanhe

  else
    checker = new HippocrateChecker(grid); //check Hippocrate constraints also
  //------------------------------------

  int iteration = 0;
  double stoppingCriteriaValue = grid.Design().cfg.ValueOf(".StoppingCriteria", 0.001);
  do
  {
    iteration++;
    wlBeforeIteration = wlAfterIteration;

    dpt.NewRow();
    dpt.SetCell(col_time, "", dpt.NumOfColumns());
    dpt.FlushLastRow();

    //GLOBAL SWAP
    if (isGS)
    {
      GlobalSwap(grid, checker);
      LegalityControl(grid);
      grid.Design().Plotter->Refresh();

      wlAfterGlobalSwap = Utils::CalculateHPWL(grid.Design(), true);

      dpt.NewRow();
      dpt.SetCell(col_time, Logger::GetUptime());
      dpt.SetCell(col_iter, iteration);
      dpt.SetCell(col_stage, "GS");
      dpt.SetCell(col_HPWL, wlAfterGlobalSwap);
      dpt.SetCell(col_Ratio, (wlBeforeIteration - wlAfterGlobalSwap) / wlBeforeIteration * 100.0, "", "%");
      dpt.SetCell(col_DPRatio, (wlBeforeIteration - wlAfterGlobalSwap) / wlBeforeDP * 100.0, "", "%");
      dpt.FlushLastRow();
    }
    else
      wlAfterGlobalSwap = wlBeforeIteration;

    //VERTICAL SEARCH
    if (isVS)
    {
      VerticalSearch(grid, checker);
      LegalityControl(grid);
      grid.Design().Plotter->Refresh();

      wlAfterVerticalSearch = Utils::CalculateHPWL(grid.Design(), true);

      dpt.NewRow();
      dpt.SetCell(col_time, Logger::GetUptime());
      dpt.SetCell(col_iter, iteration);
      dpt.SetCell(col_stage, "VS");
      dpt.SetCell(col_HPWL, wlAfterVerticalSearch);
      dpt.SetCell(col_Ratio, (wlAfterGlobalSwap - wlAfterVerticalSearch) / wlBeforeIteration * 100.0, "", "%");
      dpt.SetCell(col_DPRatio, (wlAfterGlobalSwap - wlAfterVerticalSearch) / wlBeforeDP * 100.0, "", "%");
      dpt.FlushLastRow();
    }
    else
      wlAfterVerticalSearch = wlAfterGlobalSwap;

    //HORIZONTAL SEARCH
    if (isHS)
    {
      HorizontalSearch(grid, checker);
      LegalityControl(grid);
      grid.Design().Plotter->Refresh();

      wlAfterHorizontalSearch = Utils::CalculateHPWL(grid.Design(), true);

      dpt.NewRow();
      dpt.SetCell(col_time, Logger::GetUptime());
      dpt.SetCell(col_iter, iteration);
      dpt.SetCell(col_stage, "HS");
      dpt.SetCell(col_HPWL, wlAfterHorizontalSearch);
      dpt.SetCell(col_Ratio, (wlAfterVerticalSearch - wlAfterHorizontalSearch) / wlBeforeIteration * 100.0, "", "%");
      dpt.SetCell(col_DPRatio, (wlAfterVerticalSearch - wlAfterHorizontalSearch) / wlBeforeDP * 100.0, "", "%");
      dpt.FlushLastRow();
    }

    if (isHS)
      wlAfterIteration = wlAfterHorizontalSearch;
    else if (isVS)
      wlAfterIteration = wlAfterVerticalSearch;
    else
      wlAfterIteration = wlAfterGlobalSwap;

    dpt.NewRow();
    dpt.SetCell(col_time, Logger::GetUptime());
    dpt.SetCell(col_iter, iteration);
    dpt.SetCell(col_stage, "");
    dpt.SetCell(col_HPWL, wlAfterIteration);
    dpt.SetCell(col_Ratio, (wlBeforeIteration - wlAfterIteration) / wlBeforeIteration * 100.0, "", "%");
    dpt.SetCell(col_DPRatio, (wlBeforeIteration - wlAfterIteration) / wlBeforeDP * 100.0, "", "%");
    dpt.FlushLastRow();

  } while ((1.0 - wlAfterIteration / wlBeforeIteration) > stoppingCriteriaValue);

  dpt.Print(0, false, true);
  delete checker;
  WRITELINE();

  ALERT("HPWL before detailed placement: %f", wlBeforeDP);
  ALERT("HPWL after  detailed placement: %f", wlAfterIteration);
  ALERT("Improvement after detailed placement: %f%%", 100.0 * (1.0 - wlAfterIteration / wlBeforeDP));
  
  ALERT("DETAILED PLACEMENT FINISHED");
}