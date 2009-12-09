#include "VanGinneken.h"
#include <math.h>
#include "Utils.h"
#include "Timing.h"

#define	INFINITY	20000000000.0
#define MAXBUFFERTYPE 64

using namespace Buffering;
using namespace Internal;

////////////////////////////////////
///Buffering::Internal::WireTreeEdge

void Buffering::WireTreeEdge::Clear()
{
  Parent = 0;
  LeftEdge = RightEdge = 0;
  IsStartPointRoot = IsEndPointSink = false;
  Length = 0.0;
  InsertedBuffers.clear();
}

void Buffering::WireTreeEdge::Set(WireTreeEdges* parent, HSteinerPoint start, HSteinerPoint end)
{
  Parent = parent;
  Start = start;
  End = end;

  HDesign& design = parent->Design;

  Length = ::ManhattanLength(
    design.GetDouble<HSteinerPoint::X>(start),
    design.GetDouble<HSteinerPoint::Y>(start),
    design.GetDouble<HSteinerPoint::X>(end),
    design.GetDouble<HSteinerPoint::Y>(end));

  HPin startPin = design.Get<HSteinerPoint::Pin, HPin>(start);
  HPin endPin = design.Get<HSteinerPoint::Pin, HPin>(end);
  IsStartPointRoot = !::IsNull(startPin) && ((startPin,design).Net(),design).Source() == startPin;
  IsEndPointSink = !::IsNull(endPin) && endPin != startPin;

  if (!IsEndPointSink && !this->HasLeft())
  {
    WRITELINE("!@@@@@");
  }
}

bool Buffering::WireTreeEdge::HasLeft()
{
  return Parent->Design.GetBool<HSteinerPoint::HasLeft>(End);
}

bool Buffering::WireTreeEdge::HasRight()
{
  return Parent->Design.GetBool<HSteinerPoint::HasLeft>(End);
}

//int __declspec(noinline) SetLeftEdge(Buffering::WireTreeEdges*p, int root)
//{
//  int left = p->CreateNewEdge();
//  WireTreeEdge& Root = p->Edges[root];
//  Root.LeftEdge = left;
//  p->Edges[left].Set(p, Root.End, p->Design.Get<HSteinerPoint::Left, HSteinerPoint>(Root.End));
//  return left;
//}
//
//int __declspec(noinline) SetRightEdge(Buffering::WireTreeEdges*p, int root)
//{
//  int right = p->CreateNewEdge();
//  WireTreeEdge& Root = p->Edges[root];
//  Root.RightEdge = right;
//  p->Edges[right].Set(p, Root.End, p->Design.Get<HSteinerPoint::Right, HSteinerPoint>(Root.End));
//  return right;
//}
//
//int Buffering::WireTreeEdge::GetLeftEdge()
//{
//  if (LeftEdge != 0) return LeftEdge;
//
//  return SetLeftEdge(Parent, Parent->Edges.GetIndex(this));
//}

#pragma optimize("", off)
int Buffering::WireTreeEdges::GetLeftEdge(int rootIndex)
{
  if (Edges[rootIndex].LeftEdge != 0) return Edges[rootIndex].LeftEdge;
  int left = CreateNewEdge();
  Edges[rootIndex].LeftEdge = left;
  Edges[left].Set(this, Edges[rootIndex].End, Design.Get<HSteinerPoint::Left, HSteinerPoint>(Edges[rootIndex].End));
  return left;
}

#pragma optimize("", off)
int Buffering::WireTreeEdges::GetRightEdge(int rootIndex)
{
  if (Edges[rootIndex].RightEdge != 0) return Edges[rootIndex].RightEdge;
  int right = CreateNewEdge();
  Edges[rootIndex].RightEdge = right;
  Edges[right].Set(this, Edges[rootIndex].End, Design.Get<HSteinerPoint::Right, HSteinerPoint>(Edges[rootIndex].End));
  return right;
}

#pragma optimize("", on)

//int Buffering::WireTreeEdge::GetRightEdge()
//{
//  if (RightEdge != 0) return RightEdge;
//  return SetRightEdge(Parent, Parent->Edges.GetIndex(this));
//}

double Buffering::WireTreeEdge::Xs()
{
  return Parent->Design.GetDouble<HSteinerPoint::X>(Start);
}

