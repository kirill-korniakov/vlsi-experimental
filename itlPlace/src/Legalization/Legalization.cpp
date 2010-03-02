#include "Legalization.h"
#include "Utils.h"

void Legalization(HDPGrid& grid)
{
  ConfigContext ctx = grid.Design().cfg.OpenContext("Legalization");
  bool doReport = grid.Design().cfg.ValueOf(".doReport", false);
  if (doReport)
  {
    WRITELINE("");
    ALERT("LEGALIZATION STARTED (Abacus)");
  }

  double wlBeforeL = Utils::CalculateHPWL(grid.Design(), false);
  if (doReport)
    ALERT("HPWL before legalization: %f", wlBeforeL);

  AbacusLegalization(grid);
  double wlAfterL = Utils::CalculateHPWL(grid.Design(), false);
  if (doReport)
  {
    ALERT("HPWL after legalization: %f", wlAfterL);
    ALERT("Improvement after legalization: %f%%", 100.0 * (1.0 - wlAfterL/wlBeforeL));

    ALERT("LEGALIZATION FINISHED");
  }
}