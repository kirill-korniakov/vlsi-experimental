#include "Reporting.h"
#include "Utils.h"

void ReportCellsByMacroFunction(HDesign& design)
{
  int counts[MacroType_Last+1];
  
  for (int i = 0; i <= MacroType_Last; ++i)
    counts[i] = 0;

  for (HCells::PlaceableCellsEnumeratorW cell = design.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
    counts[design[cell.Type()].Type()]++;

  WRITELINE("");
  WRITELINE("Reporting: Number of cells for each known macro function");

  for (int j = MacroType_First; j <= MacroType_Last; ++j)
    if (counts[j] != 0)
    {
      const char* name = Utils::GetMacroFunctionName((MacroType)j);
      WRITELINE("  %s%.*s%d", name,22-strlen(name),"......................", counts[j]);
    }
}