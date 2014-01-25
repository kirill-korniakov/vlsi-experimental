#include "Timing.h"
#include "Reporting.h"
#include "PathFinder.h"

#include <vector>

void AddTSPFromPI(HDesign& design, HTimingPointWrapper& tp_arrival, TemplateTypes<HNet>::queue& ready_nets) 
{
    for (HPins::PrimariesEnumeratorW primary_pad = design.Pins.GetEnumeratorW(); primary_pad.MoveNext(); )
    {
        if (primary_pad.IsPrimaryInput() && !::IsNull(primary_pad.Net()))
        {
            tp_arrival.SetNext(design.TimingPoints[primary_pad]);
            tp_arrival.GoNext();
            ready_nets.push(primary_pad.Net());
        }
    }
}

void AddTSPFromFF(HDesign& design, HTimingPointWrapper& tp_arrival, TemplateTypes<HNet>::queue& ready_nets) 
{
    for (HCells::CellsEnumeratorW cur_cell = design.Cells.GetEnumeratorW(MacroType_FirstSequential, MacroType_LastSequential);
        cur_cell.MoveNext(); )
    {
        for (HCell::PinsEnumeratorW cell_pin = cur_cell.GetPinsEnumeratorW(); cell_pin.MoveNext(); )
        {
            if (!::IsNull(cell_pin.Net())
                && design.GetBool<HTimingPoint::IsTimingStartPoint>(design.TimingPoints[cell_pin]))
            {
                tp_arrival.SetNext(design.TimingPoints[cell_pin]);
                tp_arrival.GoNext();
                ready_nets.push(cell_pin.Net());
            }
        }
    }
}

void SetReverseOrder(HDesign &design) 
{
    //inverse arrival order to fill required order
    HTimingPointWrapper prev_point = design[design.TimingPoints.TopologicalOrderRoot()];
    for (HTimingPointWrapper cur_point = prev_point; !::IsNull(cur_point.GoNext()); prev_point = cur_point)
        cur_point.SetPrevious(prev_point);

    //set the first point in required order to the last one from arrival order
    design[design.TimingPoints.TopologicalOrderRoot()].SetPrevious(prev_point);
}

int* GetnActivePinsForCell(HDesign& design, int cells_size, TemplateTypes<HCell>::queue& ready_cells)
{
    int *nActivePinsForCell = new int[cells_size];
    memset(nActivePinsForCell, 0, cells_size * sizeof(int));

    for (HCells::CellsEnumeratorW cell = design.Cells.GetEnumeratorW(); cell.MoveNext(); )
        if (!cell.IsSequential())
        {
            for (HCell::PinsEnumeratorW pin = cell.GetPinsEnumeratorW(); pin.MoveNext(); )
            {
                HNetWrapper net = design[pin.Net()];
                if (pin.Direction() == PinDirection_INPUT && !::IsNull(net))
                {
                    nActivePinsForCell[::ToID(cell)]++;
                }
            }

            //cell is ready if there are no input nets
            if (nActivePinsForCell[::ToID(cell)] == 0)
            {
                ready_cells.push(cell);
            }
        }

    return nActivePinsForCell;
}

void GetSinkCellsFromPreparedNets(HDesign& design, int*& nActivePinsForCell,
                                  HTimingPointWrapper& tp_arrival, HTimingPointWrapper& tp_required, 
                                  TemplateTypes<HNet>::queue& nextWaveNets, TemplateTypes<HCell>::queue& ready_cells) 
{
    while (!nextWaveNets.empty())
    {
        HNet net = nextWaveNets.front();
        nextWaveNets.pop();

        for (HNet::SinksEnumeratorW sink = design.Get<HNet::Sinks, HNet::SinksEnumeratorW>(net); sink.MoveNext(); )
        {
            if (design.GetBool<HTimingPoint::IsTimingEndPoint>(design.TimingPoints[sink]))
            {
                tp_required.SetPrevious(design.TimingPoints[sink]);
                tp_required.GoPrevious();
            }
            else
            {
                tp_arrival.SetNext(design.TimingPoints[sink]);
                tp_arrival.GoNext();
                if (--nActivePinsForCell[::ToID(sink.Cell())] == 0)
                {
                    ready_cells.push(sink.Cell());
                }
            }
        }
    }
}

void AddSinkPinsAndPrepareNewNets(HDesign& design, HTimingPointWrapper& tp_arrival, 
                                  TemplateTypes<HNet>::queue& nextWaveNets, TemplateTypes<HCell>::queue& ready_cells) 
{
    while (!ready_cells.empty())
    {
        HCell cell = ready_cells.front();
        ready_cells.pop();

        for (HCell::PinsEnumeratorW cell_pin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); cell_pin.MoveNext(); )
        {
            if (cell_pin.Direction() == PinDirection_OUTPUT && !::IsNull(cell_pin.Net()))
            {
                tp_arrival.SetNext(design.TimingPoints[cell_pin]);
                tp_arrival.GoNext();
                nextWaveNets.push(cell_pin.Net());
            }
        }
    }
}

void ReportDependentCell(HDesign& design, int i, int p) 
{
    HCell cycle_cell = ::__ConstructPtr<HCell>(i);
    WRITELINE("%s,%d - dependent cell", design.GetString<HCell::Name>(cycle_cell).c_str(), p);
}

