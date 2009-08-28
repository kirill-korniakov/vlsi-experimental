#include "Legalization.h"
#include "Utils.h"

void Legalization(HDPGrid& grid)
{
  ConfigContext ctx = grid.Design().cfg.OpenContext("Legalization");

  WRITELINE("");
  ALERT("LEGALIZATION STARTED (Abacus)");

  double wlBeforeL = Utils::CalculateHPWL(grid.Design(), false);
  ALERTFORMAT(("HPWL before legalization: %f", wlBeforeL));

  if (grid.Design().cfg.ValueOf("Abacus.useWeights", false))
  {
    ALERT("Weighted Abacus");
    WeightedAbacusLegalization(grid);
  }

  else
  {
    ALERT("Abacus");
    AbacusLegalization(grid);
  }

  double wlAfterL = Utils::CalculateHPWL(grid.Design(), false);
  ALERTFORMAT(("HPWL after legalization: %f", wlAfterL));
  ALERTFORMAT(("Improvement after legalization: %f%%", 100.0 * (1.0 - wlAfterL/wlBeforeL) ));

  ALERT("LEGALIZATION FINISHED");
}