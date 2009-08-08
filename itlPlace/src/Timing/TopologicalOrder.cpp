#include "Timing.h"

void FindTopologicalOrder(HDesign& design)
{
  ConfigContext ctx = design.cfg.OpenContext("TopologicalOrder");
  //ALERT("TIMING PRIOR OPERATIONS STARTED (finding topological order)...");

  HTimingPointWrapper tp_arrival = design[design.TimingPoints.TopologicalOrderRoot()];
  HTimingPointWrapper tp_required = design[design.TimingPoints.TopologicalOrderRoot()];

  TemplateTypes<HCell>::queue ready_cells;
  TemplateTypes<HNet>::queue ready_nets;

  //find all primary inputs
  for (HPins::PrimariesEnumeratorW primary_pad = design.Pins.GetEnumeratorW(); primary_pad.MoveNext(); )
  {
    if (primary_pad.IsPrimaryInput() && !::IsNull(primary_pad.Net()))
    {
      tp_arrival.SetNext(design.TimingPoints[primary_pad]);
      tp_arrival.GoNext();
      ready_nets.push(primary_pad.Net());
    }
  }//for (HPins::PrimariesEnumeratorW primary_pad

  // find all internal timing start points
  for (HCells::CellsEnumeratorW cur_cell
    = design.Cells.GetEnumeratorW(MacroType_FirstSequential, MacroType_LastSequential);
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
    }//for (HCell::PinsEnumeratorW cell_pin
  }//for (HCells::CellsEnumeratorW cur_cell

  int cells_size = design._Design.NetList.nCellsLimit;
  int *activePinsCountForCell = new int[cells_size];
  memset(activePinsCountForCell, 0, cells_size * sizeof(int));

  //calc number of nets for each cell
  for (HCells::CellsEnumeratorW cur_node = design.Cells.GetEnumeratorW(); cur_node.MoveNext(); )
    if (!cur_node.IsSequential())
      for (HCell::PinsEnumeratorW node_pin = cur_node.GetPinsEnumeratorW(); node_pin.MoveNext(); )
        if (node_pin.Direction() == PinDirection_INPUT && !::IsNull(node_pin.Net()))
          activePinsCountForCell[::ToID(cur_node)]++;

  //main cycle
  while (!ready_nets.empty() || !ready_cells.empty())
  {
    //process nets
    while (!ready_nets.empty())
    {
      HNet net = ready_nets.front();
      ready_nets.pop();

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
          if (--activePinsCountForCell[::ToID(sink.Cell())] == 0)
            ready_cells.push(sink.Cell());
        }
      }//for (HNet::SinksEnumeratorW sink
    }//while(!ready_nets.empty())

    //process cells
    while (!ready_cells.empty())
    {
      HCell cell = ready_cells.front();
      ready_cells.pop();

      for (HCell::PinsEnumeratorW cell_pin = design.Get<HCell::Pins, HCell::PinsEnumeratorW>(cell); cell_pin.MoveNext(); )
      {
        if (cell_pin.Direction() == PinDirection_OUTPUT
          && !::IsNull(cell_pin.Net()))
        {
          tp_arrival.SetNext(design.TimingPoints[cell_pin]);
          tp_arrival.GoNext();
          ready_nets.push(cell_pin.Net());
        }
      }//for (HCell::PinsEnumeratorW cell_pin
    }//while (!ready_cells.empty())

    //cycles detection
    if (ready_nets.empty())
    {
      int waited_nets_count = design.Nets.Count();
      int cycle_idx = -1;
      for (int i = 0; i < cells_size; ++i)
      {
        if (activePinsCountForCell[i] > 0)
        {//cycle detected
          if (activePinsCountForCell[i] < waited_nets_count)
          {//select node with minimal number of waited nets
           //to break less possible number of nets
            waited_nets_count = activePinsCountForCell[i];
            cycle_idx = i;
            if (waited_nets_count == 1)
              break;
          }
        }
      } //for (int i = 0
      
      if (cycle_idx > 0)
      {//break detected cycle
        HCell cycle_cell = ::__ConstructPtr<HCell>(cycle_idx);
        LOGWARNINGFORMAT(("Cycle detected. This cycle will be broken on cell %s", design.GetString<HCell::Name>(cycle_cell).c_str()));
        activePinsCountForCell[cycle_idx] = 0;
        ready_cells.push(cycle_cell);
      }
    }//if (ready_nets.empty())
  }//while (!ready_nets.empty() && !ready_cells.empty())

  //free temporary data
  delete[] activePinsCountForCell;
  activePinsCountForCell = 0;

  //put primary outputs to the end of list
  for (HTimingPointWrapper PO = design[design.TimingPoints.TopologicalOrderRoot()]; PO != tp_required; )
  {
    PO.GoPrevious();
    tp_arrival.SetNext(PO);
    tp_arrival.GoNext();
  }

  //set end of arrival list
  tp_arrival.SetNext(design.TimingPoints.Null());

  //inverse arrival order to fill required order
  HTimingPointWrapper prev_point = design[design.TimingPoints.TopologicalOrderRoot()];
  for (HTimingPointWrapper cur_point = prev_point; !::IsNull(cur_point.GoNext()); prev_point = cur_point)
    cur_point.SetPrevious(prev_point);
  //set the first point in required order to the last one from arrival oreder
  design[design.TimingPoints.TopologicalOrderRoot()].SetPrevious(prev_point);

  //count number of timing start and end points - initialize start & end points iterators
  design.TimingPoints.CountStartAndEndPoints();

  //ALERT("TIMING PRIOR OPERATIONS FINISHED");
}