double Buffering::WireTreeEdge::Ys()
{
  return Parent->Design.GetDouble<HSteinerPoint::Y>(Start);
}

double Buffering::WireTreeEdge::Xe()
{
  return Parent->Design.GetDouble<HSteinerPoint::X>(End);
}

double Buffering::WireTreeEdge::Ye()
{
  return Parent->Design.GetDouble<HSteinerPoint::Y>(End);
}

/////////////////////////////////////
///Buffering::Internal::WireTreeEdges

int Buffering::WireTreeEdges::CreateNewEdge()
{
  int id = Edges.New();
  Edges[id].Clear();
  return id;
}

//============================================================================
//============================================================================
//============================================================================

class Buffering::VanGinneken2
{
private:
  HDesign& design;
  HWirePhysicalParams& WirePhisics;
  std::vector<BufferInfo> Buffers;

  WireTreeEdges Edges;

  void LoadBuffers()
  {
    BufferInfo buf = BufferInfo::Create(design);
    Buffers.push_back(buf);
  }

public:
  VanGinneken2(HDesign& hd): design(hd), Edges(hd), WirePhisics(hd.RoutingLayers.Physics)
  {
    LoadBuffers();
  }

  int InsertBuffers(HNet net)
  {
    Edges.Edges.Clear();

    HPin sourcePin = (net,design).Source();
    HSteinerPointWrapper source = design[design.SteinerPoints[sourcePin]];

    int baseEdge = Edges.CreateNewEdge();
    Edges.Edges[baseEdge].Set(&Edges, source, source.HasRight() ? source : source.Left());
    
    Utils::DriverPhisics dph = Utils::GetDriverTimingPhisics(design, sourcePin, SignalDirection_None);

    int insertedBuffersCount = RunVanGinneken(VGNode(baseEdge), dph.R);
    if (insertedBuffersCount > 0) SplitNet(net, baseEdge);
    return insertedBuffersCount;
  }

private:
  RLnode* CreateList(VGNode& sinkNode)
  {
    RLnode *node = new RLnode();
    node->DownstreamCapacitance = sinkNode.SinkCapacitance(Edges);
    node->RequiredTime = sinkNode.SinkRequiredTime(Edges);
    return node;
  }

  void DeleteList(RLnode *l)
  {
    while (l != 0)
    {
      RLnode *tmp = l->next;
      l->com->Release();
      delete l;
      l = tmp;
    }
  }

  RLnode* RemoveRedundentNodes(RLnode *list)
  {//list is ordered by DownstreamCapacitance asc
   //and should be ordeder by RequiredTime asc
    RLnode dummy;
    dummy.next = list;
    dummy.RequiredTime = -INFINITY;//WARNING: must be big negative value

    RLnode* pred = &dummy;
    RLnode* r = list;

    for (RLnode* r = pred->next; r != 0; r = pred->next)
    {
      double x = r->RequiredTime - pred->RequiredTime;
      if (x < EPS)
      {
        pred->next = r->next;
        r->com->Release();
        delete r;
      } 
      else
        pred = r;
    }

    return dummy.next;
  }

  void AddCurrentSegmentToSolution(RLnode *list, double l)
  {
    if (l <= EPS) return;
    /* get the solution after the edge from the current node to its child */
    for (RLnode* nd = list; nd != 0; nd = nd->next)
    {
      nd->RequiredTime = nd->RequiredTime
        - 0.5 * WirePhisics.RPerDist * WirePhisics.LinearC * l * l //собственная задержка на проводе
        - WirePhisics.RPerDist * l * nd->DownstreamCapacitance; //задержка на обозреваемой ёмкости
      nd->DownstreamCapacitance += l * WirePhisics.LinearC; //wire capacitance
    }
  }

