#include "STA.h"
#include "AdaptiveRoute.h"
#include "DelayCalculation.h"
#include "Timing.h"
#include "Reporting.h"
#include "Auxiliary.h"
#include "TableFormatter.h"

LayersModel GetActualLayersModel(HDesign& design)
{
  switch (design.cfg.ValueOf("Timing.RCE.LayersUsed", 2))
  {
  case 0:
    return LayersModel_Lumped;
  case 1:
    return LayersModel_OneDirection;
  case 2:
    return LayersModel_TwoDirections;
  case 3:
    return LayersModel_Hippocrate;
  default:
    GLOGERROR(LOGINPLACE, "Unsupported layers number: %d", design.cfg.ValueOf("Timing.RCE.LayersUsed", 2));
    return LayersModel_TwoDirections;
  }
}

SignalModel GetActualSignalModel(HDesign& design)
{
  switch (design.cfg.ValueOf("Timing.SignalDirectionsUsed", 2))
  {
  case 1:
    return SignalModel_Universal;
  case 2:
    return SignalModel_RiseFall;
  default:
    GLOGERROR(LOGINPLACE, "Unsupported number of signal directions: %d", design.cfg.ValueOf("Timing.SignalDirectionsUsed", 2));
    return SignalModel_RiseFall;
  }
}

void STA(HDesign& design, bool doReport, bool reroute)
{
  //1. Route/Reroute
  if (reroute)
    AdaptiveRoute(design);

  //No timing == No STA
  if (!design.CanDoTiming()) return;

  //2. RC Extraction + delays calculation
  CalculateWireDelays(design, GetActualLayersModel(design), GetActualSignalModel(design));
  //3. Arrival & Required time propagation
  PropagateDelays(design);

  if (doReport) ReportNegativeSlacks(design);
}

template<SignalModel sm>
void CalculateDelaysNoWires(HDesign& design);

template<>
void CalculateDelaysNoWires<SignalModel_Universal>(HDesign& design)
{
  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
  {
    double cLoad = 0.0;
    for (HNet::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      double C = design.GetDouble<HPinType::Capacitance>(sink.Type());
      cLoad += C;
      HSteinerPoint pt = design.SteinerPoints[sink];
      design.Set<HSteinerPoint::ObservedC>(pt, C);
      design.Set<HSteinerPoint::PathDelay>(pt, 0.0);
    }
    HSteinerPoint src = design.SteinerPoints[net.Source()];
    design.Set<HSteinerPoint::ObservedC>(src, cLoad);
    design.Set<HSteinerPoint::PathDelay>(src, 0.0);
  }
}

template<>
void CalculateDelaysNoWires<SignalModel_RiseFall>(HDesign& design)
{
  for (HNets::ActiveNetsEnumeratorW net = design.Nets.GetActiveNetsEnumeratorW(); net.MoveNext(); )
  {
    double cLoadR = 0.0;
    double cLoadF = 0.0;
    for (HNet::SinksEnumeratorW sink = net.GetSinksEnumeratorW(); sink.MoveNext(); )
    {
      double CR = design.GetDouble<HPinType::RiseCapacitance>(sink.Type());
      double CF = design.GetDouble<HPinType::FallCapacitance>(sink.Type());
      cLoadR += CR;
      cLoadF += CF;
      HSteinerPoint pt = design.SteinerPoints[sink];
      design.Set<HSteinerPoint::RiseObservedC>(pt, CR);
      design.Set<HSteinerPoint::FallObservedC>(pt, CF);
      design.Set<HSteinerPoint::RisePathDelay>(pt, 0.0);
      design.Set<HSteinerPoint::FallPathDelay>(pt, 0.0);
    }
    HSteinerPoint src = design.SteinerPoints[net.Source()];
    design.Set<HSteinerPoint::RiseObservedC>(src, cLoadR);
    design.Set<HSteinerPoint::FallObservedC>(src, cLoadF);
    design.Set<HSteinerPoint::RisePathDelay>(src, 0.0);
    design.Set<HSteinerPoint::FallPathDelay>(src, 0.0);
  }
}

