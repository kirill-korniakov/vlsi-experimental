#include "Reporting.h"

void ReportPlacementArea(HDesign& design)
{
  double placeableArea = 0.0;

  for (HPlacementRows::EnumeratorW row = design.PlacementRows.GetEnumeratorW(); row.MoveNext(); )
    placeableArea += row.SiteHeight() * row.SiteWidth() * row.HorizSitesCount() * row.VertSitesCount();

  double cellsArea = 0.0;

  for (HCells::PlaceableCellsEnumeratorW cell = design.Cells.GetPlaceableCellsEnumeratorW(); cell.MoveNext(); )
    cellsArea += cell.Width() * cell.Height();

  WRITELINE("");
  WRITELINE("Reporting: placement area report");

  WRITELINE("  Min placement x: %f", design.Circuit.PlacementMinX());
  WRITELINE("  Min placement y: %f", design.Circuit.PlacementMinY());
  WRITELINE("  Max placement x: %f", design.Circuit.PlacementMaxX());
  WRITELINE("  Max placement y: %f", design.Circuit.PlacementMaxY());
  WRITELINE("  Placement area width:  %f", design.Circuit.PlacementWidth());
  WRITELINE("  Placement area height: %f", design.Circuit.PlacementHeight());
  WRITELINE("  Placement region area: %f nm^2", placeableArea);
  WRITELINE("  Cells area:            %f nm^2", cellsArea);
  WRITELINE("  Utilization:           %6.4f%%", cellsArea / placeableArea * 100.0);
}

void ReportBenchmarkStatistics(HDesign& design)
{
  WRITELINE("");
  WRITELINE("Reporting: benchmark statistics");
  WRITELINE("  Nets count\t= %d", design.Nets.Count());
  int nCell = 0;
  int nPin = 0;
  for (HCells::CellsEnumeratorW cell = design.Cells.GetEnumeratorW(); cell.MoveNext(); )
  {
    if (cell.PlacementStatus() != PlacementStatus_Movable
      && cell.PlacementStatus() != PlacementStatus_Fixed)
      continue;

    nCell++;
    nPin += cell.PinsCount();
  }
  WRITELINE("  Cells count\t= %d", nCell );
  WRITELINE("  Pins count\t= %d", nPin );
}