  RLnode *AddBuffers(RLnode *list, VGNode& px)
  {
    RLnode* z[MAXBUFFERTYPE];
    ERROR_ASSERT(Buffers.size() < MAXBUFFERTYPE);

    /*  consider the buffer solution */
    for (size_t i = 0; i < Buffers.size(); ++i)
    {
      RLnode* bestNode = 0;
      double qmax = -INFINITY;

      for (RLnode* y = list; y != 0; y = y->next)
      {
        double q = y->RequiredTime 
          //- Buffers[i].Tb() //own delay
          - Buffers[i].Rb() * y->DownstreamCapacitance; //driver delay on observed capacitance
        if (q > qmax)
        {
          qmax = q;
          bestNode = y;
        }
      }

      z[i] = new RLnode();
      z[i]->DownstreamCapacitance = Buffers[i].Cb();
      z[i]->RequiredTime = qmax - Buffers[i].Tb();

      bestNode->com->AddRef();
      z[i]->com = new Comp(bestNode->com, 0, &Buffers[i], px);
      z[i]->BufsCount = 1 + bestNode->BufsCount;
    }

    RLnode dummy;
    dummy.next = list;
    /* add buffer solution answer to the list */
    for (int j = (int)Buffers.size() - 1; j >= 0; --j)
    {
      RLnode* pred = &dummy;
      for (RLnode* y = dummy.next; y != 0; pred = y, y = y->next)
      {
        if (::Equals(z[j]->DownstreamCapacitance, y->DownstreamCapacitance))
        {
          if (z[j]->RequiredTime > y->RequiredTime)
          {//replace old node with new
            z[j]->next = y->next;
            pred->next = z[j];
            y->com->Release();
            delete y;
          }
          else
          {//cancel new node
            z[j]->com->Release();
            delete z[j];
          }
          break;
        }
        if (z[j]->DownstreamCapacitance < y->DownstreamCapacitance)
        {
          z[j]->next = y;
          pred->next = z[j];
          break;
        }
      }
      if (y == 0)
      {//put at the end of list
        z[j]->next = 0;
        pred->next = z[j];
      }
    }

    return dummy.next;
  }

  RLnode* MergeLists(RLnode* slefthead, RLnode* srighthead)
  {
    RLnode dummy;
    RLnode* merged = &dummy;
    RLnode* sleft = slefthead;
    RLnode* sright = srighthead;

    while (sleft != 0 && sright != 0)
    {
      merged->next = new RLnode();
      merged = merged->next;
      merged->DownstreamCapacitance = sleft->DownstreamCapacitance + sright->DownstreamCapacitance;

      if (sright->com == 0)
      {
        sleft->com->AddRef();
        merged->com = sleft->com;
      }
      else if (sleft->com == 0)
      {
        sright->com->AddRef();
        merged->com = sright->com;
      }
      else
      {
        sleft->com->AddRef();
        sright->com->AddRef();
        merged->com = new Comp(sleft->com, sright->com, 0);
      }
      merged->BufsCount = sleft->BufsCount + sright->BufsCount;

      if (sleft->RequiredTime < sright->RequiredTime)
      {
        merged->RequiredTime = sleft->RequiredTime;
        sleft = sleft->next;
      }
      else
      {
        merged->RequiredTime = sright->RequiredTime;
        sright = sright->next;
      }
    }
    merged->next = 0;

    DeleteList(slefthead);
    DeleteList(srighthead);
    return dummy.next;
  }

  RLnode* RecursiveVan(VGNode t)
  {
    ASSERT(t.EdgeID > 0);
    if (t.IsSink(Edges)) return CreateList(t);

    VGNode left = t.Left(Edges);
    RLnode* sleft = RecursiveVan(left);
    if (!t.Right(Edges).IsNull())
      sleft = MergeLists(sleft, RecursiveVan(t.Right(Edges)));

    AddCurrentSegmentToSolution(sleft, t.DistanceTo(left, Edges));
    return RemoveRedundentNodes(AddBuffers(sleft, t));
  }

  int RunVanGinneken(VGNode src, double driverResistance)
  {
    ASSERT(src.IsSource(Edges));
    RLnode* list = RecursiveVan(src);

    double van_answer = -INFINITY;
    Comp* finalVanLocation = 0;

    //choose best solution
    for (RLnode* x = list; x != 0; x = x->next)
    {
      double q = x->RequiredTime - driverResistance * x->DownstreamCapacitance;
      if (q > van_answer)
      {
        van_answer = q;
        finalVanLocation = x->com;
      }
    }

    //WRITELINE("van %10.6f", van_answer);
    int insertedBuffers = SaveInsertedBuffers(finalVanLocation);
    DeleteList(list);

    return insertedBuffers;
  }

