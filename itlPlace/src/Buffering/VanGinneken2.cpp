#include "GrowOnlyArray.h"
#include "HDesign.h"
#include "BufferInfo.h"
#include <math.h>
#include "Utils.h"

#define	INFINITY	20000000000.0
#define EPS	0.000000000000000000000001
#define MAXBUFFERTYPE 64

#define QM 1
#define PREDICT 1 /* predictive pruning */

struct InsertedBuffer
{
  HPin sink;
  HPin driver;
  double coordinate;

  bool operator<(const InsertedBuffer& ib) const
  { return coordinate < ib.coordinate; }
};

struct WireTreeEdges;

class WireTreeEdge
{
public:
  WireTreeEdges* Parent;
  int LeftEdge;
  int RightEdge;

  HSteinerPoint Start;
  HSteinerPoint End;
  double Length;
  bool IsStartPointRoot;
  bool IsEndPointSink;
  std::vector<InsertedBuffer> InsertedBuffers;

  void Set(WireTreeEdges* parent, HSteinerPoint start, HSteinerPoint end);
  bool HasLeft();
  bool HasRight();

  int GetLeftEdge();
  int GetRightEdge();

  double Xs();
  double Ys();
  double Xe();
  double Ye();

  void Clear()
  {
    Parent = 0;
    LeftEdge = RightEdge = 0;
    IsStartPointRoot = IsEndPointSink = false;
    Length = 0.0;
    InsertedBuffers.clear();
  }
};

struct WireTreeEdges
{
  GrowOnlyArray<WireTreeEdge> Edges;
  HDesign& Design;

  WireTreeEdges(HDesign& hd): Design(hd), Edges(200,100,0.1) {}
  int CreateNewEdge()
  {
    int id = Edges.New();
    Edges[id].Clear();
    return id;
  }
};


void WireTreeEdge::Set(WireTreeEdges* parent, HSteinerPoint start, HSteinerPoint end)
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
}

bool WireTreeEdge::HasLeft()
{
  return Parent->Design.GetBool<HSteinerPoint::HasLeft>(End);
}

bool WireTreeEdge::HasRight()
{
  return Parent->Design.GetBool<HSteinerPoint::HasLeft>(End);
}

int WireTreeEdge::GetLeftEdge()
{
  if (LeftEdge != 0) return LeftEdge;
  
  LeftEdge = Parent->CreateNewEdge();
  Parent->Edges[LeftEdge].Set(Parent, End, Parent->Design.Get<HSteinerPoint::Left, HSteinerPoint>(End));
  return LeftEdge;
}

int WireTreeEdge::GetRightEdge()
{
  if (RightEdge != 0) return RightEdge;
  
  RightEdge = Parent->CreateNewEdge();
  Parent->Edges[RightEdge].Set(Parent, End, Parent->Design.Get<HSteinerPoint::Right, HSteinerPoint>(End));
  return RightEdge;
}

double WireTreeEdge::Xs() { return Parent->Design.GetDouble<HSteinerPoint::X>(Start); }
double WireTreeEdge::Ys() { return Parent->Design.GetDouble<HSteinerPoint::Y>(Start); }
double WireTreeEdge::Xe() { return Parent->Design.GetDouble<HSteinerPoint::X>(End); }
double WireTreeEdge::Ye() { return Parent->Design.GetDouble<HSteinerPoint::Y>(End); }

//============================================================================
//============================================================================
//============================================================================
struct VGNode
{
  int EdgeID;
  double Coordinate;//0.0 <= Coordinate <= 1.0
#define EDGE edges.Edges[EdgeID]

  VGNode(int eid = 0, double coord = 0.0): EdgeID(eid), Coordinate(coord) {}

  double X(WireTreeEdges& edges) const { return EDGE.Xs() + (EDGE.Xe() - EDGE.Xs()) * Coordinate; }
  double Y(WireTreeEdges& edges) const { return EDGE.Ys() + (EDGE.Ye() - EDGE.Ys()) * Coordinate; }

  bool IsSource(WireTreeEdges& edges) const { return Equals(Coordinate, 0.0) && EDGE.IsStartPointRoot; }
  bool IsSink(WireTreeEdges& edges) const { return Equals(Coordinate, 1.0) && EDGE.IsEndPointSink; }
  bool IsInternal(WireTreeEdges& edges) const { return !IsSink(edges) && !IsSource(edges); }

