#include "Legalization.h"
#include "Utils.h"
#include "PlacementQualityAnalyzer.h"
#include "Auxiliary.h"

void Legalization(HDPGrid& grid)
{
    ConfigContext ctx = grid.Design().cfg.OpenContext("Legalization");

    bool doReport = grid.Design().cfg.ValueOf(".doReport", false);
    if (doReport)
    {
        WRITELINE("");
        ALERT("LEGALIZATION STARTED (Abacus)");
    }

    PlacementQualityAnalyzer* QA = 
        new PlacementQualityAnalyzer(grid.Design(), grid.Design().cfg.ValueOf("GlobalPlacement.QAcriteria"));
    QA->AnalyzeQuality(0);

    AbacusLegalization(grid);

    QA->AnalyzeQuality(1);

    if (doReport)
    {
        QA->Report("Legalization Degradation");
        ALERT("LEGALIZATION FINISHED");
    }
}