  void InsertBuffer(BufferInfo* info, const VGNode& point)
  {
    if (info == 0) return;

    HCellWrapper buffer = design[design.Cells.AllocateCell()];
    buffer.SetType(info->Type());
    buffer.SetPlacementStatus(PlacementStatus_Movable);
    buffer.SetHeight(design.GetDouble<HMacroType::SizeY>(info->Type()));
    buffer.SetWidth(design.GetDouble<HMacroType::SizeX>(info->Type()));
    
    char bufferName[32];
    sprintf(bufferName, "buf_%d", ::ToID(buffer));//TODO: create unique name
    buffer.SetName(bufferName);
    buffer.SetX(point.X(Edges) - buffer.Width() * 0.5);
    buffer.SetY(point.Y(Edges) - buffer.Height() * 0.5);
    buffer.SetOrientation(Orientation_N);
    design.Pins.AllocatePins(buffer);

    InsertedBuffer inb;
    inb.coordinate = point.Coordinate;
    inb.sink = Utils::FindCellPinByName(design, buffer, (info->InPin(),design).Name());
    inb.driver = Utils::FindCellPinByName(design, buffer, (info->OutPin(),design).Name());

    Edges.Edges[point.EdgeID].InsertedBuffers.push_back(inb);
    std::sort(Edges.Edges[point.EdgeID].InsertedBuffers.begin(), Edges.Edges[point.EdgeID].InsertedBuffers.end());
  }

  int SaveInsertedBuffers(Comp* c)
  {
    int bufsCount = 0;
    while(c != 0)
    {
      if (c->right != 0)
        bufsCount += SaveInsertedBuffers(c->right);
      if (c->BufferType != 0)
      {
        InsertBuffer(c->BufferType, c->x);
        bufsCount++;
      }
      c = c->left;
    }
    return bufsCount;
  }

  HNet Create2PinNet(HPin source, HPin sink)
  {
    HNetWrapper net = design[design.Nets.AllocateNet()];
    design.Nets.AllocatePins(net, 2);
    net.SetSource(source);
    HNet::SinksEnumeratorW netsink = net.GetSinksEnumeratorW();
    netsink.MoveNext();
    design.Nets.AssignPin(net, netsink, sink);
    return net;
  }

  void CreateEdgeNets(int EdgeID)
  {
    SplitSteinerEdge(EdgeID);
    std::vector<InsertedBuffer>& bufs = Edges.Edges[EdgeID].InsertedBuffers;
    Utils::InsertNextPoint(design, design.TimingPoints[bufs[0].driver], design.TimingPoints[bufs[0].sink]);
    if (bufs.size() < 2) return;
    for (size_t i = 1; i < bufs.size(); ++i)
    {
      Utils::InsertNextPoint(design, design.TimingPoints[bufs[i].sink], design.TimingPoints[bufs[i - 1].driver]);
      Utils::InsertNextPoint(design, design.TimingPoints[bufs[i].driver], design.TimingPoints[bufs[i].sink]);
      HNetWrapper net = design[Create2PinNet(bufs[i - 1].driver, bufs[i].sink)];
      net.SetKind(NetKind_Default);
      net.SetName(((bufs[i - 1].driver,design).Cell(),design).Name() + "_net");
      DrawTree(net, Color_Red);
    }
  }

  bool EdgeHasBuffers(int edgeID)
  {
    return Edges.Edges[edgeID].InsertedBuffers.size() > 0;
  }