void CalculateDelaysNoWires(HDesign& design, SignalModel sm)
{
  switch (sm)
  {
  case SignalModel_Universal:
    CalculateDelaysNoWires<SignalModel_Universal>(design);
    return;
  case SignalModel_RiseFall:
    CalculateDelaysNoWires<SignalModel_RiseFall>(design);
    return;
  default:
    GLOGERROR(LOGINPLACE, "Unsupported signal direction: %d", sm);
    CalculateDelaysNoWires<SignalModel_RiseFall>(design);
    return;
  }
}

void NoWireDelaySTA(HDesign& design, bool doReport)
{
  //No timing == No STA
  if (!design.CanDoTiming()) return;

  //2. RC Extraction + delays calculation
  CalculateDelaysNoWires(design, GetActualSignalModel(design));
  //3. Arrival & Required time propagation
  PropagateDelays(design);

  if (doReport) ReportNegativeSlacks(design);
}

double _distance(double x1, double y1, double x2, double y2)
{
  return abs(x1-x2) + abs(y1-y2);
}

double _distance(HDesign& hd, HPin p1, HPin p2)
{
  return _distance(hd.GetDouble<HPin::X>(p1), hd.GetDouble<HPin::Y>(p1), hd.GetDouble<HPin::X>(p2), hd.GetDouble<HPin::Y>(p2));
}

double _distance(HDesign& hd, HPin p1, double x2, double y2)
{
  return _distance(hd.GetDouble<HPin::X>(p1), hd.GetDouble<HPin::Y>(p1), x2, y2);
}

double B5CTiming(HDesign& hd, double x1, double x2, double x3, double x4, double x5, bool printTable)
{
  HCell c1 = Utils::FindCellByName(hd, "c1inv");
  HCell c2 = Utils::FindCellByName(hd, "c2or");
  HCell c3 = Utils::FindCellByName(hd, "c3inv");
  HCell c4 = Utils::FindCellByName(hd, "c4nand");
  HCell c5 = Utils::FindCellByName(hd, "c5and");

  HPin c1A = Utils::FindCellPinByName(hd, c1, "A");
  HPin c1Y = Utils::FindCellPinByName(hd, c1, "Y");
  HPin c2A = Utils::FindCellPinByName(hd, c2, "A");
  HPin c2B = Utils::FindCellPinByName(hd, c2, "B");
  HPin c2Y = Utils::FindCellPinByName(hd, c2, "Y");
  HPin c3A = Utils::FindCellPinByName(hd, c3, "A");
  HPin c3Y = Utils::FindCellPinByName(hd, c3, "Y");
  HPin c4A = Utils::FindCellPinByName(hd, c4, "A");
  HPin c4B = Utils::FindCellPinByName(hd, c4, "B");
  HPin c4Y = Utils::FindCellPinByName(hd, c4, "Y");
  HPin c5A = Utils::FindCellPinByName(hd, c5, "A");
  HPin c5B = Utils::FindCellPinByName(hd, c5, "B");
  HPin c5Y = Utils::FindCellPinByName(hd, c5, "Y");
  HPin in1 = Utils::FindPrimaryPinByName(hd, "in1");
  HPin in2 = Utils::FindPrimaryPinByName(hd, "in2");
  HPin in3 = Utils::FindPrimaryPinByName(hd, "in3");
  HPin out1 = Utils::FindPrimaryPinByName(hd, "out1");
  HPin out2 = Utils::FindPrimaryPinByName(hd, "out2");

  double r = hd.RoutingLayers.Physics.RPerDist;
  double c = hd.RoutingLayers.Physics.LinearC;

  //calculate timing

  if (printTable)
  {
    int numCols = 0;
    int colName = numCols++;
    int colX = numCols++;
    int colY = numCols++;
    int colC = numCols++;
    int colR = numCols++;
    int colT = numCols++;
    int colL = numCols++;
    int colAt = numCols++;
    int colRt = numCols++;
    int colNs = numCols++;
    int colAAT = numCols++;
    int colRAT = numCols++;

    TableFormatter tf("b5c timing details", numCols);
    tf.SetColumnAlign(colName, TableFormatter::Align_Left);
    tf.SetColumnPrecision(colX, 0);
    tf.SetColumnPrecision(colY, 0);
    for(int c = colC; c < numCols; ++c)
      tf.SetColumnPrecision(c, 4);
    tf.NewHeaderRow();
    tf.SetCell(colName, "Point Name");
    tf.SetCell(colX, "X(nm)");
    tf.SetCell(colY, "Y(nm)");
    tf.SetCell(colC, "C (pF)");
    tf.SetCell(colR, "R (kohm)");
    tf.SetCell(colT, "T (ns)");
    tf.SetCell(colL, "input WL (nm)");
    tf.SetCell(colAt, "AAT (ns)");
    tf.SetCell(colRt, "RAT (ns)");
    tf.SetCell(colNs, "Neg.Slack (ns)");
    tf.SetCell(colAAT, "STA AAT (ns)");
    tf.SetCell(colRAT, "STA RAT (ns)");

    tf.NewBorderRow();

#define b3cAddRow(name, x, y, c, r, t, l, at, rt) tf.NewRow(); \
  tf.SetCell(colName, (name)); \
  tf.SetCell(colX, (x)); \
  tf.SetCell(colY, (y)); \
  tf.SetCell(colC, (c)); \
  tf.SetCell(colR, (r)); \
  tf.SetCell(colT, (t)); \
  tf.SetCell(colL, (l)); \
  tf.SetCell(colAt, (at)); \
  tf.SetCell(colRt, (rt)); \
  tf.SetCell(colNs, max(0.0, (at) - (rt)))

#define b3cAddPinRow(pin, c, r, t, l, at, rt) \
  b3cAddRow(Utils::MakePinFullName(hd, pin), hd.GetDouble<HPin::X>(pin), hd.GetDouble<HPin::Y>(pin), c, r, t, l, at, rt); \
  tf.SetCell(colAAT, (hd,hd.TimingPoints[pin]).ArrivalTime()); \
  tf.SetCell(colRAT, (hd,hd.TimingPoints[pin]).RequiredTime())

  //fill table

#undef b3cAddPinRow
#undef b3cAddRow

    tf.Print();
  }

  //return max(0.0, Aout1 - Rout1) + max(0.0, Aout2 - Rout2);
  return 0.0;//Aout1 + Aout2;
}

