#include "DetailedPlacement.h"
#include <algorithm>

#define MAX_CELL_PINS_COUNT 2048

int GetOptimalRowIdx(HDPGrid& grid, HCell cell)
{
  HDesign& design = grid.Design();

  ASSERT(design.GetInt<HCell::PinsCount>(cell) <= MAX_CELL_PINS_COUNT);
  if (design.GetInt<HCell::PinsCount>(cell) > MAX_CELL_PINS_COUNT)
    GLOGERROR(LOGINPLACE, "pins count %d.", design.GetInt<HCell::PinsCount>(cell));
  double ySeries[2 * MAX_CELL_PINS_COUNT];
  int nSeries = 0;

  for (HCell::PinsEnumerator pins = design.Get<HCell::Pins, HCell::PinsEnumerator>(cell); pins.MoveNext(); )
  {
    HNet net = design.Get<HPin::Net, HNet>(pins);
    if (::IsNull(net)) 
      continue;
    //duplicated nets aren't problem for median
    //its value doesn't depend on duplicated nets

    double top, bottom;
    HNet::PinsEnumeratorW netPins = design.Get<HNet::Pins, HNet::PinsEnumeratorW>(net);
    DASSERT(design.GetInt<HNet::PinsCount>(net) > 1);
    do 
    {
      netPins.MoveNext();
    } while (netPins == pins);
    top = bottom = netPins.Y();
    
    while (netPins.MoveNext())
      if (netPins != pins)
      {
        double y = netPins.Y();
        if (y < bottom)
          bottom = y;
        else if (y > top)
          top = y;
      }

    //store borders
    ySeries[nSeries++] = bottom;
    ySeries[nSeries++] = top;
  }

  if (nSeries == 0)
    return grid.CellRow(cell);
  
  int half_series = nSeries / 2;
  std::partial_sort(ySeries, ySeries + half_series + 1, ySeries + nSeries);
  return grid.FindRow((ySeries[half_series] + ySeries[half_series - 1] - design.GetDouble<HCell::Height>(cell)) * 0.5);
}

void GetOptimalPosition(HDPGrid& grid, HCell cell, int& rowIdx, int& columnIdx)
{
  HDesign& design = grid.Design();

  ASSERT(design.GetInt<HCell::PinsCount>(cell) <= MAX_CELL_PINS_COUNT);
  double ySeries[2 * MAX_CELL_PINS_COUNT];
  double xSeries[2 * MAX_CELL_PINS_COUNT];
  int nSeries = 0;

  for (HCell::PinsEnumerator pins = design.Get<HCell::Pins, HCell::PinsEnumerator>(cell); pins.MoveNext(); )
  {
    HNet net = design.Get<HPin::Net, HNet>(pins);
    if (::IsNull(net)) 
      continue;
    //duplicated nets aren't problem for median
    //its value doesn't depend on duplicated nets

    double top, bottom;
    double left, right;
    HNet::PinsEnumeratorW netPins = design.Get<HNet::Pins, HNet::PinsEnumeratorW>(net);
    DASSERT(design.GetInt<HNet::PinsCount>(net) > 1);
    do 
    {
      netPins.MoveNext();
    } while (netPins == pins);
    top = bottom = netPins.Y();
    left = right = netPins.X();
    
    while (netPins.MoveNext())
      if (netPins != pins)
      {
        double y = netPins.Y();
        if (y < bottom)
          bottom = y;
        else if (y > top)
          top = y;

        double x = netPins.X();
        if (x < left)
          left = x;
        else if (x > right)
          right = x;
      }

    //store borders
    ySeries[nSeries] = bottom;
    xSeries[nSeries] = left;
    nSeries++;
    ySeries[nSeries] = top;
    xSeries[nSeries] = right;
    nSeries++;
  }

  if (nSeries == 0)
  {
    rowIdx = grid.CellRow(cell);
    columnIdx = grid.CellColumn(cell);
    return;
  }
  
  int half_series = nSeries / 2;
  std::partial_sort(ySeries, ySeries + half_series + 1, ySeries + nSeries);
  rowIdx = grid.FindRow((ySeries[half_series] + ySeries[half_series - 1] - grid.SiteHeight()) * 0.5);

  std::partial_sort(xSeries, xSeries + half_series + 1, xSeries + nSeries); 
  columnIdx = grid.FindColumn((xSeries[half_series] + xSeries[half_series - 1] - grid.SiteWidth()) * 0.5);
}