  void SplitNet(HNet net, int baseEdgeID)
  {
    std::stack<int> nextEdges;
    std::stack<int> bufferEdges;
    std::list<HPin> newNetPins;
    HPin netSource = (net,design).Source();

    int fictiveEdge = Edges.CreateNewEdge();
    Edges.Edges[fictiveEdge].LeftEdge = baseEdgeID;
    Edges.Edges[fictiveEdge].End = design.SteinerPoints[netSource];
    Edges.Edges[fictiveEdge].InsertedBuffers.push_back(InsertedBuffer());
    Edges.Edges[fictiveEdge].InsertedBuffers.back().driver = netSource;
    bufferEdges.push(fictiveEdge);

    while(!bufferEdges.empty())
    {
      int startEdge = bufferEdges.top(); bufferEdges.pop();
      HPin source = Edges.Edges[startEdge].InsertedBuffers.back().driver;
      nextEdges.push(startEdge);
      //1. traverce net
      while (!nextEdges.empty())
      {
        int id = nextEdges.top(); nextEdges.pop();
        for (; id != 0; id = Edges.Edges[id].LeftEdge)
        {
          if (id != startEdge && Edges.Edges[id].InsertedBuffers.size() > 0)
          {
            bufferEdges.push(id);
            HPin bufSink = Edges.Edges[id].InsertedBuffers[0].sink;
            Utils::InsertNextPoint(design, design.TimingPoints[bufSink], design.TimingPoints[source]);
            CreateEdgeNets(id);
            newNetPins.push_back(bufSink);
            break;
          }
          if (Edges.Edges[id].RightEdge != 0)
            nextEdges.push(Edges.Edges[id].RightEdge);
          if (Edges.Edges[id].IsEndPointSink)
          {
            newNetPins.push_back((Edges.Edges[id].End,design).Pin());
            break;
          }
        }
      }
      //2. create new net
      HNetWrapper nnet = design[design.Nets.AllocateNet()];
      design.Nets.AllocatePins(nnet, (int)newNetPins.size() + 1);
      nnet.SetSource(source);
      nnet.SetKind(NetKind_Default);
      nnet.SetName(((source,design).Cell(),design).Name() + "_net");
      std::list<HPin>::iterator pin = newNetPins.begin();
      for (HNet::SinksEnumeratorW netsink = nnet.GetSinksEnumeratorW(); netsink.MoveNext(); ++pin)
        design.Nets.AssignPin(nnet, netsink, *pin);
      newNetPins.clear();
      DrawTree(nnet, Color_Red);
    }

    design.Set<HNet::Kind>(net, NetKind_Buffered);
  }

  void SplitSteinerEdge(int edgeID)
  {
    WireTreeEdge& e = Edges.Edges[edgeID];
    HSteinerPoint left = design.Get<HSteinerPoint::Left, HSteinerPoint>(e.Start);
    
    //first point
    if (left == e.End)
      design.Set<HSteinerPoint::Left>(e.Start, design.SteinerPoints[e.InsertedBuffers.front().sink]);
    else
      design.Set<HSteinerPoint::Right>(e.Start, design.SteinerPoints[e.InsertedBuffers.front().sink]);
    design.SteinerPoints.InheritPinCoordinates(design.SteinerPoints[e.InsertedBuffers.front().sink]);

    //last point
    design.Set<HSteinerPoint::Left>(design.SteinerPoints[e.InsertedBuffers.back().driver], e.End);
    design.SteinerPoints.InheritPinCoordinates(design.SteinerPoints[e.InsertedBuffers.back().driver]);

    //middle points
    for (size_t i = 1; i < e.InsertedBuffers.size(); ++i)
    {
      design.SteinerPoints.InheritPinCoordinates(design.SteinerPoints[e.InsertedBuffers[i - 1].driver]);
      design.SteinerPoints.InheritPinCoordinates(design.SteinerPoints[e.InsertedBuffers[i].sink]);
      design.Set<HSteinerPoint::Left>(design.SteinerPoints[e.InsertedBuffers[i - 1].driver],
        design.SteinerPoints[e.InsertedBuffers[i].sink]);
    }
  }

public:
  void DrawTree(HNet net, Color col)
  {return;
    std::stack<HSteinerPoint> points;
    HPinWrapper src = ((net,design).Source(), design);
    design.Plotter.DrawCircle(src.X(), src.Y(), 4, col);
    points.push(design.SteinerPoints[src]);
    while(!points.empty())
    {
      HSteinerPointWrapper pt = (points.top(),design); points.pop();
      if (pt.HasLeft())
      {
        HSteinerPointWrapper lpt = (pt.Left(),design);
        points.push(lpt);
        design.Plotter.DrawLine(pt.X(), pt.Y(), lpt.X(), lpt.Y(), col);
      }
      if (pt.HasRight())
      {
        HSteinerPointWrapper rpt = (pt.Right(),design);
        points.push(rpt);
        design.Plotter.DrawLine(pt.X(), pt.Y(), rpt.X(), rpt.Y(), col);
      }
      design.Plotter.DrawCircle(pt.X(), pt.Y(), 2, Color_Green);
    }
  }
};

#include "STA.h"

HTimingPoint getSink(HDesign& hd, HNet net)
{
  HNet::SinksEnumeratorW sink = (net,hd).GetSinksEnumeratorW();
  sink.MoveNext();
  return hd.TimingPoints[sink];
}