  double SinkCapacitance(WireTreeEdges& edges) const
  {
    ASSERT(IsSink(edges));
    HDesign& hd = edges.Design;
    return (((EDGE.End,hd).Pin(),hd).Type(),hd).Capacitance();
  }

  double SinkRequiredTime(WireTreeEdges& edges) const
  {
    ASSERT(IsSink(edges));
    HDesign& hd = edges.Design;
    return (hd.TimingPoints[(EDGE.End,hd).Pin()],hd).RequiredTime();
  }

  bool IsNull() const {return EdgeID == 0;}

  VGNode Left(WireTreeEdges& edges) const
  {
    if (Equals(Coordinate, 1.0))
    {
      if (EDGE.HasLeft())
        return VGNode(EDGE.GetLeftEdge(), 0.0);//NOTE: if no right edge then can skip 0.0;
      else
        return VGNode(0, 0.0);
    }
    else if (EDGE.Length < EPS)
      return VGNode(EdgeID, 1.0);
    else
      return VGNode(EdgeID, Coordinate + 0.05);
  }

  VGNode Right(WireTreeEdges& edges) const
  {
    if (Equals(Coordinate, 1.0))
      return VGNode(EDGE.HasRight() ? EDGE.GetRightEdge() : 0, 0.0);
    else
      return VGNode(0, 0.0);
  }
#undef EDGE
};

//============================================================================
//============================================================================
//============================================================================
/* composition node */
struct Comp
{ 
  struct Comp *left;
  struct Comp *right;
  BufferInfo* BufferType;
  /* (x, y) means the buffer is inserted at point with index y with the direction x->y  */
  VGNode x;
  VGNode y;
  int counter;  /* use for comp deletion and comp chare */
};

//============================================================================
//============================================================================
//============================================================================
/* Ordered list used in Van Ginneken algorithm */
typedef struct rlnode
{
  struct rlnode *next;
  double DownstreamCapacitance;   /* downstream capacitance */
  double RequiredTime;  /* required arrival time at the node */
  Comp *com;    /* buffer location */
} RLnode;

//============================================================================
//============================================================================
//============================================================================

class VanGinneken2
{
private:
  HDesign& design;
  HWirePhysicalParams& WirePhisics;
  std::vector<BufferInfo> Buffers;

  WireTreeEdges Edges;
  //Comp* FinalVanLocation;
  int InsertedBuffersCount;
  double DriverResistance;

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
    InsertedBuffersCount = 0;

    HPin sourcePin = (net,design).Source();
    HSteinerPointWrapper source = design[design.SteinerPoints[sourcePin]];

    int baseEdge = Edges.CreateNewEdge();
    Edges.Edges[baseEdge].Set(&Edges, source, source.HasRight() ? source : source.Left());
    
    Utils::DriverPhisics dph = Utils::GetDriverTimingPhisics(design, sourcePin, SignalDirection_None);
    DriverResistance = dph.R;

    list_delete(van(VGNode(baseEdge)));

    if (InsertedBuffersCount > 0)
      SplitNet(net, baseEdge);

    return InsertedBuffersCount;
  }