void SolveB5C(HDesign& hd)
{
  double sizes1[] = {1,2,4};
  double sizes2[] = {1,2,4,8,16,32};

  int len1 = sizeof(sizes1)/sizeof(sizes1[0]);
  int len2 = sizeof(sizes2)/sizeof(sizes2[0]);

  TableFormatter tf("B5C solutions", 6);
  for(int c = 1; c < 6; ++c)
    tf.SetColumnPrecision(c, 1);
  tf.SetColumnPrecision(0, 4);
  tf.NewHeaderRow();
  tf.SetCell(0, "TNS");
  tf.SetCell(1, "x1");
  tf.SetCell(2, "x2");
  tf.SetCell(3, "x3");
  tf.SetCell(4, "x4");
  tf.SetCell(5, "x5");
  tf.FlushLastRow();

  double x1=1,x2=1,x3=1,x4=1,x5=1;
  double btns = B5CTiming(hd, x1, x2, x3, x4, x5, false) + 1.0;

  for (int i1 = 0; i1 < len2; ++i1)
    for (int i2 = 0; i2 < len1; ++i2)
      for (int i3 = 0; i3 < len2; ++i3)
        for (int i4 = 0; i4 < len1; ++i4)
          for (int i5 = 0; i5 < len1; ++i5)
          {
            double tns = B5CTiming(hd, sizes2[i1], sizes1[i2], sizes2[i3], sizes1[i4], sizes1[i5], false);
            if (tns < btns)
            {
              btns = tns;
              tf.NewRow();
              tf.SetCell(0, tns);
              tf.SetCell(1, x1 = sizes2[i1]);
              tf.SetCell(2, x2 = sizes1[i2]);
              tf.SetCell(3, x3 = sizes2[i3]);
              tf.SetCell(4, x4 = sizes1[i4]);
              tf.SetCell(5, x5 = sizes1[i5]);
              tf.FlushLastRow();
            }
          }

  tf.Print(0, false);
  B5CTiming(hd, x1, x2, x3, x4, x5);
}