void InsertRepeaters2(HDesign& design)
{
  struct PathBuffering
  {
    VanGinneken2 VG;
    int InsertedBuffers;

    PathBuffering(HDesign& design): VG(design), InsertedBuffers(0) {}

    void ProcessPath(HDesign& design, HCriticalPath path, int pathNumber)
    {
      for (HCriticalPath::PointsEnumeratorW point = (path,design).GetEnumeratorW(); point.MoveNext(); )
      {
        HNetWrapper net = design[((point.TimingPoint(),design).Pin(),design).OriginalNet()];
        if (net.Kind() == NetKind_Active)
        {
          int ins = VG.InsertBuffers(net);
          InsertedBuffers += ins;
          //if (ins > 0) WRITELINE("%30s %d", net.Name().c_str(), ins);
        }
      }
    }
  };

  STA(design, true, true);
  FindCriticalPaths(design);
  PathBuffering pb(design);
  Utils::IterateMostCriticalPaths(design, Utils::ALL_PATHS, Utils::CriticalPathHandler(&pb, &PathBuffering::ProcessPath));
  ALERTFORMAT(("Inserted %d buffers", pb.InsertedBuffers));
  STA(design, true, false);
}

void InsertRepeaters(HDesign& design)
{
  VanGinneken2 vg(design);
  int i = 0;
  int j = 0;
  double tns = Utils::TNS(design);
  double wns = Utils::WNS(design);
  double r = design.RoutingLayers.Physics.RPerDist;
  double c = design.RoutingLayers.Physics.LinearC;
  BufferInfo buf = BufferInfo::Create(design);
  WRITELINE("%10.8f %10.8f %5d", tns, wns, 0);
  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
  {
    //if (j++ < 107) continue;
    //if (net.PinsCount() > 2) continue;
    double dr = Utils::GetDriverTimingPhisics(design, net.Source(), SignalDirection_None).R;
    double rtime = (design.TimingPoints[net.Source()],design).RequiredTime() - (design.SteinerPoints[net.Source()],design).ObservedC() * dr;
    //double srtime = (getSink(design,net),design).RequiredTime();
    //double sc = (design.SteinerPoints[(getSink(design,net),design).Pin()],design).ObservedC();
    design.Plotter.Clear();
    design.Plotter.PlotPlacement();
    vg.DrawTree(net, Color_Black);
    int ins = vg.InsertBuffers(net);
    i += ins;

    if (ins > 0)
    {
      STA(design, false, false);
      double tns1 = Utils::TNS(design);
      double wns1 = Utils::WNS(design);
      double dr1 = Utils::GetDriverTimingPhisics(design, net.Source(), SignalDirection_None).R;
      double rtime1 = (design.TimingPoints[net.Source()],design).RequiredTime() - (design.SteinerPoints[net.Source()],design).ObservedC() * dr1;
      //double srtime1 = (getSink(design,net),design).RequiredTime();
      //double sc1 = (design.SteinerPoints[(getSink(design,net),design).Pin()],design).ObservedC();
      WRITELINE("rb  %10.6f %10.6f %10.6f %10.6f", rtime1, 0.0, dr1, 0.0);
      WRITELINE("r0  %10.6f %10.6f %10.6f %10.6f", rtime, 0.0, dr, 0.0);
      //double L = Utils::CalcNetHPWL(design, net);
      //WRITELINE("bb  %10.6f", srtime - (dr*(L*c + sc) + r*L*(L*c/2 + sc)));
      //WRITELINE("ab  %10.6f", srtime - (dr*(L*c/2+buf.Cb()) + r*L/2*(L*c/4+buf.Cb()) + buf.Tb() + buf.Rb()*(L*c/2+sc) + r*L/2*(L*c/4+sc)));
      //WRITELINE("buf %10.6f %10.6f %10.6f %10.6f", buf.Rb(), buf.Cb(), buf.Tb(), L);
      WRITELINE("%10.8f %10.8f %5d %s %s %s %d",
        tns1, wns1, ins,
        tns1 > tns ? "!" : " ",
        wns1 > wns ? "!" : " ",
        rtime > rtime1 ? "x" : " ",
        j);
      tns = tns1;
      wns = wns1;
      design.Plotter.Refresh();//rtime > rtime1 ? HPlotter::WAIT_1_SECOND : HPlotter::WAIT_1_SECOND);
      design.Plotter.SaveImage("","pix\\");
    }
    j++;
  }
  WRITELINE("Inserted %d buffers!!", i);
}