private:
  RLnode *create_list(VGNode sinkNode)
  {
    RLnode *last = new RLnode();
    last->DownstreamCapacitance = sinkNode.SinkCapacitance(Edges);
    last->RequiredTime = sinkNode.SinkRequiredTime(Edges);
    last->next = 0;
    last->com = 0;
    return last;
  }

  void list_delete(RLnode *l)
  {
    while (l != 0)
    {
      RLnode *tmp = l->next;
#if QM
      if (l->com != 0)
        l->com->counter--;
#endif
      comp_delete(l->com);
      delete l;
      l = tmp;
    }
  }

  void comp_delete(Comp *com)
  {
    if (com == 0) return;
    if (com->counter == 0)
    {
      if (com->left != 0)
      {
        com->left->counter--;
        comp_delete(com->left);
      }
      if (com->right != 0)
      {
        com->right->counter--;
        comp_delete(com->right);
      }
      delete com;
    }
  }

  RLnode *redundent(RLnode *list)
  {
    RLnode dummy;
    double x;

    dummy.DownstreamCapacitance = 0;
    dummy.RequiredTime = -INFINITY;

    dummy.com = 0;
    dummy.next = 0;
    dummy.next = list;

    RLnode* pred = &dummy;
    RLnode* r = list;

    while (r != 0)
    {
#if PREDICT
      x = (r->RequiredTime - Buffers[Buffers.size()-1].Rb() * r->DownstreamCapacitance)
        - (pred->RequiredTime - Buffers[Buffers.size()-1].Rb() * pred->DownstreamCapacitance);
#else
      x = r->RequiredTime - pred->RequiredTime;
#endif
      if (x < EPS)
      {
        pred->next = r->next;
#if QM
        if (r->com != 0)
          r->com->counter--;
#endif
        comp_delete(r->com);
        delete r;
      } 
      else
        pred = r;
      r = pred->next;
    }

    return dummy.next;
  }

  RLnode *add_buffer(double k /*distance*/, RLnode *list, VGNode px, VGNode py)
  {
    RLnode *z[MAXBUFFERTYPE];
    ERROR_ASSERT(Buffers.size() < MAXBUFFERTYPE);

    /* if the wire has length 0, then quit */
#if TREATZERO
    if (k == 0.0)
      return list;
#endif

    /* get the solution after the edge from the current node to its child */
    for (RLnode* nd = list; nd != 0; nd = nd->next)
    {
      //WARNING: delays calculation
      nd->RequiredTime = nd->RequiredTime
        - 0.5 * WirePhisics.RPerDist * WirePhisics.LinearC * k * k //собственная задержка на проводе
        - WirePhisics.RPerDist * k * nd->DownstreamCapacitance; //задержка на обозреваемой ёмкости
      nd->DownstreamCapacitance += k * WirePhisics.LinearC; //wire capacitance
#if !QM
      Comp* p = new Comp();
      p->left = nd->com;
      p->right = 0;
      if (p->left != 0)
        p->left->counter++;
      p->buffertype = 0;
      p->counter = 0;
      p->x = px;
      p->y = py;
      nd->com = p;
#endif
    }

    RLnode dummy;
    dummy.next = list;

    /*  consider the buffer solution */
    for (int i = Buffers.size() - 1; i >= 0; --i)
    {
      RLnode* y = dummy.next;
      RLnode* x = 0;
      BufferInfo* bufType = 0;
      double qmax = -INFINITY;

      while (y != 0)
      {
        //WARNING: delays calculation
        double q = y->RequiredTime 
          - Buffers[i].Tb() //own delay
          - Buffers[i].Rb() * y->DownstreamCapacitance; //driver delay on observed capacitance
        if (q > qmax)
        {
          qmax = q;
          bufType = &Buffers[i];
          x = y;
        }
        y = y->next;
      }

      z[i] = new RLnode();
      z[i]->DownstreamCapacitance = Buffers[i].Cb();
      z[i]->RequiredTime = qmax;

      Comp* p = new Comp();
#if QM
      p->left = x->com;
      if (p->left != 0)
        p->left->counter++;
      p->right = 0;
      p->counter = 1;
#else
      p->left = x->com->left;
      p->right = x->com->right;
      if (p->left != 0)
        p->left->counter++;
      if (p->right != 0)
        p->right->counter++;
      p->counter = 0;
#endif
      p->BufferType = bufType;
      p->x = px;
      p->y = py;
      z[i]->com = p;
    }

    /* add buffer solution answer to the list */
    for (int j = Buffers.size() - 1; j >= 0; --j)
    {
      RLnode* y = dummy.next;
      RLnode* pred = &dummy;
      while (y != 0)
      {
        if (fabs(z[j]->DownstreamCapacitance - y->DownstreamCapacitance) < 1e-10)
        {
          if (z[j]->RequiredTime > y->RequiredTime)
          {
            z[j]->next = y->next;
            pred->next = z[j];
#if QM
            if (y->com != 0)
              y->com->counter--;
#endif
            comp_delete(y->com);
            delete y;
          }
          else
          {
#if QM
            if (z[j]->com != 0)
              z[j]->com->counter--;
#endif
            comp_delete(z[j]->com);
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
        else
        {
          if (z[j]->DownstreamCapacitance > y->DownstreamCapacitance)
          {
            y = y->next;
            pred = pred->next;
          }
        }
      }
      if (y == 0)
      {
        z[j]->next = 0;
        pred->next = z[j];
      }
    }

    return dummy.next;
  }

  RLnode* MergeLists(RLnode* sleft, RLnode* sright, VGNode t)
  {
    RLnode dummy;
    RLnode* s = &dummy;
    dummy.DownstreamCapacitance = 0.0;
    dummy.RequiredTime = 0.0;
    dummy.com = 0;
    dummy.next = 0;
    RLnode* slefthead = sleft;
    RLnode* srighthead = sright;

    while (sleft != 0 && sright != 0)
    {
      double c = sleft->DownstreamCapacitance + sright->DownstreamCapacitance;
      double q = sright->RequiredTime;
      s->next = new RLnode();
      s = s->next;

      if (sleft->RequiredTime < sright->RequiredTime)
      {
        q = sleft->RequiredTime;
        Comp* p = new Comp();
        p->left = sleft->com;
        p->right = sright->com;
        if (p->left != 0)
          p->left->counter++;
        if (p->right != 0)
          p->right->counter++;
        p->BufferType = 0;
#if QM
        p->counter = 1;
#else
        p->counter = 0;
#endif
        p->x = t;
        p->y = t;
        s->com = p;
        sleft = sleft->next;
      }
      else
      {
        Comp* p = new Comp();
        p->left = sleft->com;
        p->right = sright->com;
        if (p->left != 0)
          p->left->counter++;
        if (p->right != 0)
          p->right->counter++;
        p->BufferType = 0;    // steiner merge point, indeed no buffer here
#if QM
        p->counter = 1;
#else
        p->counter = 0;
#endif
        p->x=t;
        p->y=t;
        s->com = p;
        sright = sright->next;
      }

      s->DownstreamCapacitance = c;
      s->RequiredTime = q;
    }
    s->next = 0;

    list_delete(slefthead);
    list_delete(srighthead);
    return dummy.next;
  }

  RLnode *van(VGNode t)
  {
    if (t.IsSink(Edges))
      return create_list(t);

    RLnode* sleft = van(t.Left(Edges));
    RLnode* sright = 0;
    double k1 = distance(t, t.Left(Edges));
    double k2 = 0;
    if (!t.Right(Edges).IsNull())
    {
      sright = van(t.Right(Edges));
      k2 = distance(t, t.Right(Edges));
    }

    if (t.IsSource(Edges))
    {
      double van_answer = -INFINITY;
      Comp* finalVanLocation = 0;
      for (RLnode* x = sleft; x != 0; x = x->next)
      {
        //WARNING: delays calculation
        double q = x->RequiredTime //node required time
          - 0.5 * WirePhisics.RPerDist * WirePhisics.LinearC * k1 * k1 //own segment delay
          - (WirePhisics.RPerDist * k1 + this->DriverResistance) * x->DownstreamCapacitance //deriver and segment delay on load
          - this->DriverResistance * k1 * WirePhisics.LinearC; //driver delay on segment
        if (q > van_answer)
        {
          van_answer = q;
          finalVanLocation = x->com; //WARNING: result returned here
        }
      }
      //WRITELINE("van %10.6f", van_answer);
      this->InsertedBuffersCount = SaveInsertedBuffers(finalVanLocation);
      return sleft;
    }
    else
    {
      if (sright != 0)
        sleft = MergeLists(sleft, sright, t);
      sleft = add_buffer(k1, sleft, t, t.Left(Edges));
      sleft = redundent(sleft);
      ///* if the node has only one child (default left), only 
      //return update solution of left child */
      //if (t.Right(Edges).IsNull())
      //  return sleft;
      //else
      //{
      //  sright = add_buffer(k2, sright, t, t.Right(Edges));
      //  sright = redundent(sright);
      //}
    }
    //return MergeLists(sleft, sright, t);
    return sleft;
  }

  double distance(VGNode& t, VGNode& t1)//only for neighbour nodes
  {
    if (t.EdgeID == t1.EdgeID)
      return (t1.Coordinate - t.Coordinate) * Edges.Edges[t.EdgeID].Length;
    else
      return (1.0 - t.Coordinate) * Edges.Edges[t.EdgeID].Length;
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
      design.Nets.AllocatePins(nnet, newNetPins.size() + 1);
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
  {
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
      //WRITELINE("rb  %10.6f %10.6f %10.6f %10.6f", rtime1, 0.0, dr1, 0.0);
      //WRITELINE("r0  %10.6f %10.6f %10.6f %10.6f", rtime, 0.0, dr, 0.0);
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