void ReportInputNets(HDesign& design, HCell cycle_cell)
{
    WRITELINE("Input nets:");
    for (HCell::PinsEnumeratorW pin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cycle_cell); pin.MoveNext(); )
    {
        if (design.Get<HPin::Direction, PinDirection>(pin) == PinDirection_INPUT)
        {
            string netName = design.GetString<HNet::Name>(pin.Net());
            WRITELINE("  %s", netName.c_str());
        }
    }
}

CellVector DetermineStoppingGroup(HDesign& design, int* nActivePinsForCell, int cells_size)
{
    CellVector notReadyList;
    for (int i = 0; i < cells_size; ++i)
    {
        if (nActivePinsForCell[i] > 0)
        {
            //ReportDependentCell(design, i, nActivePinsForCell[i]);
            HCell cell = ::__ConstructPtr<HCell>(i);
            notReadyList.push_back(cell);
        }
    }

    for (CellVector::iterator iter = notReadyList.begin(); iter != notReadyList.end(); )
    {
        HCell curCell = *iter;
        CellVector reachableCells = GetReachable(design, curCell);
        RemoveDuplicates(notReadyList, reachableCells);
        
        string curCellName = design.GetString<HCell::Name>(curCell);
        WRITELINE("Analyzed reachable from %s: %d remained", curCellName.c_str(), notReadyList.size());

        iter = find(notReadyList.begin(), notReadyList.end(), curCell);
        iter++;
    }

    return notReadyList;
}

void CheckForCyclesAndBreakOneIfExist(HDesign& design, int*& nActivePinsForCell, int cells_size,
                                      TemplateTypes<HNet>::queue& nextWaveNets, TemplateTypes<HCell>::queue& ready_cells)
{
    if (!nextWaveNets.empty()) return; //this is not a deadlock yet

    CellVector stoppers = DetermineStoppingGroup(design, nActivePinsForCell, cells_size);

    //break dependencies
    for (int i = 0; i < stoppers.size(); i++)
    {
        HCell cell = stoppers[i];
        string cellName = design.GetString<HCell::Name>(cell);
        GLOGWARNING(LOGINPLACE, "Stopping cell for TopologicalOrder found: %s. All input pins will have dummy values.", cellName.c_str());
        ReportInputNets(design, cell);
        nActivePinsForCell[::ToID(cell)] = 0;
        ready_cells.push(cell);
    }
}

void PrintTSP(HDesign& design, HTimingPoint currentArrival)
{
    HTimingPointWrapper start = design[design.TimingPoints.TopologicalOrderRoot()];
    for (HTimingPointWrapper tpIter = start; !IsNull(tpIter.GoNext()); )
    {
        WRITELINE("  tsp: %s", GetCellPinName(design, tpIter).c_str());

        if (tpIter == currentArrival)
            break;
    }
}

int CountActivePins(int cells_size, int* nActivePinsForCell) 
{
    int nPins = 0;
    for (int i = 0; i < cells_size; ++i)
    {
        if (nActivePinsForCell[i] > 0)
        {
            nPins += nActivePinsForCell[i];
        }
    }

    return nPins;
}


void FindTopologicalOrder(HDesign& design)
{
  ConfigContext ctx(design.cfg.OpenContext("TopologicalOrder"));
  //ALERT("TIMING PRIOR OPERATIONS STARTED (finding topological order)...");

  HTimingPointWrapper tp_arrival = design[design.TimingPoints.TopologicalOrderRoot()];
  HTimingPointWrapper tp_required = design[design.TimingPoints.TopologicalOrderRoot()];

  TemplateTypes<HCell>::queue ready_cells;
  TemplateTypes<HNet>::queue nextWaveNets;

  if (!::IsNull(tp_arrival.Next()) && !::IsNull(tp_required.Previous()))
  {
    LOGERROR("Method \"FindTopologicalOrder\" can be called only one time for each benchmark.\n");
    return;
  }

  int cells_size = design._Design.NetList.nCellsLimit;
  int* nActivePinsForCell = GetnActivePinsForCell(design, cells_size, ready_cells);

  AddTSPFromPI(design, tp_arrival, nextWaveNets);
  AddTSPFromFF(design, tp_arrival, nextWaveNets);
  //PrintTSP(design, tp_arrival);

  //main cycle
  while (!nextWaveNets.empty() || !ready_cells.empty())
  {
    GetSinkCellsFromPreparedNets(design, nActivePinsForCell, tp_arrival, tp_required, nextWaveNets, ready_cells);
    AddSinkPinsAndPrepareNewNets(design, tp_arrival, nextWaveNets, ready_cells);
    CheckForCyclesAndBreakOneIfExist(design, nActivePinsForCell, cells_size, nextWaveNets, ready_cells);
  }

  int nActivePins = CountActivePins(cells_size, nActivePinsForCell);
  ASSERT(nActivePins == 0);

  //free temporary data
  delete[] nActivePinsForCell;
  nActivePinsForCell = 0;

  //put timing end points to the end of list
  for (HTimingPointWrapper tep = design[design.TimingPoints.TopologicalOrderRoot()]; tep != tp_required; )
  {
    tep.GoPrevious();
    tp_arrival.SetNext(tep);
    tp_arrival.GoNext();
  }

  //set end of arrival list
  tp_arrival.SetNext(design.TimingPoints.Null());

  SetReverseOrder(design);

  //count number of timing start and end points - initialize start & end points iterators
  design.TimingPoints.CountStartAndEndPoints();

  //ReportTEPNames(design);
  //ReportTopologicalOrder(design);

  //ALERT("TIMING PRIOR OPERATIONS FINISHED");
}
