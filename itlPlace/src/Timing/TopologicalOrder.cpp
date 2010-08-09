#include "Timing.h"
#include "Reporting.h"

void AddPI(HDesign& design, HTimingPointWrapper& tp_arrival, TemplateTypes<HNet>::queue& ready_nets) 
{
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
}

void AddInternalTSP(HDesign& design, HTimingPointWrapper& tp_arrival, TemplateTypes<HNet>::queue& ready_nets) 
{
    for (HCells::CellsEnumeratorW cur_cell
        = design.Cells.GetEnumeratorW(MacroType_FirstSequential, MacroType_LastSequential);
        cur_cell.MoveNext(); )
    {//iterate over FF
        for (HCell::PinsEnumeratorW cell_pin = cur_cell.GetPinsEnumeratorW(); cell_pin.MoveNext(); )
        {
            if (!::IsNull(cell_pin.Net())
                && design.GetBool<HTimingPoint::IsTimingStartPoint>(design.TimingPoints[cell_pin]))
            {
                tp_arrival.SetNext(design.TimingPoints[cell_pin]);
                tp_arrival.GoNext();
                ready_nets.push(cell_pin.Net());
            }
            //FIXME: debugging code
            //else
            //{
            //    string cellName = design.GetString<HCell::Name>(cur_cell);
            //    string pinName = design.GetString<HPin::Name>(cell_pin);
            //    bool isTEP = design.GetBool<HTimingPoint::IsTimingEndPoint>(design.TimingPoints[cell_pin]);
            //    ALERT("Seq, not TSP %s.%s, %d", cellName.c_str(), pinName.c_str(), isTEP);
            //}
        }//for (HCell::PinsEnumeratorW cell_pin
    }//for (HCells::CellsEnumeratorW cur_cel
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

int* GetnActivePinsForCell(HDesign& design, int cells_size)
{
    int *nActivePinsForCell = new int[cells_size];
    memset(nActivePinsForCell, 0, cells_size * sizeof(int));

    //calc number of nets for each cell
    for (HCells::CellsEnumeratorW cur_node = design.Cells.GetEnumeratorW(); cur_node.MoveNext(); )
        if (!cur_node.IsSequential())
            for (HCell::PinsEnumeratorW node_pin = cur_node.GetPinsEnumeratorW(); node_pin.MoveNext(); )
                if (node_pin.Direction() == PinDirection_INPUT && !::IsNull(node_pin.Net()))
                    nActivePinsForCell[::ToID(cur_node)]++;

    return nActivePinsForCell;
}

void GetSinkCellsFromPreparedNets(HDesign& design, int*& nActivePinsForCell,
                                  HTimingPointWrapper& tp_arrival, HTimingPointWrapper& tp_required, 
                                  TemplateTypes<HNet>::queue& nextWaveNets, TemplateTypes<HCell>::queue& fullPinCells) 
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
                    fullPinCells.push(sink.Cell());
                //FIXME: debugging code
                //else if (nActivePinsForCell[::ToID(sink.Cell())] == -1)
                //{
                //    string netName = design.GetString<HNet::Name>(net);
                //    string cellName = design.GetString<HCell::Name>(sink.Cell());
                //    string pinName =  design.GetString<HPin::Name>(sink);
                //    ALERT("FUCK! net: %s\t%s.%s", netName.c_str(), cellName.c_str(), pinName.c_str());
                //}
                //ASSERT(nActivePinsForCell[::ToID(sink.Cell())] != -1);
            }
        }//for (HNet::SinksEnumeratorW sink
    }//while(!ready_nets.empty())
}

void AddSinkPinsAndPrepareNewNets(HDesign& design, 
                                  HTimingPointWrapper& tp_arrival, HTimingPointWrapper& tp_required, 
                                  TemplateTypes<HNet>::queue& nextWaveNets, TemplateTypes<HCell>::queue& ready_cells) 
{
    //process cells
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
        }//for (HCell::PinsEnumeratorW cell_pin
    }//while (!ready_cells.empty())
}

void ReportCycleCell(HDesign& design, int i, int p) 
{
    HCell cycle_cell = ::__ConstructPtr<HCell>(i);
    WRITELINE("%s,%d - not ready cell, pins", design.GetString<HCell::Name>(cycle_cell).c_str(), p);
}

void FindTopologicalOrder(HDesign& design)
{
  ConfigContext ctx = design.cfg.OpenContext("TopologicalOrder");
  //ALERT("TIMING PRIOR OPERATIONS STARTED (finding topological order)...");

  HTimingPointWrapper tp_arrival = design[design.TimingPoints.TopologicalOrderRoot()];
  HTimingPointWrapper tp_required = design[design.TimingPoints.TopologicalOrderRoot()];

  if (!::IsNull(tp_arrival.Next()) && !::IsNull(tp_required.Previous()))
  {
    LOGERROR("Method \"FindTopologicalOrder\" can be called only one time for each benchmark.\n");
    return;
  }

  //FIXME: debugging print
  ReportTEPNames(design);

  int cells_size = design._Design.NetList.nCellsLimit;
  int* nActivePinsForCell = GetnActivePinsForCell(design, cells_size);

  TemplateTypes<HCell>::queue ready_cells;
  TemplateTypes<HNet>::queue nextWaveNets;

  AddPI(design, tp_arrival, nextWaveNets);
  AddInternalTSP(design, tp_arrival, nextWaveNets);

  //main cycle
  while (!nextWaveNets.empty() || !ready_cells.empty())
  {
    GetSinkCellsFromPreparedNets(design, nActivePinsForCell, tp_arrival, tp_required, nextWaveNets, ready_cells);
    AddSinkPinsAndPrepareNewNets(design, tp_arrival, tp_required, nextWaveNets, ready_cells);

    //cycles detection
    if (nextWaveNets.empty())
    {
      int nWaitedNets = design.Nets.Count();
      int cycleIdx = -1;
      for (int i = 0; i < cells_size; ++i)
      {
        if (nActivePinsForCell[i] > 0)
        {//cycle detected
          //ReportCycleCell(design, i, nActivePinsForCell[i]);
          if (nActivePinsForCell[i] < nWaitedNets)
          {//select node with minimal number of waited nets
           //to break less possible number of nets
            nWaitedNets = nActivePinsForCell[i];
            cycleIdx = i;
            if (nWaitedNets == 1)
              break;
          }
        }
      } //for (int i = 0
      
      if (cycleIdx > 0)
      {//break detected cycle
        HCell cycle_cell = ::__ConstructPtr<HCell>(cycleIdx);
        GLOGWARNING(LOGINPLACE, "Cycle detected. This cycle will be broken on cell %s", design.GetString<HCell::Name>(cycle_cell).c_str());
        nActivePinsForCell[cycleIdx] = 0;
        ready_cells.push(cycle_cell);
      }
    }//if (ready_nets.empty())
  }//while (!ready_nets.empty() && !ready_cells.empty())

  //free temporary data
  delete[] nActivePinsForCell;
  nActivePinsForCell = 0;

  //put primary outputs to the end of list
  for (HTimingPointWrapper PO = design[design.TimingPoints.TopologicalOrderRoot()]; PO != tp_required; )
  {
    PO.GoPrevious();
    tp_arrival.SetNext(PO);
    tp_arrival.GoNext();
  }

  //set end of arrival list
  tp_arrival.SetNext(design.TimingPoints.Null());

  SetReverseOrder(design);

  //count number of timing start and end points - initialize start & end points iterators
  design.TimingPoints.CountStartAndEndPoints();

  //ALERT("TIMING PRIOR OPERATIONS FINISHED");
}